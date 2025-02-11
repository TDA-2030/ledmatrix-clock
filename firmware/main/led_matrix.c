#include "math.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "hal/gpio_hal.h"
#include "driver/gptimer.h"
#include "driver/ledc.h"
#include "led_matrix.h"

static const char *TAG = "LED_MATRIX";

#if 0
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

#else
#define GPIO_STCP      21
#define GPIO_SHCP      22
#define GPIO_OE        32
#define GPIO_SDA_R1    27
#define GPIO_SDA_G1    18
#define GPIO_SDA_R2    23
#define GPIO_SDA_G2    19

#define GPIO_A         12
#define GPIO_B         13
#define GPIO_C         14
#define GPIO_D         15
#endif

#define GPIO_OUTPUT_PIN_SEL ((1ULL << GPIO_STCP) | (1ULL << GPIO_SHCP) | (1ULL << GPIO_OE) | \
                            (1ULL << GPIO_SDA_R1) | (1ULL << GPIO_SDA_G1) | (1ULL << GPIO_SDA_R2) | (1ULL << GPIO_SDA_G2) | \
                            (1ULL << GPIO_A) | (1ULL << GPIO_B) | (1ULL << GPIO_C) | (1ULL << GPIO_D))

//输出宏定义
#define STCP_HIGH      gpio_ll_set_level(&GPIO, GPIO_STCP, 1)
#define STCP_LOW       gpio_ll_set_level(&GPIO, GPIO_STCP, 0)

#define SHCP_HIGH      gpio_ll_set_level(&GPIO, GPIO_SHCP, 1)
#define SHCP_LOW       gpio_ll_set_level(&GPIO, GPIO_SHCP, 0)

#define SDA_R_TOP_HIGH gpio_ll_set_level(&GPIO, GPIO_SDA_R1, 1)
#define SDA_R_TOP_LOW  gpio_ll_set_level(&GPIO, GPIO_SDA_R1, 0)

#define SDA_G_TOP_HIGH gpio_ll_set_level(&GPIO, GPIO_SDA_G1, 1)
#define SDA_G_TOP_LOW  gpio_ll_set_level(&GPIO, GPIO_SDA_G1, 0)

#define SDA_R_HIGH     gpio_ll_set_level(&GPIO, GPIO_SDA_R2, 1)
#define SDA_R_LOW      gpio_ll_set_level(&GPIO, GPIO_SDA_R2, 0)

#define SDA_G_HIGH     gpio_ll_set_level(&GPIO, GPIO_SDA_G2, 1)
#define SDA_G_LOW      gpio_ll_set_level(&GPIO, GPIO_SDA_G2, 0)

#define SET_LINE(dat)   {GPIO.out_w1tc = 0xf000;GPIO.out_w1ts = (dat);}

#ifndef TIMER_BASE_CLK
#define TIMER_BASE_CLK 80000000
#endif

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


static bool IRAM_ATTR on_gptimer_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
{
    led_matrix_flush();
    return false;
}

/*
 * Initialize selected timer of the timer group 0
 *
 * freq_hz - line refresh frequency
 */
static void hw_tg0_timer_init(uint32_t freq_hz)
{
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1 * 1000 * 1000, // 1MHz, 1 tick=1us
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_gptimer_alarm_cb,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));


    ESP_LOGI(TAG, "Start timer");
    gptimer_alarm_config_t alarm_config = {
        .reload_count = 0,
        .alarm_count = 1000000 / freq_hz,
        .flags.auto_reload_on_alarm = true,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));
    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));
}

static const ledc_channel_config_t ledc_channel = {
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
        .duty_resolution = LEDC_TIMER_8_BIT, // resolution of PWM duty
        .freq_hz = 20000,                      // frequency of PWM signal
        .speed_mode = LEDC_HIGH_SPEED_MODE,           // timer mode
        .timer_num = LEDC_TIMER_0,            // timer index
        .clk_cfg = LEDC_USE_APB_CLK,              // Auto select the source clock
    };

    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);
    ledc_channel_config(&ledc_channel);
}

/**
 * @brief Set the light of the led matrix
 *
 * @param val Range: 0~100
 */
void LedMatrix_SetLight(uint32_t val)
{
    if (val > 100) {
        val = 100;
    }
    ESP_LOGI(TAG, "Set light: %d", val);
    val = (val * 255 / 100);

    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, val);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
}

void LedMatrix_init(void)
{
    led_scan_line[0 ] = 0x1 << GPIO_A;
    led_scan_line[1 ] = 0x2 << GPIO_A;
    led_scan_line[2 ] = 0x3 << GPIO_A;
    led_scan_line[3 ] = 0x4 << GPIO_A;
    led_scan_line[4 ] = 0x5 << GPIO_A;
    led_scan_line[5 ] = 0x6 << GPIO_A;
    led_scan_line[6 ] = 0x7 << GPIO_A;
    led_scan_line[7 ] = 0x8 << GPIO_A;
    led_scan_line[8 ] = 0x9 << GPIO_A;
    led_scan_line[9 ] = 0xa << GPIO_A;
    led_scan_line[10] = 0xb << GPIO_A;
    led_scan_line[11] = 0xc << GPIO_A;
    led_scan_line[12] = 0xd << GPIO_A;
    led_scan_line[13] = 0xe << GPIO_A;
    led_scan_line[14] = 0xf << GPIO_A;
    led_scan_line[15] = 0x0 << GPIO_A;;

    gpio_config_t io_conf = {0};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    gpio_config(&io_conf);

    LedMatrix_Clear();
    hw_tg0_timer_init(1000);

    ledc_init();
    LedMatrix_SetLight(2);

    LedMatrix_Fill(0, 0, 63, 31, 0);
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
//color: RGB233格式
void LedMatrix_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    uint8_t pos, bx;

    if (x > LED_MATRIX_MAX_WIDTH || y > LED_MATRIX_MAX_HEIGHT) {
        return;    //超出范围了.
    }
    pos = x / 8;
    bx = x % 8;

    uint8_t mask = 1 << bx;
    if (color & 0xc0) {
        LED_Matrix_Buf.R_Buf[pos][y] |= mask;
    } else {
        LED_Matrix_Buf.R_Buf[pos][y] &= ~mask;
    }
    if (color & 0x38) {
        LED_Matrix_Buf.G_Buf[pos][y] |= mask;
    } else {
        LED_Matrix_Buf.G_Buf[pos][y] &= ~mask;
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
