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
