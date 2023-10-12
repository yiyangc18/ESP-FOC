# ESP-FOC

很多有趣的基于电机控制的开源项目，例如 [XKnob](https://github.com/SmallPond/X-Knob)，[Cubli](https://github.com/ZhaJiHu/Cubli_Mini)，[自平衡莱洛三角](https://oshwhub.com/45coll/zi-ping-heng-di-lai-luo-san-jiao_10-10-ban-ben)，它们都是基于ESP单片机和Arduino框架的simpleFOC算法实现的。基于Arduino开发简单、快速，有丰富的支持库，但是Arduino框架的缺点也很明显：它的性能和灵活性都不够好，对于一些复杂的项目，Arduino框架在运行效率、灵活性、可扩展性等方面都不够好。

ESP-IDF是由Espressif开发的官方框架，它基于FreeRTOS，旨在为嵌入式开发者提供丰富的工具和库，以方便创建针对ESP单片机的高性能、高度定制的应用程序。ESP-FOC是一个基于ESP-IDF的FOC算法库，它是"FOC控制-从理论到实践"的产物。它可以帮助您快速实现FOC算法，实现电机的闭环控制。


| 框架版本 | MCU | 电机 | 编码器 | 驱动IC| 电流环反馈 |
| --- | --- | --- | --- | --- | --- |
| idf_v5.1 | ESP32S3 | 3205BLDC | MT6701 | EG2133 | 无 |

## Features
- [x] FOC component作为易移植组件
- [x] 速度环实现
- [x] 位置环实现
- [ ] 电流环实现
- [ ] 适配模型预测控制器
- [ ] 适配自适应滑膜观测器

## How to use esp-foc
```
# clone the repo
git clnoe https://github.com/yiyangc18/ESP-FOC.git
```
基于esp-idf的项目，可以直接复制components文件夹到你的工程目录下。如果使用SPI协议的位置传感器以及三相PWM驱动IC，修改/components/hall_sensor以及motor_pwm 中的引脚定义即可。
foc算法中与硬件连接的工作在FOC/esp_hw_link.c中完成，你可以需要根据自己的硬件连接情况修改这个文件中的函数实现，例如传感器位置更新和PWM占空比更新函数。

虽然本项目是基于ESP-IDF开发的，但在实现foc算法组件时，尽量避免使用ESP-IDF的API，以便于移植到其他平台。FOC组件中只有esp_hw_link.c中的函数实现与ESP-IDF相关，如果在其他平台上使用，只需要修改这个文件中的函数实现即可。

## Example for use

可以参考： [main.c](main/main.c)，其中保留了进行传感器、pwm驱动、速度环、位置环测试相关的代码。
```
void app_main(void)
{

  foc_init();

  //speed test
  pid_control_mode_flag = SPEED_LOOP_CONTROL;
  speed_pid_handler.expect = PI;
}

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

```

## How to test & parameters tuning
FOC组件的构成如下：
```
FOC
│  CMakeLists.txt
│  encoder.c
│  esp_hw_link.c
│  fast_math.c
│  filter.c
│  foc.c
│  pid.c
│
└─include
    │  encoder.h
    │  esp_hw_link.h
    │  fast_math.h
    │  filter.h
    │  foc.h
    │  foc_config.h
    └─ pid.h
```
它们的功能分配：
| 文件 | 功能 |
| --- | --- |
| `encoder.h` | 电机转子速度、位置更新函数声明。|
| `fast_math.h` | 快速计算函数的宏定义和函数声明。|
| `filter.h` | 低通滤波器的数据结构和相关函数声明。|
| `foc.h` | 电机FOC算法的数据结构和变量声明。|
| `pid.h` | PID算法的数据结构和函数声明。|
| `foc_config.h` | FOC控制参数配置。|
| `esp_hw_link.h` | 与硬件连接相关的函数声明。|

PID调参在pid.h中进行，也可以再开发基于flash读写的动态调参功能。各源文件中保留并注释了在调试foc个子功能（滤波器、PID控制器、速度更新、focloop）日志打印。

## Acknowledgements
- [simpleFOC](https://github.com/simplefoc/Arduino-FOC) 本项目的FOC算法实现参考了simpleFOC的实现。
- [miniFOC](https://github.com/ZhuYanzhen1/miniFOC) 本项目的FOC组件框架参考了miniFOC的实现。
