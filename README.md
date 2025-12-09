# ESP8266 天气显示项目

这是一个基于 ESP8266 的嵌入式天气显示系统，使用 FreeRTOS 实现实时任务调度，通过 Wi-Fi 连接网络获取时间与天气信息，并在 OLED 屏幕（SSD1306）上显示相关信息。

## 功能特点

- Wi-Fi 连接管理（支持 SmartConfig 配网）
- 自动同步网络时间（SNTP）
- 驱动 SSD1306 OLED 显示屏
- 显示实时天气信息（温度、湿度等）
- 支持中英文字符显示
- 使用 FreeRTOS 多任务机制

## 硬件需求

- ESP8266 开发板（如 NodeMCU、Wemos D1 Mini 等）
- SSD1306 OLED 显示屏（I2C 接口，128x64 分辨率）
- 连接线若干

## 软件依赖

- [ESP8266_RTOS_SDK](https://github.com/espressif/ESP8266_RTOS_SDK) v3.2+
- [ESP8266-RTOS-SSD1306](https://github.com/Fonger/ESP8266-RTOS-SSD1306) 组件
- [ESP8266-RTOS-FONTS](https://github.com/Fonger/ESP8266-RTOS-FONTS) 组件

## 构建与烧录

1. 配置开发环境并设置 `IDF_PATH` 环境变量
2. 克隆本项目到本地
3. 进入项目目录，执行 `make menuconfig` 配置相关参数
4. 执行 `make flash` 编译并烧录固件
5. 执行 `make monitor` 查看串口输出

或者使用 CMake 构建系统：
1. 配置开发环境并设置 `IDF_PATH` 环境变量
2. 克隆本项目到本地
3. 进入项目目录，执行 `idf.py menuconfig` 配置相关参数
4. 执行 `idf.py flash` 编译并烧录固件
5. 执行 `idf.py monitor` 查看串口输出

## 使用方法

1. 烧录完成后，设备会进入 SmartConfig 配网模式
2. 使用支持 SmartConfig 的 App（如乐鑫官方 Espressif App）进行配网
3. 配网成功后，设备会自动连接 Wi-Fi 并同步网络时间
4. 设备会定期从心知天气 API 获取天气数据并在 OLED 屏幕上显示

## 代码结构

```
├── components          # 第三方组件
│   ├── ESP8266-RTOS-FONTS    # 字体库组件
│   └── ESP8266-RTOS-SSD1306  # OLED 驱动组件
├── main                # 主要应用程序
│   ├── main.c          # 应用程序入口点
│   ├── my_wifi.c/h     # Wi-Fi 连接管理
│   ├── my_smartconfig.c/h # SmartConfig 配网功能
│   ├── my_sntp.c/h     # 网络时间同步
│   ├── my_ssd1306.c/h  # OLED 显示驱动封装
│   ├── my_ui.c/h       # 用户界面显示
│   ├── weather.c/h     # 天气数据获取与解析
│   └── component.mk    # 组件构建配置
├── Makefile            # Make 构建配置
└── CMakeLists.txt      # CMake 构建配置
```

## 注意事项

1. 需要在 `weather.c` 中配置正确的心知天气 API 密钥和城市代码才能获取天气数据
2. OLED 屏幕默认使用 I2C 接口，GPIO 连接可在 `my_ssd1306.c` 中配置
3. 项目同时支持 Make 和 CMake 两种构建系统