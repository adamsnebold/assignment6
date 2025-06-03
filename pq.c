/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue using a heap.
 *
 * Name:
 * Email:
 */

#include <stdlib.h>
#include <assert.h>
#include "pq.h"
#include "dynarray.h"

/*
 * Structure for a priority queue element containing data and priority.
 */
struct pq_element {
    void* data;
    int priority;
};

/*
 * Priority queue structure using a dynamic array for heap storage.
 */
struct pq {
    struct dynarray* heap;
};

/* Helper functions */
void _swap(struct pq_element* a, struct pq_element* b) {
    struct pq_element temp = *a;
    *a = *b;
    *b = temp;
}

void _heapify_up(struct pq* pq, int index) {
    if (index == 0) return;

    int parent = (index - 1) / 2;
    struct pq_element* curr = (struct pq_element*)dynarray_get(pq->heap, index);
    struct pq_element* par = (struct pq_element*)dynarray_get(pq->heap, parent);

    if (curr->priority > par->priority) {
        _swap(curr, par);
        _heapify_up(pq, parent);
    }
}

void _heapify_down(struct pq* pq, int index) {
    int size = dynarray_length(pq->heap);
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int largest = index;

    if (left < size) {
        struct pq_element* left_child = (struct pq_element*)dynarray_get(pq->heap, left);
        struct pq_element* curr = (struct pq_element*)dynarray_get(pq->heap, largest);
        if (left_child->priority > curr->priority) {
            largest = left;
        }
    }

    if (right < size) {
        struct pq_element* right_child = (struct pq_element*)dynarray_get(pq->heap, right);
        struct pq_element* curr = (struct pq_element*)dynarray_get(pq->heap, largest);
        if (right_child->priority > curr->priority) {
            largest = right;
        }
    }

    if (largest != index) {
        _swap((struct pq_element*)dynarray_get(pq->heap, index),
              (struct pq_element*)dynarray_get(pq->heap, largest));
        _heapify_down(pq, largest);
    }
}

/* Priority Queue Functions */
struct pq* pq_create() {
    struct pq* pq = malloc(sizeof(struct pq));
    assert(pq);
    pq->heap = dynarray_create();
    return pq;
}

void pq_free(struct pq* pq) {
    int size = dynarray_length(pq->heap);
    for (int i = 0; i < size; i++) {
        free(dynarray_get(pq->heap, i));
    }
    dynarray_free(pq->heap);
    free(pq);
}

int pq_isempty(struct pq* pq) {
    return dynarray_length(pq->heap) == 0;
}

void pq_insert(struct pq* pq, void* data, int priority) {
    struct pq_element* elem = malloc(sizeof(struct pq_element));
    assert(elem);
    elem->data = data;
    elem->priority = priority;
    dynarray_insert(pq->heap, dynarray_length(pq->heap), elem);
    _heapify_up(pq, dynarray_length(pq->heap) - 1);
}

void* pq_max(struct pq* pq) {
    assert(!pq_isempty(pq));
    struct pq_element* elem = (struct pq_element*)dynarray_get(pq->heap, 0);
    return elem->data;
}

int pq_max_priority(struct pq* pq) {
    assert(!pq_isempty(pq));
    struct pq_element* elem = (struct pq_element*)dynarray_get(pq->heap, 0);
    return elem->priority;
}

void* pq_max_dequeue(struct pq* pq) {
    assert(!pq_isempty(pq));

    struct pq_element* max_elem = (struct pq_element*)dynarray_get(pq->heap, 0);
    void* max_data = max_elem->data;

    int last_index = dynarray_length(pq->heap) - 1;
    if (last_index < 0) return NULL;  // Prevent invalid access

    struct pq_element* last_elem = (struct pq_element*)dynarray_get(pq->heap, last_index);
    dynarray_set(pq->heap, 0, last_elem);
    dynarray_remove(pq->heap, last_index);

    free(max_elem);
    if (!pq_isempty(pq)) {
        _heapify_down(pq, 0);
    }

    return max_data;
}
