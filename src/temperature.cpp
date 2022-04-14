#include "util.h"
#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <set>
#include <thread>

#define THREAD_COUNT 8
#define MINUTES 60

#ifndef HOURS
    #define HOURS 72
#endif

std::mutex mutex;

bool allSensorsReady(int caller, std::vector<bool>& sensors) {
    for (int i = 0; i < static_cast<int>(sensors.size()); i++) {
        if (!sensors[i] && caller != i) {
            return false;
        }
    }
    return true;
}

void printLargestDifference(std::vector<int>& sensorReadings) {
    int step = 10;
    int startInterval = 0;
    int maxDifference = INT_MIN;

    // Because the sensor readings are all stored in one contiguous array,
    // we need to loop through the array in chunks to find the largest
    // difference for that sensor
    for (int threadIndex = 0; threadIndex < THREAD_COUNT; threadIndex++) {
        int offset = threadIndex * MINUTES;

        for (int i = offset; i < MINUTES - step + 1; i++) {
            int max = *std::max_element(sensorReadings.begin() + i, sensorReadings.begin() + i + step);
            int min = *std::min_element(sensorReadings.begin() + i, sensorReadings.begin() + i + step);
            int diff = max - min;

            if (diff > maxDifference) {
                maxDifference = diff;
                startInterval = i;
            }
        }
    }

    std::cout << "Largest temperature difference: " << maxDifference << "F"
              << " starting at minute " << startInterval
              << " and ending at minute " << (startInterval + 10) << std::endl;
}

void printHighestTemperatures(std::vector<int>& sensorReadings) {
    std::set<int> temperatures{};

    for (auto it = sensorReadings.rbegin(); it != sensorReadings.rend(); it++) {
        if (temperatures.find(*it) == temperatures.end()) {
            temperatures.insert(*it);
        }

        if (temperatures.size() == 5) {
            break;
        }
    }

    std::cout << "Highest temperatures: ";

    for (int temperature : temperatures) {
        std::cout << temperature << "F ";
    }

    std::cout << std::endl;
}

void printLowestTemperatures(std::vector<int>& sensorReadings) {
    std::set<int> temperatures{};

    for (auto it = sensorReadings.begin(); it != sensorReadings.end(); it++) {
        if (temperatures.find(*it) == temperatures.end()) {
            temperatures.insert(*it);
        }

        if (temperatures.size() == 5) {
            break;
        }
    }

    std::cout << "Lowest temperatures: ";

    for (int temperature : temperatures) {
        std::cout << temperature << "F ";
    }

    std::cout << std::endl;
}

void generateReport(int hour, std::vector<int>& sensorReadings) {
    std::cout << "[Hour " << hour + 1 << " report]" << std::endl;

    printLargestDifference(sensorReadings);

    std::sort(sensorReadings.begin(), sensorReadings.end());

    printHighestTemperatures(sensorReadings);
    printLowestTemperatures(sensorReadings);

    std::cout << std::endl
              << std::endl;
}

void measureTemperature(int threadId, std::vector<int>& sensorReadings, std::vector<bool>& sensorsReady) {
    for (int hour = 0; hour < HOURS; hour++) {
        for (int minute = 0; minute < MINUTES; minute++) {
            sensorsReady[threadId] = false;
            sensorReadings[minute + (threadId * MINUTES)] = Util::generateRandomNumber(-100, 70);
            sensorsReady[threadId] = true;

            // Make sure we wait for all sensors to take a reading before we continue
            // with another temperature reading
            while (!allSensorsReady(threadId, sensorsReady)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        if (threadId == 0) {
            mutex.lock();
            generateReport(hour, sensorReadings);
            mutex.unlock();
        }
    }
}

int main() {
    // Keeps track of every reading by the threads. Each thread can only access
    // 60 readings. For example, thread 1 writes to 0 - 59, thread 2 writes to
    // 60 - 119, and so on
    std::vector<int> sensorReadings(THREAD_COUNT * MINUTES);
    std::vector<bool> sensorsReady(THREAD_COUNT);
    std::thread threads[THREAD_COUNT] = {};

    for (int i = 0; i < THREAD_COUNT; i++) {
        threads[i] = std::thread(measureTemperature, i, std::ref(sensorReadings), std::ref(sensorsReady));
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (std::thread& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end - start);

    std::cout << "Finished in " << duration.count() << "ms" << std::endl;
}
