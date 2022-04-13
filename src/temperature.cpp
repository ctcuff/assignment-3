#include "util.h"
#include <algorithm>
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

std::set<int> getHighestTemperatures(std::vector<int>& sensorReadings) {
    std::set<int> readings{};

    for (auto it = sensorReadings.rbegin(); it != sensorReadings.rend(); it++) {
        if (readings.find(*it) == readings.end()) {
            readings.insert(*it);
        }

        if (readings.size() == 5) {
            break;
        }
    }

    return readings;
}

std::set<int> getLowestTemperatures(std::vector<int>& sensorReadings) {
    std::set<int> readings{};

    for (auto it = sensorReadings.begin(); it != sensorReadings.end(); it++) {
        if (readings.find(*it) == readings.end()) {
            readings.insert(*it);
        }

        if (readings.size() == 5) {
            break;
        }
    }

    return readings;
}

void generateReport(int hour, std::vector<int>& sensorReadings) {
    std::cout << "[Hour " << hour + 1 << " report]" << std::endl;

    std::sort(sensorReadings.begin(), sensorReadings.end());
    std::set<int> highestTemperatures = getHighestTemperatures(sensorReadings);
    std::set<int> lowestTemperatures = getLowestTemperatures(sensorReadings);

    std::cout << "Highest temperatures: ";
    for (int temperature : highestTemperatures) {
        std::cout << temperature << "F ";
    }

    std::cout << std::endl;

    std::cout << "Lowest temperatures: ";
    for (int temperature : lowestTemperatures) {
        std::cout << temperature << "F ";
    }

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
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
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
