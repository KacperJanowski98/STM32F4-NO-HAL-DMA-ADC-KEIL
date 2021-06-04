
#include "RccConfig.h"
#include "adc.h"

void ADC_init (void)
{
	/************** STEPS TO FOLLOW *******************
	1. Enable ADC and GPIO clock
	2. Set the prescalar in the Common Control Register (CCR)
	3. Set the Scan Mode and Resolution in the Control Register 1 (CR1)
	4. Set the Continuous Conversion, EOC, and Data Alignment in Control Reg 2 (CR2)
	5. Set the Sampling Time for the channels in ADC_SMPRx
	6. Set the Regular channel sequence length in ADC_SQR1
	7. Set the Respective GPIO PINs in the Analog Mode
	**************************************************/
	
	// 1. Enable ADC and GPIO clock
	RCC->APB2ENR |= (1 << 8); // 187s
	RCC->AHB1ENR |= (1 << 0); // 189s
	
	// 2. Set the prescalar in the Common Control Register (CCR)
	ADC->CCR |= (1 << 16);	// PCLK2 divided by 4 (427s)
	
	// 3. Set the Scan Mode and Resolution in the Control Register 1 (CR1) (416s)
	ADC1->CR1 |= (1 << 8);	// SCAN mode enabled
	ADC1->CR1 &= ~(1 << 24);	// 12 bit RES
	
	// 4. Set the Continuous Conversion, EOC, and Data Alignment in Control Reg 2 (CR2) (418s)
	ADC1->CR2 |= (1 << 1);	// Enable continuous conversion mode
	ADC1->CR2 |= (1 << 10);	// EOC after each conversion
	ADC1->CR2 |= (1 << 11);	// dATA Alignment RIGHT
	
	// 5. Set the Sampling Time for the channels in ADC_SMPRx (421s)
	ADC1->SMPR2 &= ~((1 << 3) | (1 << 12));	// Sampling time of 3 cycles for channel 1 and channel 4
	
	// 6. Set the Regular channel sequence length in ADC_SQR1 (422s)
	ADC1->SQR1 |= (1 << 20);	// SQR1_L = 1 for 2 conversion
	
	// 7. Set the Respective GPIO PINs in the Analog Mode (281s)
	GPIOA->MODER |= (3 << 2);	// analog mode for PA 1
	GPIOA->MODER |= (3 << 8);	// analog mode for PA 4
}

void ADC_Enable (void)
{
	/******** STEPS FOLLOW ***********
	1. Enable the ADC by setting ADON bit in CR2
	2. Wait for ADC to stabilize (approx 10us)
	**********************************/
	ADC1->CR2 |= (1 << 0);	// ADON = 1 enable ADC1 (420s)
	
	uint32_t delay = 10000;
	while (delay--);
}

void ADC_Start (int channel)
{
	/********** STPES FOLLOW *************
	1. Set the channel Sequence in the SQR Register
	2. Clear the Status register
	3. Start the Conversion by Setting the SWSTART bit in CR2
	*************************************/
	
	/* Since we will be polling for each channel, here we will keep one channel in the sequence at a time
		 ADC -> SQR3 |= (channel << 0); will just keep the respective channel in the sequence for the confersion */
	
	ADC1->SQR3 = 0;
	ADC1->SQR3 |= (channel << 0);		// conversion in regular sequence (423s)
	
	ADC1->SR = 0;			// clear the status register (415s)
	
	ADC1->CR2 |= (1 << 30);		// start the conversion (418s)
}