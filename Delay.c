
#include "RccConfig.h"
#include "Delay.h"

void TIM6Config (void)
{
	/******************** STEP TO FAOLLOW *******************
	1. Enable Timer clock
	2. Set the prescaler and the ARR
	3. Enable the Timer, and wait for the update Flag to set
	*********************************************************/
	
	// 1. Enable Timer clock
	RCC->APB1ENR |= (1 << 4);		// Enable the timer6 clock
	
	// 2. Set the prescaler and the ARR 168MHz but this line is divided into 2 or 84MHZ so ARR is 84 and we will have 1 micro second
	TIM6->PSC = 84-1;		// 84MHz/84 = 1 MHz ~ 1 us delay
	TIM6->ARR = 0xffff;	// MAX ARR value
	
	// 3. Enable the Timer, and wait for the update Flag to set
	TIM6->CR1 |= (1 << 0);	// Enable the Counter
	while (!(TIM6->SR & (1 << 0)));	// UIF: Update interrupt flag.. This bit is set by hardware when the registers are update
}

void Delay_us (uint16_t us)
{
	/******************** STEP TO FAOLLOW *******************
	1. RESET the Counter
	2. Wait for the Counter to reach the entered value as each count will take 1 us,
			the total waiting time will be the requires us delay
	*********************************************************/
	TIM6->CNT = 0;
	while (TIM6->CNT < us);
}

void Delay_ms (uint16_t ms)
{
	for(uint16_t i = 0; i < ms; i++)
	{
		Delay_us(1000);	// delay of 1 ms
	}
}