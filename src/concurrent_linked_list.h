#ifndef CONCURRENT_LINKED_LIST_H
#define CONCURRENT_LINKED_LIST_H
#include <iostream>
#include <memory>

class Node {
public:
    int data;
    Node* next;
    Node* prev;

    Node(int n) : data(n), next(nullptr){};
    ~Node() {}

    friend std::ostream& operator<<(std::ostream& os, Node* const& node);
};

class ConcurrentLinkedList {
private:
    Node* m_head;
    Node* m_tail;
    size_t m_size;

public:
    ConcurrentLinkedList() : m_head(nullptr), m_tail(nullptr), m_size(0){};
    ~ConcurrentLinkedList();
    void insertHead(int data);
    void insertTail(int data);
    size_t size();
    bool contains(int data);
    friend std::ostream& operator<<(std::ostream& os, ConcurrentLinkedList* const& list);
};

#endif
