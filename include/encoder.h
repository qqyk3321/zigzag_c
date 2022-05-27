/*******************************************************************
 * @Descripttion   : encoder���������Ҫ��������shift�����oridata�ķ��������޸�init�����е�shift��oridata��Ӧ��ϵ��encoding����ͬ��
 * @version        : qqyk_01
 * @Author         : qqyk
 * @Date           : 2022-05-23 13:01
 * @LastEditTime   : 2022-05-27 11:25
 * @Copyright (c) 2022 by qqyk/Zigzag, All Rights Reserved. 
*******************************************************************/
#ifndef __ENCODING_H__
#define __ENCODING_H__
#include "matrix.h"
#include "shift.h"
#include "ori_data.h"
#include "include.h"
typedef struct ENCODER * encoder_p;
typedef struct ENCODER
{
    UINT k;
    UINT n;
    UINT L;
    UINT shift_max;
    
    //ori_data����
    ori_data_p ori_data_p;
    //shift����
    shift_p shift_p;
    //encoding_data_matrix�洢
    matrix_p encoding_data_p;
    
    //ʵ������
    //�޸�����shift matrix�ķ�����ֻ��Ҫ�޸�init�����еĶ�Ӧ������ϵ��Ŀǰʹ�õ���random
    INT (*generate_shift)(encoder_p self);
    //�޸�����ori_data�ķ�����ֻ��Ҫ�޸�init�����еĶ�Ӧ��ϵ��Ŀǰʹ�õ���random
    INT (*generate_ori_data)(encoder_p self);
    //�޸�����encode�ķ�����ֻ��Ҫ�޸�init�����еĶ�Ӧ��ϵ��Ŀǰʹ�õ��ǵ��̷߳���
    INT (*encode)(encoder_p self);//����SHIFT_MATRIX��ԭʼ���ݰ�ORI_DATA����n���������ݰ�
    
    INT (*get_shift_member)(encoder_p self,UINT row_number,UINT col_number,VOID ** pos); 
    INT (*get_encoding_data_member)(encoder_p self,UINT row_number,UINT col_number,VOID ** pos);
    INT (*get_ori_data_member)(encoder_p self,UINT row_number,UINT col_number,VOID ** pos);
    INT (*show_encoding_data)(encoder_p self);//��ӡ����
    INT (*show_shift)(encoder_p self);
    INT (*show_ori_data)(encoder_p self);
    
    
}encoder;
INT init_encoder(encoder_p self,UINT k,UINT n,UINT L,UINT shift_max);//��ӦENCODINGģ���ʵ��������
INT destroy_encoder(encoder_p self);




#endif // !__ENCODING_H__
