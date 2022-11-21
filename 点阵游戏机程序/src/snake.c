/*
	2014-1-26 00:00:00	:���̰������Ϸģ��
	2014-3-8 23:27:05	:������Ϸ�����ųԵ�ʳ������Ӷ��ӿ���Ϸ�ٶ�
	2014-3-15 20:06:15	:����ʵ��Ӳ����ʣ���������ã���̰���߽ṹ����̫��RAM���������Ż���
*/
#include "config.h"

#define NORSPEED 360
#define ASPEED 100
#define WIDTH  8	//��Ϸ��Ļ���
#define LENGTH 16	//��Ϸ��Ļ���� 

static struct Food
{
    uint8 x;//ʳ��ĺ�����
    uint8 y;//ʳ���������
}food;


struct Snake snake;

enum direc{up, down, left, right};//�ߵ��˶�����

static void Snake_Init();		//��ʼ����Ϸ
static void Snake_Disp();	//��Ϸ��ʾ
static void Snake_Run();		//����ǰ��һ��
static void Create_Food();		//�������ʳ��

static void Create_Food()
{
	uint8 i;
	food.x = rand() % WIDTH;	//��Χ1-16;
	food.y = rand() % LENGTH;	//������������;//�ü�������Ϊ�����������
	for(i=1;i<(snake.node);i++)//��������ʳ���Ƿ����߱����ص�
	{
		if((food.x==snake.x[i])&&(food.y==snake.y[i]))
			Create_Food();	//�ص������³��� 
	}
}

static void Snake_Init()
{	
	uint8 c;	
	for(c=0;c < MAX_SNAKE;c++)//����ڴ�����
	{
		snake.x[c]=-1;
		snake.y[c]=-1;
	}
	snake.life = 1;				//����������
	snake.node = 3;			//Ĭ�ϳ���3��
	snake.direc = up;//��ʼ����
	snake.x[0]=4;
	snake.y[0]=5;
	snake.x[1]=4;
	snake.y[1]=4;
	snake.x[2]=4;
	snake.y[2]=3;
	speeds = NORSPEED; 	//��Ϸ�ٶ�
	tms = 0;
	scores = 0;
	Create_Food();		//���ɵ�һ��ʳ��
}

static void Snake_Run()	//�������㷨
{	
	uint8 i;
	for(i=(snake.node-1);i>0;i--)		//��ͷ��Ϊǰ������������������ǰ�� 
	{
		snake.y[i]=snake.y[i-1];
		snake.x[i]=snake.x[i-1];
	}
	switch (snake.direc)	//�����ߵķ����ǰ��
	{
		case up: 	snake.y[0]+=1;	break;
		case down:	snake.y[0]-=1;	break;
		case left:	snake.x[0]-=1;	break;
		case right:	snake.x[0]+=1;	break;
	}
}

static void Snake_Disp()	//��Ϸ������ʾ
{
	uint8 p = 16;
//	for(p=0;p<snake.node;p++)//������ͼ��
//	{		
//		delayus(500-duty);
//		DisPoint(snake.x[p],snake.y[p]);
//		delayus(duty/5);
//		cdata = 0xff;
//	}
//	DisPoint(food.x,food.y);//����ʳ��ͼ��
//	delayus(duty);
//	cdata = 0xff;
//	
//	for(i=0;i<16;i++)
//	{
//		DispRAM[i] = 0;
//	}
	while(p--) DispRAM[p]=0;	//�������	
	for(p=0;p<snake.node;p++)	//д���»���
	{	
		DispRAM[snake.y[p]] |= 0x80 >> (snake.x[p]);
	}
//	DispRAM[food.y] |= 0x80 >> (food.x);
	
	MatxDisp(DispRAM,duty);
	DisPoint(7-food.x,food.y);
	delayus(duty);
}

void Snake_Game()	//��Ϸ����
{
	Snake_Init();		//��ʼ����Ϸ
	Play_Music(sound_canon);
	while(snake.life)	//�������������
	{
		uint8 i;
		switch(keypad)//ֱ�Ӷ�ȡ��ֵ��̰���߲���Ҫ����
		{
			case K_UP:	if((snake.direc==left)||(snake.direc==right))	snake.direc=up;
				//����UP��ʱ��ֻ������ˮƽ����ʱ���ܸı�
				break;
			case K_DOWN:if((snake.direc==left)||(snake.direc==right))	snake.direc=down;
				//����DOWN��ʱ��ֻ������ˮƽ����ʱ���ܸı�
				break;
			case K_LEFT:if((snake.direc==up)||(snake.direc==down))		snake.direc=left;
				//����left��ʱ��ֻ�����ڴ�ֱ����ʱ���ܸı�
				break;
			case K_RIGHT:if((snake.direc==up)||(snake.direc==down))		snake.direc=right;
				//����right��ʱ��ֻ�����ڴ�ֱ����ʱ���ܸı�
				break;
			default :
				break;
		}
		
		if(KEY_OK==0) speeds = ASPEED;//���ټ�
		else speeds = NORSPEED;
		keypad = K_NULL;
		
		if(tms > speeds)	//����ǰ��һ��
		{
			tms=0;
			Snake_Run();	//����ɢ��
		}
		if((snake.x[0]==food.x) && (snake.y[0]==food.y))//�Ƿ�Ե�ʳ��
		{
			Sound_Tone(sound_ON,4,5);
//			getfood=1;//ֱ���ڴ������߳��Ȼ�����һ����ʾɨ�����һ��ԭ��������Ӱ����ͨ���ӳ�
			snake.node++;
//			game_speed-=10;	//ÿ��һ��ʳ������10���ٶ�
			Create_Food();	//������ʳ��
		}
		if((snake.x[0]>WIDTH-1)||(snake.x[0]<0)||(snake.y[0]>LENGTH-1)||(snake.y[0]<0))//�Ƿ�����
		{
			snake.life=0;	//������ǽ������
		}
		for(i=3;i<snake.node;i++)//���ߵĵ��Ľڿ�ʼ�ж��Ƿ�ײ���Լ��ˣ���Ϊ��ͷ������ײ�������Ľ�
		{
			if((snake.x[i]==snake.x[0])&&(snake.y[i]==snake.y[0]))//�Ƿ��Բ�
				snake.life=0;	//��ͷ������������
		}
		Snake_Disp();	//��ʾ��Ϸͼ��
		scores = snake.node-3;
		SMG_Display(scores,duty);
	}
	scores = snake.node-3;
	TR2_OFF;
}



