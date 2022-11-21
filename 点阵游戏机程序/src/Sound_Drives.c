#include "config.h"

uint8 freq_load[16][2];
uint16 data sound_cnt_time = 0;
uint8 (*music_p)[2] = 0;
uint8 s_p=0;//����ָ��

void Sound_Freq_Init()	//�ڿ���ʱ�����ԭʼƵ�������ڵ�ǰ����Ƶ���¶�ʱ����װ��ֵ
{
	//ԭʼƵ������
	code uint16 prime_freq[16]=
	{
		0xffff,
		/*M*/523,587,659,698,784,880,988,
		/*H*/1046,1175,1318,1397,1568,1760,1967,
		0
	};
	uint16 load_vaule = 0;
	uint8 i = 0;
	do
	{
		load_vaule = (65536-(FOSC/prime_freq[i]));//��ʼ����ʱֵ
		freq_load[i][0] = load_vaule;
		freq_load[i][1] = load_vaule >>8;
	}while(prime_freq[++i]);
}

void Sound_Init(void)		//@24.000MHz
{
    AUXR |= 0x04;                   //��ʱ��2Ϊ1Tģʽ
//  AUXR &= ~0x04;                  //��ʱ��2Ϊ12Tģʽ
//  T2L = T1MS;                     //��ʼ����ʱֵ
//  T2H = T1MS >> 8;
//  AUXR |= 0x10;                   //��ʱ��2��ʼ��ʱ
    IE2 |= 0x04;                    //����ʱ��2�ж�
    EA = 1;
	beep = 1;	//�رշ�����
	sound_cnt_time=0;
	(*music_p)[2] = 0;
	TR2_OFF;
	Sound_Freq_Init();
}

void Sound_Tone(bit ON_OFF,uint8 freq,uint8 s_tms)  //freqx>367
{
	if(ON_OFF)
	{
		T2L = freq_load[freq][0];
		T2H = freq_load[freq][1];
		if(s_tms == 0xff) sound_cnt_time = 0xffff;
		else	sound_cnt_time = (uint16)s_tms*10;
		TR2_NO;
	}
}

void Play_Music(uint8 (*sound)[2])
{
	if(sound_ON){
		s_p=0;
		sound_cnt_time = 0;
		music_p = sound;
		TR2_NO;
	}
}
