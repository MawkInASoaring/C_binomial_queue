/* C_binomial_queue
 * 
 * Copyright 2022 Hsueh-Jen Yang
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>
#include <errno.h>
#include "pq_adt.h"
#include "c_try_catch.h"
       

cmp_h cmp_func;

int minimum_priority(int a,int b) {return (a<b)? 1: (a==b)? 0 : -1; }
int maximum_priority(int a,int b) {return (a<b)? -1: (a==b)? 0 : 1; }

static priq_nodep merge_list(priq_nodep a_nodep, priq_nodep b_nodep);
static priq_nodep pairing(priq_nodep a_nodep, priq_nodep b_nodep);
static priq_nodep pq_cutfirst(pri_quep quep);

static priq_nodep merge_list(priq_nodep a_nodep, priq_nodep b_nodep) {
	
	priq_nodep tmpa, tmpb, tmpc =NULL ,tmpa_nxt=NULL, tmpa_pre=NULL, tmpb_nxt=NULL, tmpb_pre=NULL;
		//if(a_nodep==NULL&& b_nodep==NULL) return NULL;
	if(a_nodep==NULL) return b_nodep;
	if(b_nodep==NULL) return a_nodep;
	tmpa = a_nodep; tmpb= b_nodep;	 
	
	while(tmpa!=NULL && tmpb !=NULL) {
		tmpa_nxt = tmpa->next_sib;
		tmpa_pre = tmpa->pre_sib;
		tmpb_nxt = tmpb->next_sib;
		tmpb_pre = tmpb->pre_sib;
		if(tmpa->order < tmpb->order) {
			if(tmpb_pre) tmpb_pre->next_sib = tmpa;
			tmpa->next_sib= tmpb; 
			/*handle pre-link*/if(tmpb_pre) tmpa->pre_sib = tmpb_pre;
			/*handle pre-link*/tmpb->pre_sib = tmpa; 
			tmpa = tmpa_nxt; 
			/*handle pre-link*/ if(tmpa) tmpa->pre_sib = NULL;	
			if(tmpc==NULL) tmpc=a_nodep;
		}else {
			if(tmpa_pre) tmpa_pre->next_sib = tmpb;
			tmpb->next_sib= tmpa; 
			/*handle pre-link*/if(tmpa_pre) tmpb->pre_sib = tmpa_pre;
			/*handle pre-link*/tmpa->pre_sib = tmpb; 
			tmpb = tmpb_nxt; 
			/*handle pre-link*/ if(tmpb) tmpb->pre_sib = NULL;
			if(tmpc==NULL) tmpc=b_nodep;			
		}
	}
#if DBG_PRINT==1
	DBG_LIST(tmpc,"merge_list")
#endif	
	return tmpc;
}

static priq_nodep pairing(priq_nodep a_nodep, priq_nodep b_nodep) {
	
	if(cmp_func(a_nodep->key,b_nodep->key) < 0) { // a less priority  than b
		a_nodep->parent = b_nodep; a_nodep->next_sib = b_nodep->first_child;
		/*handle pre-link*/
		if(b_nodep->first_child) b_nodep->first_child->pre_sib = a_nodep;
		a_nodep->pre_sib =NULL; 
		b_nodep->first_child = a_nodep;  b_nodep->order++; 
		return b_nodep;
	}else {
		b_nodep->parent = a_nodep; b_nodep->next_sib = a_nodep->first_child;
		/*handle pre-link*/
		if(a_nodep->first_child) a_nodep->first_child->pre_sib = b_nodep;
		b_nodep->pre_sib =NULL;
		a_nodep->first_child = b_nodep; a_nodep->order++;
		return a_nodep;	
	}
	//return NULL; //
}

pri_quep pq_join(pri_quep a_quep,pri_quep b_quep) {
	priq_nodep xcur, xnxt,xpre =NULL;
	#if DBG_PRINT==1
	DBG_LIST(a_quep->hnodep,"before join que-a-level-0")
	DBG_LIST(b_quep->hnodep,"before join que-b-level-0")
	#endif
	if(a_quep->hnodep==NULL) { a_quep->hnodep=b_quep->hnodep; free(b_quep); b_quep=NULL; return a_quep;}
	if(b_quep->hnodep==NULL) { return a_quep;}
	
    a_quep->hnodep = xcur = merge_list(a_quep->hnodep, b_quep->hnodep);
    xnxt = xcur->next_sib;
    while(xnxt !=NULL) {
    	if(xcur->order < xnxt->order || 
    	   (xnxt->next_sib!=NULL && xnxt->next_sib->order==xcur->order)) {
    	   	xpre = xcur; xcur= xnxt;
    	} else if (cmp_func(xnxt->key,xcur->key) < 0) { //nxt is less priority than cur
    		xcur->next_sib = xnxt->next_sib;
    		   /*handle pre-link*/ if(xnxt->next_sib) xnxt->next_sib->pre_sib = xcur ;
    		   //xnxt->pre_sib = NULL; //assign null in the function : pairing.
    		xcur = pairing(xcur,xnxt);
    	} else {
    		if(xpre==NULL) {
    			a_quep->hnodep = xnxt;
    			/*handle pre-link*/ xnxt->pre_sib = NULL;
    		}else   { 
    			xpre->next_sib = xnxt;  
    			/*handle pre-link*/ xnxt->pre_sib = xpre;
    		}
    		//xcur->pre_sib =NULL;  //assign null in the function : pairing.
    		xcur = pairing(xcur,xnxt);
    	}
    	xnxt = xcur->next_sib;
    }	
    free(b_quep); b_quep=NULL;
    return a_quep;
}

priq_nodep pq_insert(pri_quep quep,int key,void* ele) {
	
	priq_nodep existp, nextp, curp, newp = (priq_nodep) calloc(1,sizeof(priq_node));
	if(newp==NULL) return NULL;
	newp->ele =ele; newp->key = key;
	if(quep->hnodep==NULL) { quep->hnodep=newp; return newp;}
	if(pq_getnode(quep, key)!=NULL) { printf("duplicated key for binomial heap?, this will ignore the insert op!\n"); return NULL;}
	existp = quep->hnodep; newp->next_sib = existp; 
	/*handle pre-link*/ existp->pre_sib = newp;
	curp = newp;
	 
	do{
		nextp = existp->next_sib;
		if(existp->order==curp->order) curp= pairing(existp,curp); 
		else break;
		existp = nextp;
		curp->next_sib =  existp;
		/*handle pre-link*/ if(existp) existp->pre_sib = curp;
	}while(existp);
	
	quep->hnodep = curp;
	
	return newp;
}


static priq_nodep pq_cutfirst(pri_quep quep) { // cut the first priority.
	
	priq_nodep currp, topp, prep=NULL; // topp: top-priority
	if(quep==NULL) return NULL;
	currp=topp= quep->hnodep;
	if(currp==NULL) return NULL;
	while(currp->next_sib) {
		if(cmp_func(topp->key,currp->next_sib->key) < 0) {//top_prip is less priority than currp->next_sib 
			prep = currp;
			topp = currp->next_sib;
		}
		currp = currp->next_sib;
	}
	if(prep)prep->next_sib = topp->next_sib; 
	 /*hanle pre-link*/  if(topp->next_sib) topp->next_sib->pre_sib = prep;
	 if(topp==quep->hnodep) quep->hnodep= topp->next_sib;
	  /*hanle pre-link*/if(quep->hnodep) quep->hnodep->pre_sib =NULL;
	 topp->next_sib = topp->pre_sib = NULL;
	 return topp;
}

retrun_item* pq_delete(pri_quep quep) {
	retrun_item* ret=NULL;
	pri_quep tquep;
	priq_nodep topp, currp, prep=NULL, nxtp=NULL;
	
	TRY {	
	if(quep==NULL || quep->hnodep==NULL) THROW(1);
	topp = pq_cutfirst(quep);
	currp = topp->first_child;
	// reverse-link-order (change the order from decreasing to increasing...)
	if(currp) {
		topp->first_child = NULL; 
		nxtp = currp->next_sib; 
		while(nxtp) {
			currp->next_sib = prep; currp->parent=NULL;
			/*handle pre-link*/ if(prep) prep->pre_sib = currp; 
			prep = currp;
			currp = nxtp; nxtp= nxtp->next_sib;
		}
		//handle the last sib
		currp->next_sib = prep; currp->parent=NULL;
		/*handle pre-link*/ if(prep) prep->pre_sib = currp; 
		/*handle pre-link*/ currp->pre_sib=NULL; 
		
		tquep = (pri_quep)calloc(1,sizeof(pri_que)); 
		if(tquep==NULL) THROW(2);
		tquep->hnodep = currp;
		#if DBG_PRINT==1
		DBG_LIST(currp,"reverse-link")
		#endif	
		quep = pq_join(quep,tquep); //  tquep will be free in pq_join...
		#if DBG_PRINT==1
		DBG_LIST(quep->hnodep,"after join level-0")
		if(quep->hnodep)
		DBG_LIST(quep->hnodep->first_child,"after join level-1")
		#endif
	}
	//free top_prip
	ret = calloc(1,sizeof(retrun_item)); if(ret==NULL)  THROW(2);
	ret->ele = topp->ele; ret->key = topp->key;
	
	} CATCH (1){
		printf("Err: Null or empty input queue\n");
		return NULL;
	} CATCH (2) {
		printf("Err: Null allocate\n");
		if(tquep) free(tquep); 
	} FINALLY {
		if(topp) free(topp);
	} ETRY;
	return ret;
}

int pq_isempty(pri_quep quep) {
	return ((quep==NULL)|| (quep->hnodep==NULL));
}

priq_nodep pq_getnode(pri_quep quep,int key) {
	int cmpflag;
	priq_nodep curp=NULL, prep=NULL, nxtp=NULL;
	if(pq_isempty(quep)) return NULL;
	curp = quep->hnodep;
	prep= NULL;
	while(curp) { 
		cmpflag = cmp_func(key,curp->key);
		
		switch(cmpflag) {
				 case -1: prep = (curp->next_sib)? curp->next_sib: (curp->parent)? curp->parent->next_sib:NULL; 
				 		  curp = curp->first_child; 
				 break;
				 case  1: curp = curp->next_sib;		
				 break;
				 case  0:  
				 	return curp;
				 	curp=NULL; prep=NULL;
				 break;
				 default: curp=NULL; prep=NULL; printf("err condition\n"); return NULL;
				 break;
		}
		if(curp==NULL) {
			curp=prep; prep=(prep && prep->parent)? prep->parent->next_sib:NULL; 
		}
	}
	/*
	while(curp) { 
		cmpflag = cmp_func(key,curp->key); 
		if(cmpflag < 0) { // if the root of the current tree is more priority than the end_key
			nxtp = curp->first_child; prep=NULL;
			do {
				if(nxtp==NULL) { nxtp = prep; prep= (prep && (prep->parent!=curp))? prep->parent->next_sib:NULL;}
				
				switch(cmp_func(key,nxtp->key)) {
				 case -1: prep = (nxtp->next_sib)? nxtp->next_sib: (nxtp->parent!=curp)? nxtp->parent->next_sib:NULL; 
				 		  nxtp = nxtp->first_child; 
				 break;
				 case  1: nxtp = nxtp->next_sib;		
				 break;
				 case  0:  
					return nxtp;
				 break;
				 default: nxtp=NULL; prep=NULL; printf("err condition\n"); return NULL;
				}
			}while(nxtp || prep);
		}else if(cmpflag==0) {
			return curp;
		}	
		curp = curp->next_sib;
	}*/
	
	return NULL;
} 
void pq_clear(pri_quep quep) {
 	while(!pq_isempty(quep))
		pq_delete(quep);
}

void pq_delete_queue(pri_quep* quep_ptr) {
	if(quep_ptr) {
		pq_clear(*quep_ptr);
		free(*quep_ptr);
	}	
}

//3-bit (analog bit adding) join method.
//3-bits***: carry-bit, existed-bit, new-bit.
//case 001 : direct insert and return;
//case 010 : return; (insert has been done properly)
//case 011 : pairing and insert pairing result to "carry" position. continue loop.
//case 100 : previous pairing has been inserted properly, return;
//-----------------------------------rest only occured in queues joining. 
//case 101 : pairing. 
#define bit_test(c,b,a) ((c)<<2)+((b)<<1)+(a)
