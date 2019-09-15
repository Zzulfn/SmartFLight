/**
 *********************************************************************
 *
 * @brief      Buzzer handle function.
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
//#include "timer_group.h"
#include "esp_log.h"
#include "esp_task_wdt.h"
#include <sys/time.h>
#include "driver/ledc.h"

#include "App_LED.h"


/*
 *********************************************************************
 *                             DEFINES
 *********************************************************************
 */
#define  TAG "LED_BUZZER"
#define  CHANGE_BUZZER_FRE
#define  BUZZER_TASK_STACK_SIZE  (4*1024)
#define  BUZZER_tASK_PRIORIT     (15)
#define  ESP_CORE_1              (1)

#ifdef CHANGE_BUZZER_FRE
#define  LEDC_HS_CH0_GPIO        (12)  
#define  LEDC_HS_CH0_CHANNEL     LEDC_CHANNEL_0
#define  LEDC_HS_TIMER           LEDC_TIMER_0
#define  LEDC_HS_MODE            LEDC_HIGH_SPEED_MODE
#define  LOW_LEVEL               (0)
#define  BUZZER_FRE              (8000)
#define  BUZZER_BUTY             (5000)
#else
#define  BUZZER_PIN (5) 
#define  GPIO_OUTPUT_PIN_SEL     (1ULL<<BUZZER_PIN)
#endif

/*
 *********************************************************************
 *                            VARIABLES
 *********************************************************************
 */

static Buzzer_Param_t Buzzer_Param;


/*
 *********************************************************************
 *                  PUBLIC FUNCTIONS DEFINITIONS
 *********************************************************************
 */
static void Buzzer_ON(void);
static void Buzzer_OFF(void);
static void Buzzer_Handle_Task(void* arg);





/***************************************************************
*@Brief  Buzzer init
*           
*
***************************************************************/
 void SMF_Buzzer_init(void){

	TaskHandle_t Buzzer_Task_Handle_t;
    ESP_LOGI(TAG, "SMF_BUZZER_INIT");
#ifdef CHANGE_BUZZER_FRE	
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
        .freq_hz = BUZZER_FRE,                // frequency of PWM signal
        .speed_mode = LEDC_HS_MODE,           // timer mode
        .timer_num = LEDC_HS_TIMER            // timer index
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .channel    = LEDC_HS_CH0_CHANNEL,
        .duty       = BUZZER_BUTY,
        .gpio_num   = LEDC_HS_CH0_GPIO,
        .speed_mode = LEDC_HS_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_HS_TIMER
    };
    ledc_channel_config(&ledc_channel);
	
#else
 
	gpio_config_t io_conf;
	//Set the LED_PIN(GPIO4),Buzzer_PIn(GPIO5) into output modle ,
	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	
	//bit mask of the pins 
	io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);
	
#endif
	
	xTaskCreatePinnedToCore(&Buzzer_Handle_Task,
							"Handle the Buzzer", 
							BUZZER_TASK_STACK_SIZE, 
							NULL, 
							BUZZER_tASK_PRIORIT, 
							&Buzzer_Task_Handle_t,  
							0);//ESP_CORE_1);
	if(Buzzer_Task_Handle_t == NULL){
		ESP_LOGE(TAG, "Create Buzzer Handle Task Fail");
		return ;
	}

}



/**********************************************************************
* @Brief set the Buzzer status
* @BuzzerMode: The Buzzer Mode: 
*
*
***********************************************************************/
void Buzzer_Set_Buzzer_Model(Buzzer_Model New_Buzzer_Mode){
	
    xSemaphoreTake(Buzzer_Param.Buzzer_Param_Lock, portMAX_DELAY);
	Buzzer_Param.Buzzer_Mode = New_Buzzer_Mode;
    xSemaphoreGive(Buzzer_Param.Buzzer_Param_Lock);
	
}


/**********************************************************************
* @Brief Get the Buzzer status
*  Return : the mode of the Buzeer
*
***********************************************************************/
Buzzer_Model Buzzer_Get_Buzzer_Model(void){

	Buzzer_Model Current_Buzzer_Model;
	
    xSemaphoreTake(Buzzer_Param.Buzzer_Param_Lock, portMAX_DELAY);
	Current_Buzzer_Model =Buzzer_Param.Buzzer_Mode;
    xSemaphoreGive(Buzzer_Param.Buzzer_Param_Lock);
	
	return Current_Buzzer_Model;
	
}


/**********************************************************************
* @Brief Set the Buzzer ON
*  Return : the mode of the Buzeer
*
***********************************************************************/
static void Buzzer_ON(void){
#ifdef CHANGE_BUZZER_FRE
	ledc_update_duty(LEDC_HS_MODE, LEDC_HS_CH0_CHANNEL);
#else
    gpio_set_level(BUZZER_PIN, 1);
#endif
	
}


/**********************************************************************
* @Brief Set the Buzzer OFF
*
***********************************************************************/
static void Buzzer_OFF(void){
#ifdef CHANGE_BUZZER_FRE
	ledc_stop(LEDC_HIGH_SPEED_MODE,LEDC_HS_CH0_CHANNEL,LOW_LEVEL);
#else
    gpio_set_level(BUZZER_PIN, 0);
#endif
	
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
 static void Buzzer_Handle_Task(void* arg){
 
    Buzzer_Model Current_Buzzer_Mode;
	Buzzer_Param.Buzzer_Mode = BUZZER_NORMAL_MODEL;
	Buzzer_Param.Buzzer_Param_Lock = xSemaphoreCreateBinary();
    xSemaphoreGive(Buzzer_Param.Buzzer_Param_Lock);
	
	if(Buzzer_Param.Buzzer_Param_Lock == NULL){
		ESP_LOGE(TAG, "Create Buzzer Param LOCK Fail");
		return;
	}
	
	ESP_LOGI(TAG,"Buzzer_Param.Buzzer_Mode :%d",Buzzer_Param.Buzzer_Mode);
    while(1){
		
	    Current_Buzzer_Mode = Buzzer_Get_Buzzer_Model();
		switch(Current_Buzzer_Mode){
			case BUZZER_NORMAL_MODEL:
			//	 Buzzer_Set_Buzzer_Model(BUZZER_IDLE_MODEL);
 				 Buzzer_ON();
 				 vTaskDelay(300/portTICK_RATE_MS);
 				 Buzzer_OFF();
 				 vTaskDelay(300/portTICK_RATE_MS);
				 break;
			case BUZZER_PAIR_MODEL:
			case BUZZER_UNPAIR_MODEL:
				 Buzzer_Set_Buzzer_Model(BUZZER_IDLE_MODEL);
				 Buzzer_ON();
				 vTaskDelay(300/portTICK_RATE_MS);
				 Buzzer_OFF();
				 vTaskDelay(300/portTICK_RATE_MS);
				 Buzzer_ON();
				 vTaskDelay(300/portTICK_RATE_MS);
				 Buzzer_OFF();
				 break;
			
			case BUZZER_WARNING_MODEL:
				 Buzzer_Set_Buzzer_Model(BUZZER_IDLE_MODEL);
				 Buzzer_ON();
				 vTaskDelay(1000/portTICK_RATE_MS);
				 Buzzer_OFF();

			case BUZZER_IDLE_MODEL:
			default:
				 vTaskDelay(300/portTICK_RATE_MS);
				 break;
		
		}
		
    }

	vTaskDelete(NULL);
 }


/********************************FILE END*********************************/
