#ifndef MISC_H
#define MISC_H

#include <stdio.h>

#define DBG_BREAK(str) { char ch; printf("\nDBG...%s\n",(str)); fgetc(stdin);  while((ch=getchar())!='\n' && ch!= EOF); }

#endif
