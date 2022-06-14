#ifndef _C_BASIC_STRUCT_H
#define _C_BASIC_STRUCT_H


typedef struct stack_node {
    struct stack_node* next;
    void* ele;
}stack_node;

typedef struct stack {
	stack_node* head;
}stack;

//extern stack* st;
//extern stack* head_st;

int stack_init(stack** st);

int stack_push(stack* st ,void* ele) ;

void* stack_pop(stack* st);

int stack_empty(stack* st);

void stack_clear(stack* st);

void stack_delete(stack** st);

typedef struct queue_node {
	void* ele;
	struct queue_node* next;
}queue_node;

typedef struct queue {
	queue_node *front,* rear;
}queue;
//extern queue* que;

int queue_init(queue** que_p);

int queue_push(queue* que, void* ele);

void* queue_pop (queue* que);

int queue_empty(queue* que);

#endif
