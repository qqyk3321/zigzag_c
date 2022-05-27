/*******************************************************************
 * @Descripttion   : 
 * @version        : qqyk_01
 * @Author         : qqyk
 * @Date           : 2022-05-25 09:44
 * @LastEditTime   : 2022-05-27 11:25
 * @Copyright (c) 2022 by qqyk/Zigzag, All Rights Reserved. 
*******************************************************************/
#ifndef __DECODER_H__
#define __DECODER_H__
#include "include.h"
#include "matrix.h"
#include "shift.h"
#include "ori_data.h"
#include "encoder.h"
#define DECODER_B 1//是否要备份的开关
typedef enum DECODING_METHOD
{
    ORI_ZIGZAG = 0,       /*原始方法  */
    GREEDY_ZIGZAG,/*贪心算法*/
    LOOP_ZIGZAG,/*轮转算法*/
    GREEDY_LOOP_ZIGZAG,/*贪心轮转ZIGZAG*/
			      /* 杩斿洖閿欒?? */
	
}zigzag_method;
typedef enum BIT_DECODED
{
    DECODED = 0,
    UNDECODED,       /*原始方法  */
			      /* 杩斿洖閿欒?? */
	
}bit_decoded;
typedef enum ALL_FINISHED
{
    FINISHED = 0,
    UNFINISHED,       /*原始方法  */
			      /* 杩斿洖閿欒?? */
	
}all_finished;
typedef enum MESSAGE_DECODABLE
{
    DECODEBALE = 0,
    UNDECODEBALE,       /*原始方法  */
			      /* 杩斿洖閿欒?? */
	
}message_decodeble;
typedef enum DECODE_STATUS
{
    DECODE_SUCCESS = 0,
    DECODE_UNSUCCESS,       /*原始方法  */
			      /* 杩斿洖閿欒?? */
	
}decode_status;
typedef struct DECODER * decoder_p;





typedef struct DECODER{
    UINT n;
    UINT k;
    UINT L;
    UINT shift_max;
    /**********数据存储******/
    matrix_p decoding_data_matrix_p;//真实解码时真实使用的
    #if 0
    matrix_p decoding_data_matrix_b_p;//为了打印方便或者调试方便使用。
    #endif
    shift_p shift_matirx_p;
    ori_data_p ori_data_matrix_p;
    
    /**编码数据包信息空间**/
    matrix_p phi_p;//k*1矩阵，用于存储每个source数据包，下一个要被回复的数据首位
    /*********方法*********/
    UINT __load_message_number;//用于确定已经用来接收的数据包的数量。

    //INT (*load_encoding_message_from_file)(decoder_p self,FILE * fd,UINT message_number);
    //直接获取，shift_matrix和encoding_matrix
    //INT (*load_encoding_message_from_encoder)(decoder_p self,encoder_p encoder_instance);
    INT (*decode)(decoder_p self,zigzag_method method);//需要提前已经接收了n个message
    INT (*show)(decoder_p self);//da'yin
    


}decoder;


INT init_decoder_from_encoder(decoder_p self,int n, int k,int L,int shift_max,encoder_p encoder_instance);
INT destroy_decoder(decoder_p self);
decode_status check_decoding_correct(decoder_p decoder_isntance,encoder_p encoder_instance);

#endif // !__DECODER_H__
