#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <iostream>
#include <memory>

class Node {
public:
    int data;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;

    Node(int n) : data(n), next(nullptr){};
    ~Node() {
        std::cout << "Delete Node{" << data << "}" << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& os, Node* const& node);
};

class LinkedList {
private:
    std::shared_ptr<Node> m_head;
    std::shared_ptr<Node> m_tail;
    unsigned int m_size;

public:
    LinkedList() : m_head(nullptr), m_tail(nullptr), m_size(0){};
    ~LinkedList();
    void insertHead(int data);
    void insertTail(int data);
    unsigned int size();
    friend std::ostream& operator<<(std::ostream& os, LinkedList* const& list);
};

#endif
