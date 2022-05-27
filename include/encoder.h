/*******************************************************************
 * @Descripttion   : encoder对象，如果需要调整生成shift矩阵和oridata的方法，请修改init函数中的shift和oridata对应关系，encoding方法同理。
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
    
    //ori_data对象
    ori_data_p ori_data_p;
    //shift对象
    shift_p shift_p;
    //encoding_data_matrix存储
    matrix_p encoding_data_p;
    
    //实例方法
    //修改生成shift matrix的方法，只需要修改init函数中的对应函数关系。目前使用的是random
    INT (*generate_shift)(encoder_p self);
    //修改生成ori_data的方法，只需要修改init函数中的对应关系，目前使用的是random
    INT (*generate_ori_data)(encoder_p self);
    //修改生成encode的方法，只需要修改init函数中的对应关系，目前使用的是单线程方法
    INT (*encode)(encoder_p self);//利用SHIFT_MATRIX和原始数据包ORI_DATA生成n个编码数据包
    
    INT (*get_shift_member)(encoder_p self,UINT row_number,UINT col_number,VOID ** pos); 
    INT (*get_encoding_data_member)(encoder_p self,UINT row_number,UINT col_number,VOID ** pos);
    INT (*get_ori_data_member)(encoder_p self,UINT row_number,UINT col_number,VOID ** pos);
    INT (*show_encoding_data)(encoder_p self);//打印数据
    INT (*show_shift)(encoder_p self);
    INT (*show_ori_data)(encoder_p self);
    
    
}encoder;
INT init_encoder(encoder_p self,UINT k,UINT n,UINT L,UINT shift_max);//对应ENCODING模块的实例化操作
INT destroy_encoder(encoder_p self);




#endif // !__ENCODING_H__
