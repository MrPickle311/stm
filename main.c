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

//Opis rejestrów
//CRL  - Pozwala on na wybranie ustawieñ dotycz¹cych wyprowadzeñ od Px0 do Px7.
//CRH - dzia³a on podobnie jak CRL, natomiast pozwala on na ustawianie innego zestawu pinów tj. od 8 do 15.
//IDR - s³u¿y tylko do odczytu, dziêki niemu mo¿liwe jest sprawdzenia stanu jaki wystêpuje na danym wyprowadzeniu.
//BSRR - s³u¿y wy³¹czenie do zapisu. Pozwala na w³¹czenie b¹dŸ wy³¹czenie danego pinu.

//M³odsze 16 bitów pozwalaj¹ na w³¹czenie, czyli podanie "1", jeœli zostanie podane "0" to bêdzie mia³o to ¿adnego efektu.
//Po ustawieniu bitu, zostaje automatycznie ustawiony bit w rejestrze ODR. Starsze 16 bitów s³u¿y to wy³¹czania,
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
