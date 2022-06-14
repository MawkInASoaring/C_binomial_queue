#ifndef _PQ_ADT_EXT_H
#define _PQ_ADT_EXT_H

#include <stdarg.h> // /* header for variable argument list */
#include "pq_adt.h"
#include "basic_struct.h"
typedef int (*que_iterater_cb)(pri_quep, int, size_t, priq_nodep*, void**);

priq_nodep pq_iterate(pri_quep quep,que_iterater_cb funcp, size_t fun_argc,...);

priq_nodep pq_iterate_find(pri_quep quep,que_iterater_cb funcp,int end_key, size_t fun_argc,...);

int pq_chgkey_handle(pri_quep quep, int setflg, size_t argc, priq_nodep* ret_nodep, void** var_args);//  (ret_nodep,int* newkey ,int* oldkey)

int pq_delkey_handle(pri_quep quep, int setflg, size_t argc, priq_nodep* ret_nodep, void** var_args); // (int* key)

int pq_clone_handle(pri_quep quep, int setflg, size_t argc, priq_nodep* ret_nodep, void** var_args); // 
// (quep, source queue), (priq_nodep, a new que->hnodep)
//[XXX] use =container_of(&ret_nodep ,struct priq_nodep, hnodep) to access the associate new pri_quep pointer.
int pq_printf_handle(pri_quep quep, int setflg, size_t argc, priq_nodep* ret_nodep, void** var_args);

int pq_extract_keys_handle(pri_quep quep, int setflg, size_t argc, priq_nodep* ret_nodep, void** var_args);// (stack**)

typedef struct _pq_functor {
	que_iterater_cb funcb;
	int argc;
}pq_functor;

extern const pq_functor pq_chgkey_functor_def;
extern const pq_functor pq_delkey_functor_def;
extern const pq_functor pq_printf_functor_def;
extern const pq_functor pq_clone_functor_def;
extern const pq_functor pq_extractkeys_functor_def;
#endif
