#include "linked_list.h"
#include <iostream>
#include <string>
#include <utility>

LinkedList::~LinkedList() {}

void LinkedList::insertTail(int data) {
    m_size++;

    if (!m_tail) {
        m_tail = std::make_shared<Node>(data);
        m_head = m_tail;
        return;
    }

    std::shared_ptr<Node> node = std::make_shared<Node>(data);
    node->prev = m_tail;
    m_tail->next = node;
    m_tail = std::move(node);
}

void LinkedList::insertHead(int data) {
    m_size++;

    if (!m_head) {
        m_head = std::make_shared<Node>(data);
        m_tail = m_head;
        return;
    }

    std::shared_ptr<Node> temp = std::make_shared<Node>(data);

    temp->next = m_head;
    m_head = std::move(temp);
}

unsigned int LinkedList::size() {
    return m_size;
}

std::ostream& operator<<(std::ostream& os, LinkedList* const& list) {
    Node* node = list->m_head.get();

    while (node != nullptr) {
        os << node << "->";
        node = node->next.get();
    }

    os << "(null)";

    return os;
}

std::ostream& operator<<(std::ostream& os, Node* const& node) {
    return os << std::to_string(node->data);
}
