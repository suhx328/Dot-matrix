#ifndef __SNAKE_H_
#define __SNAKE_H_

#include "typedef.h"

#define MAX_SNAKE 128
struct Snake
{
	char x[MAX_SNAKE];	//snake.x[0]Ϊ��ͷ
	char y[MAX_SNAKE];
	char node;			//�ߵĽ���
	char direc;		//unsigned char direction;/*���ƶ�����*/
	int life_time;		//�ߴ��ʱ��
	uint8 life;			//�ߵ�����,1����,0����
};

extern void Snake_Game(void);

#endif
