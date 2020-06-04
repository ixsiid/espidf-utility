#include "log.hpp"

#include <driver/uart.h>
#include <stdarg.h>
#include <stdio.h>

#define UART UART_NUM_0
#define UART_RTS 18
#define UART_CTS 19

#define MAX_LOG_BUFFER_LENGTH 1024
char Log::buffer[MAX_LOG_BUFFER_LENGTH];
bool Log::initialized = false;

Log::Log() {
	if (initialized) return;
	uart_config_t uart_config = {
	    .baud_rate			= 115200,
	    .data_bits			= UART_DATA_8_BITS,
	    .parity			= UART_PARITY_DISABLE,
	    .stop_bits			= UART_STOP_BITS_1,
	    .flow_ctrl			= UART_HW_FLOWCTRL_DISABLE,
	    .rx_flow_ctrl_thresh = 122,
	    .use_ref_tick		= false,
	};
	// Configure UART parameters
	ESP_ERROR_CHECK(uart_param_config(UART, &uart_config));

	ESP_ERROR_CHECK(uart_set_pin(UART, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_RTS, UART_CTS));

	// Setup UART buffered IO with event queue
	const int uart_buffer_size = (1024 * 2);
	QueueHandle_t uart_queue;
	// Install UART driver using an event queue here
	ESP_ERROR_CHECK(uart_driver_install(UART, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0));

	initialized = true;
}

void Log::verbose(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	int length = vsnprintf(buffer, MAX_LOG_BUFFER_LENGTH, format, ap);
	va_end(ap);
	buffer[length] = '\0';

	uart_write_bytes(UART, (const char *)buffer, length);

	ESP_ERROR_CHECK(uart_wait_tx_done(UART, 200));
}

Log * ___log = new Log();
