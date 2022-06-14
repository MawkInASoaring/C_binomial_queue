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

#include <stdlib.h>
#include "basic_struct.h"

//stack *st=NULL, *head_st=NULL;
//queue* que=NULL;
int stack_init(stack** st) {
	//should be free by previous use... not save free them here.
	*st =NULL;
	*st = calloc(1,sizeof(stack));
	if(*st==NULL) return -1;
	return 0;
}
int stack_push(stack* st ,void* ele) {
    stack_node* tmp = calloc(1,sizeof(stack_node)); 
    if(tmp==NULL) return -1;
    (tmp)->ele = ele; 
    if(!stack_empty(st)) (tmp)->next= st->head;
    st->head = tmp;
    return 0;
}
int stack_empty(stack* st) {
	return (st==NULL || st->head==NULL);
}
void* st_top(stack* st) {
	if(!stack_empty(st)) {
		return st->head->ele;
	}
	else return NULL;
}
void* stack_pop(stack* st) {
	void* tmp;
	stack_node* head_st = st->head;
	if(!stack_empty(st)) {
	    tmp = st->head->ele; 
		st->head = st->head->next;
		free(head_st);
		return tmp;
	} 
	else return NULL;
}
void stack_clear(stack* st) {
	while(!stack_empty(st)) stack_pop(st);
}
void stack_delete(stack** st) {
	if(*st==NULL) return;
	stack_clear(*st);
	free(*st);
}

int queue_init(queue** que_p) {
	*que_p =NULL;
    *que_p = calloc(1,sizeof(queue)); if(*que_p==NULL) return -1;
    (*que_p)->front=(*que_p)->rear= calloc(1,sizeof(queue_node));
    if((*que_p)->front==NULL) return -1;
    return 0; 
}

int queue_push(queue* que, void* ele) {
	if(que==NULL) return -1;
	queue_node*	currq = calloc(1,sizeof(queue_node));  
	if(currq==NULL) return -1;
	currq->ele =ele;
    if(!queue_empty(que)) (que)->front->next = currq;
    else { (que)->rear->next=currq; } 	
    (que)->rear= currq;
	return 0;
}

int queue_empty(queue* que) {
	return (que->front!=que->rear);
}

void* queue_pop (queue* que) {
	void* ele;
	if(queue_empty(que)) return NULL;
   	queue_node*	currq = que->front->next; ele=currq->ele;
   	if(que->rear==currq) { que->rear=que->front; } // last-element
   	que->front->next = currq->next; free(currq); 
 	return ele;
}
