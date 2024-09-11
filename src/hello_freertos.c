#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"

// Deining pririty levels of tasks within OS
#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2UL )
#define BLINK_TASK_PRIORITY     ( tskIDLE_PRIORITY + 1UL )

// Give each task some memory
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define BLINK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

// Helper variables for turning LED on and off
int counter;
bool led_is_on;

// Task to blink the LED
void blink_task(__unused void *params)
{
    // Helper variables for turning LED on and off
    counter = 0;
    led_is_on = false;

    // Check to make sure board gives OK signal
    hard_assert(cyw43_arch_init() == PICO_OK);

    // Set LED GPIO pin to off
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);

    // AS counter is updated, change GPIO to true/false based on counter
    while (true) {
	    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_is_on);
	    if (counter++ % 47)
	        led_is_on = !led_is_on;
        vTaskDelay(1000);
    }
}

// Main task: setting lowecase 'a' and z'z to uppercase
void main_task(__unused void *params)
{
    // Create side task on different thread
    xTaskCreate(blink_task, "BlinkThread", BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);

    // intialize helper variables
    int count = 0;
    char c;

    // l0op for changing lowercase to uppercase
    while(c = getchar()) {
	    if (c <= 'z' && c >= 'a') putchar(c - 32);
	    else if (c >= 'A' && c <= 'Z') putchar(c + 32);
	    else putchar(c);
    }
}

int main( void )
{
    stdio_init_all();
    TaskHandle_t task;
    xTaskCreate(main_task, "MainThread", MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &task);
    vTaskStartScheduler();
    return 0;
}
