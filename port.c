/*
 * port.c
 *
 *  Created on: 14.03.2021
 *      Author: Damian Wójcik
 */

#include "port.h"

static uint32_t __shiftBits4Times(uint32_t bitset, size_t pin_number)
{
	if (pin_number >= 8)
		pin_number -= 8;

	return bitset << (pin_number * 4);
}

static void __ORset__(PortType port, size_t pin_number, uint32_t conf)
{
	if (pin_number < 8)// 0-7
		port->CRL |= conf;
	else
		if (pin_number >= 8 && pin_number < 16)
	port->CRH |= conf;// 8-15
}

static void __ANDset__(PortType port, size_t pin_number, uint32_t conf)
{
	if (pin_number < 8)// 0-7
	port->CRL &= conf;
		else
	if (pin_number >= 8 && pin_number < 16)
		port->CRH &= conf;// 8-15
}

void clearPinConfiguration(PortType port, size_t pin_number)
{
	uint32_t conf = 0b1111;
	conf = __shiftBits4Times(conf, pin_number);
	conf = ~conf;
	__ANDset__(port, pin_number, conf);
}

static uint32_t __configureMode(size_t pin_number, enum OutputMode mode)
{
	uint32_t temp_mode = 0x0;
	switch (mode)
	{
		case Output_2MHz:
			temp_mode = 0b0010;
			break;
		case Output_10MHz:
			temp_mode = 0b0001;
			break;
		case Output_50MHz:
			temp_mode = 0b0011;
			break;
	}
	return __shiftBits4Times(temp_mode, pin_number);
}

static uint32_t __configureOutputConf(size_t pin_number,
		enum OutputConfig config)
{
	uint32_t temp_conf = 0x0;
	switch (config)
	{
		case PushPull:
			return temp_conf;
		case AlternatePushPull:
			temp_conf = 0b1000;
			break;
		case OpenDrain:
			temp_conf = 0b0100;
			break;
		case AlternateOpenDrain:
			temp_conf = 0b1100;
			break;
	}
	return __shiftBits4Times(temp_conf, pin_number);
}

static uint32_t __configureInputConf(size_t pin_number, enum InputConfig config)
{
	uint32_t temp_conf = 0x0;
	switch (config)
	{
		case Analog:
			return temp_conf;
		case PullUp:
			temp_conf = 0b1000;
			break;
		case FloatingInput:
			temp_conf = 0b0100;
			break;
		case Reserved:
			temp_conf = 0b1100;
			break;
	}
	return __shiftBits4Times(temp_conf, pin_number);
}

static inline void __configureOutputPin(PortType port,enum OutputMode mode,
						  	  	  	    enum OutputConfig config,
										size_t pin_number)// 0-15
{
	clearPinConfiguration(port, pin_number);
	uint32_t conf = 0x0;
	conf = __configureOutputConf(pin_number, config) |
					__configureMode(pin_number, mode);
	__ORset__(port, pin_number, conf);
}

static inline void __configureInputPin(PortType port,
									   enum InputConfig config,
									   size_t pin_number)
{
	clearPinConfiguration(port, pin_number);
	uint32_t conf = 0x0;
    conf = __configureInputConf(pin_number, config) | 0x0;//input
	__ORset__(port, pin_number, conf);
}

void __configureOutputPins(PortType port, enum OutputMode mode,
								  enum OutputConfig config, size_t pins_count, ...)
{
	va_list list;
	va_start(list, pins_count);
	for (size_t i = 0; i < pins_count; ++i)
		__configureOutputPin(port, mode, config, va_arg(list, size_t));
	va_end(list);
}

void __configureInputPins(PortType port, enum InputConfig config,
		size_t pins_count, ...)
{
	va_list list;
	va_start(list, pins_count);
	for (size_t i = 0; i < pins_count; ++i)
		__configureInputPin(port, config, va_arg(list, size_t));
	va_end(list);
}

void disablePin(PortType port,size_t pin_number)
{
	uint32_t temp = 0b1;
	temp <<=  pin_number;
	port->BRR = temp;
}

void enablePin(PortType port,size_t pin_number)
{
	uint32_t temp = 0b1;
	temp <<= pin_number;
	port->BSRR |= temp;
}

//atomic safety
void setPinState(PortType port, enum State state, size_t pin_number)
{
	if (state == Low)
		disablePin(port, pin_number);
	else if (state == High)
		enablePin(port, pin_number);
}

void __setPinsState(PortType port, enum State state, size_t pins_count,...)
{
	va_list list;
	va_start(list, pins_count);
	for (size_t i = 0; i < pins_count; ++i)
		setPinState(port, state, va_arg(list, size_t));
	va_end(list);
}

static inline void __pullUp(PortType port,size_t pin_number)
{
	setPinState(port, High, pin_number);
}

static inline void __pullDown(PortType port,size_t pin_number)
{
	setPinState(port, Low, pin_number);
}

void setPinPull(PortType port, enum Pull pull, size_t pin_number)
{
	if (pull == PullUp)
		__pullUp(port, pin_number);
	else if (pull == PullDown)
		__pullDown(port, pin_number);
}

void __setPinsPull(PortType port, enum Pull pull, size_t pins_count, ...)
{
	va_list list;
	va_start(list, pins_count);
	for (size_t i = 0; i < pins_count; ++i)
		setPinPull(port, pull, va_arg(list, size_t));
	va_end(list);
}


void acceptOutMask(PortType port, uint32_t mask)
{
	port->BSRR = mask;
}

void clearPortState(PortType port)
{
	port->BSRR = 0x0;
}

enum State readPinState(PortType port, size_t pin_number)
{
	if ((port->IDR & (0x1 << pin_number)) == 0)
		return Low;
	else
		return High;
}

bool isPinHigh(PortType port, size_t pin_number)
{
	if (readPinState(port, pin_number) == High)
		return true;
	else
		return false;
}

bool isPinLow(PortType port, size_t pin_number)
{
	if (readPinState(port, pin_number) == Low)
		return true;
	else
		return false;
}
