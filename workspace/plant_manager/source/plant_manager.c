/*
 * Copyright 2016-2025 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    plant_manager.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "manager_consts.h"

/* TODO: insert other definitions and declarations here. */
void btns_init(void);
void leds_init(void);
double get_moister_lvl(const int PIN);

/*
 * @brief   Application entry point.
 */

int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    btns_init();  // btn setup
    leds_init();  // leds setup

    PRINTF("Hello World\n");

    volatile static int i = 0;
    while(1) {
        i++ ;
        if ((GPIOC->PDIR & (1 << PTC_SELECTION_BTN_PIN)) == 0)
        {
        	printf("select pressed. Temp LED on\n");
        	GPIOE->PCOR |= (1<<PTE_TEMPURATURE_LED);
        	GPIOD->PSOR |= (1<<PTD_MOISTER_LED);
        }
        else if((GPIOC->PDIR & (1 << PTC_CONFIRM_BTN_PIN)) == 0)
        {
        	printf("confirm pressed. moister LED on\n");
        	GPIOE->PSOR |= (1<<PTE_TEMPURATURE_LED);
        	GPIOD->PCOR |= (1<<PTD_MOISTER_LED);
        }
        else
        {
        	GPIOE->PSOR |= (1<<PTE_TEMPURATURE_LED);
        	GPIOD->PSOR |= (1<<PTD_MOISTER_LED);
        }

        __asm volatile ("nop");
    }
    return 0 ;
}


void btns_init()
{
	SIM->SCGC5 |= (1 << 11);  // portC enable

	// btn1
	PORTC->PCR[PTC_CONFIRM_BTN_PIN] &= ~0x700;  // clr mux
	PORTC->PCR[PTC_CONFIRM_BTN_PIN] |= (1 << 8);  // set mux to GPIO
	GPIOC->PDDR &= ~(1<<PTC_CONFIRM_BTN_PIN);  // set PTC pin to input
	PORTC->PCR[PTC_CONFIRM_BTN_PIN] |= (1<<1);  // enable pull resistor
	PORTC->PCR[PTC_CONFIRM_BTN_PIN] |= (1<<0);  // enable pulldown

	// btn2
	PORTC->PCR[PTC_SELECTION_BTN_PIN] &= ~0x700;  // clr mux
	PORTC->PCR[PTC_SELECTION_BTN_PIN] |= (1 << 8);  // set mux to GPIO
	GPIOC->PDDR &= ~(1<<PTC_SELECTION_BTN_PIN);  // set PTC pin to input
	PORTC->PCR[PTC_SELECTION_BTN_PIN] |= (1<<1);  // enable pull resistor
	PORTC->PCR[PTC_SELECTION_BTN_PIN] |= (1<<0);  // enable pulldown
}


void leds_init()
{
	SIM->SCGC5 |= (1 << 13);  // portE enable
	SIM->SCGC5 |= (1 << 12);  // portD enable

	// LED1
	PORTE->PCR[PTE_TEMPURATURE_LED] &= ~0x700;  // clr mux
	PORTE->PCR[PTE_TEMPURATURE_LED] |= (1 << 8);  // set mux to GPIO
	GPIOE->PDDR |= (1<<PTE_TEMPURATURE_LED);  // sets PTD pin to output

	// LED2
	PORTD->PCR[PTD_MOISTER_LED] &= ~0x700;  // clr mux
	PORTD->PCR[PTD_MOISTER_LED] |= (1 << 8);  // set mux to GPIO
	GPIOD->PDDR |= (1<<PTD_MOISTER_LED);  // sets PTD pin to output
}
