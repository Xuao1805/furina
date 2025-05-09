#include "blue_led_task.h"

#include "cmsis_os.h"
#include "main.h"


void blue_led_task(void const * argument)
{
    
    
    while(1)
    {
        HAL_GPIO_WritePin(LED_B_GPIO_Port,LED_B_Pin,GPIO_PIN_SET);
        osDelay(500);
        HAL_GPIO_WritePin(LED_B_GPIO_Port,LED_B_Pin,GPIO_PIN_RESET);
        osDelay(500);
        HAL_GPIO_WritePin(LED_B_GPIO_Port,LED_B_Pin,GPIO_PIN_RESET);
        osDelay(500);
        
    }
}
