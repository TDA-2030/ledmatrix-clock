


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "unistd.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"
#include "driver/ledc.h"

#include "led_matrix.h"
#include "oledfont.h"



//用来保存字库基本信息，地址，大小等
FONT_Info_TypeDef ftinfo;



#define GPIO_STCP      32
#define GPIO_SHCP      33
#define GPIO_OE        12
#define GPIO_SDA_R_TOP 25
#define GPIO_SDA_G_TOP 26
#define GPIO_SDA_R     27
#define GPIO_SDA_G     4

#define GPIO_A 13
#define GPIO_B 14
#define GPIO_C 15
#define GPIO_D 16

#define GPIO_OUTPUT_PIN_SEL ((1ULL << GPIO_STCP) | (1ULL << GPIO_SHCP) | (1ULL << GPIO_OE) | \
                            (1ULL << GPIO_SDA_R_TOP) | (1ULL << GPIO_SDA_G_TOP) | (1ULL << GPIO_SDA_R) | (1ULL << GPIO_SDA_G) | \
                            (1ULL << GPIO_A) | (1ULL << GPIO_B) | (1ULL << GPIO_C) | (1ULL << GPIO_D))



//输出宏定义
#define STCP_HIGH	   GPIO.out1_w1ts.data = (1 << (GPIO_STCP-32))
#define STCP_LOW       GPIO.out1_w1tc.data = (1 << (GPIO_STCP-32))

#define SHCP_HIGH      GPIO.out1_w1ts.data = (1 << (GPIO_SHCP-32))
#define SHCP_LOW	   GPIO.out1_w1tc.data = (1 << (GPIO_SHCP-32))

#define OE_HIGH		   GPIO.out_w1ts = (1 << GPIO_OE)
#define OE_LOW         GPIO.out_w1tc = (1 << GPIO_OE)

#define SDA_R_TOP_HIGH GPIO.out_w1ts = (1 << GPIO_SDA_R_TOP)
#define SDA_R_TOP_LOW  GPIO.out_w1tc = (1 << GPIO_SDA_R_TOP)

#define SDA_G_TOP_HIGH GPIO.out_w1ts = (1 << GPIO_SDA_G_TOP)
#define SDA_G_TOP_LOW  GPIO.out_w1tc = (1 << GPIO_SDA_G_TOP)

#define SDA_R_HIGH	   GPIO.out_w1ts = (1 << GPIO_SDA_R)
#define SDA_R_LOW	   GPIO.out_w1tc = (1 << GPIO_SDA_R)

#define SDA_G_HIGH	   GPIO.out_w1ts = (1 << GPIO_SDA_G)
#define SDA_G_LOW	   GPIO.out_w1tc = (1 << GPIO_SDA_G)



//设置扫描的译码器 //32,33,34,35
#define SET_DATATFT(dat)	{GPIO.out_w1tc = 0x1e000;GPIO.out_w1ts = (dat);}


//屏幕显示缓存
LED_Matrix_Buf_TypeDef LED_Matrix_Buf;
static uint32_t led_scan_line[16]={0};/*16行段码*/



//屏幕的画笔颜色和背景色	   
uint16_t POINT_COLOR = 0x01;	//画笔颜色
uint16_t BACK_COLOR  = 0x00;  //背景色 

/** 
* @brief  : 定时刷新屏幕
* @param  : 
* @param  : 
* @return : 
*/
void IRAM_ATTR led_matrix_flash(void)
{
	uint8_t dat1,dat2,dat3,dat4,i,G;
	static uint8_t LINE;

		if(LINE>15)LINE = 0;
		
		for	(G=0;G<8;G++)	//往点阵屏填充 一行的 数据
		{
			dat1 = LED_Matrix_Buf.R_Buf[G][LINE];
			dat2 = LED_Matrix_Buf.R_Buf[G][LINE+16];
			dat3 = LED_Matrix_Buf.G_Buf[G][LINE];
			dat4 = LED_Matrix_Buf.G_Buf[G][LINE+16];
			for(i=0;i<8;i++)
			{
				if(dat1 & 0x01)SDA_R_TOP_LOW;
				else SDA_R_TOP_HIGH;
				
				if(dat2 & 0x01)SDA_R_LOW;
				else SDA_R_HIGH;
				
				if(dat3 & 0x01)SDA_G_TOP_LOW;
				else SDA_G_TOP_HIGH;
				
				if(dat4 & 0x01)SDA_G_LOW;
				else SDA_G_HIGH;
				
				dat1>>=1;
				dat2>>=1;
				dat3>>=1;
				dat4>>=1;
				
				SHCP_HIGH;
				SHCP_LOW;
			}
		}
		
		SET_DATATFT(led_scan_line[LINE]);
		
		//OE_LOW;
        //TIM4->CR1|=0x01;    //使能定时器4,通过延时来减少亮的时间，起到调节亮度的作用
		//OE_OFF;
		
		STCP_HIGH;
		STCP_LOW;
		
		LINE++;
}


//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void LedMatrix_Clear(void)
{
	uint8_t i,j;
	
	for(i=0;i<LED_MATRIX_MAX_HEIGHT;i++)//把R,G缓冲区全部数据清0
	{
		for(j=0;j<LED_MATRIX_MAX_WIDTH/8;j++)
		{
			LED_Matrix_Buf.R_Buf[j][i] = 0;
			LED_Matrix_Buf.G_Buf[j][i] = 0;
		}
	}
}


//画点
//x:0~63
//y:0~31
//t:1 - 红色, 0 - 清空, 2 - 绿色, 3 - 黄色
void LedMatrix_DrawPoint(uint16_t x,uint16_t y,uint16_t t)
{
	uint8_t pos,bx;
	
	if(x>64||y>32)return;//超出范围了.
	pos=x/8;
	bx=x%8;
	
	if(t == 0x01)
	{
		LED_Matrix_Buf.R_Buf[pos][y] |= (1<<bx);
		LED_Matrix_Buf.G_Buf[pos][y] &= ~(1<<bx);
	}
	else if(t == 0x02)
	{
		LED_Matrix_Buf.R_Buf[pos][y] &= ~(1<<bx);
		LED_Matrix_Buf.G_Buf[pos][y] |= (1<<bx);
	}
	else if(t == 0x03)
	{
		LED_Matrix_Buf.R_Buf[pos][y] |= (1<<bx);
		LED_Matrix_Buf.G_Buf[pos][y] |= (1<<bx);
	}
    else
	{
		LED_Matrix_Buf.R_Buf[pos][y] &= ~(1<<bx);
		LED_Matrix_Buf.G_Buf[pos][y] &= ~(1<<bx);
	}
}



//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void LedMatrix_Fill(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t dot)  
{  
	uint8_t x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)LedMatrix_DrawPoint(x,y,dot);
	}
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 12/16/24
void LedMatrix_ShowChar(uint16_t x,uint16_t y,uint8_t chr,uint8_t size)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y;
	uint8_t csize;
	csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	if(8 == size) csize=6;
	chr=chr-' ';//得到偏移后的值
    for(t=0;t<csize;t++)
    {   
		if(size==8)temp=asc2_0806[chr][t];          //调用0608字体
		else if(size==12)temp=asc2_1206[chr][t]; 	//调用1206字体
		else if(size==16)temp=asc2_1608[chr][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[chr][t];	//调用2412字体
		else return;								//没有的字库
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LedMatrix_DrawPoint(x,y,POINT_COLOR);
			else LedMatrix_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
//m^n函数
static uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void LedMatrix_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint16_t t,temp;
	uint8_t enshow=0;
	uint16_t offset;
	
	if(size==8) offset=5;
	else offset=	size/2;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LedMatrix_ShowChar(x+(offset*t),y,'0',size);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LedMatrix_ShowChar(x+(offset*t),y,temp+'0',size); 
		//x+=offset;
	}
}

//显示字符串
//x,y:起点坐标
//size:字体大小
//*p:字符串起始地址
void LedMatrix_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint8_t size)
{
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {
        //if(x>(128-(size/2))){x=0;y+=size;}
        //if(y>(64-size)){y=x=0;OLED_Clear();}
        LedMatrix_ShowChar(x,y,*p,size);
		if(8 == size)x+=6;
		else  x+=size/2;
        p++;
    }
}


/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void LedMatrix_DrawBMP(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1,const uint8_t BMP[])
{
	unsigned char x,y;
	
//	if(y1%8==0) y=y1/8;
//	else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{	
		for(x=x0;x<x1;x++)
		{
			//OLED_GRAM[x][y] = BMP[j++];
		}
	}
}

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标
void LedMatrix_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向
	else if(delta_x==0)incx=0;//垂直线
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if(delta_y==0)incy=0;//水平线
	else{incy=-1;delta_y=-delta_y;}
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
	else distance=delta_y;
	for(t=0;t<=distance+1;t++ )//画线输出
	{
		LedMatrix_DrawPoint(uRow,uCol,POINT_COLOR);//画点 
		xerr+=delta_x ;
		yerr+=delta_y ;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LedMatrix_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LedMatrix_DrawLine(x1,y1,x2,y1);
	LedMatrix_DrawLine(x1,y1,x1,y2);
	LedMatrix_DrawLine(x1,y2,x2,y2);
	LedMatrix_DrawLine(x2,y1,x2,y2);
}

//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LedMatrix_Draw_Circle(uint16_t x0,uint16_t y0,uint16_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LedMatrix_DrawPoint(x0+a,y0-b,POINT_COLOR);             //5
 		LedMatrix_DrawPoint(x0+b,y0-a,POINT_COLOR);             //0           
		LedMatrix_DrawPoint(x0+b,y0+a,POINT_COLOR);             //4               
		LedMatrix_DrawPoint(x0+a,y0+b,POINT_COLOR);             //6 
		LedMatrix_DrawPoint(x0-a,y0+b,POINT_COLOR);             //1       
 		LedMatrix_DrawPoint(x0-b,y0+a,POINT_COLOR);             
		LedMatrix_DrawPoint(x0-a,y0-b,POINT_COLOR);             //2             
  		LedMatrix_DrawPoint(x0-b,y0-a,POINT_COLOR);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	 
		else
		{
			di+=10+4*(a-b);   
			b--;
		}
	}
}



// //code 字符指针开始
// //从字库中查找出字模
// //code 字符串的开始地址,GBK码
// //mat  数据存放地址 (size/8+((size%8)?1:0))*(size) bytes大小	
// //size:字体大小
// void Get_HzMat(unsigned char *code,unsigned char *mat,uint8_t size)
// {		    
// 	unsigned char qh,ql;
// 	unsigned char i;
// 	unsigned long foffset;
// 	uint8_t csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数	 
// 	qh=*code;
// 	ql=*(++code);
// 	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//非 常用汉字
// 	{   		    
// 	    for(i=0;i<csize;i++)*mat++=0x00;//填充满格
// 	    return; //结束访问
// 	}          
// 	if(ql<0x7f)ql-=0x40;//注意!
// 	else ql-=0x41;
// 	qh-=0x81;   
// 	foffset=((unsigned long)190*qh+ql)*csize;	//得到字库中的字节偏移量  		  
// 	switch(size)
// 	{
// 		case 12:
// 			W25QXX_Read(mat,foffset+ftinfo.f12addr,csize);
// 			break;
// 		case 16:
// 			W25QXX_Read(mat,foffset+ftinfo.f16addr,csize);
// 			break;
// 		case 24:
// 			W25QXX_Read(mat,foffset+ftinfo.f24addr,csize);
// 			break;
// 		case 32:
// 			W25QXX_Read(mat,foffset+ftinfo.f32addr,csize);
// 			break;
// 		default:break;
// 	}
// }  
// //显示一个指定大小的汉字
// //x,y :汉字的坐标
// //font:汉字GBK码
// //size:字体大小
// //mode:0,正常显示,1,叠加显示	   
// void Show_Font(uint16_t x,uint16_t y,uint8_t *font,uint8_t size,uint8_t mode)
// {
// 	uint8_t temp,t,t1;
// 	uint16_t y0=y;
// 	uint8_t dzk[128];
// 	uint8_t csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数	
	
// 	if(size!=12&&size!=16&&size!=24&&size!=32)return;	//不支持的size
// 	Get_HzMat(font,dzk,size);	//得到相应大小的点阵数据 

// 	for(t=0;t<csize;t++)
// 	{   												   
// 		temp=dzk[t];			//得到点阵数据                          
// 		for(t1=0;t1<8;t1++)
// 		{
// 			if(temp&0x80)LedMatrix_DrawPoint(x,y,POINT_COLOR);
// 			else if(mode==0)LedMatrix_DrawPoint(x,y,BACK_COLOR); 
// 			temp<<=1;
// 			y++;
// 			if((y-y0)==size)
// 			{
// 				y=y0;
// 				x++;
// 				break;
// 			}
// 		}
// 	}
// }

// //在指定位置开始显示一个字符串	    
// //支持自动换行
// //(x,y):起始坐标
// //width,height:区域
// //str  :字符串
// //size :字体大小
// //mode:0,非叠加方式;1,叠加方式    	   		   
// void Show_Str(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t*str,uint8_t size,uint8_t mode)
// {					
// 	uint16_t x0=x;
// 	uint16_t y0=y;							  	  
//     uint8_t bHz=0;     //字符或者中文  
	
	
//     while(*str!=0)//数据未结束
//     { 
//         if(!bHz)
//         {
// 	        if(*str>0x80)bHz=1;//中文 
// 	        else              //字符
// 	        {
//                 if(x>(x0+width-size/2))//换行
// 				{				   
// 					y+=size;
// 					x=x0;	   
// 				}							    
// 		        if(y>(y0+height-size))break;//越界返回      
// 		        if(*str==13)//换行符号
// 		        {         
// 		            y+=size;
// 					x=x0;
// 		            str++; 
// 		        }  
// 		        else LedMatrix_ShowChar(x,y,*str,size);//有效部分写入 
// 				str++; 
// 		        x+=size/2; //字符,为全字的一半 
// 	        }
//         }else//中文 
//         {     
//             bHz=0;//有汉字库    
//             if(x>(x0+width-size))//换行
// 			{	    
// 				y+=size;
// 				x=x0;		  
// 			}
// 	        if(y>(y0+height-size))break;//越界返回  						     
// 	        Show_Font(x,y,str,size,mode); //显示这个汉字,空心显示 
// 	        str+=2; 
// 	        x+=size;//下一个汉字偏移	    
//         }						 
//     }   
// }  		




#define TIMER_DIVIDER         16  //  Hardware timer clock divider
#define TIMER_SCALE           (TIMER_BASE_CLK / TIMER_DIVIDER)  // convert counter value to seconds

/*
 * Timer group0 ISR handler
 *
 * Note:
 * We don't call the timer API here because they are not declared with IRAM_ATTR.
 * If we're okay with the timer irq not being serviced while SPI flash cache is disabled,
 * we can allocate this interrupt without the ESP_INTR_FLAG_IRAM flag and use the normal API.
 */
static void IRAM_ATTR timer_group0_isr(void *para)
{
    int timer_idx = (int) para;

    /* Retrieve the interrupt status and the counter value
       from the timer that reported the interrupt */
    uint32_t intr_status = TIMERG0.int_st_timers.val;
    TIMERG0.hw_timer[timer_idx].update = 1;
    
    /* Clear the interrupt
       and update the alarm time for the timer with without reload */
    if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_1) {
        TIMERG0.int_clr_timers.t1 = 1;
		led_matrix_flash();
    } else {
        
    }

    /* After the alarm has been triggered
      we need enable it again, so it is triggered the next time */
    TIMERG0.hw_timer[timer_idx].config.alarm_en = TIMER_ALARM_EN;
}

/*
 * Initialize selected timer of the timer group 0
 *
 * timer_idx - the timer number to initialize
 * auto_reload - should the timer auto reload on alarm?
 * timer_interval_sec - the interval of alarm to set
 */
static void example_tg0_timer_init(int timer_idx, 
    bool auto_reload,
	double timer_interval_sec,
	void (*isr_fn)(void*))
{
    /* Select and initialize basic parameters of the timer */
    timer_config_t config;
    config.divider = TIMER_DIVIDER;
    config.counter_dir = TIMER_COUNT_UP;
    config.counter_en = TIMER_PAUSE;
    config.alarm_en = TIMER_ALARM_EN;
    config.intr_type = TIMER_INTR_LEVEL;
    config.auto_reload = auto_reload;
    timer_init(TIMER_GROUP_0, timer_idx, &config);

    /* Timer's counter will initially start from value below.
       Also, if auto_reload is set, this value will be automatically reload on alarm */
    timer_set_counter_value(TIMER_GROUP_0, timer_idx, 0x00000000ULL);

    /* Configure the alarm value and the interrupt on alarm. */
    timer_set_alarm_value(TIMER_GROUP_0, timer_idx, timer_interval_sec * TIMER_SCALE);
    timer_enable_intr(TIMER_GROUP_0, timer_idx);
    timer_isr_register(TIMER_GROUP_0, timer_idx, isr_fn, 
        (void *) timer_idx, ESP_INTR_FLAG_IRAM, NULL);

    timer_start(TIMER_GROUP_0, timer_idx);
}


#define LEDC_HS_TIMER          LEDC_TIMER_0
#define LEDC_HS_MODE           LEDC_HIGH_SPEED_MODE
#define LEDC_HS_CH0_GPIO       (GPIO_OE)
#define LEDC_HS_CH0_CHANNEL    LEDC_CHANNEL_0
static ledc_channel_config_t ledc_channel = {
		.channel    = LEDC_HS_CH0_CHANNEL,
		.duty       = 0,
		.gpio_num   = LEDC_HS_CH0_GPIO,
		.speed_mode = LEDC_HS_MODE,
		.hpoint     = 0,
		.timer_sel  = LEDC_HS_TIMER
	};
static void ledc_init(void)
{
	/*
     * Prepare and set configuration of timers
     * that will be used by LED Controller
     */
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_10_BIT, // resolution of PWM duty
        .freq_hz = 5000,                      // frequency of PWM signal
        .speed_mode = LEDC_HS_MODE,           // timer mode
        .timer_num = LEDC_HS_TIMER,            // timer index
        .clk_cfg = LEDC_AUTO_CLK,              // Auto select the source clock
    };
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);

	ledc_channel_config(&ledc_channel);
}

void LedMatrix_SetLight(uint32_t val)
{
	if(val > 100)
	{
		val = 100;
	}
	val = 100 - val;
	ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, val*1024/100);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
}




static void gpio_task_example(void *arg)
{

    for (;;)
    {	
        vTaskDelay(2000 / portTICK_RATE_MS);
		
    }
}

void LedMatrix_init(void)
{
	int8_t j; 
    for(uint8_t i=0;i<16;i++)
    {
		j=i-1;//因为屏幕的电路上扫描线做了偏移，这样也也偏移一下
		if(j<0)
		{
			j+=16;
		}
        led_scan_line[j] = i<<GPIO_A;
    }
    

    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

	LedMatrix_Clear();
	example_tg0_timer_init(TIMER_1, 1, 0.001f, timer_group0_isr);
	OE_LOW;

	ledc_init();
	LedMatrix_SetLight(100);

	for(uint8_t i=0;i<12;i+=2)
	{
		LedMatrix_DrawRectangle(i, i, LED_MATRIX_MAX_WIDTH-1-i, LED_MATRIX_MAX_HEIGHT-1-i);
	}


    //start gpio task
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

 
}










