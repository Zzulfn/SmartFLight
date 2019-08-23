/**
 *********************************************************************
 *
 * @brief      LED handle function.
 *
 * @file       App_LED.c
 *
 *********************************************************************
 */


/*
 *********************************************************************
 *							INCLUDE FILES
 *********************************************************************
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_types.h"
#include "soc/timer_group_struct.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"
#include "timer_group.h"
#include "esp_log.h"
#include "App_LED.h"
#include "esp_task_wdt.h"
#include <sys/time.h>

/*
 *********************************************************************
 *                             DEFINES
 *********************************************************************
 */
#define TAG "LED_BUZZER"

/*
 *********************************************************************
 *                            VARIABLES
 *********************************************************************
 */
static uint8_t Buzzer_Mode = 0;



/*
 *********************************************************************
 *                  PUBLIC FUNCTIONS DEFINITIONS
 *********************************************************************
 */


/**********************************************************************
 *@Brief Set the Buzzer's mode 
 *@BuzzerMode  : Buzzer mode 
 *@ return NULL
 *
 ***********************************************************************/
void Buzzer_SetBuzzerMode(uint8_t BuzzerMode){
	Buzzer_Mode = BuzzerMode;
}


/**********************************************************************
 *@Brief Set the Buzzer contorl PIN(GPIO5) to Output model 
 *@ return NULL
 *
 ***********************************************************************/
 void LED_Buzzer_GPIO_Init(void){
	
	 TaskHandle_t LED_Buzzer_task_handle_t;
	 gpio_config_t io_conf;
	 
	 //Set the LED_PIN(GPIO4),Buzzer_PIn(GPIO5) into output modle ,
	 io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	 io_conf.mode = GPIO_MODE_OUTPUT;
	 
	 //bit mask of the pins 
	 io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	 io_conf.pull_down_en = 0;
	 io_conf.pull_up_en = 0;
	 gpio_config(&io_conf);
	 
	 //create the LED/Buzzer task
	 xTaskCreatePinnedToCore(&LED_Buzzer_task, "LED Buzzer task handle", 1*1024, NULL, 25, &LED_Buzzer_task_handle_t,	1);
	 if(LED_Buzzer_task_handle_t == NULL){
		 ESP_LOGE(TAG, "%s Create LED_Buzzer_task Fail ",__func__);
		 return ESP_FAIL;
	 }
	 
	 return ESP_OK;
 }

/**********************************************************************
* @Brief set the Buzzer status
* @BuzzerMode: The Buzzer Mode: 
*
*
***********************************************************************/
void Buzzer_Set_Buzzer_Status(uint8_t BuzzerMode){
	Buzzer_Mode = BuzzerMode;
}

/**********************************************************************
* @Brief Get the Buzzer status
*  Return : the mode of the Buzeer
*
***********************************************************************/
void Buzzer_Get_Buzzer_Status(void){
	return Buzzer_Mode;
}



/*
 *********************************************************************
 *                FILE LOCAL FUNCTIONS DEFINITIONS
 *********************************************************************
 */


/**********************************************************************
*@Brief the LED_BUZZER handle task
*@ return NULL
*
***********************************************************************/
 static void LED_Buzzer_task(void* arg){
 	uint8_t BuzzerMode = 0;
	
    while(1){
		
	   BuzzerMode = Buzzer_Get_Buzzer_Status();
	   
		switch(BuzzerMode){
			case BUZZER_IDLE_MODE：
				vTaskDelay(200/portTICK_RATE_MS);
				break;
				
			case BUZZER_PAIR_MODE：
			case BUZZER_UNPAIR_MODE：
				
				 BUZZER_ON;
				 vTaskDelay(300/portTICK_RATE_MS);
				 BUZZER_OFF;
				 vTaskDelay(300/portTICK_RATE_MS);
				 BUZZER_ON;
				 vTaskDelay(300/portTICK_RATE_MS);
				 BUZZER_OFF
				 break;
				
			case BUZZER_WARN_MODE：
				 BUZZER_ON;
				 vTaskDelay(300/portTICK_RATE_MS);
				 BUZZER_OFF;
				
			case BUZZER_OTHER_MODE：
				 BUZZER_ON;
				 vTaskDelay(300/portTICK_RATE_MS);
				 BUZZER_OFF;
			default:
				 vTaskDelay(200/portTICK_RATE_MS);
				 break;
		
		}
		
    }
 }


/********************************FILE END*********************************/
