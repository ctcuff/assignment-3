#ifndef LINKED_LIST_H
#define LINKED_LIST_H
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

class LinkedList {
private:
    Node* m_head;
    Node* m_tail;
    size_t m_size;

public:
    LinkedList() : m_head(nullptr), m_tail(nullptr), m_size(0){};
    ~LinkedList();
    void insertHead(int data);
    void insertTail(int data);
    size_t size();
    friend std::ostream& operator<<(std::ostream& os, LinkedList* const& list);
};

#endif
