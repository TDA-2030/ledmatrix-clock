// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef __CAPTIVE_PORTAL_H__
#define __CAPTIVE_PORTAL_H__

#ifdef __cplusplus 
extern "C" {
#endif

#include "esp_err.h"
#include "esp_wifi.h"



/**
  * @brief Start captive portal
  *
  * @return
  *    - ESP_OK: succeed
  *    - ESP_FAIL: fail
  */
esp_err_t captive_portal_start(void);


#ifdef __cplusplus 
}
#endif

#endif // __CAPTIVE_PORTAL_H__
