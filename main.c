#include "config.h"
#include "GPIO.h"
#include "Delay.h"
#include "Buzzer.h"
// ʱ����ض���
// ʱ����ض���
#define TIME_UNIT_MS    100     // ÿ��ʱֵ��λ��Ӧ�ĺ�����
#define NOTE_GAP_MS     20      // �������ʱ��(ms)
#define NOTE_DUTY_RATIO 0.9     // ��������ռ��

// ��������
#define REST  0    // ��ֹ��
#define DO    1    // ��
#define RE    2    // ��
#define MI    3    // ��
#define FA    4    // ��
#define SOL   5    // ��
#define LA    6    // ��
#define SI    7    // ��

/****************
�ֶ�����������
****************/

// �Ż�: ������������Ϊֻ�����������ڹ���
const u8 code notes[] = {
	// ǰ��
	5, 1, 3, 5,     // ǰ���һС��
	5, 1, 3, 5,     // ǰ��ڶ�С��
	
	// ���貿��
	5, 5, 6, 2,     // ������
	1, 1, 6, 5,     // ��Ը
	3, 3, 2, 1,     // ��ū����
	6, 6, 5,        // ���ǣ�
	
	5, 5, 6, 2,     // �����ǵ�
	1, 1, 6, 5,     // Ѫ������
	3, 3, 2, 1,     // �����µ�
	2, 2, 1,        // ���ǣ�
	
	6, 1, 3, 5,     // �л�����
	5, 3, 2, 1,     // ������Σ�յ�ʱ��
	6, 1, 3, 5,     // ÿ���˱�����
	5, 3, 2, 1,     // �������ĺ���
	
	// ����
	5, 5, 5,        // ������
	5, 3, 2, 1,     // ������
	5, 5, 5,        // ������
	3, 5, 6, 5,     // ��������һ��
	2, 3, 5,        // ð�ŵ��˵��ڻ�
	3, 2, 1, 6,     // ǰ����
	5, 5, 6, 2, 1   // ǰ����ǰ��������
};

// �Ż�: ��ʱֵ������Ϊֻ������
const u8 code durations[] = {
	// ǰ��
	4, 4, 4, 4,     // ǰ���һС��
	4, 4, 4, 4,     // ǰ��ڶ�С��
	
	// ���貿��
	4, 2, 2, 4,     // ������
	4, 2, 2, 4,     // ��Ը
	2, 2, 2, 2,     // ��ū����
	4, 2, 6,        // ���ǣ�
	
	4, 2, 2, 4,     // �����ǵ�
	4, 2, 2, 4,     // Ѫ������
	2, 2, 2, 2,     // �����µ�
	4, 2, 6,        // ���ǣ�
	
	4, 4, 4, 4,     // �л�����
	4, 4, 4, 4,     // ������Σ�յ�ʱ��
	4, 4, 4, 4,     // ÿ���˱�����
	4, 4, 4, 4,     // �������ĺ���
	
	// ����
	4, 4, 8,        // ������
	4, 4, 4, 4,     // ������
	4, 4, 8,        // ������
	2, 2, 2, 2,     // ��������һ��
	4, 4, 6,        // ð�ŵ��˵��ڻ�
	2, 2, 4, 4,     // ǰ����
	4, 4, 4, 4, 8   // ǰ����ǰ��������
};

// ���������ʱֵ���鳤���Ƿ�ƥ��
#if sizeof(notes) != sizeof(durations)
    #error "����������ʱֵ���鳤�Ȳ�ƥ��!"
#endif

// ��������ʵ�ʷ���ʱ��
static u16 calcNoteDuration(u8 duration) {
    return (u16)(duration * TIME_UNIT_MS * NOTE_DUTY_RATIO);
}

// �����������ʱ��
static u16 calcNoteGap(u8 duration) {
    return (u16)(duration * TIME_UNIT_MS * (1 - NOTE_DUTY_RATIO));
}

// �������������Ÿ�����ͨ����װ����ѭ����������ɶ���
static void playSong(void) {
	u8 i, len;
	u16 playTime, gapTime;
	len = sizeof(notes) / sizeof(notes[0]);

	for(i = 0; i < len; i++){
        // ���㵱ǰ�����ķ���ʱ��ͼ��ʱ��
        playTime = calcNoteDuration(durations[i]);
        gapTime = calcNoteGap(durations[i]);

		// ������ֹ������ͨ����
		if (notes[i] != REST) {
			Buzzer_beep(notes[i]);
            delay_X_ms(playTime);
            Buzzer_stop();
		} else {
            delay_X_ms(playTime + gapTime);
            continue;
		}

        // �������
        delay_X_ms(gapTime);
	}
}

// ��ʼ��GPIO
void main() {
	// ��չ�Ĵ���ʹ�ܣ���ʼ��������Ӳ��
	EAXSFR(); 
	Buzzer_init();
	EA = 1;
	
	// ����Ԥ������
	playSong();
    Buzzer_stop();  // ȷ�����Ž����������ֹͣ
	
	// ����ѭ�����ֳ�������
	while(1);
}