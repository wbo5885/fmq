#include "config.h"
#include "GPIO.h"
#include "Delay.h"
#include "Buzzer.h"
// 时间相关定义
// 时间相关定义
#define TIME_UNIT_MS    100     // 每个时值单位对应的毫秒数
#define NOTE_GAP_MS     20      // 音符间隔时间(ms)
#define NOTE_DUTY_RATIO 0.9     // 音符发声占比

// 音符定义
#define REST  0    // 休止符
#define DO    1    // 哆
#define RE    2    // 来
#define MI    3    // 咪
#define FA    4    // 发
#define SOL   5    // 索
#define LA    6    // 拉
#define SI    7    // 西

/****************
手动驱动蜂鸣器
****************/

// 优化: 将音调序列设为只读常量，便于管理
const u8 code notes[] = {
	// 前奏
	5, 1, 3, 5,     // 前奏第一小节
	5, 1, 3, 5,     // 前奏第二小节
	
	// 主歌部分
	5, 5, 6, 2,     // 起来！
	1, 1, 6, 5,     // 不愿
	3, 3, 2, 1,     // 做奴隶的
	6, 6, 5,        // 人们！
	
	5, 5, 6, 2,     // 把我们的
	1, 1, 6, 5,     // 血肉筑成
	3, 3, 2, 1,     // 我们新的
	2, 2, 1,        // 长城！
	
	6, 1, 3, 5,     // 中华民族
	5, 3, 2, 1,     // 到了最危险的时候
	6, 1, 3, 5,     // 每个人被迫着
	5, 3, 2, 1,     // 发出最后的吼声
	
	// 副歌
	5, 5, 5,        // 起来！
	5, 3, 2, 1,     // 起来！
	5, 5, 5,        // 起来！
	3, 5, 6, 5,     // 我们万众一心
	2, 3, 5,        // 冒着敌人的炮火
	3, 2, 1, 6,     // 前进！
	5, 5, 6, 2, 1   // 前进！前进！进！
};

// 优化: 将时值序列设为只读常量
const u8 code durations[] = {
	// 前奏
	4, 4, 4, 4,     // 前奏第一小节
	4, 4, 4, 4,     // 前奏第二小节
	
	// 主歌部分
	4, 2, 2, 4,     // 起来！
	4, 2, 2, 4,     // 不愿
	2, 2, 2, 2,     // 做奴隶的
	4, 2, 6,        // 人们！
	
	4, 2, 2, 4,     // 把我们的
	4, 2, 2, 4,     // 血肉筑成
	2, 2, 2, 2,     // 我们新的
	4, 2, 6,        // 长城！
	
	4, 4, 4, 4,     // 中华民族
	4, 4, 4, 4,     // 到了最危险的时候
	4, 4, 4, 4,     // 每个人被迫着
	4, 4, 4, 4,     // 发出最后的吼声
	
	// 副歌
	4, 4, 8,        // 起来！
	4, 4, 4, 4,     // 起来！
	4, 4, 8,        // 起来！
	2, 2, 2, 2,     // 我们万众一心
	4, 4, 6,        // 冒着敌人的炮火
	2, 2, 4, 4,     // 前进！
	4, 4, 4, 4, 8   // 前进！前进！进！
};

// 检查音符和时值数组长度是否匹配
#if sizeof(notes) != sizeof(durations)
    #error "音符数组与时值数组长度不匹配!"
#endif

// 计算音符实际发声时间
static u16 calcNoteDuration(u8 duration) {
    return (u16)(duration * TIME_UNIT_MS * NOTE_DUTY_RATIO);
}

// 计算音符间隔时间
static u16 calcNoteGap(u8 duration) {
    return (u16)(duration * TIME_UNIT_MS * (1 - NOTE_DUTY_RATIO));
}

// 新增函数：播放歌曲，通过封装播放循环提升代码可读性
static void playSong(void) {
	u8 i, len;
	u16 playTime, gapTime;
	len = sizeof(notes) / sizeof(notes[0]);

	for(i = 0; i < len; i++){
        // 计算当前音符的发声时间和间隔时间
        playTime = calcNoteDuration(durations[i]);
        gapTime = calcNoteGap(durations[i]);

		// 处理休止符和普通音符
		if (notes[i] != REST) {
			Buzzer_beep(notes[i]);
            delay_X_ms(playTime);
            Buzzer_stop();
		} else {
            delay_X_ms(playTime + gapTime);
            continue;
		}

        // 音符间隔
        delay_X_ms(gapTime);
	}
}

// 初始化GPIO
void main() {
	// 扩展寄存器使能，初始化蜂鸣器硬件
	EAXSFR(); 
	Buzzer_init();
	EA = 1;
	
	// 播放预设乐曲
	playSong();
    Buzzer_stop();  // 确保播放结束后蜂鸣器停止
	
	// 无限循环保持程序运行
	while(1);
}