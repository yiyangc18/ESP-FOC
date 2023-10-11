#include <stdio.h>
#include "timer_loop.h"
#include "esp_log.h"
#include "esp_timer.h"

static esp_timer_handle_t timer_handle[6];

void loop_timer_init(loop_timer_config_t *config){

    const esp_timer_create_args_t periodic_timer_args = {
            .callback = config->callback,
            /* name is optional, but may help identify the timer when debugging */
            .name = config->name
    };

    esp_timer_handle_t *periodic_timer = &timer_handle[config->timer_index];
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, periodic_timer));
    /* The timer has been created but is not running yet */

    /* Start the timers */
    ESP_ERROR_CHECK(esp_timer_start_periodic(*periodic_timer, config->period));

}

void loop_timer_deniit(loop_timer_config_t *config){
    esp_timer_handle_t *periodic_timer = &timer_handle[config->timer_index];
    ESP_ERROR_CHECK(esp_timer_stop(*periodic_timer));
    ESP_ERROR_CHECK(esp_timer_delete(*periodic_timer));
}

