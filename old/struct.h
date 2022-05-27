/*******************************************************************
 * @Descripttion   : struct.c 的头文件
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
    int k;//分组中原始数据包的个数
    int L;//分组中原始数据包的长度
    void (*generate_ori_data)(struct ORI_DATA *self);//随机产生数据
    unsigned char * data_p;//数据所在的位置
    //char data_p[9];
    int (*print_ori_data)(struct ORI_DATA *self);//打印数据
    
    int (*close_ori_data)(struct ORI_DATA *self);//关闭
};
int init_ori_data(struct ORI_DATA * self,int k,int L);//为其data_p分配k*L*sizeof(char)大小的内存空间。    



struct SHIFT_MATRIX
{
     int k;
     int n;
     int shift_max;
     unsigned char * shift_p;//数据所在的位置
     void (*generate_shift)(struct SHIFT_MATRIX *self);//随机产生移位矩阵
     int (*print_shift)(struct SHIFT_MATRIX *self);//打印数据
     int (*close_shift)(struct SHIFT_MATRIX *self);//关闭

};
int init_shift(struct SHIFT_MATRIX * self,int k,int n,int shift_max);//。    

struct ENCODING
{
    int k;
    int n;
    int L;
    int shift_max;
    unsigned char * encoding_data_p;//编码数据包保存地方。
    struct SHIFT_MATRIX * shift_matrix;
    struct ORI_DATA * ori_data;
    void (*encode)(struct ENCODING *self);//利用SHIFT_MATRIX和原始数据包ORI_DATA生成n个编码数据包
    
    int (*generate_encoding_data)(struct ENCODING *self);
    int (*print_encoding_data)(struct ENCODING *self);//打印数据
    int (*close_encoding)(struct ENCODING *self);
};
int init_encoding(struct ENCODING *self,int k,int n,int L,int shift_max);//对应ENCODING模块的实例化操作


#endif