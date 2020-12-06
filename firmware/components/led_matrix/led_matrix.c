#include "math.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/timer.h"
#include "driver/ledc.h"

#include "led_matrix.h"
#include "oledfont.h"


#define GPIO_STCP      18
#define GPIO_SHCP      19
#define GPIO_OE        23
#define GPIO_SDA_R1    22
#define GPIO_SDA_G1    27
#define GPIO_SDA_R2    21
#define GPIO_SDA_G2    32

#define GPIO_A         12
#define GPIO_B         13
#define GPIO_C         14
#define GPIO_D         15

#define GPIO_OUTPUT_PIN_SEL ((1ULL << GPIO_STCP) | (1ULL << GPIO_SHCP) | (1ULL << GPIO_OE) | \
                            (1ULL << GPIO_SDA_R1) | (1ULL << GPIO_SDA_G1) | (1ULL << GPIO_SDA_R2) | (1ULL << GPIO_SDA_G2) | \
                            (1ULL << GPIO_A) | (1ULL << GPIO_B) | (1ULL << GPIO_C) | (1ULL << GPIO_D))



//输出宏定义
#define STCP_HIGH      GPIO.out_w1ts = (1ULL << (GPIO_STCP))
#define STCP_LOW       GPIO.out_w1tc = (1ULL << (GPIO_STCP))

#define SHCP_HIGH      GPIO.out_w1ts = (1ULL << (GPIO_SHCP))
#define SHCP_LOW       GPIO.out_w1tc = (1ULL << (GPIO_SHCP))

#define OE_HIGH        GPIO.out_w1ts = (1ULL << GPIO_OE)
#define OE_LOW         GPIO.out_w1tc = (1ULL << GPIO_OE)

#define SDA_R_TOP_HIGH GPIO.out_w1ts = (1ULL << GPIO_SDA_R1)
#define SDA_R_TOP_LOW  GPIO.out_w1tc = (1ULL << GPIO_SDA_R1)

#define SDA_G_TOP_HIGH GPIO.out_w1ts = (1ULL << GPIO_SDA_G1)
#define SDA_G_TOP_LOW  GPIO.out_w1tc = (1ULL << GPIO_SDA_G1)

#define SDA_R_HIGH     GPIO.out_w1ts = (1ULL << (GPIO_SDA_R2))
#define SDA_R_LOW      GPIO.out_w1tc = (1ULL << (GPIO_SDA_R2))

#define SDA_G_HIGH     GPIO.out1_w1ts.data = (1ULL << (GPIO_SDA_G2-32))
#define SDA_G_LOW      GPIO.out1_w1tc.data = (1ULL << (GPIO_SDA_G2-32))


#define SET_LINE(dat)   {GPIO.out_w1tc = 0xf000;GPIO.out_w1ts = (dat);}

//屏幕显示缓存
typedef struct {
    uint8_t R_Buf[LED_MATRIX_MAX_WIDTH / 8][LED_MATRIX_MAX_HEIGHT];
    uint8_t G_Buf[LED_MATRIX_MAX_WIDTH / 8][LED_MATRIX_MAX_HEIGHT];
} LED_Matrix_Buf_t;

//屏幕显示缓存
static LED_Matrix_Buf_t LED_Matrix_Buf;
static uint32_t led_scan_line[16] = {0}; /*16行段码*/



// 定时刷新屏幕
void IRAM_ATTR led_matrix_flush(void)
{
    uint8_t dat1, dat2, dat3, dat4, i, G;
    static uint8_t LINE;

    if (LINE > 15) {
        LINE = 0;
    }

    for (G = 0; G < 8; G++) { //往点阵屏填充 一行的 数据
        dat1 = LED_Matrix_Buf.R_Buf[G][LINE];
        dat2 = LED_Matrix_Buf.R_Buf[G][LINE + 16];
        dat3 = LED_Matrix_Buf.G_Buf[G][LINE];
        dat4 = LED_Matrix_Buf.G_Buf[G][LINE + 16];
        for (i = 0; i < 8; i++) {
            if (dat1 & 0x01) {
                SDA_R_TOP_LOW;
            } else {
                SDA_R_TOP_HIGH;
            }

            if (dat2 & 0x01) {
                SDA_R_LOW;
            } else {
                SDA_R_HIGH;
            }

            if (dat3 & 0x01) {
                SDA_G_TOP_LOW;
            } else {
                SDA_G_TOP_HIGH;
            }

            if (dat4 & 0x01) {
                SDA_G_LOW;
            } else {
                SDA_G_HIGH;
            }

            dat1 >>= 1;
            dat2 >>= 1;
            dat3 >>= 1;
            dat4 >>= 1;

            SHCP_HIGH;
            SHCP_LOW;
        }
    }

    SET_LINE(led_scan_line[LINE]);

    STCP_HIGH;
    STCP_LOW;

    LINE++;
}

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
        led_matrix_flush();
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
static void hw_tg0_timer_init(int timer_idx,
                              bool auto_reload,
                              double timer_interval_sec,
                              void (*isr_fn)(void *))
{
    /* Select and initialize basic parameters of the timer */
    timer_config_t config;
    config.divider = 16;
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
    timer_set_alarm_value(TIMER_GROUP_0, timer_idx, timer_interval_sec * (TIMER_BASE_CLK / config.divider));
    timer_enable_intr(TIMER_GROUP_0, timer_idx);
    timer_isr_register(TIMER_GROUP_0, timer_idx, isr_fn,
                       (void *) timer_idx, ESP_INTR_FLAG_IRAM, NULL);

    timer_start(TIMER_GROUP_0, timer_idx);
}

static ledc_channel_config_t ledc_channel = {
    .channel    = LEDC_CHANNEL_0,
    .duty       = 0,
    .gpio_num   = GPIO_OE,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .hpoint     = 0,
    .timer_sel  = LEDC_TIMER_0
};
static void ledc_init(void)
{
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_12_BIT, // resolution of PWM duty
        .freq_hz = 5000,                      // frequency of PWM signal
        .speed_mode = LEDC_HIGH_SPEED_MODE,           // timer mode
        .timer_num = LEDC_TIMER_0,            // timer index
        .clk_cfg = LEDC_USE_APB_CLK,              // Auto select the source clock
    };

    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);
    ledc_channel_config(&ledc_channel);
}

void LedMatrix_SetLight(uint32_t val)
{
    float set;
    float input= (float)val / 4095.0f;
    set = powf(input, 2);
    val = (uint32_t)(set * 4095.0f);

    if (val > 4095) {
        val = 4095;
    }
    val = 4095 - val;
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, val);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
}

void LedMatrix_init(void)
{
    int8_t j;
    for (uint8_t i = 0; i < 16; i++) {
        j = i;
        if (j < 0) {
            j += 16;
        }
        led_scan_line[j] = i << GPIO_A;
    }

    gpio_config_t io_conf={0};
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    gpio_config(&io_conf);

    LedMatrix_Clear();
    hw_tg0_timer_init(TIMER_1, true, 0.0005f, timer_group0_isr);
    OE_LOW;

    ledc_init();
    LedMatrix_SetLight(20);

    LedMatrix_Fill(0, 0, 63, 31, COLOR_BLACK);
}


//清屏函数
void LedMatrix_Clear(void)
{
    uint8_t i, j;

    for (i = 0; i < LED_MATRIX_MAX_HEIGHT; i++) { //把R,G缓冲区全部数据清0
        for (j = 0; j < LED_MATRIX_MAX_WIDTH / 8; j++) {
            LED_Matrix_Buf.R_Buf[j][i] = 0;
            LED_Matrix_Buf.G_Buf[j][i] = 0;
        }
    }
}


//画点
//x:0~63
//y:0~31
//t:1 - 红色, 0 - 清空, 2 - 绿色, 3 - 黄色
void LedMatrix_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    uint8_t pos, bx;

    if (x > LED_MATRIX_MAX_WIDTH || y > LED_MATRIX_MAX_HEIGHT) {
        return;    //超出范围了.
    }
    pos = x / 8;
    bx = x % 8;

    if (color == 0x01) {
        LED_Matrix_Buf.R_Buf[pos][y] |= (1 << bx);
        LED_Matrix_Buf.G_Buf[pos][y] &= ~(1 << bx);
    } else if (color == 0x02) {
        LED_Matrix_Buf.R_Buf[pos][y] &= ~(1 << bx);
        LED_Matrix_Buf.G_Buf[pos][y] |= (1 << bx);
    } else if (color == 0x03) {
        LED_Matrix_Buf.R_Buf[pos][y] |= (1 << bx);
        LED_Matrix_Buf.G_Buf[pos][y] |= (1 << bx);
    } else {
        LED_Matrix_Buf.R_Buf[pos][y] &= ~(1 << bx);
        LED_Matrix_Buf.G_Buf[pos][y] &= ~(1 << bx);
    }
}


//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dot:0,清空;1,填充
void LedMatrix_Fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dot)
{
    uint8_t x, y;
    for (x = x1; x <= x2; x++) {
        for (y = y1; y <= y2; y++) {
            LedMatrix_DrawPoint(x, y, dot);
        }
    }
}


void LedMatrix_DrawBMP(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, const uint8_t *BMP)
{
    uint16_t x, y;
    uint16_t x1 = x0 + width ;
    uint16_t y1 = y0 + height ;

    for (y = y0; y < y1; y++) {
        for (x = x0; x < x1; x++) {
            LedMatrix_DrawPoint(x, y, *BMP++);
        }
    }
}


//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
// void LedMatrix_Draw_Circle(uint16_t x0, uint16_t y0, uint16_t r)
// {
//     int a, b;
//     int di;
//     a = 0; b = r;
//     di = 3 - (r << 1);       //判断下个点位置的标志
//     while (a <= b) {
//         LedMatrix_DrawPoint(x0 + a, y0 - b, POINT_COLOR);       //5
//         LedMatrix_DrawPoint(x0 + b, y0 - a, POINT_COLOR);       //0
//         LedMatrix_DrawPoint(x0 + b, y0 + a, POINT_COLOR);       //4
//         LedMatrix_DrawPoint(x0 + a, y0 + b, POINT_COLOR);       //6
//         LedMatrix_DrawPoint(x0 - a, y0 + b, POINT_COLOR);       //1
//         LedMatrix_DrawPoint(x0 - b, y0 + a, POINT_COLOR);
//         LedMatrix_DrawPoint(x0 - a, y0 - b, POINT_COLOR);       //2
//         LedMatrix_DrawPoint(x0 - b, y0 - a, POINT_COLOR);       //7
//         a++;
//         //使用Bresenham算法画圆
//         if (di < 0) {
//             di += 4 * a + 6;
//         } else {
//             di += 10 + 4 * (a - b);
//             b--;
//         }
//     }
// }


