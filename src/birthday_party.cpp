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

#define THREAD_COUNT 1
// Note: the number of guests numbers must be a multiple of THREAD_COUNT
#define NUM_GUESTS 4
#define TASK_ADD_PRESENT 0
#define TASK_WRITE_CARD 1
#define TASK_SEARCH_FOR_PRESENT 2

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

void completeTask(ConcurrentLinkedList* list, std::unordered_set<int>* gifts, std::unordered_set<int>* cards) {
    while (cards->size() != NUM_GUESTS) {
        int task = generateRandomNumber(0, 2);

        switch (task) {
            case TASK_ADD_PRESENT: {
                if (gifts->empty()) {
                    continue;
                }

                mutex.lock();
                auto iter = gifts->begin();
                int num = *iter;
                gifts->erase(iter);
                mutex.unlock();

                list->orderedInsert(num);
                break;
            }
            case TASK_WRITE_CARD: {
                if (!list->empty()) {
                    int guest = list->removeHead();
                    std::cout << "Thank you guest" << guest << "!" << std::endl;
                    cards->insert(guest);
                }
                break;
            }
            case TASK_SEARCH_FOR_PRESENT: {
                int randomGuest = generateRandomNumber(0, NUM_GUESTS - 1);
                bool found = list->contains(randomGuest);

                std::cout << "Minotaur: guest with ID "
                          << randomGuest
                          << " was " << (found ? "found" : "not found")
                          << std::endl;
                break;
            }
        }
    }
}

int main() {
    std::unique_ptr<ConcurrentLinkedList> list = std::make_unique<ConcurrentLinkedList>();
    std::unique_ptr<std::unordered_set<int>> cards = std::make_unique<std::unordered_set<int>>();
    std::thread threads[THREAD_COUNT] = {};

    std::cout << "Generating numbers..." << std::endl;

    auto numbers = generateUnorderedSet(NUM_GUESTS);

    for (int i = 0; i < THREAD_COUNT; i++) {
        threads[i] = std::thread(completeTask, list.get(), numbers.get(), cards.get());
    }

    std::cout << "Starting tasks..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    for (std::thread& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end - start);

    std::cout << "Finished in " << duration.count() << "ms" << std::endl;
}
