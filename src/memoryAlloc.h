//
// Created by ernesto on 4/12/15.
//

#ifndef GC_DOJO_MEMORYALLOC_H
#define GC_DOJO_MEMORYALLOC_H
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


typedef long Align;                      /* for alignment to long boundary */

typedef union header                     /* block header */
{
    struct
    {
        union header *ptr;                   /* next block if on free list */
        size_t size;                         /* size of this block */
    } s;

    Align x;                               /* force alignment of blocks */

} Header;


static Header base = {0};                /* empty list to get started */
static Header* freeptr = NULL;           /* start of free list */

#define nalloc 1024

#endif //GC_DOJO_MEMORYALLOC_H
