#include "config.h"

#define NORSPEED 500	//������Ϸ�ٶ�
#define MAXSPEED 28		//����DOWN����Ϸ�ٶ�
#define KEYSET	50		//�����ж���ʱ

static char x=0,y=15;		//��̬�����λ�ñ�
static uint8 boxID;			//��ǰ������
static uint8 showbox[4];	//��̬���黺��
static uint8 sblock[16];	//��̬���黺��

uint8 code getscore[5] = {0,1,3,7,15};	//һ�����������÷�
uint8 code rotlist[19]={0,2,1,4,3,6,5,8,9,10,7,12,13,14,11,16,17,18,15};//������ת����
uint8 code blocks[19][4]=
{
	{0x00,0x00,0x18,0x18},	//��	0
	{0x10,0x10,0x10,0x10},	// |	1
	{0x00,0x00,0x00,0x3c},	// һ	2
	{0x00,0x08,0x18,0x10},	//����	3
	{0x00,0x00,0x30,0x18},	//	Z	4
	{0x00,0x10,0x18,0x08},	//		5
	{0x00,0x00,0x18,0x30},	//		6
	{0x00,0x08,0x08,0x18},	//	��	7
	{0x00,0x00,0x38,0x08},	//		8
	{0x00,0x18,0x10,0x10},	//		9
	{0x00,0x00,0x20,0x38},	//		10
	{0x00,0x10,0x10,0x18},	//	L	11
	{0x00,0x00,0x08,0x38},	//		12
	{0x00,0x18,0x08,0x08},	//		13
	{0x00,0x00,0x38,0x20},	//		14
	{0x00,0x00,0x10,0x38},	//	͹	15
	{0x00,0x08,0x18,0x08},	//		16
	{0x00,0x00,0x38,0x10},	//		17
	{0x00,0x10,0x18,0x10},	//		18
};


static void RandBox()	//���װ��һ������
{
	uint8 i;
	boxID = rand() % 19;
	   if(KEY_LEFT == 0 && KEY_RIGHT == 0)	boxID = 2;//��Ϸ����,�㶮�ģ�^.^
	for(i=0;i<4;i++)	//װ�ط���
	{
		showbox[i] = blocks[boxID][3-i];//����Ӳ�����ֵ�ԭ�򣬵���װ�أ�����������
	}
}

static char Check_LR(uint8 lr)	//0��1��;;�����ƶ�����1�������ϰ�����0
{
	uint8 i;
	if(lr)
	{
		for(i=0;i<4;i++)
		{
			if ((showbox[i] & 0x01) || (showbox[i] >>1 & sblock[i+y]))//�߽磬��̬����
				return 0;	//��⵽�Ƴ��߽���˳�
		}
		return 1;
	}
	else 
	{
		for(i=0;i<4;i++)
		{
			if((showbox[i] & 0x80) ||  (showbox[i] <<1 & sblock[i+y]))//
				return 0;	//��⵽�Ƴ��߽���˳�
		}
		return 1;
	}
}
static void moveone(bit lr, uint8 *buf)
{
	uint8 i;
	if(lr)
	{
		for(i=0;i<4;i++)
		{
			buf[i] = buf[i]>>1;
		}
	}
	else 
	{
		for(i=0;i<4;i++)
		{
			buf[i] = buf[i]<<1;
		}
	}
}

static char BoxMover(char lr,uint8 *buf)
{
	char i,j;
	if(lr>0)
	{
		for(j=0;j<lr;j++)
		{
			for(i=0;i<4;i++)
			{
				if(buf[i] & 0x80)
				{	
					return  0;
				}
				buf[i] = buf[i] <<1;
				
				if(j+1 == lr)
					if(buf[i]  & sblock[i+y])
						return  0;
			}
		}
		return  1;
	}
	
	if(lr<0)
	{
		for(j=0;j<(-lr);j++)
		{
			for(i=0;i<4;i++)
			{
				if(buf[i] & 0x01)
				{	
					return  0;
				}
				buf[i] = buf[i] >>1;
				
				if(j+1 == (-lr))
					if(buf[i]  & sblock[i+y])
						return  0;
			}
		}
		return  1;
	}
	return 1;	
}

static void SpinBox()
{
	uint8 i,tmpid,tmpbuf[4]={0};
	tmpid = rotlist[boxID];
	for(i=0;i<4;i++)
		tmpbuf[i] = blocks[tmpid][3-i];//���������任��һ��ͼ��

	if(x > 0)
	{
		if(BoxMover(x,tmpbuf))
		{
			boxID = tmpid;
			for(i=0;i<4;i++)
				showbox[i] = tmpbuf[i];
		}
	}

	else if(x < 0)
	{
		if(BoxMover(x,tmpbuf))
		{
			boxID = tmpid;
			for(i=0;i<4;i++)
				showbox[i] = tmpbuf[i];
		}
	}
	
	else
	{
		bit f = 1;
		for(i=0;i<4;i++)
		{
			if(tmpbuf[i] & sblock[i+y])
			{
				f = 0;
				break;
			}
		}
		if( f )
		{	
			for(i=0;i<4;i++)
				showbox[i] = tmpbuf[i];
			boxID = tmpid;
		}
	}
}
static char Check_Down()	//��ⷽ���½�һ���Ƿ��뾲̬�����ص�
{
	if((sblock[15] & showbox[0]) | (sblock[15] & showbox[1]))
		return -1;
	
	if(y==0)							return 1;
	if(sblock[y-1] & showbox[0])		return 1;
	if(sblock[y]   & showbox[1])		return 1;
	if(y<15)
		if(sblock[y+1] & showbox[2])	return 1;
	if(y<14)
		if(sblock[y+2] & showbox[3])	return 1;
	
	return 0;
}

static void TetrisDisp() //0:ֻɨ�農̬ͼ�� 1:��̬�Ͷ�̬ͼ��ͬʱɨ��
{
	uint p;
	for(p=0;p<16;p++)
		DispRAM[p] = sblock[p];
	
	for(p=y;p<y+4;p++)
		if(p<16) DispRAM[p] |= showbox[p-y];	

	MatxDisp(DispRAM,duty);
}

static void RowFull()	//����Ƿ����в���ش���
{
	uint8 p,k;
	uint8 tmps=0;
	for(p=0;p<16;p++)
	{
		if(sblock[p] == 0xff)
		{
			char c = 4;	//��˸����
			tmps++;
			
			while(c--)
			{
				#define BLINKTIME 128	//������˸�ٶ�
				uint tmpct = tms+BLINKTIME;
				Sound_Tone(sound_ON,5,3);
				sblock[p] = ~sblock[p];
				while(tmpct > tms)
				{
					MatxDisp(sblock,duty);
					SMG_Display(scores,duty);
				}
			}
			for(k=p;k<15;k++)
			{
				sblock[k] = sblock[k+1];
				if(sblock[k] == 0x00)
					break;
			}
			p--;
		}
	}
	scores += getscore[tmps];
}

static void Tetris_Keyscan()	//
{
	int8 k = Get_Kvuale(150);

	switch (k)
	{
		case K_NULL:break;
		
		case K_LEFT:if(Check_LR(0))
					{
						moveone(0, showbox);
						x++;
					}
				break;			
		case K_RIGHT:if(Check_LR(1))
					{
						moveone(1, showbox);
						x--;
					}
				break;
		case K_OK:	
				Sound_Tone(sound_ON & !(AUXR & 0x10),2,8);
				SpinBox();				
				break;
	}
	
	if(KEY_DOWN==0)
		speeds = MAXSPEED;//��down����ʱ�л�������ٶȷ�����׹
	else speeds = NORSPEED;//û��ʱ�ָ������ٶ�
}
static void Tetris_Init()
{
	uint i;
	for(i=0;i<16;i++)	//�����̬�ڴ�
		sblock[i]=0x00;
	scores=0;
	keycont=0;
	keypad=0;
	tms=0;
	speeds = NORSPEED;
	RandBox();	//���װ��һ������
	Glife=1;
}

void Tetris_Game()
{
	Tetris_Init();
	Play_Music(sound_korobelniki);
	while(Glife)
	{
		Tetris_Keyscan();		
		if(tms > speeds)	//��Ϸ�ٶȽ���
		{
			tms=0;
			if(Check_Down() == 1)	//��⶯̬�����Ƿ����޷����½�
			{
				sblock[y]   |= showbox[0];	//��̬�����뾲̬ͼ����
				if(y<15)	sblock[y+1] |= showbox[1];
				if(y<14)	sblock[y+2] |= showbox[2];
				if(y<13)	sblock[y+3] |= showbox[3];

				RowFull();	//����Ƿ�������,��ִ���ȹش���				 
				RandBox();	//�������һ���·���
				x=0;		
				y=15;		//����̬���������λ,ӭ����һ���·���								
			}
			else if(Check_Down() == -1)//��⵽�����Ѷѻ�������
			{
				Glife = 0;//������Ϸ
			}
			else 
			{
				y--;	//�����½�һ��
			}
		}
		TetrisDisp();//ˢ����Ϸͼ��
		SMG_Display(scores,duty);
	}
	TR2_OFF;
}







