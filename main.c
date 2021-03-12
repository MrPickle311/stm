/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f1xx.h"
#include "stm32f1xx_nucleo.h"

void clockConfig()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
}

//Opis rejestr�w
//CRL  - Pozwala on na wybranie ustawie� dotycz�cych wyprowadze� od Px0 do Px7.
//CRH - dzia�a on podobnie jak CRL, natomiast pozwala on na ustawianie innego zestawu pin�w tj. od 8 do 15.
//IDR - s�u�y tylko do odczytu, dzi�ki niemu mo�liwe jest sprawdzenia stanu jaki wyst�puje na danym wyprowadzeniu.
//BSRR - s�u�y wy��czenie do zapisu. Pozwala na w��czenie b�d� wy��czenie danego pinu.

//M�odsze 16 bit�w pozwalaj� na w��czenie, czyli podanie "1", je�li zostanie podane "0" to b�dzie mia�o to �adnego efektu.
//Po ustawieniu bitu, zostaje automatycznie ustawiony bit w rejestrze ODR. Starsze 16 bit�w s�u�y to wy��czania,
//czyli podanie 1 nie ustawi bitu w ODR tylko go skasuje.

//https://elektronika327.blogspot.com/2016/08/1-stm32-m3-nucleo-f103rb-rejestry-porty.html
int main(void)
{
	172
	clockConfig();
	uint32_t temp = GPIOA->
	temp = GPIO_

	GPIOA->CLR
	for(;;);
}
