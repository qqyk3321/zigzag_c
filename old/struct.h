/*******************************************************************
 * @Descripttion   : struct.c ��ͷ�ļ�
 * @version        : qqyk_01
 * @Author         : qqyk
 * @Date           : 2022-05-18 12:55
 * @LastEditTime   : 2022-05-19 13:04
 * @Copyright (c) 2022 by qqyk/X1000, All Rights Reserved. 
*******************************************************************/
#ifndef __STRUCT_H__
#define __STRUCT_H__
#include "need_include.h"

struct ORI_DATA
{
    int k;//������ԭʼ���ݰ��ĸ���
    int L;//������ԭʼ���ݰ��ĳ���
    void (*generate_ori_data)(struct ORI_DATA *self);//�����������
    unsigned char * data_p;//�������ڵ�λ��
    //char data_p[9];
    int (*print_ori_data)(struct ORI_DATA *self);//��ӡ����
    
    int (*close_ori_data)(struct ORI_DATA *self);//�ر�
};
int init_ori_data(struct ORI_DATA * self,int k,int L);//Ϊ��data_p����k*L*sizeof(char)��С���ڴ�ռ䡣    



struct SHIFT_MATRIX
{
     int k;
     int n;
     int shift_max;
     unsigned char * shift_p;//�������ڵ�λ��
     void (*generate_shift)(struct SHIFT_MATRIX *self);//���������λ����
     int (*print_shift)(struct SHIFT_MATRIX *self);//��ӡ����
     int (*close_shift)(struct SHIFT_MATRIX *self);//�ر�

};
int init_shift(struct SHIFT_MATRIX * self,int k,int n,int shift_max);//��    

struct ENCODING
{
    int k;
    int n;
    int L;
    int shift_max;
    unsigned char * encoding_data_p;//�������ݰ�����ط���
    struct SHIFT_MATRIX * shift_matrix;
    struct ORI_DATA * ori_data;
    void (*encode)(struct ENCODING *self);//����SHIFT_MATRIX��ԭʼ���ݰ�ORI_DATA����n���������ݰ�
    
    int (*generate_encoding_data)(struct ENCODING *self);
    int (*print_encoding_data)(struct ENCODING *self);//��ӡ����
    int (*close_encoding)(struct ENCODING *self);
};
int init_encoding(struct ENCODING *self,int k,int n,int L,int shift_max);//��ӦENCODINGģ���ʵ��������


#endif