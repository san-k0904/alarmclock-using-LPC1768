#ifndef interrupt_h
#define interrupt_h
#include <LPC17xx.h>
#include <stdlib.h>
void interrupt(){
	LPC_PINCON->PINSEL1&=0XFFFFF7FF;
	LPC_GPIO0->FIODIR=0X0<<21;
	
	
}
#endif
