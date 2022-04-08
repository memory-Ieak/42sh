#pragma once

#include <stdlib.h>

#include "../parser/ast_tree.h"

struct queue
{
    void *data;
    struct queue *next;
};

struct queue *queue_init(void);
struct queue *queue_add(struct queue *q, void *data);
struct queue *queue_pop(struct queue *q);
void *queue_head(struct queue *q);
int queue_is_empty(struct queue *q);