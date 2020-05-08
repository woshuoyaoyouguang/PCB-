/****************************************Copyright (c)**************************************************
**--------------File Info-------------------------------------------------------------------------------
** File name:			io.c
** Created by:			ZuoYouPaPa
** Last modified Date:  2020-3-10
** Last Version:		1.0
** Descriptions:		IOģʽ����
********************************************************************************************************/
#include "io.h"

/*******************************************************************************************************
�������ƣ�GPIO_Init
���������GPIO_Choice �˿�ѡ��
          *GPIOx��ʼ���ṹ��ĵ�ַ
�����������
�������ܣ����ö˿ڵĹ���ģʽ
*******************************************************************************************************/
void GPIO_Init(GPIO_Port GPIO_Choice,GPIO_InitTypeDef *GPIOx)
{

    if(GPIO_Choice == GPIO_P0)
    {
        if(GPIOx->GPIO_Mode == GPIO_Mode_PullUp)		P0M1 &= ~GPIOx->GPIO_Pin,	P0M0 &= ~GPIOx->GPIO_Pin;	 //Up-drawing alignment bi-directional port|����׼˫���
        if(GPIOx->GPIO_Mode == GPIO_Mode_HightZ)		P0M1 |=  GPIOx->GPIO_Pin,	P0M0 &= ~GPIOx->GPIO_Pin;	 //Floating input|��������
        if(GPIOx->GPIO_Mode == GPIO_Mode_Out_OD)		P0M1 |=  GPIOx->GPIO_Pin,	P0M0 |=  GPIOx->GPIO_Pin;	 //Open drain output|��©���
        if(GPIOx->GPIO_Mode == GPIO_Mode_Out_PP)		P0M1 &= ~GPIOx->GPIO_Pin,	P0M0 |=  GPIOx->GPIO_Pin;	 //push-pull|�������
    }
    if(GPIO_Choice == GPIO_P1)
    {
        if(GPIOx->GPIO_Mode == GPIO_Mode_PullUp)		P1M1 &= ~GPIOx->GPIO_Pin,	P1M0 &= ~GPIOx->GPIO_Pin;	 //Up-drawing alignment bi-directional port|����׼˫���
        if(GPIOx->GPIO_Mode == GPIO_Mode_HightZ)		P1M1 |=  GPIOx->GPIO_Pin,	P1M0 &= ~GPIOx->GPIO_Pin;	 //Floating input|��������
        if(GPIOx->GPIO_Mode == GPIO_Mode_Out_OD)		P1M1 |=  GPIOx->GPIO_Pin,	P1M0 |=  GPIOx->GPIO_Pin;	 //Open drain output|��©���
        if(GPIOx->GPIO_Mode == GPIO_Mode_Out_PP)		P1M1 &= ~GPIOx->GPIO_Pin,	P1M0 |=  GPIOx->GPIO_Pin;	 //push-pull|�������
    }
    if(GPIO_Choice == GPIO_P2)
    {
        if(GPIOx->GPIO_Mode == GPIO_Mode_PullUp)		P2M1 &= ~GPIOx->GPIO_Pin,	P2M0 &= ~GPIOx->GPIO_Pin;	 //Up-drawing alignment bi-directional port|����׼˫���
        if(GPIOx->GPIO_Mode == GPIO_Mode_HightZ)		P2M1 |=  GPIOx->GPIO_Pin,	P2M0 &= ~GPIOx->GPIO_Pin;	 //Floating input|��������
        if(GPIOx->GPIO_Mode == GPIO_Mode_Out_OD)		P2M1 |=  GPIOx->GPIO_Pin,	P2M0 |=  GPIOx->GPIO_Pin;	 //Open drain output|��©���
        if(GPIOx->GPIO_Mode == GPIO_Mode_Out_PP)		P2M1 &= ~GPIOx->GPIO_Pin,	P2M0 |=  GPIOx->GPIO_Pin;	 //push-pull|�������
    }
    if(GPIO_Choice == GPIO_P3)
    {
        if(GPIOx->GPIO_Mode == GPIO_Mode_PullUp)		P3M1 &= ~GPIOx->GPIO_Pin,	P3M0 &= ~GPIOx->GPIO_Pin;	 //Up-drawing alignment bi-directional port|����׼˫���
        if(GPIOx->GPIO_Mode == GPIO_Mode_HightZ)		P3M1 |=  GPIOx->GPIO_Pin,	P3M0 &= ~GPIOx->GPIO_Pin;	 //Floating input|��������
        if(GPIOx->GPIO_Mode == GPIO_Mode_Out_OD)		P3M1 |=  GPIOx->GPIO_Pin,	P3M0 |=  GPIOx->GPIO_Pin;	 //Open drain output|��©���
        if(GPIOx->GPIO_Mode == GPIO_Mode_Out_PP)		P3M1 &= ~GPIOx->GPIO_Pin,	P3M0 |=  GPIOx->GPIO_Pin;	 //push-pull|�������
    }
    if(GPIO_Choice == GPIO_P4)
    {
        if(GPIOx->GPIO_Mode == GPIO_Mode_PullUp)		P4M1 &= ~GPIOx->GPIO_Pin,	P4M0 &= ~GPIOx->GPIO_Pin;	 //Up-drawing alignment bi-directional port|����׼˫���
        if(GPIOx->GPIO_Mode == GPIO_Mode_HightZ)		P4M1 |=  GPIOx->GPIO_Pin,	P4M0 &= ~GPIOx->GPIO_Pin;	 //Floating input|��������
        if(GPIOx->GPIO_Mode == GPIO_Mode_Out_OD)		P4M1 |=  GPIOx->GPIO_Pin,	P4M0 |=  GPIOx->GPIO_Pin;	 //Open drain output|��©���
        if(GPIOx->GPIO_Mode == GPIO_Mode_Out_PP)		P4M1 &= ~GPIOx->GPIO_Pin,	P4M0 |=  GPIOx->GPIO_Pin;	 //push-pull|�������
    }
    if(GPIO_Choice == GPIO_P5)
    {
        if(GPIOx->GPIO_Mode == GPIO_Mode_PullUp)		P5M1 &= ~GPIOx->GPIO_Pin,	P5M0 &= ~GPIOx->GPIO_Pin;	 //Up-drawing alignment bi-directional port|����׼˫���
        if(GPIOx->GPIO_Mode == GPIO_Mode_HightZ)		P5M1 |=  GPIOx->GPIO_Pin,	P5M0 &= ~GPIOx->GPIO_Pin;	 //Floating input|��������
        if(GPIOx->GPIO_Mode == GPIO_Mode_Out_OD)		P5M1 |=  GPIOx->GPIO_Pin,	P5M0 |=  GPIOx->GPIO_Pin;	 //Open drain output|��©���
        if(GPIOx->GPIO_Mode == GPIO_Mode_Out_PP)		P5M1 &= ~GPIOx->GPIO_Pin,	P5M0 |=  GPIOx->GPIO_Pin;	 //push-pull|�������
    }
}


/*******************************************************************************************************
�������ƣ�GPIO_ReadInputDataBit
���������GPIO_Choice �˿�ѡ��
          *GPIOx��ʼ���ṹ��ĵ�ַ
�����������
�������ܣ���ȡ����IO��״̬
˵    ������ȡĳ��IO�ڵ�״̬֮ǰ��Ҫ��ʹ���ڲ�����������
*******************************************************************************************************/
void GPIO_ReadSingIOStatus(uint8 GPIO_Port,uint8 GPIO_Pin)
{

}

/*******************************************************************************************************
�������ƣ�GPIO_ResetBits
���������GPIO_Port �˿�ѡ��  GPIO_Pin����ѡ��
�����������
�������ܣ���λIO�ڣ�����ʹ��sbit����һ���˿�
*******************************************************************************************************/
void GPIO_ResetBits(uint8 GPIO_Port,uint8 GPIO_Pin)
{
    switch(GPIO_Port)
    {
    case GPIO_P0:
        P0&=~GPIO_Pin;
        break;
    case GPIO_P1:
        P1&=~GPIO_Pin;
        break;
    case GPIO_P2:
        P2&=~GPIO_Pin;
        break;
    case GPIO_P3:
        P3&=~GPIO_Pin;
        break;
    case GPIO_P4:
        P4&=~GPIO_Pin;
        break;
    case GPIO_P5:
        P5&=~GPIO_Pin;
        break;
    default:
        break;
    }

}

/*******************************************************************************************************
�������ƣ�GPIO_SetBits
���������GPIO_Port �˿�ѡ��  GPIO_Pin����ѡ��
�����������
�������ܣ���λIO�ڣ�����ʹ��sbit����һ���˿�
*******************************************************************************************************/
void GPIO_SetBits(uint8 GPIO_Port,uint8 GPIO_Pin)
{
    switch(GPIO_Port)
    {
    case GPIO_P0:
        P0|=GPIO_Pin;
        break;
    case GPIO_P1:
        P1|=GPIO_Pin;
        break;
    case GPIO_P2:
        P2|=GPIO_Pin;
        break;
    case GPIO_P3:
        P3|=GPIO_Pin;
        break;
    case GPIO_P4:
        P4|=GPIO_Pin;
        break;
    case GPIO_P5:
        P5|=GPIO_Pin;
        break;
    default:
        break;
    }
}

//����2���ų�ʼ��
void USART2_Init()
{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_All;
//    GPIO_Init(GPIO_P2,&GPIO_InitStructure);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
    GPIO_Init(GPIO_P1,&GPIO_InitStructure);

}
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

