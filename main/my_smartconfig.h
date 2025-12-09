#ifndef _MY_SMARTCONFIG_H_
#define _MY_SMARTCONFIG_H_

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化并启动SmartConfig配网
 * 
 * 该函数会设置SmartConfig类型为ESPTOUCH，注册事件处理程序，
 * 并使用默认配置启动SmartConfig。
 */
void smartconfig_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _MY_SMARTCONFIG_H_ */