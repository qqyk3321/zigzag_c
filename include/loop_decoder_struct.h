/*******************************************************************
 * @Descripttion   : �����򵥵����ݽṹ���ڴ���loop_zigzag�е����⡣
 *                    ��һ�����ݽṹ��˳����У����ڱ�����ת�жϽ׶�k�ν���ģ��������ݰ���ź�ԭʼ���ݰ���ţ�
 * 
 *                    �ڶ������ݽṹ���ڶ������ݽṹ�����ж��Ƿ��������K��Ԫ�صĽ�ϡ�
 * @version        : qqyk_01
 * @Author         : qqyk
 * @Date           : 2022-05-26 13:02
 * @LastEditTime   : 2022-05-27 10:56
 * @Copyright (c) 2022 by qqyk/Zigzag, All Rights Reserved. 
*******************************************************************/
#ifndef __LOOP_DECODER_STRUCT_H__
#define __LOOP_DECODER_STRUCT_H__


#include "include.h"

/******�������node �Ľṹ***************/
typedef struct QUEUE_NODE * queue_node_p;
typedef struct QUEUE_NODE{
    queue_node_p next;
    queue_node_p pre;
    UINT i;
    UINT j;  
    /********�����б�*******/
    INT (* set)(queue_node_p self,UINT i,UINT j);
    INT (* link_head)(queue_node_p self,queue_node_p pre);
    INT (* link_tail)(queue_node_p self,queue_node_p tail);
    INT (* break_pre)(queue_node_p self);
    INT (* break_tail)(queue_node_p self);
    is_head(*check_head)(queue_node_p self);
    is_tail(*check_tail)(queue_node_p self);
    

    
}queue_node;
INT init_node(queue_node_p self);
INT destroy_node(queue_node_p self);
/****ö������****/
typedef enum IS_HEAD{
    HEAD,
    UHEAD,
}is_head;
typedef enum IS_TAIL{
    TAIL,
    UTAIL,
}is_tail;




/*******************�������***************/
typedef struct QUEUE * queue_p;
typedef struct QUEUE{
    queue_node_p head;
    queue_node_p tail;
        
    /***�����б�****/
    INT (*append)(queue_p self,queue_node_p node_2_insert);
    INT (*remove)(queue_p self,queue_node_p node_2_remove);
    INT (*is_empty)(queue_p self);
}queue;
INT init_queue(queue_p self);
INT destroy_queue(queue_p self);
typedef enum IS_EMPTY{
    EMPTY,
    UEMPTY,
}is_empty;
/********************����˳�򼯺�***********/
typedef struct SEQ_SET * seq_set_p;
typedef struct SEQ_SET{
    //����ĳһλΪ1��ʾ��ǰ�����У����Ҷ���һ��UINT ��ʾ��ǰ�Ѿ���ɵ���Ŀ��
    //��ǰԤ��õĴ�С������ʾ8*8*8�����ݵ�����
    //
    UCHAR flag[64];
    UINT count;
    is_in (*num_is_in)(seq_set_p self,UINT number);
    is_empty(* set_is_empty)(seq_set_p self);
    INT (*pop)(seq_set_p self,UINT number);
    INT (*add)(seq_set_p self,UINT number);
}seq_set;
INT init_seq_set(seq_set_p self);
INT destroy_seq_set(seq_set_p self);
typedef enum IS_IN{
    IN,
    UIN,
}is_in;
#endif //end #ifndef __