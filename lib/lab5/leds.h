/**
 * ******************************************************************************
 * @file    : leds.h
 * @brief   : LEDs module header file
 * @details : LED initialization and interaction
 * 
 * @author  : Akshay Narang
 * @date    : 09-2025
 * ******************************************************************************
*/

#ifndef _LEDS_H_
#define _LEDS_H_


/**
 * @brief Initialize LED1
 * @hint You might want to check out the schematics in the MSP User Guide
 *       The IOMUX has a hardware inversion bit
*/
void LED1_init(void);


/**
 * @brief Initialize LED2
 * @note You must account for each LED color
*/
void LED2_init(void);


/**
 * @brief Set LED1 output state
 * @note ON, OFF, TOGGLE
*/
void LED1_set(int stateLED1);


/**
 * @brief Set LED2 output state
 * @note RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, OFF
 * 			 1, 	2,		 3,	   4,    5,       6,      7,		 0
*/
void LED2_set(int stateLED2);



/*
brief: keeps LEDs on in order to cycle through colors for LED2rgb with S2 input
note: only used in main.c
*/
void keepLEDon(void);

#endif // _LEDS_H_
