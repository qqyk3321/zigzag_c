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
#define DECODER_B 1//�Ƿ�Ҫ���ݵĿ���
typedef enum DECODING_METHOD
{
    ORI_ZIGZAG = 0,       /*ԭʼ����  */
    GREEDY_ZIGZAG,/*̰���㷨*/
    LOOP_ZIGZAG,/*��ת�㷨*/
    GREEDY_LOOP_ZIGZAG,/*̰����תZIGZAG*/
			      /* 返回错�?? */
	
}zigzag_method;
typedef enum BIT_DECODED
{
    DECODED = 0,
    UNDECODED,       /*ԭʼ����  */
			      /* 返回错�?? */
	
}bit_decoded;
typedef enum ALL_FINISHED
{
    FINISHED = 0,
    UNFINISHED,       /*ԭʼ����  */
			      /* 返回错�?? */
	
}all_finished;
typedef enum MESSAGE_DECODABLE
{
    DECODEBALE = 0,
    UNDECODEBALE,       /*ԭʼ����  */
			      /* 返回错�?? */
	
}message_decodeble;
typedef enum DECODE_STATUS
{
    DECODE_SUCCESS = 0,
    DECODE_UNSUCCESS,       /*ԭʼ����  */
			      /* 返回错�?? */
	
}decode_status;
typedef struct DECODER * decoder_p;





typedef struct DECODER{
    UINT n;
    UINT k;
    UINT L;
    UINT shift_max;
    /**********���ݴ洢******/
    matrix_p decoding_data_matrix_p;//��ʵ����ʱ��ʵʹ�õ�
    #if 0
    matrix_p decoding_data_matrix_b_p;//Ϊ�˴�ӡ������ߵ��Է���ʹ�á�
    #endif
    shift_p shift_matirx_p;
    ori_data_p ori_data_matrix_p;
    
    /**�������ݰ���Ϣ�ռ�**/
    matrix_p phi_p;//k*1�������ڴ洢ÿ��source���ݰ�����һ��Ҫ���ظ���������λ
    /*********����*********/
    UINT __load_message_number;//����ȷ���Ѿ��������յ����ݰ���������

    //INT (*load_encoding_message_from_file)(decoder_p self,FILE * fd,UINT message_number);
    //ֱ�ӻ�ȡ��shift_matrix��encoding_matrix
    //INT (*load_encoding_message_from_encoder)(decoder_p self,encoder_p encoder_instance);
    INT (*decode)(decoder_p self,zigzag_method method);//��Ҫ��ǰ�Ѿ�������n��message
    INT (*show)(decoder_p self);//da'yin
    


}decoder;


INT init_decoder_from_encoder(decoder_p self,int n, int k,int L,int shift_max,encoder_p encoder_instance);
INT destroy_decoder(decoder_p self);
decode_status check_decoding_correct(decoder_p decoder_isntance,encoder_p encoder_instance);

#endif // !__DECODER_H__
