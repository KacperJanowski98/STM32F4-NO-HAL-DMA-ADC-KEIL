
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

void DMA_Config (uint32_t srcAdd, uint32_t destAdd, uint16_t size)
{
	DMA2_Stream0->NDTR = size;		// Set the size of the transfer
	
	DMA2_Stream0->PAR = srcAdd;		// Source address is peripheral address
	
	DMA2_Stream0->M0AR = destAdd;		// Destination Address is memory address
	
	// Enable the DMA Stream
	DMA2_Stream0->CR |= (1<<0);		// EN = 1
}

uint16_t RxData[3];
float Temperature;

int main ()
{
	SysClockConfig ();
	TIM6Config ();
	
	ADC_Init ();
	DMA_Init ();
	
	DMA_Config ((uint32_t) &ADC1->DR, (uint32_t) RxData, 2);
	
	ADC_Start ();

	while(1)
	{
		Temperature = (((float)(3.3*RxData[2]/(float)4095) - 0.76) / 0.0025) + 25;
	}
}




