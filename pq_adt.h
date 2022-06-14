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
