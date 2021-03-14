/*
 * interrupt.h
 *
 *  Created on: 13.03.2021
 *      Author: Damian Wójcik
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "port.h"

//wiem ,¿e opis nie powinien byc po polsku ,ale raz przymknê na to oko

//nale¿y pamiêtaæ o tym i¿ przerwania maj¹ wspó³dzielone wektory np. dla PA1 oraz PC1, czy dla linii od 10 do 15.
//Wobec tego tylko jeden port mo¿e obs³u¿yc mi jedn¹ liniê

//Mogê miec do 16 niezale¿nych œcie¿ek przerwañ ( 5 unikalnych + 2 wspó³dzielone )

//np. PC1,PA1,PB1,PD1 wspó³dziel¹ liniê 1

//Wa¿ne rejestry

//Wszystkie rejestry s¹ 32 bitowe ,ale do dyspozycji mamy bity 0-19 z czego 19 powinien byc 0
//bity 16-18 s¹ sprzêtowe , ich tutaj nie ruszam

/*	EXTI_IMR - rejestr wyst¹pieñ przerwañ dla uk³adu
 * Ka¿dy z pinów mo¿e przyjmowaæ jeden stan 0 lub 1 (bity od 0 do 19).
 * Zero oznacza, ¿e przerwanie na linii zosta³o maskowane, 1 ¿e zosta³o niezamaskowane
 * Reszta to 0
 *
 * EXTI_EMR - rejestr wyst¹pieñ zdarzeñ
 * Wygl¹da on dok³adnie tak samo jak IMR. Zero oznacza, ¿e
 * zdarzenie na linii jest maskowane, jedynka natomiast, ¿e jest niemaskowane.
 *
 * EXTI_RTSR - wybranie zbocza rosn¹cego dla danego pinu:
 * Jedynka ustawia wyzwalanie zboczem rosn¹cym.
 *
 * EXTI_FTSR - ustawia wyzwalanie zboczem opadaj¹cym:
 * Zbocze opadaj¹ce w³¹czone gdy ustawiona jedynka.
 *
 * Jeœli oba s¹ ustawione na 1 ,to oba generuj¹ przerwanie , czyli przerwanie
 * bêdzie teoretycznie(drgania styków) po wciœniêciu i zwolnieniu przycisku
 *
 * EXTI_SWIER - ustawienie przerwania systemowego na okreœlonej linii
 *
 * Gdy przerwanie jest w³¹czone dla EXTI_IMR to zmiana stanu z 0 na 1 powoduje ustawienie bitu
 * w rejestrze EXTI_PR. Powoduje to wygenerowanie ¿¹danego przerwania.
 *
 * W tym rejestrze jak ju¿ wspomnia³em jest ustawiana jedynka po pojawieniu siê przerwania.
 * Czyszczenie natomiast nastêpuje poprzez wpisanie jedynki do bitu.
 *
 * EXTI_PR - pozwala na zweryfikowanie czy wyst¹pi³o przerwanie. 0 oznacza ¿e przerwanie nie mia³o miejsca,
 * jedynka natomiast, 1 ¿e wyst¹pi³o przerwanie wyzwolone wczeœniej ustawionym zboczem.
 *
 * Wa¿na informacja z dokumentacji:
 * The 112 GPIOs are connected to the 16 external interrupt/event lines
 * ,czyli jest 16 linii , wiêc do 4 pinów jest przypisany jedna linia
 * Potrzebna jest tak¿e konfiguracja funkcji pinów jako alternatywne za pomoc¹ struktury AFIO (Alternate
 * Function IO)
 *
 *Jest tam ukryta 4-ro elementowa tablica rejestrów EXTI[4] , s¹ to rejestry 32-bitowe , ale bity 16-31 s¹ zarezerwowane.
 *strona w dokumentacji : 191
 *
 */

//Zegar funkcji alternatywnych
#define initInterrupts() RCC->APB2ENR |= RCC_APB2ENR_AFIOEN //wymaga initPort()

enum ExtiPort
{
	PortA = 0b0000,
	PortB = 0b0001,
	PortC = 0b0010,
	PortD = 0b0011
};

enum EdgeType
{
	FallingEdge,
	RisingEdge,
	Both
};

void setupExternalInerrupt(size_t pin_number, enum ExtiPort port,enum EdgeType edge);

//makrodefinicje u³atwiaj¹ce ¿ycie

#define externalInterrput(pins) __attribute__((interrupt)) void EXTI##pins##_IRQHandler(void)
//przyjmowane wartoœci ,to 15_10 ,5_9 , 4 ,3 ,2 ,1 ,0

//poni¿sze makra przyjmuj¹ numery pinów 0-15
#define causedBy(pin) ( EXTI->PR & EXTI_PR_PR##pin )
#define resetFlag(pin) EXTI->PR = EXTI_PR_PR##pin


#endif /* INTERRUPT_H_ */
