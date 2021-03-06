#include "concurrent_linked_list.h"
#include <climits>
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
}

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

// Removes the first element of the linked list and returns
// its value. If this method is called on an empty linked
// list object, this will return INT_MIN
int ConcurrentLinkedList::removeHead() {
    m_mutex.lock();

    if (m_head == nullptr) {
        m_mutex.unlock();
        return INT_MIN;
    }

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
    return m_head == nullptr;
}

// Inserts a node into the linked list while keepind the list
// sorted in ascending order
void ConcurrentLinkedList::insert(int data) {
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

    if (m_head == nullptr) {
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

// Utility function to print a ConcurrentLinkedList object using std::cout
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

// Utility function to print a Node object using std::cout
std::ostream& operator<<(std::ostream& os, Node* const& node) {
    return os << (node == nullptr ? "(null)" : std::to_string(node->data));
}
