/*
 * irisstmutils.h
 *
 *  Created on: Dec 19, 2024
 *      Author: ARNN5115
 *     Version: 1.0
 * 
 *  Just some utility functions for STM32.
 *  
 *  Made for ITS's IRIS Robotics Team purpose.
 * 	Team Website: https://iris.its.ac.id.
 */

#ifndef IRIS_STM_UTILS_H
#define IRIS_STM_UTILS_H

#include "main.h"
// #include "core_cm3.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Structure representing a ring buffer.
 */
typedef struct {
    uint8_t *buffer;
    uint32_t head;
    uint32_t tail;
    uint32_t size;
} RingBuffer;

/**
 * @brief Enum representing the status of the UART RX data.
 */
typedef enum UART_RxDataStatus
{
  VALID = 200,
  HEADER_MISMATCH = 300,
  DATA_INVALID = 400,
  DATA_INPUT_NULL = 401,
  HEADER_INPUT_NULL = 402
}UART_RxDataStatus;

/**
 * @brief Get the number of milliseconds since the system started.
 * 
 * @return Number of milliseconds since the system started.
 * @note This function is trying to imitate the Arduino millis function.
 */
uint32_t millis(void);

/**
 * @brief Get the number of microseconds since the system started.
 * 
 * @return Number of microseconds since the system started.
 * @note This function is trying to imitate the Arduino micros function.
 */
uint32_t micros(void);

/**
 * @brief Delay the program for a number of milliseconds.
 * 
 * @param ms Number of milliseconds to delay.
 * @note This function is trying to imitate the Arduino delay function.
 */
void delay_us(uint32_t us);

/**
 * @brief Map a value from one range to another.
 * 
 * @param x Value to map.
 * @param in_min Minimum value of the input range.
 * @param in_max Maximum value of the input range.
 * @param out_min Minimum value of the output range.
 * @param out_max Maximum value of the output range.
 * @return Mapped value.
 * @note This function is trying to imitate the Arduino map function.
 * @see https://www.arduino.cc/reference/en/language/functions/math/map/
 */
uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);

/**
 * @brief Map a value from one range to another.
 * 
 * @param x Value to map.
 * @param in_min Minimum value of the input range.
 * @param in_max Maximum value of the input range.
 * @param out_min Minimum value of the output range.
 * @param out_max Maximum value of the output range.
 * @return Mapped value.
 * @note This function is trying to imitate the Arduino map function.
 * @see https://www.arduino.cc/reference/en/language/functions/math/map/
 */
uint32_t mapf(float x, float in_min, float in_max, float out_min, float out_max);

/**
 * @brief Constrain a value to a range.
 * 
 * @param x Value to constrain.
 * @param min Minimum value of the range.
 * @param max Maximum value of the range.
 * @return Constrained value.
 * @note This function is trying to imitate the Arduino constrain function.
 * @see https://www.arduino.cc/reference/en/language/functions/math/constrain/
 */
uint32_t constrain(uint32_t x, uint32_t min, uint32_t max);

/**
 * @brief Initialize a ring buffer.
 * 
 * @param size Size of the ring buffer.
 * @return Pointer to the initialized ring buffer.
 */
RingBuffer* ringBufferInit(uint16_t size);

/**
 * @brief Push a byte to the ring buffer.
 * 
 * @param rb Pointer to the ring buffer.
 * @param data Data to push.
 * @return 0 on success, -1 on failure.
 */
int ringBufferPush(RingBuffer* rb, uint8_t data);

/**
 * @brief Pop a byte from the ring buffer.
 * 
 * @param rb Pointer to the ring buffer.
 * @param data Pointer to the data to pop.
 * @return 0 on success, -1 on failure.
 */
int ringBufferPop(RingBuffer* rb, uint8_t* data);

/**
 * @brief Set the PWM duty cycle from a 16-bit input.
 * 
 * @param tim Timer handler.
 * @param channel PWM channel.
 * @param duty Duty cycle in 16-bit resolution.
 * @return 0 on success, -1 on failure.
 */
int setPWMDuty16bit(TIM_HandleTypeDef *tim, uint8_t channel, uint16_t duty);

/**
 * @brief Sort UART RX data into an array.
 * 
 * @param unsorted_array Pointer to the unsorted input array.
 * @param header Pointer to store header characters.
 * @param sorted_array Pointer to store sorted output array.
 * @param data_len Length of data.
 * @return 0 on success, -1 on failure.
 */
UART_RxDataStatus processUART_RxData(const uint8_t *unsorted_array, const char *header, uint8_t *sorted_array, size_t data_len);

// /**
//  * @brief Process 8-bit buttons state from a list of button names.
//  * 
//  * @param btn_list Array of 8 button names, e.g: "E01"
//  * @return Combined 8-bit value representing button states
//  */
// uint8_t processButton8bit(char btn_list[8][3]);

#endif /* IRIS_STM_UTILS_H */
