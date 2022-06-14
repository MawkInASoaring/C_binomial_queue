#ifndef _PQ_HELP_H
#define _PQ_HELP_H

typedef void (*buider_h)(int reset,void* queue_addr,int key_id, int data_value,...);
void build_binomial_pq(int reset, void* queue_addr,int key_id, int data_value);
int read_pq_file(void* queue_addr, char* argv_path, buider_h buiders[], int builder_num);

#endif
