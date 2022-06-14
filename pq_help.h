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

#ifndef _PQ_HELP_H
#define _PQ_HELP_H

typedef void (*buider_h)(int reset,void* queue_addr,int key_id, int data_value,...);
void build_binomial_pq(int reset, void* queue_addr,int key_id, int data_value);
int read_pq_file(void* queue_addr, char* argv_path, buider_h buiders[], int builder_num);

#endif
