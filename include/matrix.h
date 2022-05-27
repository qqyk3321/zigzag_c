/*
 * @Author: taikongren0723@163.com taikongren0723@163.com
 * @Date: 2022-05-21 16:19:28
 * @LastEditors: taikongren0723@163.com taikongren0723@163.com
 * @LastEditTime: 2022-05-22 20:43:41
 * @FilePath: /zigzag_c/matrix.h
 * @Description: 矩阵最基本的操作，
 */



#ifndef __MATRIX_H__
#define __MATRIX_H__
#include "include.h"
typedef enum MENBER_TYPE
{
    MEMBER_INT,
    MEMBER_CHAR,
    MEMBER_UCHAR,
}member_type;

typedef struct MATRIX * matrix_p;

typedef struct  MATRIX
{
    INT row_size;
    INT col_size;
    
    VOID * p;
    size_t type_size;
    //方法类型
    //打印
    INT (*show)(matrix_p self,CHAR * title);
    //销毁该类和该类所使用的所有
    
    //返回行最小值和第二小值所在位置
    INT (*row_min)(matrix_p self,UINT row_number,UINT *pos_1,UINT *pos_2);
    //随机产生所有数据
    INT(*random_generate)(matrix_p self,UINT member_max); 
    //返回第row_number,col_number对应的元素
    INT(*get_member)(matrix_p self,UINT row_number,UINT col_number, VOID ** pos);
    //列加increase
    INT(*col_add)(matrix_p self,UINT col_number,UINT increase);
    //行减decrease
    INT (*row_subtraction)(matrix_p self,UINT row_number,UINT decrease);
    //确保每一行中最小值为0
    INT (*norm)(matrix_p self);
}matrix;


INT init_matrix(matrix_p self,UINT row_size,UINT col_size,char * type);
INT destroy_matrix(matrix_p self);

#endif