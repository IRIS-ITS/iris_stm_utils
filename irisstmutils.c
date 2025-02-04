/*
 * irisstmutils.c
 *
 *  Created on: Dec 19, 2024
 *      Author: ARNN5115
 *     Version: 1.0
 */

#include "irisstmutils.h"

uint32_t millis(void) {
    return HAL_GetTick();
}

uint32_t micros(void) {
    return HAL_GetTick() * 1000 + ((SysTick->LOAD - SysTick->VAL) * 1000) / HAL_RCC_GetHCLKFreq();
}

void delay_us(uint32_t us) {
    uint32_t start = micros();
    while (micros() - start < us);
}

uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint32_t mapf(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint32_t constrain(uint32_t x, uint32_t min, uint32_t max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

RingBuffer* ringBufferInit(uint16_t size) {
    RingBuffer* rb = (RingBuffer*)malloc(sizeof(RingBuffer));
    if (!rb) return NULL;
    
    rb->buffer = (uint8_t*)malloc(size);
    if (!rb->buffer) {
        free(rb);
        return NULL;
    }
    
    rb->size = size;
    rb->head = 0;
    rb->tail = 0;
    return rb;
}

int ringBufferPush(RingBuffer* rb, uint8_t data) {
    if (!rb) return -1;
    
    uint16_t next = (rb->head + 1) % rb->size;
    if (next == rb->tail) return -1;
    
    rb->buffer[rb->head] = data;
    rb->head = next;
    return 0;
}

int ringBufferPop(RingBuffer* rb, uint8_t* data) {
    if (!rb || rb->head == rb->tail) return -1;
    
    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % rb->size;
    return 0;
}

int setPWMDuty16bit(TIM_HandleTypeDef *tim, uint8_t channel, uint16_t duty)
{
    uint32_t period = tim->Init.Period + 1;
    uint32_t scaled_duty = (uint32_t)duty * period;
    uint32_t pulse = scaled_duty >> 16;

    uint64_t tim_channel;
    switch (channel)
    {
    case 1:
        tim_channel = TIM_CHANNEL_1;
        break;

    case 2:
        tim_channel = TIM_CHANNEL_2;
        break;

    case 3:
        tim_channel = TIM_CHANNEL_3;
        break;
    
    case 4:
        tim_channel = TIM_CHANNEL_4;
        break;
    
    default:
        Error_Handler();
        return 1;
    }

    __HAL_TIM_SET_COMPARE(tim, tim_channel, pulse);
    return 0;
}

uint16_t power(uint16_t base, uint16_t exp){
    if(exp == 0){
        return 1;
    } else {
        return base * power(base, exp - 1);
    }
}

UART_RxDataStatus processUART_RxData(const uint8_t *unsorted_array, const char *header, uint8_t *sorted_array, size_t data_len) {
    if (unsorted_array == NULL || header == NULL || sorted_array == NULL) {
        return DATA_INPUT_NULL;
    }
    
    size_t header_len = strlen(header);
    if (header_len == 0) return HEADER_INPUT_NULL;
    
    // Search for header using modified KMP algorithm
    int8_t header_pos = -1;
    int lps[header_len];
    
    // Preprocess the pattern (calculate lps array)
    int len = 0;
    lps[0] = 0;
    int i = 1;
    while (i < header_len) {
        if (header[i] == header[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    
    // Search for the header in the unsorted_array, checking twice the length to handle wrap-around
    i = 0;
    int j = 0;
    int search_len = data_len * 2;
    while (i < search_len) {
        // Use modulo to wrap around the array
        uint8_t current_char = unsorted_array[i % data_len];
        
        if (header[j] == current_char) {
            if (j == 0) {
                // Record potential start of header
                header_pos = i % data_len;
            }
            j++;
            i++;
            
            if (j == header_len) {
                // Verified full header match
                break;
            }
        } else {
            // Reset search if mismatch occurs
            i = i - j + 1;
            j = 0;
            header_pos = -1;
        }
    }

    if (header_pos == -1) {
        return HEADER_MISMATCH;
    }
    
    // Copy data starting from header position
    for (size_t i = 0; i < data_len; i++) {
        size_t pos = (header_pos + i) % data_len;
        sorted_array[i] = unsorted_array[pos];
    }
    
    if (sorted_array[0] != header[0] || sorted_array[1] != header[1] || sorted_array[2] != header[2] || sorted_array[data_len - 1] != 1) {
        return DATA_INVALID;
    }
    return VALID;
}

// uint8_t processButton8bit(char btn_list[8][3]){
//     uint8_t button_flag = 0;
//     uint64_t button_letters[8];
//     uint64_t button_numbers[8];
//     for (int i = 0; i < 8; i++){
//         if (btn_list[i] == 'A'){
//             button_letters[i] = GPIOA;
//         } else if (btn_list[i] == 'B'){
//             button_letters[i] = GPIOB;
//         } else if (btn_list[i] == 'C'){
//             button_letters[i] = GPIOC;
//         } else if (btn_list[i] == 'D'){
//             button_letters[i] = GPIOD;
//         } else if (btn_list[i] == 'E'){
//             button_letters[i] = GPIOE;
//         } else {
//             Error_Handler();
//         }

//         uint8_t button_number = (btn_list[i][1] - '0')*10 + btn_list[i][2];

//         if (HAL_GPIO_ReadPin(button_letters[i], button_numbers[i]) == GPIO_PIN_SET){
//             button_flag |= power(2, i);
//         }
//     }
//   return button_flag;
// }
