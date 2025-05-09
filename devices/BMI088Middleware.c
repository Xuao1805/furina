/* BMI088 Ӳ�������ʵ�� (����STM32 HAL��) */

#include "BMI088Middleware.h"
#include "main.h"          // STM32 HAL����ͷ�ļ�
#include "cmsis_os.h"      // FreeRTOS�ӿ�
#include "bsp_delay.h"     // �Զ�����ʱ����
extern SPI_HandleTypeDef hspi1;  // �����ⲿSPI���

/*------------------------- Ӳ����ʼ������ ------------------------*/

/**
 * @brief �պ���-ʵ��GPIO��ʼ��������CubeMX���
 * @note ��δʹ��CubeMX�����ڴ�ʵ�����¹��ܣ�
 *   - ����CS/INT����Ϊ�������
 *   - ����SPI SCK/MISO/MOSI����Ϊ��������
 */
void BMI088_GPIO_init(void)
{
    /* ����ʵ��ʾ����
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    // ����CS����
    GPIO_InitStruct.Pin = CS1_ACCEL_Pin | CS1_GYRO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    */
}

/**
 * @brief �պ���-ʵ��SPI��ʼ��������CubeMX���
 * @note ��δʹ��CubeMX�����ڴ����ã�
 *   - SPIʱ��ģʽ��CPOL/CPHA��
 *   - ʱ��Ԥ��Ƶ�������10MHz��
 *   - ����λ��8-bit��
 */
void BMI088_com_init(void)
{
    /* ����ʵ��ʾ����
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;   // ���ݴ�����������
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;       // ���ݴ�����������
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    HAL_SPI_Init(&hspi1);
    */
}

/*------------------------- ��ʱ���� -----------------------------*/

/**
 * @brief ���뼶��ʱ������FreeRTOS��
 * @param ms ��ʱ������
 * @warning ��ֹ���ж�������ʹ��
 */
void BMI088_delay_ms(uint16_t ms)
{
    osDelay(ms);  // �����������
}

/**
 * @brief ΢�뼶��ʱ������Ӳ����ʱ����
 * @param us ��ʱ΢����
 * @note ��Ҫʵ��bsp_delay.h�е�delay_us()
 */
void BMI088_delay_us(uint16_t us)
{
    delay_us(us);  // ʵ���豣֤���ȣ�ͨ����DWT��ʱ����
}

/*------------------------- SPIƬѡ���� --------------------------*/

// ���ٶȼ�Ƭѡ����
void BMI088_ACCEL_NS_L(void) {
    HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_RESET);
}
void BMI088_ACCEL_NS_H(void) {
    HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_SET);
}

// ������Ƭѡ����
void BMI088_GYRO_NS_L(void) {
    HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_RESET);
}
void BMI088_GYRO_NS_H(void) {
    HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_SET);
}

/*------------------------- SPI���ݴ��� --------------------------*/

/**
 * @brief SPI���ֽ�ȫ˫��ͨ��
 * @param txdata ���͵��ֽ�
 * @return ���յ����ֽ�
 * @note ��ʱʱ����Ϊ1000ms��ʵ��Ӧ����SPIʱ�ӵ���
 */
uint8_t BMI088_read_write_byte(uint8_t txdata)
{
    uint8_t rx_data;
    HAL_SPI_TransmitReceive(&hspi1, &txdata, &rx_data, 1, 1000);
    return rx_data;
}
