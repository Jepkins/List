#include <stdio.h>
#include "list.h"

int main()
{
    mylist ls;
    list_elm_t val = 0;
    for (size_t i = 0; i < 500; i++, val+=10)
        ls.insert_before(val, 0);

    for (size_t i = 1; i <= 500; i++)
    {
        if (i % 2 || i % 3 || i % 5)
        ls.erase(i);
    }

    size_t ind = ls.getprev(0);
    while (ls.getprev(ind) != 0)
    {
        printf("[%lu] = %d\n", ind, ls.at(ind));
        ind = ls.getprev(ind);
    }
    printf("[%lu] = %d\n", ind, ls.at(ind));

    printf("cap = %lu, [2] = %d, next[2] = %lu, prev[2] = %ld\n", ls.getcap(), ls.at(2), ls.getnext(2), ls.getprev(2));
    return 0;
}
