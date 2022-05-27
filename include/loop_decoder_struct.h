/*******************************************************************
 * @Descripttion   : 两个简单的数据结构用于处理loop_zigzag中的问题。
 *                    第一个数据结构：顺序队列，用于保存轮转判断阶段k次解码的（编码数据包序号和原始数据包序号）
 * 
 *                    第二个数据结构：第二个数据结构用于判断是否包含所有K个元素的结合。
 * @version        : qqyk_01
 * @Author         : qqyk
 * @Date           : 2022-05-26 13:02
 * @LastEditTime   : 2022-05-27 10:56
 * @Copyright (c) 2022 by qqyk/Zigzag, All Rights Reserved. 
*******************************************************************/
#ifndef __LOOP_DECODER_STRUCT_H__
#define __LOOP_DECODER_STRUCT_H__


#include "include.h"

/******定义队列node 的结构***************/
typedef struct QUEUE_NODE * queue_node_p;
typedef struct QUEUE_NODE{
    queue_node_p next;
    queue_node_p pre;
    UINT i;
    UINT j;  
    /********方法列表*******/
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
/****枚举类型****/
typedef enum IS_HEAD{
    HEAD,
    UHEAD,
}is_head;
typedef enum IS_TAIL{
    TAIL,
    UTAIL,
}is_tail;




/*******************定义队列***************/
typedef struct QUEUE * queue_p;
typedef struct QUEUE{
    queue_node_p head;
    queue_node_p tail;
        
    /***方法列别****/
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
/********************定义顺序集合***********/
typedef struct SEQ_SET * seq_set_p;
typedef struct SEQ_SET{
    //定义某一位为1表示当前集合有，并且定义一个UINT 表示当前已经完成的数目。
    //提前预设好的大小，最多表示8*8*8个数据的问题
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