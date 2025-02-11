/*

  */
#ifndef _LV_PORT_H_
#define _LV_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

void lv_port(int width, int height);

bool example_lvgl_lock(int timeout_ms);
void example_lvgl_unlock(void);

#ifdef __cplusplus
}
#endif

#endif
