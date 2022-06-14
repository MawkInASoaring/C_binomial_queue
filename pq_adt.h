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

#ifndef _PQ_ADT_H
#define _PQ_ADT_H


typedef struct pri_que* pri_quep;
typedef struct priq_node* priq_nodep;

typedef struct pri_que {
	priq_nodep hnodep;	
}pri_que;

typedef struct priq_node {
	void* ele;
	int key; int order;
	priq_nodep pre_sib;	 //previous sibling
	priq_nodep next_sib; //next sibling
	priq_nodep parent;
	priq_nodep first_child; // child with the maximum order
}priq_node;

typedef struct retrun_item {
	void* ele;
	int key; // key-reuse for  callers.
}retrun_item;

int minimum_priority(int a,int b) ;
int maximum_priority(int a,int b) ;

typedef int (*cmp_h)(int,int);
extern cmp_h cmp_func;


int pq_isempty(pri_quep quep);

void pq_clear(pri_quep quep);

void pq_delete_queue(pri_quep* quep);

priq_nodep pq_insert(pri_quep quep,int key,void* ele);

priq_nodep pq_getnode(pri_quep quep,int key);

retrun_item* pq_delete(pri_quep quep); // del_min, or del_max...depend on minq or maxq

pri_quep pq_join(pri_quep a_quep,pri_quep b_quep);

#define DBG_PRINT 0
#define DBG_LIST(x,y) { printf("\ndebug: show root-list for %s\n",(y)); \
	priq_nodep dbgp=(x); 					 \
	while(dbgp) {						 	 \
		printf("dbg-key: %d ",dbgp->key );   \
		dbgp= dbgp->next_sib;           	 \
	}                                    	 \
	printf("\n");      }  


#endif
