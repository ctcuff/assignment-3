#include "linked_list.h"
#include <iostream>
#include <string>
#include <utility>

template <typename T>
LinkedList<T>::~LinkedList() {
    Node<T>* temp;

    while (m_head != nullptr) {
        temp = m_head;
        m_head = m_head->next;
        delete temp;
    }
}

template <typename T>
void LinkedList<T>::insertTail(T data) {
    Node<T>* node = new Node<T>(data);

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

template <typename T>
void LinkedList<T>::insertHead(T data) {
    Node<T>* node = new Node<T>(data);

    if (m_head == nullptr) {
        m_head = node;
        m_tail = m_head;
    } else {
        node->next = m_head;
        m_head = node;
    }

    m_size++;
}

template <typename T>
unsigned int LinkedList<T>::size() {
    return m_size;
}

template class LinkedList<int>;
