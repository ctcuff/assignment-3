#include "concurrent_linked_list.h"
#include <iostream>
#include <string>
#include <utility>
#include <climits>

ConcurrentLinkedList::~ConcurrentLinkedList() {
    Node* temp;

    while (m_head != nullptr) {
        temp = m_head;
        m_head = m_head->next;
        delete temp;
    }
}

void ConcurrentLinkedList::insertTail(int data) {
    Node* node = new Node(data);
    m_mutex.lock();

    if (m_tail == nullptr) {
        m_tail = node;
        m_head = m_tail;
    } else {
        node->prev = m_tail;
        m_tail->next = node;
        m_tail = node;
    }

    m_size++;
    m_mutex.unlock();
}

void ConcurrentLinkedList::insertHead(int data) {
    Node* node = new Node(data);
    m_mutex.lock();

    if (m_head == nullptr) {
        m_head = node;
        m_tail = m_head;
    } else {
        node->next = m_head;
        m_head->prev = node;
        m_head = node;
    }

    m_size++;
    m_mutex.unlock();
}

// TODO: Don't lock the entire list at once
void ConcurrentLinkedList::remove(int key) {
    if (m_head == nullptr) {
        return;
    }

    Node* curr = m_head;
    m_mutex.lock();

    if (curr->data == key) {
        Node* temp = m_head;
        m_head = m_head->next;

        delete temp;

        if (m_head != nullptr) {
            m_head->prev = nullptr;
        }

        m_size--;
    } else {
        while (curr->next != nullptr) {
            if (curr->next->data == key) {
                Node* temp = curr->next;
                curr->next = curr->next->next;

                if (curr->next != nullptr) {
                    curr->next->prev = curr;
                }

                delete temp;
                m_size--;
                break;
            }
            curr = curr->next;
        }
    }
    m_mutex.unlock();
}

int ConcurrentLinkedList::removeHead() {
    if (m_head == nullptr) {
        return INT_MIN;
    }

    m_mutex.lock();

    int value = m_head->data;
    Node* temp = m_head;
    
    m_head = m_head->next;

    delete temp;

    if (m_head != nullptr) {
        m_head->prev = nullptr;
    }

    m_size--;
    m_mutex.unlock();
    return value;
}

std::size_t ConcurrentLinkedList::size() {
    return m_size;
}

bool ConcurrentLinkedList::empty() {
    return m_size == 0;
}

// Inserts a node into the linked list while keepind the list
// sorted in ascending order
void ConcurrentLinkedList::orderedInsert(int data) {
    m_mutex.lock();

    Node* newNode = new Node(data);

    if (m_head == nullptr) {
        m_head = newNode;
        m_tail = newNode;
    } else if (m_head->data >= newNode->data) {
        newNode->next = m_head;
        newNode->next->prev = newNode;
        m_head = newNode;
    } else {
        Node* curr = m_head;

        while (curr->next != nullptr && curr->next->data < newNode->data) {
            curr = curr->next;
        }

        newNode->next = curr->next;

        if (curr->next != nullptr) {
            newNode->next->prev = newNode;
        }

        curr->next = newNode;
        newNode->prev = curr;

        // In this case, we've reached the end of the list so
        // we need to update the tail node
        if (newNode->next == nullptr) {
            m_tail = newNode;
        }
    }

    m_size++;
    m_mutex.unlock();
}

bool ConcurrentLinkedList::contains(int key) {
    m_mutex.lock();

    if (m_size == 0) {
        m_mutex.unlock();
        return false;
    }

    Node* temp = m_head;

    while (temp != nullptr) {
        if (temp->data == key) {
            m_mutex.unlock();
            return true;
        }

        temp = temp->next;
    }

    m_mutex.unlock();
    return false;
}

bool ConcurrentLinkedList::isSorted() {
    Node* curr = m_head;

    while (curr != nullptr && curr->next != nullptr) {
        if (curr->data > curr->next->data) {
            return false;
        }

        curr = curr->next;
    }

    return true;
}

std::ostream& operator<<(std::ostream& os, std::unique_ptr<ConcurrentLinkedList> const& list) {
    if (list == nullptr) {
        return os << "(null)";
    }
    
    Node* temp = list->m_head;

    while (temp != nullptr) {
        os << "[" << temp << "]->";
        temp = temp->next;
    }

    os << "(null)";

    return os;
}

std::ostream& operator<<(std::ostream& os, Node* const& node) {
    return os << (node == nullptr ? "(null)" : std::to_string(node->data));
}
