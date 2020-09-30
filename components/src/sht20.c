/* i2c - Example

   For other examples please check:
   https://github.com/espressif/esp-idf/tree/master/examples

   See README.md file to get detailed usage of this example.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "sht20.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define I2C_MASTER_SCL_IO 33               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 32               /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_1       /*!< I2C port number for master dev */


//#define WRITE_BIT 1                /*!< I2C master write */
//#define READ_BIT  2                /*!< I2C master read */
#define ACK_CHECK_EN 0x1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */


#define WRITE_BIT I2C_MASTER_WRITE //写:0
#define READ_BIT  I2C_MASTER_READ //读:1




/*SHT20 设备操作相关宏定义，详见手册*/
#define SHT20_IIC_ADDRESS  0X40
#define SHT20_CMD_RH_HM    0XE5
#define SHT20_CMD_T_HM     0XE3
#define SHT20_CMD_RH_NHM   0XF5
#define SHT20_CMD_T_NHM    0XF3
#define SHT20_CMD_READ_USER_REG      0XE7
#define SHT20_CMD_WRITE_USER_REG     0XE6
#define SHT20_CMD_SOFT_RESET         0XFE

const int16_t POLYNOMIAL = 0x131;

SHT20_INFO sht20Info;


/**
 * @brief i2c master initialization
 */
esp_err_t sht20_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100000;
    i2c_param_config(i2c_master_port, &conf);
    return i2c_driver_install(i2c_master_port, conf.mode,
                              0,
                              0, 0);
    vTaskDelay(10 / portTICK_RATE_MS);
    SHT20_reset();
}

/**
 * @brief test code to read esp-i2c-slave
 *        We need to fill the buffer of esp slave device, then master can read them out.
 *
 * _______________________________________________________________________________________
 * | start | slave_addr + rd_bit +ack | read n-1 bytes + ack | read 1 byte + nack | stop |
 * --------|--------------------------|----------------------|--------------------|------|
 *
 */
static esp_err_t i2c_master_read_slave(i2c_port_t i2c_num, uint8_t *data_rd, size_t size)
{
    if (size == 0) {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SHT20_IIC_ADDRESS << 1) | READ_BIT, ACK_CHECK_EN);
    if (size > 1) {
        i2c_master_read(cmd, data_rd, size - 1, I2C_MASTER_ACK);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief Test code to write esp-i2c-slave
 *        Master device write data to slave(both esp32),
 *        the data will be stored in slave buffer.
 *        We can read them out from slave buffer.
 *
 * ___________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write n bytes + ack  | stop |
 * --------|---------------------------|----------------------|------|
 *
 */
static esp_err_t i2c_master_write_slave(i2c_port_t i2c_num, uint8_t *data_wr, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SHT20_IIC_ADDRESS << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}


/*
************************************************************
*	函数名称：	SHT20_reset
*
*	函数功能：	SHT20复位
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void SHT20_reset(void)
{
    //I2C_WriteByte(SHT20_IIC_ADDRESS, SHT20_SOFT_RESET, (void *)0);
	
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SHT20_IIC_ADDRESS << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, SHT20_CMD_SOFT_RESET, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
}

/*
************************************************************
*	函数名称：	SHT20_read_user_reg
*
*	函数功能：	SHT20读取用户寄存器
*
*	入口参数：	无
*
*	返回参数：	读取到的用户寄存器的值
*
*	说明：		
************************************************************
*/
unsigned char  SHT20_read_user_reg(void)
{
	
    unsigned char val = 0;
	
    //I2C_WriteByte(SHT20_IIC_ADDRESS, SHT20_READ_REG, &val);
	int ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, SHT20_IIC_ADDRESS << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, SHT20_CMD_READ_USER_REG, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        return ret;
    }
    vTaskDelay(30 / portTICK_RATE_MS);
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, SHT20_IIC_ADDRESS << 1 | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, &val, I2C_MASTER_ACK);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    return val;
	
}

/*
************************************************************
*	函数名称：	SHT2x_CheckCrc
*
*	函数功能：	检查数据正确性
*
*	入口参数：	data：读取到的数据
*				nbrOfBytes：需要校验的数量
*				checksum：读取到的校对比验值
*
*	返回参数：	校验结果
*
*	说明：		0-成功		1-失败
************************************************************
*/
uint8_t SHT2x_CheckCrc(uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum)
{
    uint8_t crc = 0;
    uint8_t bit = 0;
    uint8_t byteCtr = 0;
	
    //calculates 8-Bit checksum with given polynomial
    for(byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
    {
        crc ^= (data[byteCtr]);
        for ( bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
            else crc = (crc << 1);
        }
    }
	
    if(crc != checksum)
		return 1;
    else
		return 0;
	
}

/*
************************************************************
*	函数名称：	SHT2x_CalcTemperatureC
*
*	函数功能：	温度计算
*
*	入口参数：	u16sT：读取到的温度原始数据
*
*	返回参数：	计算后的温度数据
*
*	说明：		
************************************************************
*/
float SHT2x_CalcTemperatureC(unsigned short u16sT)
{
	
    float temperatureC = 0;            // variable for result

    u16sT &= ~0x0003;           // clear bits [1..0] (status bits)
    //-- calculate temperature [癈] --
    temperatureC = -46.85 + 175.72 / 65536 * (float)u16sT; //T= -46.85 + 175.72 * ST/2^16
	
    return temperatureC;
	
}

/*
************************************************************
*	函数名称：	SHT2x_CalcRH
*
*	函数功能：	湿度计算
*
*	入口参数：	u16sRH：读取到的湿度原始数据
*
*	返回参数：	计算后的湿度数据
*
*	说明：		
************************************************************
*/
float SHT2x_CalcRH(unsigned short u16sRH)
{
	
    float humidityRH = 0;              // variable for result
	
    u16sRH &= ~0x0003;          // clear bits [1..0] (status bits)
    //-- calculate relative humidity [%RH] --
    //humidityRH = -6.0 + 125.0/65536 * (float)u16sRH; // RH= -6 + 125 * SRH/2^16
    humidityRH = ((float)u16sRH * 0.00190735) - 6;
	
    return humidityRH;
	
}

/*
************************************************************
*	函数名称：	SHT2x_MeasureHM
*
*	函数功能：	测量温湿度
*
*	入口参数：	cmd：测量温度还是湿度
*				pMeasurand：不为空则保存为ushort值到此地址
*
*	返回参数：	测量结果
*
*	说明：		
************************************************************
*/
float SHT2x_MeasureHM(unsigned char readcmd, unsigned short *pMeasurand)
{
	
    uint8_t  checksum = 0;  //checksum
    uint8_t  data[2];    //data array for checksum verification
	esp_err_t ret;
    unsigned short tmp = 0;
    float t = 0;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

	i2c_master_start(cmd);
    i2c_master_write_byte(cmd, SHT20_IIC_ADDRESS << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, readcmd, ACK_CHECK_EN);
	
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        return ret;
    }
    
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, SHT20_IIC_ADDRESS << 1 | READ_BIT, ACK_CHECK_EN);
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);//printf("cmd_begin1 %x",ret);
    
    ret = i2c_master_read_byte(cmd, &data[0], I2C_MASTER_ACK);
    ret = i2c_master_read_byte(cmd, &data[1], I2C_MASTER_ACK);
    ret = i2c_master_read_byte(cmd, &checksum, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);//printf("cmd_begin2 %x\n",ret);
    i2c_cmd_link_delete(cmd);
    
    //i2c_master_write_slave(I2C_MASTER_NUM,&readcmd,1);
    vTaskDelay(100 / portTICK_RATE_MS);
    //printf("iic %x,%x,%x\n",data[0],data[1],checksum);
    
	ret = SHT2x_CheckCrc(data, 2, checksum);
    if (ret) {
        return ret;
    }
    tmp = (data[0] << 8) + data[1];
    if(readcmd == SHT20_CMD_T_HM)
    {
        t = SHT2x_CalcTemperatureC(tmp);
    }
    else
    {
        t = SHT2x_CalcRH(tmp);
    }
	
    if(pMeasurand)
    {
        *pMeasurand = (unsigned short)t;
    }
	
    return t;
	
}

/*
************************************************************
*	函数名称：	SHT20_GetValue
*
*	函数功能：	获取温湿度数据
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		温湿度结果保存在SHT20结构体里
************************************************************
*/
void SHT20_GetValue(void)
{
	
	unsigned char val = 0;
	
	//SHT20_read_user_reg();
	//vTaskDelay(10 / portTICK_PERIOD_MS);
	
	sht20Info.tempreture = SHT2x_MeasureHM(SHT20_CMD_T_HM, (void *)0);
	
	sht20Info.humidity = SHT2x_MeasureHM(SHT20_CMD_RH_HM, (void *)0);
	
	//SHT20_read_user_reg();
	//vTaskDelay(25 / portTICK_PERIOD_MS);
	
	//I2C_WriteByte(SHT20_IIC_ADDRESS, SHT20_WRITE_REG, &val);
    /*i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SHT20_IIC_ADDRESS << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, SHT20_WRITE_REG, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

	vTaskDelay(5 / portTICK_PERIOD_MS);*/
	
	//SHT20_read_user_reg();
	//vTaskDelay(50 / portTICK_PERIOD_MS);
	
	//SHT20_reset();
	

}



