
#include "motor_pwm.h"

static const char *TAG = "motor_pwm";

void set_motorpwm_dutycycle(float ch0, float ch1, float ch2) {
    // 更新 PWM 占空比
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, ch0);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, ch1);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_A, ch2);
    
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_1);
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, MCPWM_DUTY_MODE_1);
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_A, MCPWM_DUTY_MODE_1);

}

void motor_pwm_init(void){
    // 初始化 MCPWM
    mcpwm_gpio_init(MCPWM_UNIT_0,MCPWM0A,MO1_PIN);
    mcpwm_gpio_init(MCPWM_UNIT_0,MCPWM1A,MO2_PIN);
    mcpwm_gpio_init(MCPWM_UNIT_0,MCPWM2A,MO3_PIN);

    mcpwm_config_t pwm_config = {
        .frequency = 20000,  // 设置 PWM 频率为 20kHz
        .cmpr_a = 0,  // 初始占空比为 0
        .cmpr_b = 0,    
        .counter_mode = MCPWM_UP_COUNTER,    
        .duty_mode = MCPWM_DUTY_MODE_0  // 设置占空比计算方式为 MCPWM_DUTY_MODE_0
    };

    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_2, &pwm_config);

    ESP_LOGI(TAG, "motor pwm init success");
    ESP_LOGI(TAG, "MCPWM0A,MO1_PIN:%d",MO1_PIN);
    ESP_LOGI(TAG, "MCPWM1A,MO2_PIN:%d",MO2_PIN);
    ESP_LOGI(TAG, "MCPWM2A,MO3_PIN:%d",MO3_PIN);
}

void motorPwmDriverTask(void *pvParameters) {

    // motor_pwm_init();

    while (1) {
        // 测试任务 依次设置三个电相的 PWM 占空比为 80%
        set_motorpwm_dutycycle(80, 0.0, 0.0);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        set_motorpwm_dutycycle(0.0, 80, 0.0);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        set_motorpwm_dutycycle(0.0, 0.0, 80);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
