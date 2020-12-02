
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "iot_fonts.h"
#include "esp_err.h"
#include "esp_log.h"

static const char *TAG = "font16_gbk";

#define FONT_GBK 1

#define WIDTH 16
#define HEIGHT 16

static int get_gbk_fontdata(const char *code, uint8_t *mat, uint16_t *len)
{
    uint8_t font_size = 16;
    uint8_t qh, ql;
    uint8_t i;
    uint32_t foffset;
    char file_path[64] = {0};

    uint8_t csize = HEIGHT * (WIDTH / 8 + (WIDTH % 8 ? 1 : 0)); //得到字体一个字符对应点阵集所占的字节数
    qh = *code;
    ql = *(++code);
#if FONT_GBK
    if (qh < 0x81 || ql < 0x40 || ql == 0xff || qh == 0xff) { //非 常用汉字
        for (i = 0; i < csize; i++) {
            *mat++ = 0x00;    //填充满格
        }
        ESP_LOGW(TAG, "Uncommon words 0x%04x", qh<<8|ql);
        return ESP_OK;            //结束访问
    }
    if (ql < 0x7f) {
        ql -= 0x40;    //注意!
    } else {
        ql -= 0x41;
    }
    qh -= 0x81;
    foffset = ((uint32_t)190 * qh + ql) * csize; //得到字库中的字节偏移量
#elif FONT_GB2312
    ql -= 0xa1;
    qh -= 0xa1;
    foffset = ((uint32_t)94 * qh + ql) * csize;
#endif

    ESP_LOGD(TAG, "code=[%d,%d], csize=%d, foffset=%d", qh, ql, csize, foffset);

    switch (font_size) {
    case 16:
        sprintf(file_path, "%s/%s", "/spiffs", "16x16.DZK");
        break;
    default:
        ESP_LOGE(TAG, "can't find font file");
        return ESP_FAIL;
        break;
    }

    FILE *fd;
    fd = fopen(file_path, "rb");
    if (!fd) {
        ESP_LOGE(TAG, "Failed to read file : %s", file_path);
        return ESP_FAIL;
    }

    size_t chunksize = 0;
    if (0 != fseek(fd, foffset, SEEK_SET)) {
        ESP_LOGE(TAG, "file fseek failed");
        fclose(fd);
        return ESP_FAIL;
    }
    chunksize = fread(mat, 1, csize, fd);
    if (chunksize != csize) {
        ESP_LOGE(TAG, "file read failed");
        fclose(fd);
        return ESP_FAIL;
    }
    *len = chunksize;
    fclose(fd);

    return 0;
}

const sFONT Font16_gbk = {
    .get_fontdata = get_gbk_fontdata,
    .Width = WIDTH, /* Width */
    .Height = HEIGHT, /* Height */
    .data_size = HEIGHT * (WIDTH / 8 + (WIDTH % 8 ? 1 : 0)),
};
