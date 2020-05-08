#ifndef _KNN_H_
#define _KNN_H_


typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	�޷���8λ���ͱ���  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		�з���8λ���ͱ���  */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	�޷���16λ���ͱ��� */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		�з���16λ���ͱ��� */

typedef struct
{
    int distance;  //����ֵ
    char label;     //ѵ�����ݾ����ǩ
} Distance;

//typedef struct
//{
//    uint8 NumCounter;  //���ֵĴ���
//    uint8 label;     //���ݱ�ǩ
//} PredictValue;


#define   Numtrain     27  // ����ģ�����ݵ����� 
#define   AllNumtrain     57  // 2����Ƭ��ģ�����ݵ��ܸ���
#define   Numpredict   1  //Ԥ���������� 
#define   Testpredict  1  //������������
#define   JZLenght  1024  //�������ݳ���


#endif