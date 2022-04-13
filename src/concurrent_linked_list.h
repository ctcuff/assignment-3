#ifndef CONCURRENT_LINKED_LIST_H
#define CONCURRENT_LINKED_LIST_H
#include <cstddef>
#include <iostream>
#include <memory>
#include <mutex>

class Node {
public:
    int data;
    Node* next;
    Node* prev;
    std::mutex mutex;

    Node(int n) : data(n), next(nullptr), prev(nullptr){};
    ~Node() {}

    friend std::ostream& operator<<(std::ostream& os, Node* const& node);
};

class ConcurrentLinkedList {
private:
    Node* m_head;
    Node* m_tail;
    size_t m_size;
    std::mutex m_mutex;

public:
    ConcurrentLinkedList() : m_head(nullptr), m_tail(nullptr), m_size(0){};
    ~ConcurrentLinkedList();

    void insert(int data);
    void remove(int key);
    bool contains(int data);
    int removeHead();
    std::size_t size();
    bool empty();

    friend std::ostream& operator<<(std::ostream& os, std::unique_ptr<ConcurrentLinkedList> const& list);
};

#endif
