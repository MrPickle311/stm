/*
 * port.h
 *
 *  Created on: 13.03.2021
 *      Author: Damian Wójcik
 */

#ifndef PORT_H_
#define PORT_H_

//Opis rejestrów

//32bit CRL  - Pozwala on na wybranie ustawieñ dotycz¹cych wyprowadzeñ od Px0 do Px7.

//32bit CRH - dzia³a on podobnie jak CRL, natomiast pozwala on na ustawianie innego zestawu pinów tj. od 8 do 15.

//16bit IDR - s³u¿y tylko do odczytu, dziêki niemu mo¿liwe jest sprawdzenia stanu jaki wystêpuje na danym wyprowadzeniu.

//32bit BSRR - s³u¿y wy³¹czenie do zapisu. Pozwala na w³¹czenie b¹dŸ wy³¹czenie danego pinu.

//BSRR sk³ada siê z dwóch "podrejestrów" -> BR oraz BS
//Bity 0-15 BR(reset) s³u¿¹ do wprowadzania pinów w stan niski poprzez ustawienie 1 , ustawienie 0 nic nie robi
//Bity 16-31 BS(set) s³u¿¹ do wprowadzania pinów w stan wysoki poprzez ustawienie 1 , ustawienie 0 nic nie robi

//Wa¿na informacja z dokumentacji strona 173
//Note: If both BSx and BRx are set, BSx has priority.

//Jednak by nie korzystac z wbudowanych makr -> bêdê korzystac z rejestrów -> dla czytelnoœci

//16bit BRR -  podobnie jak BSRR s³u¿y tylko i wy³¹cznie do zapisu.  Dziêki niemu mo¿liwe jest tylko i wy³¹cznie
//kasowanie bitów w rejestrze ODR poprzez wprowadzenie "1". Wpisanie "0" nie przynosi ¿adnego efektu.
//M³odsze 16 bitów pozwalaj¹ na w³¹czenie, czyli podanie "1", jeœli zostanie podane "0" to bêdzie mia³o to ¿adnego efektu.
//Po ustawieniu bitu, zostaje automatycznie ustawiony bit w rejestrze ODR. Starsze 16 bitów s³u¿y to wy³¹czania,
//czyli podanie 1 nie ustawi bitu w ODR tylko go skasuje.

//16bit ODR jest to rejestr do którego mo¿liwe jest wpisanie oraz wprowadzenie danych. Ustawia on odpowiedni stan na wyprowadzeniach

//Rejestry BSRR oraz BRR uzyskuj¹ dostêp do rejestru ODR. Operacje na rejestrach BSRR oraz BRR s¹ operacjami atomowymi
//jeœli podczas operacji na ODR wyst¹pi przerwanie , to bêdzie s³abo

//16bit LCKR - pozwala na blokowanie danego wyprowadzenia, gdy zosta³y do niego ju¿ wprowadzone dane.

//16bit LCKK - s³u¿y do blokowania mo¿liwoœci zmiany, natomiast LCK0 do LCK15 blokuje mo¿liwoœæ wprowadzenia modyfikacji dla okreœlonego
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
