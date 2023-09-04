#include "stack.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct stack {
    uint32_t capacity;
    uint32_t top;
    uint32_t *items;
} Stack;

Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    s->capacity = capacity;
    s->top = 0;
    // We need enough memory for <capacity> numbers
    s->items = calloc(s->capacity, sizeof(uint32_t));
    // We created our stack, return it!
    return s;
}

void stack_free(Stack **sp) {
    // sp is a double pointer, so we have to check if it,
    // or the pointer it points to is null.
    if (sp != NULL && *sp != NULL) {
        // Of course, we have to remember to free the
        // memory for the array of items first,
        // as that was also dynamically allocated!
        // If we freed the Stack first then we would
        // not be able to access the array to free it.
        if ((*sp)->items) {
            free((*sp)->items);
            (*sp)->items = NULL;
        }
        // Free memory allocated for the stack
        free(*sp);
    }
    if (sp != NULL) {
        // Set the pointer to null! This ensures we dont ever do a double free!
        *sp = NULL;
    }
}

bool stack_push(Stack *s, uint32_t val) {
    if (stack_full(s)) {
        return false;
    }
    s->items[s->top] = val;
    s->top++;
    return true;
}

bool stack_pop(Stack *s, uint32_t *val) {
    if (s->top > 0) {
        s->top--;
        *val = s->items[s->top];
        return true;
    }
    return false;
}

bool stack_peek(const Stack *s, uint32_t *val) {
    if (s->top > 0) {
        *val = s->items[(s->top) - 1];
        return true;
    }
    return false;
}

bool stack_empty(const Stack *s) {
    if (s->top == 0) {
        return true;
    }
    return false;
}

bool stack_full(const Stack *s) {
    if ((s->top) == s->capacity) {
        return true;
    }
    return false;
}

uint32_t stack_size(const Stack *s) {
    return s->top;
}

//ADD ASSERTS ADD ASSERS ADD ASSERTS
void stack_copy(Stack *dst, const Stack *src) {
    dst->top = 0;
    for (uint32_t i = 0; i < src->top; i++) {
        if ((dst->capacity) < i) {
            break;
        }
        dst->items[i] = src->items[i];
        (dst->top)++;
    }
}

// This function will print out the stack as a list of elements, given a list of vertex names, starting with the
// bottom of the stack. Every item in the stack should be less than the number of vertices so you can index the
// array successfully.
void stack_print(const Stack *s, FILE *f, char *vals[]) {
    for (uint32_t i = 0; i < s->top; i += 1) {
        fprintf(f, "%s\n", vals[s->items[i]]);
    }
}
