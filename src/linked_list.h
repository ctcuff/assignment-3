#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <iostream>
#include <memory>

template <typename T>
class Node {
public:
    T data;
    Node<T>* next;
    Node<T>* prev;

    Node(T n) : data(n), next(nullptr), prev(nullptr){};
    ~Node() {}

    friend std::ostream& operator<<(std::ostream& os, Node<T>* const& node) {
        return os << std::to_string(node->data);
    }
};

template <typename T>
class LinkedList {
private:
    Node<T>* m_head;
    Node<T>* m_tail;
    unsigned int m_size;

public:
    LinkedList() : m_head(nullptr), m_tail(nullptr), m_size(0){};
    ~LinkedList();

    void insertHead(T data);
    void insertTail(T data);
    unsigned int size();

    friend std::ostream& operator<<(std::ostream& os, LinkedList<T>* const& list) {
        Node<T>* node = list->m_head;

        while (node != nullptr) {
            os << node << "->";
            node = node->next;
        }

        os << "(null)";

        return os;
    }
};

#endif
