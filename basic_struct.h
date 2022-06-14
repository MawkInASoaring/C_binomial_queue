/* C_binomial_queue
 * 
 * Copyright 2002 Hsueh-Jen Yang
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public Licens
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
