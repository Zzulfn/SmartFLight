#ifndef APP_LED_H#define APP_LED_H

#define LED_PIN   	(4) 
#define BUZZER_PIN (5) 
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<LED_PIN) | (1ULL<<BUZZER_PIN))

#define LED_ON 	gpio_set_level(LED_PIN, 1)
#define LED_OFF 	gpio_set_level(LED_PIN, 0)
#define BUZZER_ON  gpio_set_level(BUZZER_PIN, 1)
#define BUZZER_OFF gpio_set_level(BUZZER_PIN, 0) #define BUZZER_IDLE_MODE (0x00) #define BUZZER_PAIR_MODE (0x01) #define BUZZER_UNPAIR_MODE  (0x02) #define BUZZER_WARN_MODE  (0x03) #define BUZZER_OTHER_MODE (0x04)
void LED_Buzzer_GPIO_Init(void);
void Buzzer_Set_Buzzer_Status(void);
#endif 
