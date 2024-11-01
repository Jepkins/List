#include <stdio.h>
#include "list.h"

int main()
{
    mylist ls(6);
    printf("cap = %lu, [3] = %d, next[3] = %lu\n", ls.getcap(), ls.at(3), ls.getnext(3));
    return 0;
}
