/****************************************Copyright (c)**************************************************
**--------------File Info-------------------------------------------------------------------------------
** File name:			io.h
** Created by:			ZuoYouPaPa
** Last modified Date:  2020-3-10
** Last Version:		1.0
** Descriptions:		IOģʽ����
********************************************************************************************************/
#ifndef _IO_H
#define _IO_H

#include "STC8A.h"
#include "KNN.h"
/*
M1 M0
0  0    ׼˫��ڣ���ͳ8051�˿�ģʽ���������� ������ɴ�20mA��������Ϊ270��150��A������������
0  1    ���������ǿ����������ɴ�20mA��Ҫ���������裩
1  0    �������루�����Ȳ�������Ҳ����������
1  1    ��©�����Open-Drain�����ڲ���������Ͽ� ��©ģʽ�ȿɶ��ⲿ״̬Ҳ�ɶ���������ߵ�ƽ��͵�ƽ����
        ��Ҫ��ȷ���ⲿ״̬����Ҫ��������ߵ�ƽ��������������裬����������ⲿ״̬��Ҳ�����䲻���ߵ�ƽ��
*/


typedef enum
{
    GPIO_Mode_HightZ = 0x04, //��������Ȳ��������Ҳ���������
    GPIO_Mode_PullUp = 0x28,  //����׼˫���(Ĭ��)
    GPIO_Mode_Out_OD = 0x14,  //Open drain output ��©���
    GPIO_Mode_Out_PP = 0x10, //push-pull  �������
} GPIOMode_TypeDef;

//ʹ��ö�����ͣ����������������Χʱ�������ܱ���
typedef enum
{
    GPIO_P0=0,
    GPIO_P1=1,
    GPIO_P2=2,
    GPIO_P3=3,
    GPIO_P4=4,
    GPIO_P5=5,
} GPIO_Port;


typedef struct
{
    unsigned char	GPIO_Mode;		//IO Mode|IOģʽ
    unsigned char	GPIO_Pin;		//Port|�˿�
} GPIO_InitTypeDef;

#define	GPIO_Pin_0		0x01	//IO pin Px.0|IO���� Px.0
#define	GPIO_Pin_1		0x02	//IO pin Px.1|IO���� Px.1
#define	GPIO_Pin_2		0x04	//IO pin Px.2|IO���� Px.2
#define	GPIO_Pin_3		0x08	//IO pin Px.3|IO���� Px.3
#define	GPIO_Pin_4		0x10	//IO pin Px.4|IO���� Px.4
#define	GPIO_Pin_5		0x20	//IO pin Px.5|IO���� Px.5
#define	GPIO_Pin_6		0x40	//IO pin Px.6|IO���� Px.6
#define	GPIO_Pin_7		0x80	//IO pin Px.7|IO���� Px.7
#define	GPIO_Pin_All	0xFF	//IO All pin |IO��������

void GPIO_Init(GPIO_Port GPIO_Choice,GPIO_InitTypeDef *GPIOx);
void GPIO_ResetBits(uint8 GPIO_Port,uint8 GPIO_Pin);
void GPIO_SetBits(uint8 GPIO_Port,uint8 GPIO_Pin);
void USART2_Init();
#endif