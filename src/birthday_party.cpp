#include "concurrent_linked_list.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <climits>
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <thread>
#include <unordered_set>
#include <vector>

#define THREAD_COUNT 4
#define NUM_GUESTS 500'000

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

// Returns a shuffled unordered set containing numbers from 0 to size - 1
std::unique_ptr<std::unordered_set<int>> generateUnorderedSet(int size) {
    std::unique_ptr<std::vector<int>> vec = std::make_unique<std::vector<int>>();

    for (int i = 0; i < size; i++) {
        vec->push_back(i);
    }

    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(vec->begin(), vec->end(), std::default_random_engine(seed));

    return std::make_unique<std::unordered_set<int>>(vec->begin(), vec->end());
}

// This function takes the linked list, a randomized set of integers (giftBag),
// and an initially empty set of integers (cards). Every iteration of the while
// loop, we'll choose a random task. This continues until NUM_GUESTS cards have been
// written (i.e., when the cards set has NUM_GUESTS elemnets).
void completeTask(ConcurrentLinkedList* list, std::unordered_set<int>* giftBag, std::unordered_set<int>* cards) {
    while (cards->size() < NUM_GUESTS) {
        int task = generateRandomNumber(0, 2);

        switch (task) {
            // For this task, we'll take a gift from the gift bag and add it
            // to the linked list. Because the gift bag is randomized, we can
            // just grab the first element.
            case TASK_ADD_PRESENT: {
                mutex.lock();

                if (giftBag->empty() || giftBag->begin() == giftBag->end()) {
                    mutex.unlock();
                    continue;
                }

                // Remove the first item from the unordered set and
                // add it to the linked list
                std::unordered_set<int>::iterator iter = giftBag->begin();
                int num = *iter;
                giftBag->erase(iter);
                mutex.unlock();

                list->orderedInsert(num);
                break;
            }
            case TASK_WRITE_CARD: {
                if (list->empty()) {
                    continue;
                }

                int guest = list->removeHead();

                if (guest == INT_MIN) {
                    continue;
                }

#ifdef VERBOSE
                std::cout << "Thank you guest " << guest << "! "
                          << "(guests remaining: " << list->size() << ")"
                          << std::endl;
#endif

                mutex.lock();
                cards->insert(guest);
                mutex.unlock();
                break;
            }
            case TASK_SEARCH_FOR_PRESENT: {
                int randomGuest = generateRandomNumber(0, NUM_GUESTS - 1);
                bool found __attribute__((unused)) = list->contains(randomGuest);

#ifdef VERBOSE
                std::cout << "Minotaur: guest with ID "
                          << randomGuest
                          << " was " << (found ? "found" : "not found")
                          << " (cards written: " << cards->size() << "/" << NUM_GUESTS << ")"
                          << std::endl;
#endif
                break;
            }
        }
    }
}

int main() {
    std::unique_ptr<ConcurrentLinkedList> list = std::make_unique<ConcurrentLinkedList>();
    std::unique_ptr<std::unordered_set<int>> cards = std::make_unique<std::unordered_set<int>>();
    std::thread threads[THREAD_COUNT] = {};

    std::cout << "Generating " << NUM_GUESTS << " numbers..." << std::endl;

    std::unique_ptr<std::unordered_set<int>> giftBag = generateUnorderedSet(NUM_GUESTS);

    for (int i = 0; i < THREAD_COUNT; i++) {
        threads[i] = std::thread(completeTask, list.get(), giftBag.get(), cards.get());
    }

    std::cout << "Starting " << THREAD_COUNT << " tasks..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    for (std::thread& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end - start);

    std::cout << "Finished in " << duration.count() << "ms" << std::endl;
}
