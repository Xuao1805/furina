#include "bsp_can.h"
#include "main.h"


extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
//滤波器控制
void can_filter_init(void)
{

    CAN_FilterTypeDef can_filter_st;
    can_filter_st.FilterActivation = ENABLE;//开启
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;//使用掩码
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;//选择32位
    can_filter_st.FilterIdHigh = 0x0000;
    can_filter_st.FilterIdLow = 0x0000;
    can_filter_st.FilterMaskIdHigh = 0x0000;
    can_filter_st.FilterMaskIdLow = 0x0000;
    can_filter_st.FilterBank = 0;//滤波器组
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;// 过滤器FIFO配置给FIFO0  
    HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);//配置滤波器
    HAL_CAN_Start(&hcan1);//使能
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);//can读FIFO的中断

		//can2 滤波器配置
    can_filter_st.SlaveStartFilterBank = 14;
    can_filter_st.FilterBank = 14;
    HAL_CAN_ConfigFilter(&hcan2, &can_filter_st);
    HAL_CAN_Start(&hcan2);
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
}
