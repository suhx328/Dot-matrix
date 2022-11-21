#ifndef __DISPLAY_DRIVES_H__
#define __DISPLAY_DRIVES_H__

#include "typedef.h"
#define cdata	P0	//�����ݿ�
#define IORow_A P1	//��ѡ
#define IORow_B P2

extern void ROW_data(uint8 rdata);		//��ѡ��ģ��
extern void DisPoint(uint8 x,uint8 y);	//��ʾָ��
extern void MatxDisp(uint8 *buf,uint8 duty);//ˢ����ʾ
extern void SMG_Display(uint16 goal,uint8 duty);

#endif

