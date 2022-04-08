#include "queue.h"

struct queue *queue_init(void)
{
    return NULL;
}

struct queue *create_queue(void *data)
{
    struct queue *q = malloc(sizeof(struct queue));
    q->data = data;
    q->next = NULL;
    return q;
}

struct queue *queue_add(struct queue *q, void *data)
{
    if (!q)
        return create_queue(data);
    else
    {
        struct queue *tmp = q;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = create_queue(data);
    }
    return q;
}

struct queue *queue_pop(struct queue *q)
{
    struct queue *next = q->next;
    free(q);
    return next;
}

void *queue_head(struct queue *q)
{
    return q->data;
}

int queue_is_empty(struct queue *q)
{
    if (q)
        return 0;
    return 1;
}