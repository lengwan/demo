#ifndef __MYKEY_H
#define __MYKEY_H	 
#include "sys.h"
#define KEY0 PEin(4)
#define KEY1 PEin(3)
void KEY_Init(void);

u8 KEY_Scan(u8 mode);
	 				    
#endif
