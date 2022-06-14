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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>
#include <errno.h>
#include "pq_help.h"
#include "pq_adt.h"

void build_binomial_pq(int reset, void* queue_addr,int key_id, int data_value) {
   static pri_quep quep;
   retrun_item* tmp=NULL;
   if(reset) {
    	quep = ((pri_quep)queue_addr);
    	if( ((void*)quep)==NULL) { 
    	    fprintf(stderr,"\ngraph addres %p error, exit\n",(void*)queue_addr);
    	    exit(EXIT_FAILURE);
   	 	}   
    	while(!pq_isempty(quep)) {
        	if((tmp = pq_delete(quep))!=NULL)   {	
        		printf("clear key %d, data %p\n",tmp->key,tmp->ele);
        		free(tmp);
        	}
        }
        return;
    }
   int* ele = calloc(1,sizeof(int)); *ele = data_value;
   pq_insert(quep,key_id, ele);
}

int read_pq_file(void* queue_addr, char* argv_path, buider_h buiders[], int builder_num) {
  
  char* line = NULL;
  size_t sz = 0, pq_cnt=0;// tnode_cnt=0;
  ssize_t ln;
  int i;
  int que_cnt = 0 ;
  intmax_t key_id, data_value; 
  
  const char token[3] = " ";
  char *take_str;

  FILE* fd =fopen(argv_path,"r");
  if(fd==NULL) {
    fprintf(stderr,"\nOpen file %s error, exit\n",argv_path);
    exit(EXIT_FAILURE);
  }
  for(i=0;i<builder_num;i++) {
         buiders[i](1,(void*)queue_addr,0,0); // 1= reset.
  }
  
   while((ln = getline(&line, &sz, fd)) !=-1) {
       take_str=strtok(line,token);
        
       if(take_str) {
            key_id = strtoimax(take_str, NULL,10);
            printf("\nkey %ld",key_id); que_cnt++;
            if(key_id== INTMAX_MIN || key_id==INTMAX_MAX || key_id==0) {
                fprintf(stderr, "\nFile parse of (key_id) %ld str %s with err-info %s\n",key_id,take_str,strerror(errno));
                exit(EXIT_FAILURE); 
            } 

            while(take_str!=NULL) {
               take_str=strtok(NULL,token);
                 if(take_str){
                    data_value = strtoimax(take_str, NULL,10);
                    if(data_value== INTMAX_MIN || data_value==INTMAX_MAX || data_value==0) {
                          fprintf(stderr, "\nFile parse of (data_value) %ld str %s with err-info %s\n",data_value,take_str,strerror(errno));
                          exit(EXIT_FAILURE); 
                    } 
                    for(i=0;i<builder_num;i++) {
                        buiders[i](0,NULL,key_id,data_value);
                    }
                 }// if-parsing next string of current line success.  
            }//while
           pq_cnt++;   
       }// if-parsing first string of line success
  }//iterate over the file.
   printf("\nend file parsing-success %ld\n",pq_cnt);
  printf("\n"); 
  fclose(fd);
  if(line) 
    free(line);
  return que_cnt;
}

