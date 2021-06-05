
#include "RccConfig.h"
#include "Delay.h"
#include "adc.h"

void DMA_Init (void)
{
	// Enable the DMA2 Clock
	RCC->AHB1ENR |= (1<<22);	// DMA2EN = 1
	
	// Select the Data Direction
	DMA2_Stream0->CR &= ~(3<<6);	// Peripheral to memory
	
	// Select Circular mode
	DMA2_Stream0->CR |= (1<<8);		// CIRC = 1
	
	// Enable Memoty Adress Increment
	DMA2_Stream0->CR |= (1<<10);		// MINC = 1
	
	// Set the size for data
	DMA2_Stream0->CR |= (1<<11) | (1<<13);	// PSIZE = 01, MSIZE = 01, 16 bit data
	
	// Select channel for the stream
	DMA2_Stream0->CR &= ~(7<<25);		// Channel 0 selected
}

int main ()
{

	while(1)
	{
		
	}
}