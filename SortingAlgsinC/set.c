#include "set.h"

#include <stdio.h>

Set set_empty(void) {
    Set empty;
    return empty;
}

Set set_universal(void) {
    Set full = 0b11111111;
    return full;
}

//turns the bit at position x into 1 if the thing is zero
Set set_insert(Set s, int x) {
    Set insert = 0b01;
    insert = insert << x;
    return s | insert;
}

//turns the bit at position x into 0
Set set_remove(Set s, int x) {
    Set remove = 0b01;
    remove = remove << x;
    return s & ~remove;
}

bool set_member(Set s, int x) {
    Set check = 0b01 << x;
    if ((s & check) != 0b00000000) {
        return true;
    } else {
        return false;
    }
}

Set set_union(Set s, Set t) {
    return s | t;
}

Set set_intersect(Set s, Set t) {
    return s & t;
}

Set set_difference(Set s, Set t) {
    return s & ~t;
}

Set set_complement(Set s) {
    return ~s;
}
