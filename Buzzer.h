#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "Config.h"

#define GPIO_PORT		GPIO_P0
#define GPIO_PIN		GPIO_Pin_0

// 初始化蜂鸣器
void Buzzer_init();

// 按照指定频率播放
void Buzzer_play(u16 hz_val);

void Buzzer_beep(u16 idx);

// 停止播放
void Buzzer_stop();


#endif