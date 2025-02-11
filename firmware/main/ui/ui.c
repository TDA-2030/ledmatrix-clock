#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "lvgl.h"
#include "lv_port.h"


static const char *TAG = "UI";

LV_FONT_DECLARE(digit_16);
LV_FONT_DECLARE(week_zh_10);
LV_FONT_DECLARE(montserrat_9);
LV_FONT_DECLARE(heiti_9);

static void update_time(lv_timer_t *timer);

static lv_obj_t *rollers[6];
static lv_obj_t *text_label, *date_panel, *date_label, *week_label, *temperature_label;

static lv_obj_t *screen;


void init_lvgl_ui()
{
    // Create a screen
    screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(screen, 64, 32);
    lv_obj_center(screen);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(screen, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_radius(screen, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(screen, 0, LV_PART_MAIN);

    lv_obj_t *clock_panel = lv_obj_create(screen);
    lv_obj_set_size(clock_panel, 64, 16);
    lv_obj_align(clock_panel, LV_ALIGN_CENTER, 0, -11);
    lv_obj_clear_flag(clock_panel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(clock_panel, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_radius(clock_panel, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(clock_panel, 0, LV_PART_MAIN);

    // Create a style for the rollers
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, &digit_16);
    lv_style_set_text_color(&style, lv_color_make(0, 255, 0));
    lv_style_set_bg_opa(&style, LV_OPA_TRANSP);
    lv_style_set_pad_all(&style, 0);
    lv_style_set_radius(&style, 0);
    lv_style_set_min_height(&style, 0);
    lv_style_set_border_width(&style, 0);
    lv_style_set_outline_width(&style, 0);
    lv_style_set_shadow_width(&style, 0);
    lv_style_set_outline_pad(&style, 10);
    lv_style_set_anim_time(&style, 500); // Set animation duration

    // Create rollers for hours, minutes, and seconds
    for (int i = 0; i < 6; i++) {
        rollers[i] = lv_roller_create(clock_panel);
        lv_obj_clear_flag(rollers[i], LV_OBJ_FLAG_CLICK_FOCUSABLE);
        lv_obj_clear_state(rollers[i], LV_STATE_FOCUSED);
        lv_obj_remove_style(rollers[i], NULL, LV_PART_ANY);
        lv_obj_remove_style(rollers[i], NULL, LV_STATE_ANY);
        lv_obj_add_style(rollers[i], &style, LV_PART_MAIN);
        lv_obj_set_width(rollers[i], 9);
        lv_obj_set_height(rollers[i], 16);
        lv_roller_set_visible_row_count(rollers[i], 1);
    }

    lv_obj_align(rollers[0], LV_ALIGN_LEFT_MID, -10, 0);
    lv_roller_set_options(rollers[0], "2\n1\n0", LV_ROLLER_MODE_INFINITE);

    lv_obj_align_to(rollers[1], rollers[0], LV_ALIGN_OUT_RIGHT_TOP, 0, 0);
    lv_roller_set_options(rollers[1], "9\n8\n7\n6\n5\n4\n3\n2\n1\n0", LV_ROLLER_MODE_INFINITE);

    lv_obj_t *colon1 = lv_label_create(clock_panel);
    lv_obj_set_style_text_color(colon1, lv_color_make(255, 255, 0), 0);
    lv_obj_align_to(colon1, rollers[1], LV_ALIGN_OUT_RIGHT_MID, -1, 0);
    lv_label_set_text(colon1, ":");

    lv_obj_align_to(rollers[2], colon1, LV_ALIGN_OUT_RIGHT_MID, 1, 0);
    lv_roller_set_options(rollers[2], "6\n5\n4\n3\n2\n1\n0", LV_ROLLER_MODE_INFINITE);

    lv_obj_align_to(rollers[3], rollers[2], LV_ALIGN_OUT_RIGHT_TOP, 0, 0);
    lv_roller_set_options(rollers[3], "9\n8\n7\n6\n5\n4\n3\n2\n1\n0", LV_ROLLER_MODE_INFINITE);

    lv_obj_t *colon2 = lv_label_create(clock_panel);
    lv_obj_set_style_text_color(colon2, lv_color_make(255, 255, 0), 0);
    lv_obj_align_to(colon2, rollers[3], LV_ALIGN_OUT_RIGHT_MID, -1, 0);
    lv_label_set_text(colon2, ":");

    lv_obj_align_to(rollers[4], colon2, LV_ALIGN_OUT_RIGHT_TOP, 1, -1);
    lv_roller_set_options(rollers[4], "6\n5\n4\n3\n2\n1\n0", LV_ROLLER_MODE_INFINITE);

    lv_obj_align_to(rollers[5], rollers[4], LV_ALIGN_OUT_RIGHT_TOP, 0, 0);
    lv_roller_set_options(rollers[5], "9\n8\n7\n6\n5\n4\n3\n2\n1\n0", LV_ROLLER_MODE_INFINITE);

    for (int i = 0; i < 6; i++) {
        lv_roller_set_selected(rollers[i], lv_roller_get_option_cnt(rollers[i]) - 1, LV_ANIM_OFF);
    }
    // Create a timer to update the time every second
    lv_timer_create(update_time, 1000, NULL);


    date_panel = lv_obj_create(screen);
    lv_obj_set_size(date_panel, 64, 18);
    lv_obj_align(date_panel, LV_ALIGN_CENTER, 0, 7);
    lv_obj_clear_flag(date_panel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(date_panel, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_radius(date_panel, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(date_panel, 0, LV_PART_MAIN);
    // Date label
    date_label = lv_label_create(date_panel); // Update this line to use the global date_label
    lv_obj_set_size(date_label, 64, 12);
    lv_obj_set_style_bg_color(date_label, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_radius(date_label, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(date_label, 0, LV_PART_MAIN);
    lv_label_set_text_fmt(date_label, "%s", "2025-00-00");
    lv_obj_set_style_anim_speed(date_label, 30, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(date_label, lv_color_make(0, 255, 0), 0);
    lv_obj_set_style_text_font(date_label, &montserrat_9, 0);
    lv_obj_align(date_label, LV_ALIGN_CENTER, 0, -3);

    // Week label 一二三四五六七日周
    week_label = lv_label_create(date_panel);
    lv_obj_set_size(week_label, 12, 12);
    lv_obj_align(week_label, LV_ALIGN_CENTER, 28, -4);
    lv_obj_clear_flag(week_label, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(week_label, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_radius(week_label, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(week_label, 0, LV_PART_MAIN);
    lv_label_set_text(week_label, "五");
    lv_label_set_long_mode(week_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_style_anim_speed(week_label, 30, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(week_label, lv_color_make(0, 255, 0), 0);
    lv_obj_set_style_text_font(week_label, &week_zh_10, 0);


    // temperature calendar label 一二三四五六七八九十冬腊正二三四五六七八九十冬腊正
    temperature_label = lv_label_create(date_panel);
    lv_obj_set_size(temperature_label, 64, 12);
    lv_obj_align(temperature_label, LV_ALIGN_CENTER, 0, 5);
    lv_obj_clear_flag(temperature_label, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(temperature_label, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_radius(temperature_label, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(temperature_label, 0, LV_PART_MAIN);
    lv_label_set_text(temperature_label, "10℃ 99%");
    lv_label_set_long_mode(temperature_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_style_anim_speed(temperature_label, 30, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(temperature_label, lv_color_make(0, 255, 0), 0);
    lv_obj_set_style_text_font(temperature_label, &heiti_9, 0);


    // Add a scrollbar to the screen
    /*Create a font*/
#ifdef ESP_PLATFORM
    const char *name = "/rootfs/sourcehansanssc-medium-min.ttf";
    extern const char root_start[] asm("_binary_SourceHanSansSC_Medium_Min_ttf_start");
    extern const char root_end[] asm("_binary_SourceHanSansSC_Medium_Min_ttf_end");
    lv_font_t *font = lv_tiny_ttf_create_data_ex(root_start, root_end - root_start, 14, 16384);
#else
    const char *name = "/home/zhouli/esp/lv_port_pc_vscode/SourceHanSansSC-Medium-Min.ttf";
    lv_font_t *font = lv_tiny_ttf_create_file(name, 14);
#endif

    text_label = lv_label_create(screen);
    lv_obj_set_size(text_label, 64, 16);
    lv_obj_align(text_label, LV_ALIGN_CENTER, 0, 7);
    lv_obj_clear_flag(text_label, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(text_label, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_radius(text_label, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(text_label, 0, LV_PART_MAIN);
    lv_label_set_long_mode(text_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_style_anim_speed(text_label, 20, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(text_label, lv_color_make(0, 255, 0), 0);
    lv_obj_set_style_text_font(text_label, font, 0);
    lv_obj_add_flag(text_label, LV_OBJ_FLAG_HIDDEN);

}


void show_text_start(const char *text)
{
    if (example_lvgl_lock(-1)) {
        lv_label_set_text_fmt(text_label, " %s", text);
        lv_obj_add_flag(date_panel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(text_label, LV_OBJ_FLAG_HIDDEN);
        example_lvgl_unlock();
    }

}

void show_text_stop()
{
    if (example_lvgl_lock(-1)) {
        lv_obj_add_flag(text_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(date_panel, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text_fmt(text_label, "%s", "~~");
        example_lvgl_unlock();
    }
}

static void update_time(lv_timer_t *timer)
{
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    int hour_tens = timeinfo.tm_hour / 10;
    int hour_units = timeinfo.tm_hour % 10;
    int min_tens = timeinfo.tm_min / 10;
    int min_units = timeinfo.tm_min % 10;
    int sec_tens = timeinfo.tm_sec / 10;
    int sec_units = timeinfo.tm_sec % 10;

    lv_roller_set_selected(rollers[0], 2 - hour_tens, LV_ANIM_ON);
    lv_roller_set_selected(rollers[1], 9 - hour_units, LV_ANIM_ON);

    lv_roller_set_selected(rollers[2], 6 - min_tens, LV_ANIM_ON);
    lv_roller_set_selected(rollers[3], 9 - min_units, LV_ANIM_ON);

    lv_roller_set_selected(rollers[4], 6 - sec_tens, LV_ANIM_ON);
    lv_roller_set_selected(rollers[5], 9 - sec_units, LV_ANIM_ON);

    // Update date label
    char date_buf[11];
    strftime(date_buf, sizeof(date_buf), "%Y-%m-%d", &timeinfo);
    lv_label_set_text_fmt(date_label, " %s", date_buf);

    // Update week label
    const char *week_days[] = {"日", "一", "二", "三", "四", "五", "六"};
    lv_label_set_text(week_label, week_days[timeinfo.tm_wday]);
}
