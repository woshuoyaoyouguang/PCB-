/*����51��Ƭ����KNN�㷨��ʵ��*/
/*����51��Ƭ����KNN�㷨��ʵ��*/
/*����51��Ƭ����KNN�㷨��ʵ��*/
/*����51��Ƭ����KNN�㷨��ʵ��*/
#include "STC8A.h"
#include "KNN.h"
#include "train.h"
#define USART2_ReceLenght  93    //����2�������ݳ���

//ע��xdata����������ܹ����籣�֣����ҵ�Ƭ����λ�󲢲��Ḵλxdata��������
//RAM���ϵ���ʼֵ������ģ���Ҫ�����Լ�����ֵ
char xdata TestData[JZLenght+1];  //���ղ��������ݴ棬��ʱ�������һ�����ݳ�1025�ֽ�
unsigned char xdata USART2_RecBuff[USART2_ReceLenght];

uint8 code SmgDuan[11]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0xf3};
uint8 code SmgWaitDuan[6]={0x01,0x02,0x04,0x08,0x10,0x20};
//����1���ݽ�����ɱ�־�Լ��������ݵļ���
int ReceiveCounter=0;
uint8 RecOverFlag=0;
int Timer0_Counter=0;  //��ʱ������

sbit LED = P2^6;  //LEDָʾ��
sbit USART2_AllowSendPort=P1^3;

bit USART1_BUSY;   //����1��æ��־
bit USART2_BUSY;   //����2��æ��־
bit SmgWait=0;
uint8 SmgCounter=0;
//����2���ݽ�����ɱ�־�Լ��������ݵļ���
uint8 USART2_RecOverFlag=0;
uint8 USART2_ReceCount=0;


//�ô洢���ǹ̶��ģ������Ҹģ�����������н������ȷ
Distance xdata distance[AllNumtrain]; //����ֵ�ݴ�,���е�Ƭ���������Ļ���

void exchange(Distance *in, uint8 index1,uint8 index2);
void selectSort(Distance *in, uint8 length);
void USART1_SendData(char Data);
//128 �ֽ��ڲ�ֱ�ӷ��� RAM��DATA��
//128 �ֽ��ڲ���ӷ��� RAM��IDATA��
//8192 �ֽ��ڲ���չ RAM���ڲ� XDATA��
//�ⲿ������չ 64K �ֽ� RAM���ⲿ XDATA��



void USART1_SendData(uint8 Data)
{
    while (USART1_BUSY);
    USART1_BUSY = 1;
    SBUF = Data;
}

void USART1_SendStr(uint8 *Data,int Lenght)
{
    int i;
    for(i=0; i<Lenght; i++)
    {
        USART1_SendData(Data[i]);
    }
}


//����2��������
void USART2_SendData(uint8 Data)
{
    while (USART2_BUSY);
    USART2_BUSY = 1;
    S2BUF = Data;
}

//����2�����ַ���
void USART2_SendStr(uint8 *USARTData,int Lenght)
{
    int j=0;
    for(j=0; j<Lenght; j++)
    {
        USART2_SendData(USARTData[j]);
    }
}
//��ŷʽ����
//digit1 ��������
//digit2 ѵ������
//����ֵ:int����
int calDistance(char *digit1, char *digit2)
/*�����*/
{
    int i, squareSum=0.0;
    for(i=0; i<JZLenght; i++)
    {
        //�ô���������Ŀ������������ٶȣ���Ϊѵ�����ݱ걾��ֻ��0��1
        //���������㷨����ֻ�����ڱ걾��0��1��ѵ������
        if(digit1[i]-digit2[i]!=0) squareSum+=1;
    }
//		USART1_SendDataUint8(squareSum>>8);   //���ڵ���
//		USART1_SendDataUint8(squareSum);
    return  squareSum;  //���ؾ���ֵ
}

//����in һ���ṹ������
//index1 �ṹ�������������ǰһ����
//index2 �ṹ���������������һ����
//�����ṹ�廥��
void exchange(Distance *in, uint8 index1,uint8 index2)
{
    Distance tmp;
    tmp = in[index1];
    in[index1] = in[index2];
    in[index2] = tmp;
}


//����in  һ���ṹ������
//lenght ѵ������ģ������
//��С��������
void selectSort(Distance *in, uint8 length)
{
    uint8 i, j, min;
    uint8 N = length;
    for(i=0; i<N-1; i++)
    {
        min = i;
        //j=i+1 �������ݺ��������
        //�㷨˼��:ÿ��ѭ�����Ƚ�С������ǰ��һ�Σ�ֱ����������������ȷ
        //���ж�ǰһ�������Ƿ���ں�һ�����ݣ���������򽻻����������򲻽���
        //���i*j�ο��������
        for(j=i+1; j<N; j++)
        {
            if(in[j].distance<in[min].distance) min = j;
        }
        exchange(in,i,min);
    }
}

//in  �������ݾ���
//train �������ݾ���
//K ѡ������Ŀ��ֵ��������ݵĸ���
//��ά������Ϊ���������ʱ�򳤶�û�м�1���������ݱ�ǩȫ��0
//��Ϊ���ݱ�ǩ�洢�ڵ�1025���ֽ�
void prediction(uint8 K, char *in, char train[][JZLenght+1])
{
    int i, it;
    int predict = 0;
//	 Distance distance[Numtrain]; //����ֵ�ݴ�,Numtrainѵ������ģ�峤��
    /*��ȡ����digit��ѵ�����ݵľ���*/
	  //�������ݴ洢���±�0-18��
    for(it=0; it<Numtrain; it++)
    {
        distance[it].distance = calDistance(in,train[it]);  //�����������ѵ�����ݵľ���
        distance[it].label = train[it][JZLenght];  //ѵ�����ݵ����ֱ�Ŵ�����������һ�������ֽ���		  
    }
    
}
void USART1_Init()
{
    //���θ�ֵ��ñ��û����㣬��BUG
    //���ﲻ֪����ʲôԭ��,������оƬ�ϵ�һ��ȱ��
    SCON=0x50;        //REN=1�����н���״̬�����ڹ���ģʽ1,9λ�ɱ䲨����
    TMOD = 0x00;       //��ʱ��1Ϊģʽ0��16λ�Զ�����)
    AUXR=0X40;		 //��ʱ��1����1Tģʽ
    //9600�����ʣ�ע�Ⲩ���ʲ�������̫�ߣ�֮ǰ���Ե�ʱ��115200�����ʵ�ʱ��������
    TL1=(65536-(11059200/4/9600));    //���ò�������װֵ
    TH1=(65536-(11059200/4/9600))>>16;

	  //����1�ķ��Ϳ�����Ϊ�������,�ֲ�����˵���������������һ��
	  //�����õĻ���ȻҲ���������ͣ����ǰ�STC�ֲ��ϵ����ɣ�Ҫ��Ȼ�ֻ��п�
    P3M1 &= ~0x02;
    P3M0 |=  0x02;  
	
    TR1  = 1;        //������ʱ��1,,��Ϊ�����ʷ�����
    ES   = 1;        //�������ж�
    EA   = 1;        // �����ж�

    USART1_BUSY=0;   //����1æ��־��ʼ״̬Ϊ0
}

void USART2_Init()
{
    S2CON = 0x50;   //ģʽ0,8λ�ɱ䲨����,�����ڽ�������
    T2L = (65536-(11059200/4/9600));   //���ö�ʱ��2��ֵ�����ڲ����ʷ���������ֵ����������
    T2H = (65536-(11059200/4/9600))>>8;
    AUXR |= 0x14;   //����ʱ��2,ʱ��Ϊ1Tģʽ����Ϊ�����ʷ�����
    IE2 = 0x01;  //��������2���ж�
    EA=1;



	  P1M1 &= ~0x01;   //P1.0 ���ڽ��տ����ó�˫���
    P1M0 &= ~0x01;
	  
		P1M1 &= ~0x02;
		P1M0 |=  0x02;    //P1.1��������ǰ���ó��������		
	
	  USART2_BUSY=0;   //����1æ��־��ʼ״̬Ϊ0
}
void Clearn_TestData()
{
    int i;
    for(i=0; i<JZLenght; i++)
    {
        TestData[i]=0x00;
    }
}

//����2�������ݵĺ�У��
uint8 USART2_ReceSumCheck()
{ 
	  uint8 temp;
	  uint8 Hbyte=0,Lbyte=0;
	  unsigned int Sum=0;
	  for(temp=0;temp<USART2_ReceLenght-2;temp++)
		{
			  Sum+=USART2_RecBuff[temp];  //��У��
		}
		
//		for(temp=0;temp<USART2_ReceLenght;temp++)  //���ڵ������
//		{
//			  USART1_SendData(USART2_RecBuff[temp]);
//		}
//		//��У���ж�,��8λ��ǰ����8λ�ں�
//		USART1_SendData(Sum>>8);
//		USART1_SendData(Sum);
		
		
		Hbyte=Sum>>8;  //���ֽ�,����Ҫ�����������ø�ֵȥ�ж�
		Lbyte=Sum;   //���ֽ�
		if((Hbyte==USART2_RecBuff[USART2_ReceLenght-2])&&(USART2_RecBuff[USART2_ReceLenght-1]==Lbyte))
		{
			  LED=~LED;
			  return 1;  //��У����ȷ
		}
	  return 0;   //��У�����
}

void Timer0_Init(void)
{
    AUXR &= ~0x80;	//��ʱ��ʱ��12Tģʽ,��ʱ��12��Ƶ(FOSC/12)
    TMOD |= 0x00;  //ģʽ0,16λ�Զ���װ��
    TL0 = 0x00;		//���ö�ʱ��ֵ
	  TH0 = 0x4C;		//���ö�ʱ��ֵ
	  TF0 = 0;		//���TF0��־
	  TR0 = 0;		//��ʼ��ʱ�رն�ʱ��
    ET0 = 1;      //ʹ�ܶ�ʱ���ж�
    EA = 1;
}
void main() 			 //������
{
	  uint8 temp1,temp2,temp3,temp4;
	  int DistanceTemp;
	  bit HostRelady=0,Slave1Relady=0;  //�������ݴ�����ɱ�־λ
	  uint8 USART2_ReceDataErrorCounter=0;
	  uint8 HostPredictFlag=0;
    //����16MHz��ʱ��Ƶ�������е���ʱ��һ�������
    //��Ҫ�������Ŀ��Ƶ�ʵ�2��3����Ȼ���ٽ��з�Ƶ
    //����ʮ����Ҫ�������ڴ��ڶ�ͨѶ��ʱ��ᵼ�²����ʲ�׼ȴ��Ȼ���������
	  //����ʹ��ISP����Ƚ�Ƶ�ʵ���22.184MHz,�ٽ��з�Ƶ
    P_SW2 = 0x80;
    CKSEL = 0x00; //ѡ���ڲ� IRC ( Ĭ�� )
    CLKDIV = 0x02; //2��Ƶ
    P_SW2 = 0x00;
	  
	
	  USART1_Init();
    USART2_Init();
	  Timer0_Init();
	  P0M1 &= ~0xFF;
		P0M0 =  0xFF;    //P0�����ó�ǿ�������,�������������
	  P0=SmgDuan[10]; 
		TR0=1;  //����ʱ��
		SmgWait=1;  //����ܶ�̬��ˮ��ʾ
    while(1)
    {
        if(RecOverFlag==1)
        {   
//            P0=SmgDuan[10];
            SmgWait=1;  //����ܶ�̬��ˮ��ʾ	
            TR0=1;  //����ʱ��					
            prediction(1, TestData,train1);  //�����������ֵ
								 					  
					  HostRelady=1;  //�������ݴ�����ɱ�־					  
					  RecOverFlag=0;			
						ReceiveCounter=0;
					  USART2_ReceDataErrorCounter=0;
					  LED=~LED;	 //���յ����ݣ�״̬ȡ��һ��
					  USART2_AllowSendPort=0; //�ôӻ�1׼����������
        }
        if(USART2_RecOverFlag==1)  //�ӻ�1�������ļ�������
        {				   	
					    if(USART2_ReceSumCheck()==1)
							{
										if(USART2_RecBuff[0]==0x01)  //�ӻ�1������������
										{
												for(temp1=Numtrain;temp1<AllNumtrain;temp1++)
												{
													 DistanceTemp=0x0000;
													 distance[temp1].label = USART2_RecBuff[(temp1-Numtrain)*3+1];
													 DistanceTemp|=USART2_RecBuff[(temp1-Numtrain)*3+2]<<8;  //��8λ
													 DistanceTemp|=USART2_RecBuff[(temp1-Numtrain)*3+3];  //��8λ
													 distance[temp1].distance = DistanceTemp;											 
												}
												LED=~LED;	 //���յ����ݣ�״̬ȡ��һ��
												Slave1Relady=1;  //�ӻ�1���ݴ�����ɱ�־
												USART2_AllowSendPort=1;  //�ӻ�1���ݴ�����ϣ���ֹ�ٷ�����
										}										       					
							}
							else
							{
								 USART2_ReceDataErrorCounter++;
								 if(USART2_ReceDataErrorCounter<3)
								 {
										 //��У����󣬻ظ��ӻ���Ҫ���ٴη���
										 USART2_SendData(0x01);  //���ʹӻ���ַ
										 USART2_SendData(0x2C);  //���ͺ�У�����
								 }
								 else
								 {
									  HostPredictFlag=1;   //�ﵽ����ط������������ط�����ʱ��������Ԥ����Ϊ��
									  USART2_AllowSendPort=1;  //�ӻ�1���ݴ�����ϣ���ֹ�ٷ�����
								 }
							}
            USART2_RecOverFlag=0;  //�������2�Ľ�����ɱ�־
						USART2_ReceCount=0;  //�������2�ļ���					
        }
				
				if(HostRelady==1&&Slave1Relady==1)  //���������ݴ������
				{		
            TR0=0;  //����ʱ��					
						SmgWait=0;  //����ܶ�̬��ˮ��ʾ					
					  selectSort(distance, AllNumtrain);  //�Ծ���ֵ�����������	
					  P0=SmgDuan[distance[0].label];   //�������ʾԤ����
						HostRelady=0;
						Slave1Relady=0;
					  LED=~LED;		 //���������ݣ�״̬ȡ��һ��		  
				}
				if(HostPredictFlag==1)  //ͨѶ���ϣ���ʱ��������һ��Ԥ��Ϊ׼
				{
					  selectSort(distance, Numtrain);  //�Ծ���ֵ�����������	
					  P0=SmgDuan[distance[0].label];   //�������ʾԤ����				
            HostPredictFlag=0;
						HostRelady=0;
						Slave1Relady=0;
					  LED=~LED;				
				}

    }
}


//����1�жϷ������
void USART1_Handle(void) interrupt 4
{
    if(RI)
    {
        TestData[ReceiveCounter++]=SBUF;
			  //���ﲻ�ö�ʱ��20ms��һ֡���ݽ������жϣ���������BUG
			  //��Ϊ��֪�������ݵĳ��ȣ���������ֱ���ó������ж�
        if(ReceiveCounter>=JZLenght)  //���ڵ���1024��˵�����������
				{
					 RecOverFlag=1;
					 ReceiveCounter=0;
				}
				RI=0;
    }
    if(TI)
    {
        TI=0;
        USART1_BUSY=0;
    }
}


//����2�жϷ������
void  USART2_Handle(void) interrupt 8
{
	  if(S2CON & S2TI)  //���ͱ�־
    { 
			  USART2_BUSY = 0;               //��æ��־
        S2CON &= ~S2TI;         //���S3TIλ
    }
    if(S2CON & S2RI)  //���ձ�־
    {		  
        USART2_RecBuff[USART2_ReceCount++]=S2BUF;
        if(USART2_ReceCount>=USART2_ReceLenght)
				{
					 USART2_RecOverFlag=1;  //���ݽ�����ɱ�־
					 USART2_ReceCount=0;  //�������2�ļ���
				}
        S2CON &= ~0x01;  //��������ж�
    }   
}

//��ʱ��0��������ʱ�ȴ��ӻ�1���ݴ�����ɣ�����2���ӻ�ͬʱ����������ɳ�ͻ
void TIME0_Handle(void) interrupt 1
{
	  Timer0_Counter++;
	  if(Timer0_Counter>=2&&SmgWait)  //��ʱ100ms,�ȴ���������������
		{		 
			 P0=SmgWaitDuan[SmgCounter];
			 SmgCounter++;
			 if(SmgCounter>5)
			 {
				  SmgCounter=0;
			 }
			 Timer0_Counter=0;
		}     
}

