#include <iostream>
#include "linked_list.h"
#include <memory>

int main() {
    std::unique_ptr<LinkedList> list = std::make_unique<LinkedList>();

    list->insertHead(10);
    list->insertHead(20);
    list->insertHead(30);
    list->insertHead(40);

    list->insertTail(1000);
    list->insertTail(2000);
    list->insertTail(3000);
    list->insertTail(4000);

    std::cout << list.get() << std::endl;
    std::cout << "Size: " << list->size() << std::endl;
}
