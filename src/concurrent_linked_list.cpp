#include "concurrent_linked_list.h"
#include <iostream>
#include <string>
#include <utility>

ConcurrentLinkedList::~ConcurrentLinkedList() {
    Node* temp;

    while (m_head != nullptr) {
        temp = m_head;
        m_head = m_head->next;
        delete temp;
    }

    std::cout << "Deleted" << std::endl;
}

void ConcurrentLinkedList::insertTail(int data) {
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

void ConcurrentLinkedList::insertHead(int data) {
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

size_t ConcurrentLinkedList::size() {
    return m_size;
}

bool ConcurrentLinkedList::contains(int data) {
    if (m_size == 0) {
        return false;
    }

    Node* temp = m_head;

    while (temp != nullptr) {
        if (temp->data == data) {
            return true;
        }

        temp = temp->next;
    }

    return false;
}

std::ostream& operator<<(std::ostream& os, ConcurrentLinkedList* const& list) {
    Node* temp = list->m_head;

    while (temp != nullptr) {
        os << "[" << temp << "]->";
        temp = temp->next;
    }

    os << "(null)";

    return os;
}

std::ostream& operator<<(std::ostream& os, Node* const& node) {
    return os << node->data;
}
