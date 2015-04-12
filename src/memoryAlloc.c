//
// Created by ernesto on 4/12/15.
//

#include "memoryAlloc.h"

/* minimum #units to request */
/* morecore: ask system for more memory */
Header* morecore(size_t units)
{
    char *cp;
    Header *hdr;

    if(units < nalloc)
        units = nalloc;

    cp = (char *) sbrk(units * sizeof(Header));
    if(cp == (char *) -1)
        return NULL;
    hdr = (Header *) cp;
    hdr->s.size = units;
    return hdr;
}


/* malloc: general-purpose storage allocator */
void* malloc (size_t nbytes)
{
    Header*  p;
    Header*  prevptr;
    size_t   nunits;
    void*    result;
    bool     is_allocating;

    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;

    prevptr = freeptr;
    if (prevptr == NULL)                   /* no free list yet */
    {
        base.s.ptr  = &base;
        freeptr     = &base;
        prevptr     = &base;
        base.s.size = 0;
    }

    is_allocating = true;
    for (p = prevptr->s.ptr; is_allocating; p = p->s.ptr)
    {
        if (p->s.size >= nunits)             /* big enough */
        {
            if (p->s.size == nunits)           /* exactly */
            {
                prevptr->s.ptr = p->s.ptr;
            }
            else                               /* allocate tail end */
            {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }

            freeptr = prevptr;
            result = p+1;
            is_allocating = false;             /* we are done */
        }

        if (p == freeptr)                    /* wrapped around free list */
        {
            p = morecore(nunits);
            if (p == NULL)
            {
                result = NULL;                   /* none left */
                is_allocating = false;
            }
        }
        prevptr = p;
    } /* for */

    return result;
}

/* free: put block ap in free list */
void free(void *ptr)
{
    Header *bp, *p;
    if(ptr == NULL)
        return;

    bp = (Header *)ptr - 1;    /* point to block header */

    /* K&R code. */
    /* break if bp is inbetween p and p->s.next or is outside of p->s.next and p when the list loops back */
    for (p = freeptr; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
            break; /* freed block at start or end of arena */

    if (bp + bp->s.size == p->s.ptr) {    /* join to upper nbr */
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else
        bp->s.ptr = p->s.ptr;
    if (p + p->s.size == bp) {            /* join to lower nbr */
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else
        p->s.ptr = bp;

    freeptr = p;
}

void *realloc(void *ptr, size_t size)
{
    Header *hdr;
    size_t old_size;
    void *new_ptr;

    /* basic cases with NULL and size == 0 */
    if(ptr == NULL)
        return malloc(size);

    if(size == 0)
    {
        free(ptr);
        return NULL;
    }

    /* allocate another memory area, copy data, then free the old area */
    hdr = ((Header *)ptr) - 1;
    old_size = (hdr->s.size-1) * sizeof(Header);
    if(old_size > size)
        old_size = size;

    new_ptr = malloc(size);
    if(new_ptr == NULL)
        return NULL;

    memcpy(new_ptr, ptr, old_size);
    free(ptr);

    return new_ptr;
}

