#ifndef APP_SHOW_TEXT_H_
#define APP_SHOW_TEXT_H_

#ifdef __cplusplus 
extern "C" {
#endif

#include "esp_err.h"
#include "cc936.h"

char *Utf8ToUnicode(const char *pInput, char *pOutput);

uint16_t utf8_to_gbk_strlen(const char *str);

void utf8_to_gbk_str(const char *str, char *out);

#ifdef __cplusplus 
}
#endif

#endif
