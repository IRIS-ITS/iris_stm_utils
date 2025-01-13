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

int sortUART_RXData(const char *unsorted_array, char *header, char *sorted_array){
    // Buat pointer yang menunjuk ke elemen pertama header (i)
    char *id = strstr(unsorted_array, header);
    // Cek jika pointer NULL -> header tidak ditemukan
    if (id == NULL){
        return -1;
    } // Urutkan data JIKA DAN HANYA JIKA header ditemukan
    uint8_t len = sizeof(unsorted_array);
    for (size_t i = 0; i < len; i++, id++){
        // Handel overflow
        if (id >= (char *)(unsorted_array+len)) 
            id -= len; 
        sorted_array[i] = *id;
    }
    return 0;
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
