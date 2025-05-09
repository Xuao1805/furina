/* BMI088 硬件抽象层实现 (基于STM32 HAL库) */

#include "BMI088Middleware.h"
#include "main.h"          // STM32 HAL库主头文件
#include "cmsis_os.h"      // FreeRTOS接口
#include "bsp_delay.h"     // 自定义延时函数
extern SPI_HandleTypeDef hspi1;  // 声明外部SPI句柄

/*------------------------- 硬件初始化函数 ------------------------*/

/**
 * @brief 空函数-实际GPIO初始化可能在CubeMX完成
 * @note 若未使用CubeMX，需在此实现以下功能：
 *   - 配置CS/INT引脚为推挽输出
 *   - 配置SPI SCK/MISO/MOSI引脚为复用推挽
 */
void BMI088_GPIO_init(void)
{
    /* 典型实现示例：
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    // 配置CS引脚
    GPIO_InitStruct.Pin = CS1_ACCEL_Pin | CS1_GYRO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    */
}

/**
 * @brief 空函数-实际SPI初始化可能在CubeMX完成
 * @note 若未使用CubeMX，需在此配置：
 *   - SPI时钟模式（CPOL/CPHA）
 *   - 时钟预分频（建议≤10MHz）
 *   - 数据位宽（8-bit）
 */
void BMI088_com_init(void)
{
    /* 典型实现示例：
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;   // 根据传感器规格调整
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;       // 根据传感器规格调整
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    HAL_SPI_Init(&hspi1);
    */
}

/*------------------------- 延时函数 -----------------------------*/

/**
 * @brief 毫秒级延时（基于FreeRTOS）
 * @param ms 延时毫秒数
 * @warning 禁止在中断上下文使用
 */
void BMI088_delay_ms(uint16_t ms)
{
    osDelay(ms);  // 触发任务调度
}

/**
 * @brief 微秒级延时（基于硬件定时器）
 * @param us 延时微秒数
 * @note 需要实现bsp_delay.h中的delay_us()
 */
void BMI088_delay_us(uint16_t us)
{
    delay_us(us);  // 实现需保证精度（通常用DWT或定时器）
}

/*------------------------- SPI片选控制 --------------------------*/

// 加速度计片选控制
void BMI088_ACCEL_NS_L(void) {
    HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_RESET);
}
void BMI088_ACCEL_NS_H(void) {
    HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_SET);
}

// 陀螺仪片选控制
void BMI088_GYRO_NS_L(void) {
    HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_RESET);
}
void BMI088_GYRO_NS_H(void) {
    HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_SET);
}

/*------------------------- SPI数据传输 --------------------------*/

/**
 * @brief SPI单字节全双工通信
 * @param txdata 发送的字节
 * @return 接收到的字节
 * @note 超时时间设为1000ms，实际应根据SPI时钟调整
 */
uint8_t BMI088_read_write_byte(uint8_t txdata)
{
    uint8_t rx_data;
    HAL_SPI_TransmitReceive(&hspi1, &txdata, &rx_data, 1, 1000);
    return rx_data;
}
