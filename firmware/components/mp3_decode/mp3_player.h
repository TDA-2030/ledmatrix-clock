// Copyright 2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __MP3_DECODER_HELIX_H__
#define __MP3_DECODER_HELIX_H__


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t mp3_player_start(const char *path);

esp_err_t mp3_player_stop(void);

esp_err_t mp3_player_wait(TickType_t xTicksToWait);

esp_err_t mp3_player_set_volume(int8_t volume);

esp_err_t mp3_player_init(void);
esp_err_t mp3_player_deinit(void);

#ifdef __cplusplus
}
#endif

#endif