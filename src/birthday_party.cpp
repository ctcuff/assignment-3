#include "concurrent_linked_list.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <unordered_set>
#include <vector>

void printListSize(const std::unique_ptr<ConcurrentLinkedList>& list) {
    std::cout << "Size: " << list->size() << std::endl;
}

std::unordered_set<int>* generateUnorderedSet(int size) {
    auto vec = std::make_unique<std::vector<int>>();

    for (int i = 0; i < size; i++) {
        vec->push_back(i);
    }

    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(vec->begin(), vec->end(), std::default_random_engine(seed));

    std::unordered_set<int>* numbers = new std::unordered_set<int>(vec->begin(), vec->end());

    return numbers;
}

int main() {
    // std::unique_ptr<ConcurrentLinkedList> list = std::make_unique<ConcurrentLinkedList>();

    // list->insertHead(40);
    // list->insertHead(30);
    // list->insertHead(20);
    // list->insertHead(10);

    // list->insertTail(50);
    // list->insertTail(60);
    // list->insertTail(70);
    // list->insertTail(80);

    // std::cout << list.get() << std::endl;
    // std::cout << (list->contains(20) ? "YES" : "NO") << std::endl;

    // printListSize(list);

    std::unordered_set<int>* numbers = generateUnorderedSet(20);

    for (int n : *numbers) {
        std::cout << n << std::endl;
    }

    delete numbers;
}
