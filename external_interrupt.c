/*
 * external_interrupt.c
 *
 *  Created on: 14.03.2021
 *      Author: Damian Wójcik
 */

#include "external_interrupt.h"

static inline void clearPosEXTI(size_t exti_index,size_t shift_value)//czyœci pozycjê przed zapisem
{
	uint32_t brush = 0b1111;
	brush <<= shift_value;
	AFIO->EXTICR[exti_index] &= ~brush;
}

static inline uint32_t shiftEXTI(enum ExtiPort port,size_t shift_value)
{
	uint32_t shift = 0;
	shift |= port << ( 4 * shift_value );
	return shift;
}

static size_t getIndexEXTI(size_t pin_number)
{
	size_t exti_index = 0;
	if( ( pin_number >  3 ) && ( pin_number < 8 ) )
		exti_index = 1;
	else if( ( pin_number >  7 ) && ( pin_number < 12 ) )
		exti_index = 2;
	else if( ( pin_number >  11 ) && ( pin_number < 16 ) )
		exti_index = 3;
	return exti_index;
}

static inline void setupEXTI(size_t pin_number,enum ExtiPort port)
{
	size_t exti_index = getIndexEXTI(pin_number);
	size_t shift_value = pin_number - 4 * exti_index;
	clearPosEXTI(exti_index, shift_value);
	uint32_t pin_mask = shiftEXTI(port, shift_value);
	AFIO->EXTICR[exti_index] |= pin_mask;
}

static inline void setupIMR(size_t pin_number)
{
	EXTI->IMR |= 0x1 << pin_number;
}

static void setupEdge(size_t pin_number,enum EdgeType edge)
{
	switch(edge)
	{
		case FallingEdge:
			EXTI->FTSR |= 0x1 << pin_number;
			break;
		case RisingEdge:
			EXTI->RTSR |= 0x1 << pin_number;
			break;
		case Both:
			EXTI->FTSR |= 0x1 << pin_number;
			EXTI->RTSR |= 0x1 << pin_number;
			break;
		default: break;
	}
}

static void enableLowerEXTI(size_t pin_number)
{
	switch(pin_number)
	{
		case 0:
			NVIC_EnableIRQ(EXTI0_IRQn);
			break;
		case 1:
			NVIC_EnableIRQ(EXTI1_IRQn);
			break;
		case 2:
			NVIC_EnableIRQ(EXTI2_IRQn);
			break;
		case 3:
			NVIC_EnableIRQ(EXTI3_IRQn);
			break;
		case 4:
			NVIC_EnableIRQ(EXTI4_IRQn);
			break;
		default: return;
	}
}


static inline void enableHigherEXTI(size_t pin_number)
{
	if( ( pin_number >= 5 ) && ( pin_number <= 9 ) )
		NVIC_EnableIRQ(EXTI9_5_IRQn);
	else if ( ( pin_number >= 10 ) && ( pin_number <= 15 ) )
		NVIC_EnableIRQ(EXTI15_10_IRQn);
}

static inline void enableIRQ(size_t pin_number)
{
	if(pin_number < 5)
		enableLowerEXTI(pin_number);
	else if ( ( pin_number >= 5 ) && ( pin_number < 16 ) )
		enableHigherEXTI(pin_number);
}

void setupExternalInerrupt(size_t pin_number, enum ExtiPort port,
		enum EdgeType edge)
{
	setupEXTI(pin_number, port);
	setupIMR(pin_number);
	setupEdge(pin_number, edge);
	enableIRQ(pin_number);
}
