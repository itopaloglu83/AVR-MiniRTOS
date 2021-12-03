#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <avr/sleep.h>

// Something to do when there is nothing else to do.
void vApplicationIdleHook(void)
{
    // Select sleep mode.
    set_sleep_mode(SLEEP_MODE_IDLE);
    // Enable sleep and disable brown-out detection.
    portENTER_CRITICAL();
    sleep_enable();
    sleep_bod_disable();
    portEXIT_CRITICAL();
    // Put CPU to sleep.
    sleep_cpu();
    // CPU woke up. Disable sleep mode.
    sleep_disable();
}

// Blink builtin LED.
void TaskBlink(void *pvParameters)
{
    // Set the LED pin as output.
    pinMode(LED_BUILTIN, OUTPUT);
    while (1)
    {
        // Turn the LED on.
        digitalWrite(LED_BUILTIN, HIGH);
        // Delay task. Do not block.
        vTaskDelay(500 / portTICK_PERIOD_MS);
        // Turn the LED off.
        digitalWrite(LED_BUILTIN, LOW);
        // Delay task. Do not block.
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

int main(void)
{
    // Port fix. Resolves malloc memory exhaustion errors.
    // Malloc calls in tasks would fail, because stacks are stored in heap.
    __malloc_heap_end = (char *)(RAMEND - configMINIMAL_STACK_SIZE);
    // Create the blink task.
    xTaskCreate(TaskBlink, "Blink", 128, NULL, 2, NULL);
    // Start scheduler. This does not return.
    vTaskStartScheduler();
    // Program flow should never reach here.
    while (1)
    {
        _NOP();
    }
}
