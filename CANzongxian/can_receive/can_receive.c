#include "can_receive.h"
#include "main.h"
#include "pid.h"


extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;              

#define get_motor_measure(ptr, data)                                    \
    {                                                                   \
        (ptr)->last_ecd = (ptr)->ecd;                                   \
        (ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);            \
        (ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]);      \
        (ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]);  \
        (ptr)->temperate = (data)[6];                                   \
    }
//ecd 电机转子角度
//temperate   温度
//speed_rpm   电机转子速度
//given_current  控制电流

static motor_measure_t motor_chassis[7];
static CAN_TxHeaderTypeDef  chassis_tx_message;
static uint8_t              chassis_can_send_data[8];
    
    
    
    
    
    
void HAL_CAN_RXFifo0MsgPendingCallback(CAN_HandleTypeDef*hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];
    
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
    
    switch(rx_header.StdId)
    {
        case CAN_3508_M1_ID:
        case CAN_3508_M2_ID:
        case CAN_3508_M3_ID:
        case CAN_3508_M4_ID:
        {
         static uint8_t i = 0; 
//get motor id
         i = rx_header.StdId - CAN_3508_M1_ID;
         get_motor_measure(&motor_chassis[i], rx_data);
         break;   
               
        }
    
    }
}
    
void CAN_cmd_chassis_reset_ID(void)
{
    uint32_t send_mail_box;
    chassis_tx_message.StdId = 0x700;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = 0;
    chassis_can_send_data[1] = 0;
    chassis_can_send_data[2] = 0;
    chassis_can_send_data[3] = 0;
    chassis_can_send_data[4] = 0;
    chassis_can_send_data[5] = 0;
    chassis_can_send_data[6] = 0;
    chassis_can_send_data[7] = 0;
    
    HAL_CAN_AddTxMessage(&CHASSIS_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}    

    
void CAN_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box;
    chassis_tx_message.StdId = CAN_CHASSIS_ALL_ID;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = motor1 >> 8;
    chassis_can_send_data[1] = motor1;
    chassis_can_send_data[2] = motor2 >> 8;
    chassis_can_send_data[3] = motor2;
    chassis_can_send_data[4] = motor3 >> 8;
    chassis_can_send_data[5] = motor3;
    chassis_can_send_data[6] = motor4 >> 8;
    chassis_can_send_data[7] = motor4;
    
    HAL_CAN_AddTxMessage(&CHASSIS_CAN,&chassis_tx_message,chassis_can_send_data,&send_mail_box);
}

void CAN_1_send(uint8_t can_id,uint8_t data[8])
{
    uint32_t send_mail_box;
    uint8_t can_send_data[8];
    static CAN_TxHeaderTypeDef tx_message;
    
   tx_message.IDE = CAN_ID_STD;
    tx_message.StdId = can_id;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    
    tx_message.DLC = 0x08;
    
    can_send_data[0] = data[0];
    can_send_data[1] = data[1];
    can_send_data[2] = data[2];
    can_send_data[3] = data[3];
    can_send_data[4] = data[4];
    can_send_data[5] = data[5];
    can_send_data[6] = data[6];
    can_send_data[7] = data[7];
     
    HAL_CAN_AddTxMessage(&hcan1,&tx_message,can_send_data,&send_mail_box);
}
        
    
    
    
    


    
    
    
    
    
    
