#include "linked_list.h"
#include <iostream>
#include <memory>

int main() {
    std::unique_ptr<LinkedList> list = std::make_unique<LinkedList>();

    list->insertHead(40);
    list->insertHead(30);
    list->insertHead(20);
    list->insertHead(10);

    list->insertTail(50);
    list->insertTail(60);
    list->insertTail(70);
    list->insertTail(80);

    std::cout << list.get() << std::endl;
    std::cout << "Size: " << list->size() << std::endl;
}
