#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>
#include <errno.h>
#include "pq_adtext.h"
#include "c_try_catch.h"
#include "misc.h"
extern cmp_h cmp_func;
static void position_chg(priq_nodep parentp, priq_nodep childp);

const pq_functor pq_chgkey_functor_def= {(que_iterater_cb)pq_chgkey_handle, 2};
const pq_functor pq_delkey_functor_def={(que_iterater_cb)pq_delkey_handle, 1};
const pq_functor pq_printf_functor_def={(que_iterater_cb)pq_printf_handle, 0};
const pq_functor  pq_clone_functor_def={(que_iterater_cb)pq_clone_handle, 1};
const pq_functor  pq_extractkeys_functor_def={(que_iterater_cb)pq_extract_keys_handle, 1};
//[TODO]
//priq_nodep pq_getnode(pri_quep quep,int key)

//[TODO]
//priq_nodep pq_chgkey(pri_quep quep, int oldkey, int newkey) ;

//[TODO]
//priq_nodep pq_delkey(pri_quep quep, int key) ;

//[FIXME]
static void position_chg(priq_nodep rootp, priq_nodep childp) {
	priq_nodep tmp=NULL, tmpchild=NULL, tmproot=NULL, tmprootchild=NULL, tmp1stchid, tmpr1stchild; 
	int tmpi=-1;
	if(rootp==NULL|| childp==NULL)return;
	
	
	// change parent, and handle if any are their parent's first_child (by case!!)
	tmp = childp->parent; 
    if(tmp) tmpchild = tmp->first_child;
	tmproot = rootp->parent;
	if(tmproot) tmprootchild = tmproot->first_child;
	if(tmprootchild==rootp)
		tmproot->first_child = childp;
			
	tmp1stchid = childp->first_child;
	tmpr1stchild = rootp->first_child;
	
	rootp->first_child =  tmp1stchid;
	while(tmp1stchid) {
		tmp1stchid->parent = rootp;
		tmp1stchid = tmp1stchid->next_sib;
	}
	
	// change their first child , and change where all their children's parent pointers.
	if(childp->parent!=rootp) {
		rootp->parent = tmp;
		if(tmpchild==childp)
		 tmp->first_child = rootp;
		
		childp->first_child = tmpr1stchild;
		while(tmpr1stchild) {
			tmpr1stchild->parent = childp;
			tmpr1stchild= tmpr1stchild->next_sib;
		} 
	} else {
		rootp->parent = childp;
		if(tmpchild==childp)  {
			//  tmpchild  is  original childp->parent->frist_child
			//  tmpr1stchild  is original rootp->first_child,  
			//  now, since childp->parent == rootp ,  tmpr1stchild == tmpchild... 
			childp->first_child = rootp;
			tmpr1stchild= tmpr1stchild->next_sib; // skip childp itself..
		} else	{
			childp->first_child = tmpr1stchild;
		}
		while(tmpr1stchild) {
			tmpr1stchild->parent = childp;	
			do {  tmpr1stchild= tmpr1stchild->next_sib;
			}while((tmpr1stchild && tmpr1stchild==childp));	// skip childp itself..childp->parent ==rootp, but not its first child...
		}
	}
	childp->parent=tmproot;

	// change next_sib
	tmp= childp->next_sib;
	tmproot=rootp->next_sib;
	childp->next_sib = tmproot;
	if(tmproot) tmproot->pre_sib = childp;
	rootp->next_sib = tmp;
	if(tmp) tmp->pre_sib = rootp;
	 
	 // change pre_sib
	tmp = childp->pre_sib;
	tmproot=rootp->pre_sib;
	childp->pre_sib =tmproot;
	if(tmproot) tmproot->next_sib = childp;
	rootp->pre_sib = tmp;
	if(tmp) tmp->next_sib = rootp;
	
	tmpi = childp->order;
	childp->order = rootp->order;
	rootp->order = tmpi;	
}

//[FIXME]
int pq_chgkey_handle(pri_quep quep, int setflg, size_t argc, priq_nodep* ret_nodep, void** var_args) {
	static priq_nodep targp, curp, prep, nxtp, tmpp;
	static int endkey= -1;
	static int newkey=-1;
	int tmpkey;
	
	if(setflg==1)  {
		if(argc < 2) return -1;
		endkey= *(int*)var_args[0];
		newkey= *(int*)var_args[1];
		targp = curp = prep = nxtp = tmpp = NULL;
		return ++setflg;
	}	
	if(setflg>1) {
		curp = targp = (priq_nodep) var_args[argc];
		if( endkey<0 || newkey <0 ) return -1; //an illegal function call.
		if(cmp_func(endkey,targp->key)!=0) {printf("error input node(var_args[argc]) %p\n", &(var_args[argc])); return -1;}
		
		if(cmp_func(newkey,endkey)<0) {	// decrease priority		  
		 nxtp = curp->first_child;
		 while(nxtp) {
		   	   tmpp = nxtp; tmpkey = nxtp->key;
				while(nxtp->next_sib) {
					if(cmp_func(tmpkey,nxtp->next_sib->key)<0 ) {
						tmpp = nxtp->next_sib; tmpkey = tmpp->key;
					}
					nxtp=nxtp->next_sib;
				}	
				if(cmp_func(newkey,tmpkey)<0) {
					if(targp==quep->hnodep) quep->hnodep = tmpp; 
		    		position_chg(targp, tmpp);
		    		nxtp = targp->first_child;
		    	}else 
		       		 break;				
		  }
		  
		} else if (cmp_func(newkey,endkey)>0){ // increase priority	
		 	nxtp = curp->parent;
		 	while(nxtp){
		    	tmpp = nxtp; tmpkey = tmpp->key;
		    	nxtp = tmpp->parent;			
				if(cmp_func(tmpkey,newkey)<0 ) {
					if(tmpp==quep->hnodep) quep->hnodep = targp;
					position_chg(tmpp,targp);
				}	
			}
		 		
		}// if-else
		targp->key= newkey;
		*ret_nodep= targp;
		return setflg; // success...
 	}
 	*ret_nodep=NULL;		
	return 0;
}

//[FIXME] // error handling
int pq_delkey_handle(pri_quep quep, int setflg, size_t argc, priq_nodep* ret_nodep, void** var_args) {
	static priq_nodep  targp, curp, nxtp, prep, victp, tmpp; // victim
    static int end_key;
	pri_quep tquep;
	if(setflg==1)  {
		if(argc < 1) return -1;
		end_key = *(int*)var_args[0];
		prep =  curp = nxtp = NULL; 
		return ++setflg;
	}
	if(setflg>1){
	
		if(argc < 1) return -1;
		victp = targp = (priq_nodep) var_args[argc]; 
		if(cmp_func(end_key,targp->key)!=0) {printf("error input node(var_args[argc]) %p\n", &(var_args[argc])); return -1;}
		
		while(victp->parent!=NULL) victp=victp->parent;
		
		if(victp!=targp) {
			position_chg(victp,targp);
		}
		//now targp is at root-list,link both side of the cut-out (targp).
		if(targp==quep->hnodep) quep->hnodep=targp->next_sib;
		if(targp->next_sib)targp->next_sib->pre_sib = targp->pre_sib;
		if(targp->pre_sib) targp->pre_sib->next_sib = targp->next_sib;
		targp->next_sib = targp->pre_sib = NULL; // cut-out.
		
		curp = targp->first_child;
		// reverse-link-order (change the order from decreasing to increasing...)
		if(curp) {
			targp->first_child = NULL;
			nxtp = curp->next_sib;
			while(nxtp) {
				curp->next_sib = prep; curp->parent=NULL;
				/*handle pre-link*/ if(prep) prep->pre_sib = curp; 
				prep = curp;
				curp = nxtp; nxtp= nxtp->next_sib;
			}
			//handle the last sib
			 curp->next_sib = prep; curp->parent=NULL;
			/*handle pre-link*/ if(prep) prep->pre_sib = curp; 
			/*handle pre-link*/ curp->pre_sib=NULL;
		
			tmpp = curp; //
		
			if(victp!=targp) {
		 		//float-up victp, since this branch is cutting-out from original root-list. float-up can only up to the new formed root-list.
		 		curp = prep = NULL;
		 		curp = victp->parent;
		 		while(curp) {		 	 
		         	nxtp = curp->parent;	
			     	if(cmp_func(curp->key,victp->key)< 0) { // serch until visit the node where has priority higher than victp, or to the root-list.
			    		prep = curp; 
			    		if(tmpp==prep) tmpp=victp;
			    		position_chg(prep,victp);
			    		curp = nxtp;
			 	 	}else break;		
		    	}
		   	    #if DBG_PRINT==1
				DBG_LIST(victp,"victim-link")
				DBG_BREAK("victim-break")
				#endif	
		 	}
		 	
			tquep = (pri_quep)calloc(1,sizeof(pri_que)); 
			if(tquep==NULL) return -1;
			tquep->hnodep = tmpp;
			quep = pq_join(quep,tquep);
			#if DBG_PRINT==1
				DBG_LIST(quep->hnodep,"victim-link, after rejoin")
				DBG_BREAK("pq_delkey_handle-break")
		    #endif	
	  	 }
	  *ret_nodep = targp;
	  return setflg;
  }
  *ret_nodep=NULL;   
  return 0;
}

//[FIXME]
int pq_clone_handle(pri_quep quep, int setflg, size_t argc, priq_nodep* ret_nodep, void** var_args) {
	static priq_nodep  curp;
	static pri_quep new_quep;
	if(setflg==1) {
		if(argc < 1 ) return -1;
		if(new_quep) {
			pq_delete_queue(&new_quep);
		}
		new_quep = (pri_quep) calloc(1,sizeof(pri_que));
		*(pri_quep*)(var_args[0]) = new_quep;
		curp=NULL;
		return ++setflg;	
	}
	if(setflg>1){
		if(argc < 1 ) return -1;
		curp = (priq_nodep) var_args[argc];
		//newp = calloc(1,sizeof(priq_node));
		if(curp==NULL) return -1;		
		*ret_nodep = pq_insert(new_quep,curp->key,curp->ele);//pq_getnode(new_quep,curp->key);
		return setflg; // success...
	}
	return 0;
}
int pq_printf_handle(pri_quep quep, int setflg, size_t argc, priq_nodep* ret_nodep, void** var_args) {
	static priq_nodep  curp;
	
	if(setflg==1) {
		curp=NULL;
		return ++setflg;	
	}
	if(setflg>1){
		*ret_nodep = curp = (priq_nodep) var_args[argc];
		//newp = calloc(1,sizeof(priq_node));
		if(curp==NULL) return -1;		
		printf("key: %d, data: %d\n",curp->key,*(int*)curp->ele);
		return setflg; // success...
	}
	return 0;
}

int pq_extract_keys_handle(pri_quep quep, int setflg, size_t argc, priq_nodep* ret_nodep, void** var_args) {
	static stack* st;
	static priq_nodep  curp; int* ele;
	if(setflg==1) {
		if(argc < 1 ) return -1;
		stack_delete(&st);
		stack_init(&st);
		*(stack**)(var_args[0]) = st; // the only way.
		curp=NULL;
		return ++setflg;	
	}
	if(setflg >1) {
		*ret_nodep = curp = (priq_nodep) var_args[argc];
		if(curp==NULL) return -1;
		ele= calloc(1,sizeof(int)); *ele=curp->key;
		stack_push(st,(void*)ele);
		return setflg; // success...
	}
	return 0;
}

//[NOTE]
// use variable args. :  get the 
// funcp : input priq_nodep, 
//	     : return end/continue iteration flag.
// print with priority order...shall use another implmentation: clone and delete...
//[FIXME] //[XXX]
priq_nodep pq_iterate_find(pri_quep quep,que_iterater_cb funcp,int end_key, size_t fun_argc,...){
	va_list varied_args;
	void** arg_ptrs;
	unsigned int i;
	
	int fstate=1, cmpflag=0;
	
	priq_nodep curp=NULL, prep=NULL,  ret_nodep=NULL; //nxtp
	
	if(pq_isempty(quep)) return NULL;
	curp = quep->hnodep;
	
	arg_ptrs= (void**)calloc((fun_argc+1),sizeof(void*));
	va_start(varied_args, fun_argc);
	arg_ptrs[0]= (void*)&end_key;
	for(i=1;i<fun_argc;i++)
		arg_ptrs[i] = va_arg(varied_args, void*);
	va_end(varied_args);
	TRY {
	
	if((fstate=funcp(NULL,fstate,fun_argc,NULL,arg_ptrs)) < 0) THROW(1);
	
	curp = quep->hnodep;
	prep= NULL;
	while(curp) { 
		cmpflag = cmp_func(end_key,curp->key);
		
		switch(cmpflag) {
				 case -1: prep = (curp->next_sib)? curp->next_sib: (curp->parent)? curp->parent->next_sib:NULL; 
				 		  curp = curp->first_child; 
				 break;
				 case  1: curp = curp->next_sib;		
				 break;
				 case  0:  
				 	arg_ptrs[fun_argc] = (void*)curp;
				 	if((fstate=funcp(quep,fstate,fun_argc,&ret_nodep,arg_ptrs))<0) THROW(2);
				 	curp=NULL; prep=NULL;
				 break;
				 default: curp=NULL; prep=NULL; printf("err condition\n"); THROW(2);
				 break;
		}
		if(curp==NULL) {
			curp=prep; prep=(prep && prep->parent)? prep->parent->next_sib:NULL; 
		}
	}	

	} CATCH (1){
		printf("operation function initialize failed, end iteration\n");
	} CATCH (2){
		printf("operation error with function state %d, end iteration\n", fstate);
	} FINALLY {
		if(!ret_nodep) printf("un-successful operation, retrun a NULL pointer\n");
		if(arg_ptrs) free(arg_ptrs);
	} 
	ETRY;
	return ret_nodep;
}

priq_nodep pq_iterate(pri_quep quep,que_iterater_cb funcp, size_t fun_argc,...){
	va_list varied_args;
	void** arg_ptrs;
	unsigned int i;
	
	int fstate=1;
	
	priq_nodep curp=NULL, prep=NULL, nxtp=NULL, ret_nodep=NULL;
	
	if(pq_isempty(quep)) return NULL;
	curp = quep->hnodep;
	
	arg_ptrs= (void**)calloc((fun_argc+1),sizeof(void*));
	va_start(varied_args, fun_argc);
	for(i=0;i<fun_argc;i++)
		arg_ptrs[i] = va_arg(varied_args, void*);
	va_end(varied_args);
	TRY {
	
	if((fstate=funcp(NULL,fstate,fun_argc,NULL,arg_ptrs))<0) THROW(1);
	
	curp = quep->hnodep;
	
	while(curp) { 
		arg_ptrs[fun_argc] = (void*)curp;
		if((fstate=funcp(quep,fstate,fun_argc,&ret_nodep,arg_ptrs))<0) THROW(2);
		nxtp = curp->first_child; prep = NULL;
		if(nxtp) {
			do {
				if(nxtp==NULL) {
					nxtp = prep; 
					prep = ( prep && (prep->parent!=curp))? prep->parent->next_sib:NULL;
				}
				arg_ptrs[fun_argc] = (void*)nxtp;	
				if((fstate=funcp(quep,fstate,fun_argc,&ret_nodep,arg_ptrs))<0) THROW(2);
				if(fstate!=0) { 
					if(nxtp->first_child) {
						prep = (nxtp->next_sib)? nxtp->next_sib : ( prep && (prep->parent!=curp))? prep->parent->next_sib:NULL;  
						nxtp = nxtp->first_child; 
					} else 
						nxtp = nxtp->next_sib; 
				}
				else THROW(3);
			 }while (nxtp || prep);
		}	 
		curp = curp->next_sib;
	}
	} CATCH (1){
		printf("operation function initialize failed, end iteration\n");
	} CATCH (2){
		printf("operation error with function state %d, end iteration\n", fstate);
	} CATCH (3){
		printf("stop iteration\n");
	} FINALLY {
		if(!ret_nodep) printf("un-successful operation, retrun a NULL pointer\n");
		if(arg_ptrs) free(arg_ptrs);
	} 
	ETRY;
	return ret_nodep;
}


