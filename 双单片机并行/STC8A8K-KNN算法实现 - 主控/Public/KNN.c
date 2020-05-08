#include "KNN.h"
Distance distance1;
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
//�����ṹ�����黥��
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
uint8 prediction(uint8 K, char *in, char train[][JZLenght])
{
    int i, it;
    int predict = 0;
    Distance distance[Numtrain]; //����ֵ�ݴ�,Numtrainѵ������ģ�峤��
    /*��ȡ����digit��ѵ�����ݵľ���*/
    for(it=0; it<Numtrain; it++)
    {
        distance[it].distance = calDistance(in,train[it]);  //�����������ѵ�����ݵľ���
        distance[it].label = it;  //�������鱾���ǰ�0-9ģ�������±��,��������ֱ�Ӹ�����
    }
    /*������ľ�������*/

    selectSort(distance, Numtrain);  //�Ծ���ֵ�����������
    for(i=0; i<K; i++)
    {
        predict += distance[i].label;  //ȡ����Ŀ��ֵ�����K��ֵ
    }
    return predict/K;  //����Ԥ��ֵ
}

