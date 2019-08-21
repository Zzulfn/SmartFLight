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


#define TAG "LED_BUZZER"

static uint8_t Buzzer_Mode = 0;

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
*@Brief the LED_BUZZER handle task
*@ return NULL
*
***********************************************************************/
 static void LED_Buzzer_task(void* arg){
    
    while(1){
		//
		switch(){
			case ：
				
					vTaskDelay(1000/portTICK_RATE_MS);
					break;
				
			case 接收配对命令：
				｛
					高500ms
					低250ms
					高500ms
					低
					将其改为Idle 模式
					break;
				｝
			case 接收解配命令：
				｛
					高500ms
					低250ms
					高500ms
					低
					将其改为Idle 模式
					break;
				｝
			case 接收其他命令：
				｛
					高1000
					低；
					将其改为Idle 模式
					break;
				｝
				
			case 报警：
				｛
					高1000ms
					低1000ms
				｝
			case default:
				vTaskDelay(1000/portTICK_RATE_MS);
				break;
		
		}
		BUZZER_ON;
		vTaskDelay(10);
		BUZZER_OFF;
		vTaskDelay(10);
		
    }
 }


/********************************FILE END*********************************/
