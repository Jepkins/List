#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef int list_elm_t;

class mylist {
private:
    list_elm_t* buff = nullptr;
    size_t* prev = nullptr;
    size_t* next = nullptr;
    size_t ifree = 1;
    size_t head = 0;
    size_t tail = 0;
    size_t size = 0;
    size_t cap = 0;
    bool inited = false;

    mylist(const mylist&) = delete;             // FUCK: make the copy ctor
    mylist& operator=(const mylist&) = delete;  // FUCK: make the assignment
    // FUCK: make moves

    size_t request_free();
public:
    mylist(size_t base_cap);
    ~mylist();
    size_t getsize();
    size_t getcap();
    size_t gethead();
    size_t gettail();
    size_t getnext(size_t ind);
    size_t getprev(size_t ind);
    int insert_after(list_elm_t elm, size_t ind);
    int insert_before(list_elm_t elm, size_t ind);
    int insert_at(list_elm_t elm, size_t ind);
    int erase(size_t ind);
    list_elm_t at(size_t ind);
};

#endif // LIST_H
