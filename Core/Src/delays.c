/*
 * delays.c
 *
 *	The MIT License.
 *  Created on: 11.07.2018
 *      Author: Mateusz Salamon
 *      www.msalamon.pl
 *
 */
#include "main.h"
#include "stm32f4xx_hal.h"
#include "tim.h"

#include "delays.h"

void Delay_us(uint16_t us)
{
	htim3.Instance->CNT = 0;
	while(htim3.Instance->CNT <= us);
}

void Delay_us_new(uint32_t us)
{
    uint32_t start = __HAL_TIM_GET_COUNTER(&htim3);
    uint32_t ticks = us; // bo masz prescaler ustawiony na 84-1 = 83 -> 1 tick = 1 us

    while ((__HAL_TIM_GET_COUNTER(&htim3) - start) < ticks) {
        // czekamy aż odliczy się żądana liczba mikrosekund
    }
}
