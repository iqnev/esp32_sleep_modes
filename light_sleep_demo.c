#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "esp_sleep.h"

void app_main(void)
{
    while (true) {
        /* Wake up in 3 seconds*/
        esp_sleep_enable_timer_wakeup(3000000);

        printf("Entering light sleep\n");

        /* Get timestamp before entering sleep */
        int64_t t_before_us = esp_timer_get_time();

        /* Enter sleep mode */
        esp_light_sleep_start();
        /* Execution continues here after wakeup */

        /* Get timestamp after waking up from sleep */
        int64_t t_after_us = esp_timer_get_time();

        /* Determine wake up reason */
        switch (esp_sleep_get_wakeup_cause()) {
            case ESP_SLEEP_WAKEUP_TIMER:
                printf("Returned from light sleep, reason: timer, t=%lld ms, slept for %lld ms\n", t_after_us / 1000, (t_after_us - t_before_us) / 1000);
                break;
            default:
                printf("Returned from light sleep, reason: other, t=%lld ms, slept for %lld ms\n", t_after_us / 1000, (t_after_us - t_before_us) / 1000);
                break;
        }   
    }
}