#ifndef APP_LED_H#define APP_LED_H

typedef enum  BUZZER_MODE_ENUM{	BUZZER_IDLE_MODEL = 0,	BUZZER_NORMAL_MODEL,	BUZZER_PAIR_MODEL,	BUZZER_UNPAIR_MODEL,	BUZZER_WARNING_MODEL	}Buzzer_Model;typedef struct BUZZER_PARAM_T {	Buzzer_Model Buzzer_Mode;	SemaphoreHandle_t Buzzer_Param_Lock;	}Buzzer_Param_t,*pBuzzer_Param_t;	void SMF_Buzzer_init(void);void Buzzer_Set_Buzzer_Model(Buzzer_Model New_Buzzer_Mode);Buzzer_Model Buzzer_Get_Buzzer_Model(void);#endif 
