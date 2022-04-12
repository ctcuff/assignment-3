#include "concurrent_linked_list.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <thread>
#include <unordered_set>
#include <vector>

#define THREAD_COUNT 4

std::mutex mutex;

// Note: the bounds for min and max are both inclusive
unsigned int generateRandomNumber(int min, int max) {
    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
    return dist(rng);
}

std::unique_ptr<std::unordered_set<int>> generateUnorderedSet(int size) {
    std::unique_ptr<std::vector<int>> vec = std::make_unique<std::vector<int>>();

    for (int i = 0; i < size; i++) {
        vec->push_back(i);
    }

    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(vec->begin(), vec->end(), std::default_random_engine(seed));

    return std::make_unique<std::unordered_set<int>>(vec->begin(), vec->end());
}

void addToList(ConcurrentLinkedList* list, std::unordered_set<int>* numbers) {
    while (!numbers->empty()) {
        mutex.lock();
        auto iter = numbers->begin();
        int num = *iter;
        numbers->erase(iter);
        mutex.unlock();

        list->orderedInsert(num);
    }
}

int main() {
    std::unique_ptr<ConcurrentLinkedList> list = std::make_unique<ConcurrentLinkedList>();
    std::thread threads[THREAD_COUNT] = {};

    std::cout << "Generating numbers..." << std::endl;

    // Note: the number of random numbers must be a multiple of THREAD_COUNT
    auto numbers = generateUnorderedSet(50);

    for (int i = 0; i < THREAD_COUNT; i++) {
        threads[i] = std::thread(addToList, list.get(), numbers.get());
    }

    std::cout << "Inserting..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    for (std::thread& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end - start);

    std::cout << "Finished in " << duration.count() << "ms" << std::endl;
    std::cout << list << std::endl;
    std::cout << "Size: " << list->size() << std::endl;

    if (!list->isSorted()) {
        std::cout << "Oh no!!! The list is unsorted???" << std::endl;
    }
}
