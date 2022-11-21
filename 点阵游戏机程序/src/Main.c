/*
//���ܣ��������϶���˹������Ϸ��Դ���룬������ϷGUI������˹���顢̰���ߡ�����������Ϸ������ϰ�����Ϸ
ע��ϵͳ����Ƶ��Ϊ24MHz
*/

#include "config.h"
volatile uint8 data keypad = K_NULL;	//����״̬
volatile uint8 data keycont = 0;		//���̼��������ʱ��
volatile uint16 data tms = 0;			//1ms T0��ʱ��
bit key_state = 0;						//����״̬

uint16 data speeds = 0;		//��Ϸ�ٶ�
uint16 data scores = 0;		//��Ϸ�Ʒ�
bit Glife = 1;				//��Ϸ����
bit sound_ON = 1;			//��Ч����
uint8 data duty = 80;		//PWM��Ĭ������
uint8 data DispRAM[16]={0};
uint8 code bitman[8]={1,2,4,8,16,32,64,128};//����������Ŷ�λ0~7bitλ

uint8 code num[10][5]=		//����
{
	{0x7,0x5,0x5,0x5,0x7},
	{0x2,0x6,0x2,0x2,0x7},
	{0x7,0x1,0x7,0x4,0x7},
	{0x7,0x1,0x7,0x1,0x7},
	{0x5,0x5,0x7,0x1,0x1},
	{0x7,0x4,0x7,0x1,0x7},
	{0x7,0x4,0x7,0x5,0x7},
	{0x7,0x1,0x1,0x1,0x1},
	{0x7,0x5,0x7,0x5,0x7},
	{0x7,0x5,0x7,0x1,0x7}
};

/*********************PORT_INIT********************/
void Port_Init()
{
	P0M0 = 0Xff;
	P0M1 = 0X00;
	P1M0 = 0Xff;
	P1M1 = 0X00;
	P2M0 = 0Xff;
	P2M1 = 0X00;
	P3M0 = 0X00;
	P3M1 = 0X00;
	P4M0 = 0Xff;
	P4M1 = 0X00;
	P5M0 = 0Xff;
	P5M1 = 0X00;
}

void Init_Timer()
{
/******************��ʱ��0�ж�����**********************/
	AUXR |= 0x80;                   //��ʱ��0Ϊ1Tģʽ
    TMOD = 0x00;                    //���ö�ʱ��Ϊģʽ0(16λ�Զ���װ��)
    TL0 = T1MS;                     //��ʼ����ʱֵ
    TH0 = T1MS >> 8;
    TR0 = 1;                        //��ʱ��0��ʼ��ʱ
    ET0 = 1;                        //ʹ�ܶ�ʱ��0�ж�
	
/*****************�ⲿ�ж�����*************************/	
	IT0 = 1;                    //����INT0�½��ش���
    EX0 = 1;                    //ʹ���ⲿ�ж�INT0
	IT1 = 1;                    //����INT1�½��ش���
    EX1 = 1;                    //ʹ���ⲿ�ж�INT1
	INT_CLKO |= 0x70;           //ʹ��INT2,INT3,INT4(EX4 = 1,EX3 = 1,EX2 = 1
	
	EA = 1;	
}

void Array_CLR(uint8 *p)
{
	uint8 i=0;
	while(i!=16)
	{
		p[i] = 0x00;
		i++;
	}
}

void ShowScore(uint score)
{
	uint8 i;
	uint8 qx,bx,sx,gx;
	qx = score /1000;
	bx = score %1000 /100;
	sx = score %100/10;
	gx = score %10;

	Array_CLR(DispRAM);

	for(i=0;i<5;i++)
		DispRAM[8-i] = (num[sx][i]<<4) | (num[gx][i]);
	
	for(i=0;i<5;i++)
		DispRAM[14-i] = (num[qx][i]<<5) | (num[bx][i]<<1);
				
	MatxDisp(DispRAM,duty);
}

void main()
{
	Port_Init();
	Sound_Init();
	Init_Timer();
	Sound_Tone(1,14,5);//��
	
	srand(TL0);		
	delayms(100);
	Play_Music(sound_game_start);
	Flash_Screen_Clear();
	while(1) 
	{
		tms = 0;
		keypad = K_NULL;//�������״̬
		switch (GUI_Main())	//�����û���Ϸѡ����棬����ѡ�����Ϸ
		{
			case 1:	Tetris_Game();	break;
			case 2:	Snake_Game();	break;
			case 3:	RAC_Game();		break;
			case 4:	Shot_Game();	break;
		}
		Play_Music(sound_game_over);
		Flash_Screen_Clear();
		ShowScore(scores);	//�����������Դ�
		delayms(300);
		
		keypad = K_NULL;
		while(keypad != K_UP)	//��û����OK��ʱ���ַ�����ʾ
		{
			MatxDisp(DispRAM,duty);
			SMG_Display(scores,duty);
		}
	}	
}

/* Timer0 interrupt routine */
void T0_Timer_1ms_int() interrupt 1 using 1
{
	if(tms<0xfffe)	tms++;	//��ֹ���
	if(key_state){ if(!--keycont) key_state=0;}

	if(PIN_TR2)//TR2����򿪣���ʼ������Ч
	{
		if(!sound_cnt_time--)
		{
			TR2_OFF;
			beep = 1;
			if(music_p[s_p][1])
			{
				Sound_Tone(sound_ON,music_p[s_p][0],music_p[s_p][1]);
				s_p++; //�Զ�װ����һ��������ʵ����Ч����Ϸ��ͬʱ������
			}
		}
	}
	if(KEY_DOWN==0 & KEY_UP==0)	IAP_CONTR=0x60;//�����λ���س���
}

void T2_Timer_Sound_freq() interrupt 12           //�ж����
{
     beep = ~beep; //������Ƶ����
}

uint8 Get_Kvuale(uint8 key_delay)//��ֵ������������Ϊ�����ȵ���
{
	uint8 kvuale = K_NULL;	//���صĳ�ʼֵΪ��ֵ
	if(keypad != K_NULL)	//����ֵ��ΪNULLʱ,˵���а�������
	{	
		if(!key_state)		//������״̬λstateΪ0ʱ˵���ǵ�һ�δ�����Ч,Ϊ1ʱΪ�ظ�����
		{
			key_state=1;	//��1����ֹ�ظ�����
			kvuale = keypad;//��ö�ȡ��ֵ
			keycont = key_delay;//��������ʱ��
		}
		keypad = K_NULL;	//�����ֵ
	}
	return kvuale;
}
/*********�ⲿ�жϰ�����**********/
void exint0() interrupt 0       //INT0
{
	keypad = 0;
}
void exint1() interrupt 2       //INT1
{
	keypad = 1;	
}
void exint2() interrupt 10      //INT2
{
	keypad = 2;
}
void exint3() interrupt 11      //INT3
{
	keypad = 3;
}
void exint4() interrupt 16      //INT4
{	
	keypad = 4;
}


