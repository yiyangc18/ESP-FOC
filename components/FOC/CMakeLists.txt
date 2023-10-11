
idf_component_register(SRCS "esp_hw_link.c" "foc.c" "encoder.c" "fast_math.c" "filter.c" "pid.c" "esp_hw_link"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES driver freertos
                                hall_sensor_spi motor_pwm esp_timer)


