

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "cJSON.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "esp_tls.h"

#include "weather.h"

static const char *TAG = "weather";

#define ITEM_CHECK(a)  if(!(a)) {                                             \
        ESP_LOGE(TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, "pointer invalid");      \
        break;                                                                   \
        }

static weather_info_t g_weather_info;//天气结构体


//心知天气获取连接：
//https://api.thinkpage.cn/v3/weather/daily.json?key=1uqo4k3in0oluhpo&location=jian&language=en&unit=c
//https://api.thinkpage.cn/v3/weather/daily.json?key=KEY&location=[/url]城市&language=zh-Hans&unit=c&start=-1&days=5

static const char* HOST = "api.thinkpage.cn";
static const char* APIKEY = "1uqo4k3in0oluhpo";        //API KEY
static const char* NOW_API = "/weather/now.json";
static const char* DAILY_API = "/weather/daily.json";

static const char* weather_code[] = {
    "晴",
    "晴",
    "晴",
    "晴",
    "多云",
    "晴间多云",
    "晴间多云",
    "大部多云",
    "大部多云",
    "阴",
    "阵雨",
    "雷阵雨",
    "雷阵雨伴有冰雹",
    "小雨",
    "中雨",
    "大雨",
    "暴雨",
    "大暴雨",
    "特大暴雨",
    "冻雨",
    "雨夹雪",
    "阵雪",
    "小雪",
    "中雪",
    "大雪",
    "暴雪",
    "浮尘",
    "扬沙",
    "沙尘暴",
    "强沙尘暴",
    "雾",
    "霾",
    "风",
    "大风",
    "飓风",
    "热带风暴",
    "龙卷风",
    "冷",
    "热",
    "未知",
};

char *weather_code2str(uint8_t code)
{
    return (char*)weather_code[code];
}


static int16_t my_asc2num(char *ptr)
{
	uint16_t Data=0;
	int16_t res=0;
	uint8_t i=0;
	if(*ptr == '-') 
	{
		ptr++;
		res=-1;
	}
	while(*ptr >= '0' && *ptr <= '9')		//判断是否是数
	{
		Data = (((Data << 2) + Data) << 1) + ((*ptr++)-'0'); // Data*10 + c
		if(++i>5) return 0xffff;
	}
	if(res<0) res = -(int16_t)Data;
	else      res = (int16_t)Data;
	return res;
}

static void my_num2asc(uint16_t val,char *ptr)
{
	uint8_t i=0,j=0;
	
	ptr[i++] = val%10+'0';
	while(val/=10)
	{
		ptr[i++] = val%10+'0';
	}
	while(j<(i/2))
	{
		ptr[i-1-j]=ptr[i-1-j]^ptr[j];
		ptr[j]    =ptr[i-1-j]^ptr[j];
		ptr[i-1-j]=ptr[i-1-j]^ptr[j];
		j++;
	}
	ptr[i]='\0';
}

uint8_t weather_get(const char *cityid, weather_type_t type)
{
    char *buf;
    const uint16_t buf_size=4096;
    
    buf = malloc(buf_size);
    // We now create a URI for the request
    //心知天气
    memset(buf, 0, buf_size);
    strcpy(buf,"GET ");
    strcat(buf, "/v3");
    if(WEATHER_TYPE_NOW == type)strcat(buf, NOW_API);
    else strcat(buf, DAILY_API);
    strcat(buf, "?key=");
    strcat(buf, APIKEY);
    strcat(buf, "&location=");
    strcat(buf, cityid);
    strcat(buf, "&language=");
    strcat(buf, "en");
    strcat(buf, "&unit=");
    strcat(buf, "c");
    strcat(buf, " HTTP/1.1\r\nhost: ");
    strcat(buf, HOST);
    strcat(buf, "\r\nConnection: close\r\n\r\n");

#if WEATHER_USE_HTTPS
    
    const char *WEATHER_URL = "https://api.thinkpage.cn";
    struct esp_tls *tls;
    extern const uint8_t server_root_cert_pem_start[] asm("_binary_server_root_cert_pem_start");
    extern const uint8_t server_root_cert_pem_end[]   asm("_binary_server_root_cert_pem_end");
    esp_tls_cfg_t cfg = {
        .cacert_pem_buf  = server_root_cert_pem_start,
        .cacert_pem_bytes = server_root_cert_pem_end - server_root_cert_pem_start,
    };
    
    ESP_LOGI(TAG, "esp_tls_conn_http_new");
    tls = esp_tls_conn_http_new(WEATHER_URL, &cfg);
    
    if(tls != NULL) {
        ESP_LOGI(TAG, "Connection established...");
    } else {
        ESP_LOGE(TAG, "Connection failed...");
        esp_tls_conn_delete(tls);
        return 1;
    }
    
    char *REQUEST=buf;
    size_t written_bytes = 0;
    do {
        ret = esp_tls_conn_write(tls, 
                                    REQUEST + written_bytes, 
                                    strlen(REQUEST) - written_bytes);
        if (ret >= 0) {
        ESP_LOGI(TAG, "%d bytes written", ret);
            written_bytes += ret;
        } else if (ret != MBEDTLS_ERR_SSL_WANT_READ  && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
            ESP_LOGE(TAG, "esp_tls_conn_write  returned 0x%x", ret);
            esp_tls_conn_delete(tls);
            return 2;
        }
    } while(written_bytes < strlen(REQUEST));

    do
    {
        int len = sizeof(buf) - 1;
        bzero(buf, sizeof(buf));
        ret = esp_tls_conn_read(tls, (char *)buf, len);
        if(ret == MBEDTLS_ERR_SSL_WANT_WRITE  || ret == MBEDTLS_ERR_SSL_WANT_READ)
            continue;
        
        if(ret < 0)
        {
            ESP_LOGE(TAG, "esp_tls_conn_read  returned -0x%x", -ret);
            break;
        }

        if(ret == 0)
        {
            ESP_LOGI(TAG, "connection closed");
            break;
        }
        len = ret;
        ESP_LOGD(TAG, "%d bytes read", len);
        
    } while(1);
    
    esp_tls_conn_delete(tls);

#else
    
    const struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *res;
    int sock=-1;
    int err = getaddrinfo(HOST, "80", &hints, &res);

    do{
        if(err != 0 || res == NULL) {
            ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);
            break;
        }
        sock =  socket(res->ai_family, res->ai_socktype, IPPROTO_IP);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        err = connect(sock, res->ai_addr, res->ai_addrlen);
        if (err != 0) {
            ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
            break;
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
        err = send(sock, buf, strlen(buf), 0);
        if (err < 0) {
            ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
            break;
        }
        int len = recv(sock, buf, buf_size - 1, 0);
        // Error occured during receiving
        if (len < 0) {
            ESP_LOGE(TAG, "recv failed: errno %d", errno);
            break;
        }
        // Data received
        else {
            buf[len] = 0; // Null-terminate whatever we received and treat like a string
            ESP_LOGI(TAG, "Received %d bytes from %s:", len,HOST);
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }while(0);

    if (sock != -1) {
        shutdown(sock, SHUT_RDWR);
        close(sock);
        }
#endif

    do {
        char *dataPtr;
        dataPtr = strstr((const char*)buf, "\r\n\r\n");	//跳过http头
        if(dataPtr == NULL)
        {
            ESP_LOGE(TAG, "Can't find http data payload");
            break;
        }
        dataPtr += 2;

        cJSON *root = NULL;
        cJSON *results = NULL;

        root = cJSON_Parse(dataPtr);
        if (!root) 
        {
            ESP_LOGE(TAG, "Error before: [%s]\n",cJSON_GetErrorPtr());
            break;
        }

        ESP_LOGD(TAG, "\n%s\n\n", cJSON_Print(root));
        if(WEATHER_TYPE_NOW == type)
        {
            cJSON *array_item = NULL;
            cJSON *now = NULL;
            cJSON *item = NULL;

            results = cJSON_GetObjectItem(root, "results");
            ITEM_CHECK(NULL != results);
            
            array_item = cJSON_GetArrayItem(results, 0);
            ITEM_CHECK(NULL != array_item);

            now = cJSON_GetObjectItem(array_item, "now");
            ITEM_CHECK(NULL != now);
            
            item = cJSON_GetObjectItem(now, "code");
            ITEM_CHECK(NULL != item);
            g_weather_info.now.code = my_asc2num(item->valuestring);

            item = cJSON_GetObjectItem(now, "temperature");
            ITEM_CHECK(NULL != item);
            g_weather_info.now.temp = my_asc2num(item->valuestring);

            ESP_LOGI(TAG, "[now] code=%d(%s), temp=%d", g_weather_info.now.code, 
            weather_code2str(g_weather_info.now.code), g_weather_info.now.temp);
        } else {
            cJSON *item = NULL;
            cJSON *daily_item = NULL;
            cJSON *array_item = NULL;

            results = cJSON_GetObjectItem(root, "results");
            ITEM_CHECK(NULL != results);
            
            array_item = cJSON_GetArrayItem(results, 0);
            ITEM_CHECK(NULL != array_item);

            array_item = cJSON_GetObjectItem(array_item, "daily");
            ITEM_CHECK(NULL != array_item);

            uint32_t cnt = cJSON_GetArraySize(array_item);
            for(int i = 0; i < cnt; i++)
            {
                daily_item = cJSON_GetArrayItem(array_item, i);
                ITEM_CHECK(NULL != daily_item);
                
                item = cJSON_GetObjectItem(daily_item, "code_day");
                ITEM_CHECK(NULL != item);
                g_weather_info.day[i].code_day = my_asc2num(item->valuestring);

                item = cJSON_GetObjectItem(daily_item, "code_night");
                ITEM_CHECK(NULL != item);
                g_weather_info.day[i].code_night = my_asc2num(item->valuestring);

                item = cJSON_GetObjectItem(daily_item, "high");
                ITEM_CHECK(NULL != item);
                g_weather_info.day[i].temp_high = my_asc2num(item->valuestring);

                item = cJSON_GetObjectItem(daily_item, "low");
                ITEM_CHECK(NULL != item);
                g_weather_info.day[i].temp_low = my_asc2num(item->valuestring);

                item = cJSON_GetObjectItem(daily_item, "wind_direction_degree");
                ITEM_CHECK(NULL != item);
                g_weather_info.day[i].wind_direction_degree = my_asc2num(item->valuestring);

                item = cJSON_GetObjectItem(daily_item, "wind_speed");
                ITEM_CHECK(NULL != item);
                g_weather_info.day[i].wind_speed = my_asc2num(item->valuestring);

                item = cJSON_GetObjectItem(daily_item, "humidity");
                ITEM_CHECK(NULL != item);
                g_weather_info.day[i].humidity = my_asc2num(item->valuestring);

                ESP_LOGI(TAG, "[daily] code_day=%d(%s), code_night=%d(%s), temp=%d~%d, wind_speed=%d, wind_degree=%d, humidity=%d", 
                g_weather_info.day[i].code_day, weather_code2str(g_weather_info.day[i].code_day), 
                g_weather_info.day[i].code_night, weather_code2str(g_weather_info.day[i].code_night), 
                g_weather_info.day[i].temp_low, g_weather_info.day[i].temp_high,
                g_weather_info.day[i].wind_speed, g_weather_info.day[i].wind_direction_degree, 
                g_weather_info.day[i].humidity
                );
            }
        }
        
        cJSON_Delete(root);
        
    }while(0);

    free(buf);
    return 0;
}

