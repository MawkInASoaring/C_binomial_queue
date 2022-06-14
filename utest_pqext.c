#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>
#include "pq_help.h"
#include "pq_adt.h"
#include "pq_adtext.h"
#include "misc.h"
#include "basic_struct.h"
extern cmp_h cmp_func;
int test_pq_clone(pri_quep quep,char* argv, buider_h buildq) {
	int i=0,cnt=0;
	stack* st;
	pri_quep new_queue;
	cnt=read_pq_file((void*)quep,argv,&buildq,1);
	printf("queue item cnts %d\n",cnt);
	printf("printf qeueu\n");
	pq_iterate(quep,pq_printf_functor_def.funcb,pq_printf_functor_def.argc);
	pq_iterate(quep,pq_clone_functor_def.funcb,pq_clone_functor_def.argc,&new_queue);
	
	pq_iterate(new_queue,pq_extractkeys_functor_def.funcb,pq_extractkeys_functor_def.argc,&st);
	printf("\nkey pop from new queue: \n");
	while(!stack_empty(st)) { 
		int *kptr = (int*) stack_pop(st);  
		if(kptr) {   printf("%d ",*kptr); free(kptr); }
	}
	printf("printf new qeueu\n");
	pq_iterate(quep,pq_printf_functor_def.funcb,pq_printf_functor_def.argc);
	return 0;
}
int test_pq_basic(pri_quep quep,char* argv, buider_h buildq) {
	retrun_item* retm; 
	int cnt;
	cnt = read_pq_file((void*)quep,argv,&buildq,1);
	printf("queue item cnts %d\n",cnt);
	pq_iterate(quep,pq_printf_functor_def.funcb,pq_printf_functor_def.argc);
	
	printf("test-delete!\n");
	while(!pq_isempty(quep)) {
		 retm = pq_delete(quep);
		 if(retm) {
		 	printf("delet:  key %d, data %d\n",retm->key,*(int*)retm->ele); free(retm->ele);  free(retm);}
		 else 
		 	printf("delet return NULL\n");	 		
	}
	printf("re-insert\n");
	read_pq_file((void*)quep,argv,&buildq,1);
	pq_iterate(quep,pq_printf_functor_def.funcb,pq_printf_functor_def.argc);
	return 0;
}
int test_pq_del(pri_quep quep,char* argv, buider_h buildq) { 
	int i=0,cnt=0,k=1,nk;
	int* keypool= calloc(1,sizeof(int)*cnt);
	stack* st;
	cnt=read_pq_file((void*)quep,argv,&buildq,1);
	printf("printf qeueu\n");
	pq_iterate(quep,pq_printf_functor_def.funcb,pq_printf_functor_def.argc);
	pq_iterate(quep,pq_extractkeys_functor_def.funcb,pq_extractkeys_functor_def.argc,&st);
	printf("\nkey pop: \n");
	while(!stack_empty(st)) { 
		int *kptr = (int*) stack_pop(st);  
		if(kptr) {  keypool[i++] =*kptr; printf("%d ",*kptr); free(kptr); }
	}
	k=6;
	printf("\nqeueu del key: %d \n", k);
	pq_iterate_find(quep,pq_delkey_functor_def.funcb,k,pq_delkey_functor_def.argc);
	
	printf("\nprintf qeueu\n");
	pq_iterate(quep,pq_printf_functor_def.funcb,pq_printf_functor_def.argc);
	
	pq_iterate(quep,pq_extractkeys_functor_def.funcb,pq_extractkeys_functor_def.argc,&st);
	while(!stack_empty(st)) { 
		int *kptr = (int*) stack_pop(st);  
		if(kptr) {  printf("%d ",*kptr); free(kptr); }
	}
	
	k=3, nk=23;
	printf("\nqueue change key %d  to new-key %d\n",k, nk);
	pq_iterate_find(quep,pq_chgkey_functor_def.funcb,k,pq_chgkey_functor_def.argc,&nk);
	printf("\nprintf qeueu\n");
	pq_iterate(quep,pq_printf_functor_def.funcb,pq_printf_functor_def.argc);
	return 0;
}

int test_pq_key_modify(pri_quep quep,char* argv, buider_h buildq) {
	int i=0,cnt=0,k=1,nk=-1,alt=0,tk=0;
	stack* st;
	srand(time(NULL));
	cnt=read_pq_file((void*)quep,argv,&buildq,1);
	int* keypool= calloc(1,sizeof(int)*cnt);
	printf("queue item cnts %d\n",cnt);
 	printf("printf qeueu\n");
	pq_iterate(quep,pq_printf_functor_def.funcb,pq_printf_functor_def.argc);
	pq_iterate(quep,pq_extractkeys_functor_def.funcb,pq_extractkeys_functor_def.argc,&st);
	printf("\nkey pop: \n");
	while(!stack_empty(st)) { 
		int *kptr = (int*) stack_pop(st);  
		if(kptr) {  keypool[i++] =*kptr; printf("%d ",*kptr); free(kptr); }
	}
	printf("\n");
	k= keypool[rand() % cnt];
	printf("\nqeueu del key: %d \n", k);
	pq_iterate_find(quep,pq_delkey_functor_def.funcb,k,pq_delkey_functor_def.argc);
	
	#if DBG_PRINT==1	
		DBG_BREAK("printf qeueu-break")
	#endif	
	printf("\nprintf qeueu\n");
	pq_iterate(quep,pq_printf_functor_def.funcb,pq_printf_functor_def.argc);
	
	free(keypool);
	cnt--;
	//stack_delete(&st);
	keypool= calloc(1,sizeof(int)*cnt);
	pq_iterate(quep,pq_extractkeys_functor_def.funcb,pq_extractkeys_functor_def.argc,&st);
	printf("\nkey pop: \n");
	i=0;
	while(!stack_empty(st)) { 
		int *kptr = (int*) stack_pop(st);  
		if(kptr) {  keypool[i++] =*kptr; printf("%d ",*kptr); free(kptr); }
	}
	srand(time(NULL));
	do {
		nk= keypool[rand() % cnt];
		if(alt) {alt=0; nk=(nk+tk)/2+tk;}else {alt=1; nk= (tk> nk)? (tk-nk):(nk-tk);} 
		for(i=0;i<cnt;i++) {if(keypool[i]==nk){ tk=nk; nk=-1; break; }}
	}while(nk==-1); // find a non-repeated new-key!
	k= keypool[rand() % cnt];
	printf("\nqueue change key %d  to new-key %d\n",k, nk);
	pq_iterate_find(quep,pq_chgkey_functor_def.funcb,k,pq_chgkey_functor_def.argc,&nk);
	
	printf("\nprintf qeueu\n");
	pq_iterate(quep,pq_printf_functor_def.funcb,pq_printf_functor_def.argc);
	return 0;
}

int main(int argc, char* argv[]) {
	buider_h buildq = (buider_h) build_binomial_pq;
	pri_quep quep = calloc(1,sizeof(pri_que));
	cmp_func = minimum_priority;

	// test_pq_basic(quep,argv[1],buildq);
	 //test_pq_del(quep,argv[1],buildq);
	 test_pq_key_modify(quep,argv[1],buildq);
	 //test_pq_clone(quep,argv[1],buildq);
	return 0;
}
