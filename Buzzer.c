#include "Buzzer.h"
#include "GPIO.h"
#include "STC8H_PWM.h"
#include "NVIC.h"
#include "Switch.h"

//			 		 C`	   D`     E`   F`	   G`	   A`	   B`    C``
u16 hz[] = {1047, 1175, 1319, 1397, 1568, 1760, 1976, 2093};

//			   		C	 	 D    E 	 F	  G	   A	  B	   C`
//u16 hz[] = {523, 587, 659, 698, 784, 880, 988, 1047};

static void GPIO_config(void) {
  GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
  GPIO_InitStructure.Pin  = GPIO_PIN;		//ָ��Ҫ��ʼ����IO,
  GPIO_InitStructure.Mode = GPIO_OUT_PP;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
  GPIO_Inilize(GPIO_PORT, &GPIO_InitStructure);//��ʼ��
}
void	PWM_config(u16 hz_val)
{
  PWMx_InitDefine		PWMx_InitStructure;
	
	u16 period = (MAIN_Fosc / hz_val);	// ����ÿ�����ڵļ���ֵ
	
  // ����PWM5
  PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
  PWMx_InitStructure.PWM_Duty   	 	= (u16)(period * 0.01);	//PWMռ�ձ�ʱ��, 0~Period
  PWMx_InitStructure.PWM_EnoSelect    = ENO5P;			//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
  PWM_Configuration(PWM5, &PWMx_InitStructure);			//��ʼ��PWM,  PWMA,PWMB

  // ����PWMB
  PWMx_InitStructure.PWM_Period   = period - 1;			//����ʱ��,   0~65535
  PWMx_InitStructure.PWM_DeadTime = 0;					//��������������, 0~255
  PWMx_InitStructure.PWM_MainOutEnable= ENABLE;			//�����ʹ��, ENABLE,DISABLE
  PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;			//ʹ�ܼ�����, ENABLE,DISABLE
  PWM_Configuration(PWMB, &PWMx_InitStructure);			//��ʼ��PWMͨ�üĴ���,  PWMA,PWMB

  // �л�PWMͨ��
  PWM5_SW(PWM5_SW_P00);					//PWM5_SW_P20,PWM5_SW_P17,PWM5_SW_P00,PWM5_SW_P74

  // ��ʼ��PWMB���ж�
  NVIC_PWM_Init(PWMB,DISABLE,Priority_0);
}
// ��ʼ��������
void Buzzer_init(){
	// ��չ�Ĵ���ʹ��
	EAXSFR(); 
  GPIO_config();
}

// ����ָ��Ƶ�ʲ���
void Buzzer_play(u16 hz_val){
	PWM_config(hz_val);
}

// ��������ȡ����Ӧ������
void Buzzer_beep(u16 idx){
	Buzzer_play(hz[idx - 1]);
}

// ֹͣ����
void Buzzer_stop(){
	
  PWMx_InitDefine		PWMx_InitStructure;
  PWMx_InitStructure.PWM_MainOutEnable= DISABLE;			//�����ʹ��, ENABLE,DISABLE
  PWMx_InitStructure.PWM_CEN_Enable   = DISABLE;			//ʹ�ܼ�����, ENABLE,DISABLE
  PWM_Configuration(PWMB, &PWMx_InitStructure);			//��ʼ��PWMͨ�üĴ���,  PWMA,PWMB
}