#ifndef _CONFIG_H_
#define _CONFIG_H_

#define FOSC 24000000L
#define T1MS (65536-FOSC/1000)	//1Tģʽ
/*******************include*********************/
#include <stdlib.h>
#include "intrins.h"

#include "STC15F2K60S2.H"
#include "typedef.h"
//#include "common.h"
#include "delay.h"

#include "Display_Drives.h"
#include "Sound_Drives.h"
#include "GUI.h"

#include "Tetris.h"
#include "snake.h"
#include "RAC.h"
#include "Shot.h"

/*******************PIN_CONFIG*********************/
//��Դ������
sbit beep = P5^5;		
/*******keyborad******/
sbit KEY_UP		=P3^2;	
sbit KEY_DOWN	=P3^6;
sbit KEY_LEFT	=P3^0;
sbit KEY_RIGHT	=P3^3;
sbit KEY_OK		=P3^7;
//exint�����������ڼ����ⲿ�ж�����
#define K_UP		0
#define K_DOWN		2
#define K_RIGHT		1
#define K_LEFT		4
#define K_OK		3
#define K_NULL		255	//δ����ֵ
/*******����ܹܽ�����********/
sbit SMG_S1 = P2^3;		//�����λѡ
sbit SMG_S2 = P2^5;
sbit SMG_S3 = P4^2;

sbit SMG_A = P4^1;
sbit SMG_B = P2^4;
sbit SMG_C = P2^1;
sbit SMG_D = P2^0;
sbit SMG_E = P4^4;
sbit SMG_F = P2^2;
sbit SMG_G = P1^4;
/*********����ܽ�����*********/
//�������ݿ�
sbit LED_D0 = P1^5;
sbit LED_D1 = P4^1;
sbit LED_D2 = P2^2;
sbit LED_D3 = P1^4;
sbit LED_D4 = P2^1;
sbit LED_D5 = P4^4;
sbit LED_D6 = P2^0;
sbit LED_D7 = P2^4;
//������ѡ
sbit LED_R16= P2^6;
sbit LED_R15= P4^5;
sbit LED_R14= P0^4;
sbit LED_R13= P2^7;
sbit LED_R12= P0^0;
sbit LED_R11= P0^3;
sbit LED_R10= P0^1;
sbit LED_R9 = P0^2;
sbit LED_R8 = P0^5;
sbit LED_R7 = P0^7;
sbit LED_R6 = P1^3;
sbit LED_R5 = P0^6;
sbit LED_R4 = P1^0;
sbit LED_R3 = P1^2;
sbit LED_R2 = P1^1;
sbit LED_R1 = P1^6;
/******************ͨ��ȫ�ֱ���*******************/
#define VIEW_FPS		600		//��Ϸѡ����֡���ʱ��
#define SMOVE_SPEED		28		//��Ϸѡ���滭��ƽ���ٶ�

extern volatile uint8 data keypad;	//����״̬
extern volatile uint16 data scores;	//��Ϸ�Ʒ�
extern volatile uint8 data keycont;	//���̼��������ʱ��
extern uint16 data tms;				//1ms T0��ʱ��
extern uint16 data speeds;			//��Ϸ�ٶ�
extern uint8 data duty;				//��ʾռ�ձ�
extern bit sound_ON;				//��������
extern bit Glife;					//��Ϸ����
extern uint8 data DispRAM[16];		//��ʾ������
extern uint8 code bitman[8];		//�������ֶ�λ
extern uint8 code num[10][5];

void ShowScore(uint score);
//void Copier(uint8 *dat,uint8 *cpy);
uint8 Get_Kvuale(uint8 key_delay);

#endif