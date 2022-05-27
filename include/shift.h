/*******************************************************************
 * @Descripttion   : 
 * @version        : qqyk_01
 * @Author         : qqyk
 * @Date           : 2022-05-23 10:29
 * @LastEditTime   : 2022-05-23 12:28
 * @Copyright (c) 2022 by qqyk/Zigzag, All Rights Reserved. 
*******************************************************************/
#ifndef __SHIFT_H__
#define __SHIFT_H__
#include "include.h"
#include "matrix.h"

typedef struct  SHIFT * shift_p;
typedef struct SHIFT{
    INT k;
    INT n;
    INT shift_max;
    matrix_p shift_matrix_p;
    INT (*random_generate_shift)(shift_p self);//随机产生移位矩阵
    INT (*show_shift)(shift_p self);//打印数据
    INT (*get_shift_member)(shift_p self,UINT row_number,UINT col_number,VOID **pos);//获取某个元素
    INT (*shift_row_min)(shift_p self,UINT row_number,UINT *pos_1,UINT *pos_2);//获取某一行中最小的元素的两个元素的位置。
    INT (*shift_col_add)(shift_p self,UINT col_number,UINT increase);//获取列加1
   

}shift ;
INT init_shift(shift_p self,UINT k,UINT n,UINT shift_max);//实例模块
INT destroy_shift(shift_p self);//释放模块
#endif