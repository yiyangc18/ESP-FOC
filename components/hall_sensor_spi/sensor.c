
#include "sensor.h"

spi_device_handle_t spi;

static const char *TAG = "sonsor_spi";

/**
 * @brief Reads the hall sensor using SPI and returns the angle in radians.
 * 
 * @return float The angle in radians.
 */
float hall_sensor_read_angle_in_radians(void) {
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 16;
    t.rxlength = 16;
    t.flags = SPI_TRANS_USE_RXDATA;
    ret = spi_device_transmit(spi, &t);
    assert(ret == ESP_OK);
    uint16_t ag = (t.rx_data[0] << 8) | t.rx_data[1];
    ag = ag >> 2;
    float rad = (float)ag * TWO_PI / 16384;
    if (rad < 0) {
        rad += TWO_PI;
    }
    return rad;
}

/**
 * @brief Initializes the SPI bus and device.
 */
void hall_sensor_init(void) {
    esp_err_t ret;

    // 配置 SPI 总线
    spi_bus_config_t buscfg = {
        .miso_io_num = MT6701_MISO,
        .mosi_io_num = -1,
        .sclk_io_num = MT6701_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    // 配置 SPI 设备
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = SPI_CLK,
        .mode = SPI_MODE0,
        .spics_io_num = MT6701_SS,
        .queue_size = 1,
    };

    // 初始化 SPI 总线
    ret = spi_bus_initialize(HSPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    assert(ret == ESP_OK);
    ESP_LOGI(TAG,"spi bus is initialized.");

    // 添加 SPI 设备到总线
    ret = spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
    assert(ret == ESP_OK);
    ESP_LOGI(TAG,"spi sensor is add to spi bus.");
    ESP_LOGI(TAG,"hall sensor is initialized.");
}


/**
 * @brief freeRTOS测试任务readSensorTask 
 */
void readSensorTask(void *pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 500 / portTICK_PERIOD_MS; // 100ms

    // Initialize the last wake time
    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        // Call the hall_sensor_read_angle_in_radians() function
        float sensorValue = hall_sensor_read_angle_in_radians();

        // Log the sensor value
        ESP_LOGI(TAG, "Sensor value: %f", sensorValue);

        // Wait for the next cycle
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
