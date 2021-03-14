/*
 * port.h
 *
 *  Created on: 13.03.2021
 *      Author: Damian W�jcik
 */

#ifndef PORT_H_
#define PORT_H_

//Opis rejestr�w

//32bit CRL  - Pozwala on na wybranie ustawie� dotycz�cych wyprowadze� od Px0 do Px7.

//32bit CRH - dzia�a on podobnie jak CRL, natomiast pozwala on na ustawianie innego zestawu pin�w tj. od 8 do 15.

//16bit IDR - s�u�y tylko do odczytu, dzi�ki niemu mo�liwe jest sprawdzenia stanu jaki wyst�puje na danym wyprowadzeniu.

//32bit BSRR - s�u�y wy��czenie do zapisu. Pozwala na w��czenie b�d� wy��czenie danego pinu.

//BSRR sk�ada si� z dw�ch "podrejestr�w" -> BR oraz BS
//Bity 0-15 BR(reset) s�u�� do wprowadzania pin�w w stan niski poprzez ustawienie 1 , ustawienie 0 nic nie robi
//Bity 16-31 BS(set) s�u�� do wprowadzania pin�w w stan wysoki poprzez ustawienie 1 , ustawienie 0 nic nie robi

//Wa�na informacja z dokumentacji strona 173
//Note: If both BSx and BRx are set, BSx has priority.

//Jednak by nie korzystac z wbudowanych makr -> b�d� korzystac z rejestr�w -> dla czytelno�ci

//16bit BRR -  podobnie jak BSRR s�u�y tylko i wy��cznie do zapisu.  Dzi�ki niemu mo�liwe jest tylko i wy��cznie
//kasowanie bit�w w rejestrze ODR poprzez wprowadzenie "1". Wpisanie "0" nie przynosi �adnego efektu.
//M�odsze 16 bit�w pozwalaj� na w��czenie, czyli podanie "1", je�li zostanie podane "0" to b�dzie mia�o to �adnego efektu.
//Po ustawieniu bitu, zostaje automatycznie ustawiony bit w rejestrze ODR. Starsze 16 bit�w s�u�y to wy��czania,
//czyli podanie 1 nie ustawi bitu w ODR tylko go skasuje.

//16bit ODR jest to rejestr do kt�rego mo�liwe jest wpisanie oraz wprowadzenie danych. Ustawia on odpowiedni stan na wyprowadzeniach

//Rejestry BSRR oraz BRR uzyskuj� dost�p do rejestru ODR. Operacje na rejestrach BSRR oraz BRR s� operacjami atomowymi
//je�li podczas operacji na ODR wyst�pi przerwanie , to b�dzie s�abo

//16bit LCKR - pozwala na blokowanie danego wyprowadzenia, gdy zosta�y do niego ju� wprowadzone dane.

//16bit LCKK - s�u�y do blokowania mo�liwo�ci zmiany, natomiast LCK0 do LCK15 blokuje mo�liwo�� wprowadzenia modyfikacji dla okre�lonego
//wyprowadzenia

#include "stm32f1xx.h"
#include "stm32f1xx_nucleo.h"
#include "arghelper.h"

#define port(port_name) GPIO##port_name

#define initPort(port_name) RCC->APB2ENR |= RCC_APB2ENR_IOP##port_name##EN

enum OutputMode
{
	Output_2MHz,// 10
	Output_10MHz,//01
	Output_50MHz//11
};

enum InputConfig
{
	Analog,//00
	PullUpPullDown,//10
	FloatingInput,//01
	Reserved//11
};

enum OutputConfig
{
	PushPull,//00
	AlternatePushPull,//10
	OpenDrain,//01
	AlternateOpenDrain//11
};

enum Pull{ PullDown, PullUp };

enum State { Low , High };

typedef GPIO_TypeDef* PortType;

void clearPinConfiguration(PortType port, size_t pin_number);

static inline void __configureOutputPin(PortType port,enum OutputMode mode,
						  	  	  	    enum OutputConfig config,
										size_t pin_number);// 0-15

static inline void __configureInputPin(PortType port,
									   enum InputConfig config,
									   size_t pin_number);

void __configureOutputPins(PortType port, enum OutputMode mode,
								  enum OutputConfig config, size_t pins_count, ...);

void __configureInputPins(PortType port, enum InputConfig config,
								 size_t pins_count, ...);


#define configureOutputPins(port, mode, config, ...) \
		__configureOutputPins(port, mode, config, PP_NARG(__VA_ARGS__), __VA_ARGS__)

#define configureInputPins(port, config, ...) \
		__configureInputPins(port, config, PP_NARG(__VA_ARGS__), __VA_ARGS__)

void clearPortState(PortType port);

inline void disablePin(PortType port,size_t pin_number);

inline void enablePin(PortType port,size_t pin_number);

//atomic safety

void setPinState(PortType port, enum State state, size_t pin_number);

void clearPortState(PortType port);

void __setPinsState(PortType port, enum State state, size_t pins_count, ...);

#define setPinsState(port, state, ...) \
		__setPinsState(port,state, PP_NARG(__VA_ARGS__), __VA_ARGS__)

void setPinPull(PortType port, enum Pull pull, size_t pin_number);

void __setPinsPull(PortType port, enum Pull pull, size_t pins_count, ...);

#define setPinsPull(port, pull, ...) \
		__setPinsPull(port,pull, PP_NARG(__VA_ARGS__), __VA_ARGS__)

void acceptOutMask(PortType port, uint32_t mask);

enum State readPinState(PortType port, size_t pin_number);

bool isPinHigh(PortType port, size_t pin_number);

bool isPinLow(PortType port, size_t pin_number);


#endif /* PORT_H_ */
