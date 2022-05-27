/*******************************************************************
 * @Descripttion   : 
 * @version        : qqyk_01
 * @Author         : qqyk
 * @Date           : 2022-05-23 12:50
 * @LastEditTime   : 2022-05-23 12:59
 * @Copyright (c) 2022 by qqyk/Zigzag, All Rights Reserved. 
*******************************************************************/
#ifndef __ORI_DATA_H__
#define __ORI_DATA_H__
#include "matrix.h"
#include "include.h"

typedef struct  ORI_DATA * ori_data_p;
typedef struct ORI_DATA{
    INT k;
    INT L;
    matrix_p ori_data_matrix_p;
    INT (*random_generate_ori_data)(ori_data_p self);//���������λ����
    INT (*show_ori_data)(ori_data_p self);//��ӡ����
    INT (*get_ori_data_member)(ori_data_p self,UINT row_number,UINT col_number,VOID **pos);//��ȡĳ��Ԫ��
    INT (*ori_data_row_min)(ori_data_p self,UINT row_number,UINT *pos_1,UINT *pos_2);//��ȡĳһ������С��Ԫ�ص�����Ԫ�ص�λ�á�
   
   

}ori_data ;
INT init_ori_data(ori_data_p self,UINT k,UINT L);//ʵ��ģ��
INT destroy_ori_data(ori_data_p self);//�ͷ�ģ��
#endif



