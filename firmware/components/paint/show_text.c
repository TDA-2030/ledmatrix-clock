#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

#include "file_manage.h"
#include "show_text.h"

static const char *TAG = "text show";


/**
 *
 * Reference from https://blog.csdn.net/bladeandmaster88/article/details/54837338
 */
static int UnicodeToUtf8(char *pInput, char *pOutput)
{
    int len = 0; //记录转换后的Utf8字符串的字节数
    while (*pInput) {
        //处理一个unicode字符
        char low = *pInput;//取出unicode字符的低8位
        pInput++;
        char high = *pInput;//取出unicode字符的高8位
        unsigned  wchar = (high << 8) + low; //高8位和低8位组成一个unicode字符,加法运算级别高

        if (wchar <= 0x7F ) { //英文字符
            pOutput[len] = (char)wchar;  //取wchar的低8位
            len++;
        } else if (wchar >= 0x80 && wchar <= 0x7FF) { //可以转换成双字节pOutput字符
            pOutput[len] = 0xc0 | ((wchar >> 6) & 0x1f); //取出unicode编码低6位后的5位，填充到110yyyyy 10zzzzzz 的yyyyy中
            len++;
            pOutput[len] = 0x80 | (wchar & 0x3f);  //取出unicode编码的低6位，填充到110yyyyy 10zzzzzz 的zzzzzz中
            len++;
        } else if (wchar >= 0x800 && wchar < 0xFFFF) { //可以转换成3个字节的pOutput字符
            pOutput[len] = 0xe0 | ((wchar >> 12) & 0x0f); //高四位填入1110xxxx 10yyyyyy 10zzzzzz中的xxxx
            len++;
            pOutput[len] = 0x80 | ((wchar >> 6) & 0x3f);  //中间6位填入1110xxxx 10yyyyyy 10zzzzzz中的yyyyyy
            len++;
            pOutput[len] = 0x80 | (wchar & 0x3f);  //低6位填入1110xxxx 10yyyyyy 10zzzzzz中的zzzzzz
            len++;
        }

        else { //对于其他字节数的unicode字符不进行处理
            return -1;
        }
        pInput ++;//处理下一个unicode字符
    }
    //utf8字符串后面，有个\0
    pOutput [len] = 0;
    return len;
}
/*************************************************************************************************
* 将UTF8编码转换成Unicode（UCS-2LE）编码  高地址存低位字节
* 参数：
*    char* pInput     输入字符串
*    char*pOutput   输出字符串
* 返回值：转换后的Unicode字符串的字节数，如果出错则返回-1
**************************************************************************************************/
//utf8转unicode
char *Utf8ToUnicode(const char *pInput, char *pOutput)
{
    int outputSize = 0; //记录转换后的Unicode字符串的字节数
    char *p = pOutput;

    if (*pInput > 0x00 && *pInput <= 0x7F) { //处理单字节UTF8字符（英文字母、数字）
        *pOutput = *pInput;
        pOutput++;
        *pOutput = 0; //小端法表示，在高地址填补0
    } else if (((*pInput) & 0xE0) == 0xC0) { //处理双字节UTF8字符
        char high = *pInput;
        pInput++;
        char low = *pInput;
        if ((low & 0xC0) != 0x80) { //检查是否为合法的UTF8字符表示
            ESP_LOGE(TAG, "utf-8 encounter illegal character");
            return ++pInput;
        }

        *pOutput = (high << 6) + (low & 0x3F);
        pOutput++;
        *pOutput = (high >> 2) & 0x07;
    } else if (((*pInput) & 0xF0) == 0xE0) { //处理三字节UTF8字符
        char high = *pInput;
        pInput++;
        char middle = *pInput;
        pInput++;
        char low = *pInput;
        if (((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80)) {
            ESP_LOGE(TAG, "utf-8 encounter illegal character");
            return ++pInput;
        }
        *pOutput = (middle << 6) + (low & 0x3F);//取出middle的低两位与low的低6位，组合成unicode字符的低8位
        pOutput++;
        *pOutput = (high << 4) + ((middle >> 2) & 0x0F); //取出high的低四位与middle的中间四位，组合成unicode字符的高8位
    } else { //对于其他字节数的UTF8字符不进行处理
        ESP_LOGE(TAG, "Unsupport utf-8 character [%x]", *pInput);
        return ++pInput;
    }
    outputSize += 2;

    // 字节顺序翻转
    for (size_t i = 0; i < outputSize; i += 2) {
        char t = p[i];
        p[i] = p[i + 1];
        p[i + 1] = t;
    }

    return ++pInput;
}


uint16_t utf8_to_gbk_strlen(const char *str)
{
    uint16_t len = 0;
    char unicode[2];
    while (*str != 0) {
        str = Utf8ToUnicode(str, unicode);
        len += 2;
    }
    return len;
}

void utf8_to_gbk_str(const char *str, char *out)
{
    char unicode[2];
    while (*str != 0) {
        str = Utf8ToUnicode(str, unicode);
        font_unicode2gbk(unicode, out);
        out += 2;
    }
}