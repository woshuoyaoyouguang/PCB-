/*����51��Ƭ����KNN�㷨��ʵ��*/
/*����51��Ƭ����KNN�㷨��ʵ��*/
/*����51��Ƭ����KNN�㷨��ʵ��*/
/*����51��Ƭ����KNN�㷨��ʵ��*/
#include "STC8A.h"
#include "KNN.h"
#include "train.h"
#define USART2_ReceLenght  2

//ע��xdata����������ܹ����籣�֣����ҵ�Ƭ����λ�󲢲��Ḵλxdata��������
//RAM���ϵ���ʼֵ������ģ���Ҫ�����Լ�����ֵ
char xdata TestData[JZLenght+1];  //���ղ��������ݴ棬��ʱ�������һ�����ݳ�1025�ֽ�
unsigned char  USART2_RecBuff[USART2_ReceLenght];

unsigned char  USART2_SendBuff[Numtrain*3+3];

int Timer0_Counter=0;
bit USART2_AllowSendFlag=0;

//����1���ݽ�����ɱ�־�Լ��������ݵļ���
int ReceiveCounter=0;
uint8 RecOverFlag=0;
sbit LED = P2^6;  //LEDָʾ��
sbit ReceDataReadly=P1^3;

bit USART1_BUSY;   //����1��æ��־
bit USART2_BUSY;   //����2��æ��־

//����2���ݽ�����ɱ�־�Լ��������ݵļ���
uint8 USART2_RecOverFlag=0;
uint8 USART2_ReceCount=0;

Distance xdata distance[Numtrain]; //����ֵ�ݴ�,Numtrainѵ������ģ�峤��

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
    int it;
    /*��ȡ����digit��ѵ�����ݵľ���*/
    for(it=0; it<Numtrain; it++)
    {
        distance[it].distance = calDistance(in,train[it]);  //�����������ѵ�����ݵľ���
        distance[it].label = train[it][JZLenght];  //ѵ�����ݵ����ֱ�Ŵ�����������һ�������ֽ���		  
    }			
}

void USART1_Init()
{
    //���θ�ֵ��ñ��û����㣬��BUG
    //���ﲻ֪����ʲôԭ��
    SCON=0x50;        //REN=1�����н���״̬�����ڹ���ģʽ1,9λ�ɱ䲨����
    TMOD = 0x00;       //��ʱ��1Ϊģʽ0��16λ�Զ�����)
    AUXR=0X40;		 //��ʱ��1����1Tģʽ
    //9600�����ʣ�ע�Ⲩ���ʲ�������̫�ߣ�֮ǰ���Ե�ʱ��115200�����ʵ�ʱ��������
    TL1=(65536-(11059200/4/9600));    //���ò�������װֵ
    TH1=(65536-(11059200/4/9600))>>16;

	  //����1�ķ��Ϳ�����Ϊ�������,�ֲ�����˵���������������һ��
	  //�����õĻ���ȻҲ���������ͣ����ǰ�STC�ֲ��ϵ����ɣ�Ҫ��Ȼ�ֻ��п�
	  //�ӻ�����1����Ҫ�������ݣ��������óɸ���״̬�����ٵ��������Ի���USB���Ϳڵ�����ѹ��
	  //����1���տ�������Ϊ˫���
    P3M1 &= ~0x01;  //P3.0�����ó�˫���
    P3M0 &= ~0x01; 

	  P3M1 |=  0x02; //P3.1�����óɸ���״̬
    P3M0 &= ~0x02;
	
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

 	 
    
	  
    P1M1 &= ~0x01; //P1.0 ���ڽ��տ����ó�˫���
    P1M0 &= ~0x01;
	  
		P1M1 &= ~0x02;
		P1M0 |=  0x02;    //P1.1��������ǰ���ó��������	
	
	  USART2_BUSY=0;   //����1æ��־��ʼ״̬Ϊ0
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

void Clearn_TestData()
{
    int i;
    for(i=0; i<JZLenght; i++)
    {
        TestData[i]=0x00;
    }
}

//����2����������Ԥ��ļ���ֵ�����洢�ڽṹ���е�����ȫ�����͹�ȥ
//����У��
void USART2_SendPredictData()
{	
	 uint8 temp1;
	 unsigned int Sum=0;
	 USART2_SendBuff[0]=0x01;  //�ӻ���ַ
	 	for(temp1=0;temp1<Numtrain;temp1++)
		{ 
			  USART2_SendBuff[temp1*3+1]=distance[temp1].label;  //���ݴ��뷢�ͻ�����
			  USART2_SendBuff[temp1*3+2]=distance[temp1].distance>>8;  //��8λ
			  USART2_SendBuff[temp1*3+3]=distance[temp1].distance;   //��8λ			  					 
		}
		for(temp1=0;temp1<Numtrain*3+1;temp1++)
		{
			   Sum+=USART2_SendBuff[temp1];  //���
		}
		USART2_SendBuff[Numtrain*3+1]=Sum>>8;  //��8λ
		USART2_SendBuff[Numtrain*3+2]=Sum;  //��8λ	
		
		for(temp1=0;temp1<Numtrain*3+3;temp1++)  
		{
			 USART2_SendData(USART2_SendBuff[temp1]);  //��������
		}
}

void main() 			 //������
{
	  uint8 ReceErrorCounter=0;  //����2���ʹ����������������2�η��ʹ��������ط�����
    //����16MHz��ʱ��Ƶ�������е���ʱ��һ�������
    //��Ҫ�������Ŀ��Ƶ�ʵ�2��3����Ȼ���ٽ��з�Ƶ
    //����ʮ����Ҫ�������ڴ��ڶ�ͨѶ��ʱ��ᵼ�²����ʲ�׼ȴ��Ȼ���������
    P_SW2 = 0x80;
    CKSEL = 0x00; //ѡ���ڲ� IRC ( Ĭ�� )
    CLKDIV = 0x02; //2��Ƶ
    P_SW2 = 0x00;
	
	  USART1_Init();
    USART2_Init();
	  Timer0_Init();
    while(1)
    {
        if(RecOverFlag==1)
        {      				
            prediction(1, TestData,train1);  //Ԥ��ֵ����
			      LED = ~LED;
					  TR0=1; //����ʱ����ʱ
					  while(!USART2_AllowSendFlag);		//�ȴ���������������		
            USART2_SendPredictData();			 //����2����Ԥ��ֵ�������������	  
					  RecOverFlag=0;			  //������ɱ�־���
						ReceiveCounter=0;
					  ReceErrorCounter=0;  //����2���ʹ��������0
        } 				
				if(USART2_RecOverFlag==1)  //����2�����������,˵�����͹����������д��ٷ���һ�β�������
				{
					 USART1_SendData(0x01);  //����1����λ��������Ϣ����֪��λ����������Ƭ�����ݽ���ʱͨѶ�и��ţ��������ݷ��ʹ���
					 USART1_SendData(0x88);  //ͨѶ�����־
					 LED = ~LED;
					 if(USART2_RecBuff[0]==0x01&&USART2_RecBuff[1]==0x2C)  //�ϴη������ݴ�������г���
					 {
						   ReceErrorCounter++;
							 if(ReceErrorCounter<3)  //���ʹ������С��2��
							 {
								   
									 USART2_SendPredictData();	//�����ط�
							 }		
					 }
           USART2_RecOverFlag=0;
					 USART2_ReceCount=0;					 
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
					 USART2_RecOverFlag=1;
					 USART2_ReceCount=0;
				}
        S2CON &= ~0x01;  //��������ж�
    }   
}
//��ʱ��0��������ʱ�ȴ��ӻ�1���ݴ�����ɣ�����2���ӻ�ͬʱ����������ɳ�ͻ
void TIME0_Handle(void) interrupt 1
{
	  Timer0_Counter++;
	  if(Timer0_Counter>=18)  //��ʱ900ms,�ȴ���������������
		{		 
			 TR0=0;
			 LED = ~LED;
			 Timer0_Counter=0;
			 USART2_AllowSendFlag=1;  //����2������������
		}     
}


