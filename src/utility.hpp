#ifndef _UTIL_H
#define _UTIL_H

#include <esp_log.h>
#include <stdbool.h>
#include <stdint.h>

#include <cstddef>

#include "log.hpp"
#include "wifi.hpp"

#ifndef NODEBUG
#define _v(format, ...) Log::verbose("[V] " format, ##__VA_ARGS__)
#define _e(format, ...) Log::verbose("[E] " format, ##__VA_ARGS__)
#define _i(format, ...) Log::verbose("[I] " format, ##__VA_ARGS__)
#define _d(format, ...) Log::verbose("[D] " format, ##__VA_ARGS__)

#define ESP_ERROR_PRINT(x)                                                    \
	do {                                                                     \
		esp_err_t __err_rc = (x);                                           \
		if (__err_rc != ESP_OK) {                                           \
			_v("[%s:%s] %s: %s\n", __FILE__, __LINE__, __ASSERT_FUNC, #x); \
		}                                                                   \
	} while (0)

#define ESP_ERROR_PRINT_RETURN_FALSE(x)                                       \
	do {                                                                     \
		esp_err_t __err_rc = (x);                                           \
		if (__err_rc != ESP_OK) {                                           \
			_v("[%s:%s] %s: %s\n", __FILE__, __LINE__, __ASSERT_FUNC, #x); \
			return false;                                                  \
		}                                                                   \
	} while (0)
#else
#define _v(...)
#define _e(...)
#define _i(...)
#define _d(...)

#define ESP_ERROR_PRINT(x)                 \
	do {                                  \
		esp_err_t __err_rc = (x);        \
		if (__err_rc != ESP_OK) abort(); \
	} while (0)

#define ESP_ERROR_PRINT_RETURN_FALSE(x)    \
	do {                                  \
		esp_err_t __err_rc = (x);        \
		if (__err_rc != ESP_OK) abort(); \
	} while (0)
#endif

uint32_t getNowMilliseconds();

#endif  // _UTIL_H
