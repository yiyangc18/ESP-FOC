#ifndef TIMER_LOOP_H_
#define TIMER_LOOP_H_

typedef void (*loop_timer_cb)(void);

/**
 * @brief Timer configuration structure
 */
typedef struct {
    loop_timer_cb callback;        //!< Function to call when timer expires
    const char* name;               //!< Timer name, used in esp_timer_dump function
    int period;                //!< Period of timer (in microseconds)
    int timer_index;
} loop_timer_config_t;

void loop_timer_init(loop_timer_config_t *config);


#endif //TIMER_LOOP_H_
