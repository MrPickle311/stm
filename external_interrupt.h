/*
 * interrupt.h
 *
 *  Created on: 13.03.2021
 *      Author: Damian W�jcik
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "port.h"

//wiem ,�e opis nie powinien byc po polsku ,ale raz przymkn� na to oko

//nale�y pami�ta� o tym i� przerwania maj� wsp�dzielone wektory np. dla PA1 oraz PC1, czy dla linii od 10 do 15.
//Wobec tego tylko jeden port mo�e obs�u�yc mi jedn� lini�

//Mog� miec do 16 niezale�nych �cie�ek przerwa� ( 5 unikalnych + 2 wsp�dzielone )

//np. PC1,PA1,PB1,PD1 wsp�dziel� lini� 1

//Wa�ne rejestry

//Wszystkie rejestry s� 32 bitowe ,ale do dyspozycji mamy bity 0-19 z czego 19 powinien byc 0
//bity 16-18 s� sprz�towe , ich tutaj nie ruszam

/*	EXTI_IMR - rejestr wyst�pie� przerwa� dla uk�adu
 * Ka�dy z pin�w mo�e przyjmowa� jeden stan 0 lub 1 (bity od 0 do 19).
 * Zero oznacza, �e przerwanie na linii zosta�o maskowane, 1 �e zosta�o niezamaskowane
 * Reszta to 0
 *
 * EXTI_EMR - rejestr wyst�pie� zdarze�
 * Wygl�da on dok�adnie tak samo jak IMR. Zero oznacza, �e
 * zdarzenie na linii jest maskowane, jedynka natomiast, �e jest niemaskowane.
 *
 * EXTI_RTSR - wybranie zbocza rosn�cego dla danego pinu:
 * Jedynka ustawia wyzwalanie zboczem rosn�cym.
 *
 * EXTI_FTSR - ustawia wyzwalanie zboczem opadaj�cym:
 * Zbocze opadaj�ce w��czone gdy ustawiona jedynka.
 *
 * Je�li oba s� ustawione na 1 ,to oba generuj� przerwanie , czyli przerwanie
 * b�dzie teoretycznie(drgania styk�w) po wci�ni�ciu i zwolnieniu przycisku
 *
 * EXTI_SWIER - ustawienie przerwania systemowego na okre�lonej linii
 *
 * Gdy przerwanie jest w��czone dla EXTI_IMR to zmiana stanu z 0 na 1 powoduje ustawienie bitu
 * w rejestrze EXTI_PR. Powoduje to wygenerowanie ��danego przerwania.
 *
 * W tym rejestrze jak ju� wspomnia�em jest ustawiana jedynka po pojawieniu si� przerwania.
 * Czyszczenie natomiast nast�puje poprzez wpisanie jedynki do bitu.
 *
 * EXTI_PR - pozwala na zweryfikowanie czy wyst�pi�o przerwanie. 0 oznacza �e przerwanie nie mia�o miejsca,
 * jedynka natomiast, 1 �e wyst�pi�o przerwanie wyzwolone wcze�niej ustawionym zboczem.
 *
 * Wa�na informacja z dokumentacji:
 * The 112 GPIOs are connected to the 16 external interrupt/event lines
 * ,czyli jest 16 linii , wi�c do 4 pin�w jest przypisany jedna linia
 * Potrzebna jest tak�e konfiguracja funkcji pin�w jako alternatywne za pomoc� struktury AFIO (Alternate
 * Function IO)
 *
 *Jest tam ukryta 4-ro elementowa tablica rejestr�w EXTI[4] , s� to rejestry 32-bitowe , ale bity 16-31 s� zarezerwowane.
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

//makrodefinicje u�atwiaj�ce �ycie

#define externalInterrput(pins) __attribute__((interrupt)) void EXTI##pins##_IRQHandler(void)
//przyjmowane warto�ci ,to 15_10 ,5_9 , 4 ,3 ,2 ,1 ,0

//poni�sze makra przyjmuj� numery pin�w 0-15
#define causedBy(pin) ( EXTI->PR & EXTI_PR_PR##pin )
#define resetFlag(pin) EXTI->PR = EXTI_PR_PR##pin


#endif /* INTERRUPT_H_ */
