/*----------------------------------------------
 * Author: Elanor Thomas
 * Base code from Aline Normoyle and Johan Montelius
 * Date: Apr 19 2023
 * Description: This file contains a homemade
 * implementation of the malloc method in c.
 * It is a greatly simplified version.
 * The malloc method checks if there is a block
 * of memory in the freed list  that is big
 * enough to hold the new piece of data, allocating
 * new space if there is not. The free method
 * simply takes a piece of memory and adds it
 * to the linked list storing freed blocks of
 * memory.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
    int size; // size of memory allocated from sbrk
    int used; // amount currently in use
    struct chunk *next;
};
struct chunk *flist = NULL;

void *malloc (size_t size){
    // TODO: Implement malloc with a free list (See reading for details)
    if( size == 0 ){
        return NULL;
    }

    struct chunk *next = flist;
    struct chunk *prev = NULL;

    while (next != NULL){
        if (next->size >= size){
            if (prev != NULL){
                prev->next = next->next;
            } else {
                flist = next->next;
            }
            next->used = size;
            return (void*)(next + 1);
        } else {
            prev = next;
            next = next->next;
        }
    }

    //OR use sbrk to allocate new memory

    void *memory = sbrk(size + sizeof(struct chunk));
    if (memory == (void *) - 1){
        return NULL;
    } else {
        struct chunk *cnk = (struct chunk*)memory;
        cnk->size = size;
        cnk->used = size;
        return (void*)(cnk + 1);
    }
}

void free(void *memory) {
    // TODO: Implement malloc with a free list (See reading for details)
    if(memory != NULL) {/* we're jumping back one chunk position */
        struct chunk *cnk = (struct chunk*)((struct chunk*)memory - 1);
        cnk->next = flist;
        flist = cnk;
    }
    return;
}
