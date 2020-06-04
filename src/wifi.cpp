#include "wifi.hpp"

#include <esp_event.h>
#include <esp_wifi.h>
#include <lwip/inet.h>
#include <nvs_flash.h>
#include <stdbool.h>
#include <string.h>

#include "utility.hpp"

bool WiFi::initialized = false;

ip4_addr_t WiFi::ip;
ip4_addr_t WiFi::gateway;
ip4_addr_t WiFi::subnetmask;
bool WiFi::connected = false;

WiFi::WiFi() {
	_v("set callback\n");
	ESP_ERROR_PRINT(esp_event_loop_init([](void *ctx, system_event_t *event) {
		_v("WiFi Event loop %d\n", event->event_id);
		if (event->event_id == SYSTEM_EVENT_STA_GOT_IP) {
			ip		 = event->event_info.got_ip.ip_info.ip;
			gateway	 = event->event_info.got_ip.ip_info.gw;
			subnetmask = event->event_info.got_ip.ip_info.netmask;
			connected	 = true;
		}
		return ESP_OK;
	}, NULL));

	_v("NVS initialize\n");
	ESP_ERROR_PRINT(nvs_flash_init());
	wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_PRINT(esp_wifi_init(&wifi_config));

	_v("Set wifi mode to STA\n");
	ESP_ERROR_PRINT(esp_wifi_set_mode(WIFI_MODE_STA));

	// esp_wifi_set_ps(WIFI_PS_MIN_MODEM);
	// esp_wifi_set_country({.cc = "JP", .schan=1, .nchan=14})

	initialized = true;
}

bool WiFi::Connect(const char *ssid, const char *password) {
	if (!initialized) {
		_v("WiFi initializing\n");
		WiFi();
	}

	_v("Connecting WiFi\n");
	wifi_config_t config;
	memset(&config, 0, sizeof(config));
	strcpy((char *)config.sta.ssid, ssid);
	strcpy((char *)config.sta.password, password);
	// config.sta.scan_method = WIFI_FAST_SCAN;
	// config.sta.bssid_set = false; // APのMACアドレスをチェックしない
	// config.sta.channel = 0; // APのチャンネルが不明
	_v("Set config\n");
	ESP_ERROR_PRINT_RETURN_FALSE(esp_wifi_set_config(WIFI_IF_STA, &config));

	_v("tcp/ip adapter initializing\n");
	tcpip_adapter_init();

	_v("start wifi\n");
	ESP_ERROR_PRINT_RETURN_FALSE(esp_wifi_start());
	_v("connect wifi\n");
	ESP_ERROR_PRINT_RETURN_FALSE(esp_wifi_connect());
	while (!connected) vTaskDelay(10 / portTICK_PERIOD_MS);

	return true;
}

bool WiFi::Disconnect(bool release) {
	ESP_ERROR_PRINT_RETURN_FALSE(esp_wifi_disconnect());
	ESP_ERROR_PRINT_RETURN_FALSE(esp_wifi_stop());
	ESP_ERROR_PRINT_RETURN_FALSE(esp_wifi_deinit());

	return true;
}

ip4_addr_t *WiFi::getIp() {
	if (!connected) return nullptr;
	return &ip;
}
