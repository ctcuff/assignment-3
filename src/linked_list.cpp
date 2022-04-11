#include "linked_list.h"
#include <iostream>
#include <string>
#include <utility>

LinkedList::~LinkedList() {
    Node* temp;

    while (m_head != nullptr) {
        temp = m_head;
        m_head = m_head->next;
        delete temp;
    }
}

void LinkedList::insertTail(int data) {
    Node* node = new Node(data);

    if (m_tail == nullptr) {
        m_tail = node;
        m_head = m_tail;
    } else {
        node->prev = m_tail;
        m_tail->next = node;
        m_tail = node;
    }

    m_size++;
}

void LinkedList::insertHead(int data) {
    Node* node = new Node(data);

    if (m_head == nullptr) {
        m_head = node;
        m_tail = m_head;
    } else {
        node->next = m_head;
        m_head->prev = node;
        m_head = node;
    }

    m_size++;
}

size_t LinkedList::size() {
    return m_size;
}

std::ostream& operator<<(std::ostream& os, LinkedList* const& list) {
    Node* node = list->m_head;

    while (node != nullptr) {
        os << "[" << node << "]->";
        node = node->next;
    }

    os << "(null)";

    return os;
}

std::ostream& operator<<(std::ostream& os, Node* const& node) {
    return os << node->data;
}
