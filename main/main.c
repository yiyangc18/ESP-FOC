/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "main.h"

static const char *TAG = "main";


void foc_init(void)
{
    /* initialize the motor */
  motor_pwm_init();

  /* initialize the sensor */
  hall_sensor_init();

  /* initialize the low pass filter */
  init_lpf(&velocity_filter, TWO_PI/SPEED_UP_FREQ);

  /* initialize the PID controller */
  pid_config(SPEED_LOOP_CONTROL);

  /* initialize the FOC phase */
  foc_calibrate_phase();
  
  /* correct the mechanical angle zero deviation */
  encoder_zeroing();

  /* create Task for foc_loop, controller_loop and speed update loop */
  loop_timer_config_t foc_loop_timer_config = {
    .callback = foc_loop,
    .name = "foc_loop",
    .period = 1000000/FOC_LOOP_FREQ,
    // .period = 1000000/1000
    .timer_index = 0
  };
  loop_timer_init(&foc_loop_timer_config);

  loop_timer_config_t controller_loop_timer_config = {
    .callback = controller_loop,
    .name = "controller_loop",
    .period = 1000000/PID_LOOP_FREQ,
    .timer_index = 1
  };
  loop_timer_init(&controller_loop_timer_config);

  loop_timer_config_t speed_update_loop_timer_config = {
    .callback = encoder_update_speed,
    .name = "report_angle_speed",
    .period = 1000000/SPEED_UP_FREQ,
    .timer_index = 2
  };
  loop_timer_init(&speed_update_loop_timer_config);

}



void app_main(void)
{
  print_hello_Wrold();

  foc_init();

  speed_pid_handler.expect = 3;
  // FOC_Struct.current_q = -1.0f;

  // hall_sensor_init();
  // xTaskCreate(readSensorTask, "readSensorTask", 2048, NULL, 5, NULL);

  // xTaskCreate(motorPwmDriverTask, "motorPwmDriverTask", 2048, NULL, 6, NULL);

}


void print_hello_Wrold(void)
{
  printf("Hello world!\n");

  /* Print chip information */
  esp_chip_info_t chip_info;
  uint32_t flash_size;
  esp_chip_info(&chip_info);
  printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
        CONFIG_IDF_TARGET,
        chip_info.cores,
        (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
        (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
        (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
        (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

  unsigned major_rev = chip_info.revision / 100;
  unsigned minor_rev = chip_info.revision % 100;
  printf("silicon revision v%d.%d, ", major_rev, minor_rev);
  if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
    printf("Get flash size failed");
    return;
  }

  printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
        (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

  printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
}