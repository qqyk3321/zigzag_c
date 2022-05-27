/*******************************************************************
 * @Descripttion   : 
 * @version        : qqyk_01
 * @Author         : qqyk
 * @Date           : 2022-05-23 13:01
 * @LastEditTime   : 2022-05-25 15:44
 * @Copyright (c) 2022 by qqyk/Zigzag, All Rights Reserved. 
*******************************************************************/
#include "encoder.h"



/*******************encoder对象函数操作声明***********************/
INT generate_shift(encoder_p self);
    //修改生成ori_data的方法，只需要修改init函数中的对应关系，目前使用的是random
INT generate_ori_data(encoder_p self);
    //修改生成encode的方法，只需要修改init函数中的对应关系，目前使用的是单线程方法
INT encode(encoder_p self);//利用SHIFT_MATRIX和原始数据包ORI_DATA生成n个编码数据包

INT get_shift_member(encoder_p self,UINT row_number,UINT col_number,VOID ** pos); 
INT get_encoding_data_member(encoder_p self,UINT row_number,UINT col_number,VOID ** pos);
INT get_ori_data_member(encoder_p self,UINT row_number,UINT col_number,VOID ** pos);
    
INT show_encoding_data(encoder_p self);//打印数据
INT show_shift(encoder_p self);
INT show_ori_data(encoder_p self);
/*******static 函数声明**************/
static INT __random_generate_shift(encoder_p self);
static INT __random_generate_ori_data(encoder_p self);
static INT __encode_s_thread(encoder_p self);
    


//实体部分
/*************************有关ori_data_MATRIX 对象******************************/
INT init_encoder(encoder_p self,UINT k,UINT n,UINT L,UINT shift_max)//。  
{
    self->k=k;
    self->L=L;
    self->n=n;
    self->shift_max=shift_max;
    shift_p shift_matrix_p=(shift_p)malloc(sizeof(shift));
    ori_data_p ori_data_matrix_p=(ori_data_p)malloc(sizeof(ori_data));
    init_ori_data(ori_data_matrix_p,k,L);
    init_shift(shift_matrix_p,k,n,shift_max);

    self->ori_data_p=ori_data_matrix_p;
    self->shift_p=shift_matrix_p;


    //方法声明
    
    self->generate_shift=generate_shift;
    
    self->generate_ori_data=generate_ori_data;

    self->encode=encode;

    self->get_encoding_data_member=get_encoding_data_member;
    self->get_ori_data_member=get_ori_data_member;
    self->get_shift_member=get_shift_member;
    self->show_encoding_data=show_encoding_data;//打印数据
    self->show_shift=show_shift;
    self->show_ori_data=show_ori_data;
    
    matrix_p encoding_data_p;
    encoding_data_p=(matrix_p)malloc(sizeof(matrix));

    if(NULL == encoding_data_p)
    {
        INFO_ERR("malloc error");
    }
    init_matrix(encoding_data_p,n,(L+shift_max-1),"uchar");

    self->encoding_data_p=encoding_data_p;
    //printf("end __init\n");
    
    return RETURN_SUCCESS;
    
}
INT destroy_encoder(encoder_p self)//关闭
{
    
    destroy_matrix(self->encoding_data_p);
    //INFO_LOG("free encoding_data_p");
    destroy_ori_data(self->ori_data_p);
    //INFO_LOG("free destroy_ori_data");
    destroy_shift(self->shift_p);
    //INFO_LOG("free destroy_shift");
    free(self);
    return RETURN_SUCCESS;
}

INT generate_shift(encoder_p self){
    return __random_generate_shift(self);
    
}
    
INT generate_ori_data(encoder_p self){
    return __random_generate_ori_data(self);
    
}
    
INT encode(encoder_p self){
    return __encode_s_thread(self);
}
    
INT get_shift_member(encoder_p self,UINT row_number,UINT col_number,VOID ** pos){
    return self->shift_p->get_shift_member(self->shift_p,row_number,col_number,pos);
}
INT get_encoding_data_member(encoder_p self,UINT row_number,UINT col_number,VOID ** pos){
    return self->encoding_data_p->get_member(self->encoding_data_p,row_number,col_number,pos);
}
INT get_ori_data_member(encoder_p self,UINT row_number,UINT col_number,VOID ** pos){
    return self->ori_data_p->get_ori_data_member(self->ori_data_p,row_number,col_number,pos);
}
INT show_encoding_data(encoder_p self){

    return self->encoding_data_p->show(self->encoding_data_p,"encoding_data");
}
INT show_shift(encoder_p self){
    return self->shift_p->show_shift(self->shift_p);
    
}
INT show_ori_data(encoder_p self){
    return self->ori_data_p->show_ori_data(self->ori_data_p);
}
/*******static 函数声明**************/
static INT __random_generate_shift(encoder_p self){
    self->shift_p->random_generate_shift(self->shift_p);
    return RETURN_SUCCESS;
}
static INT __random_generate_ori_data(encoder_p self){
    //INFO_LOG("in __random_generate_ori_data");
    self->ori_data_p->random_generate_ori_data(self->ori_data_p);
    //INFO_LOG("finished __random_generate_ori_data");
    return RETURN_SUCCESS;  
}
static INT __encode_s_thread(encoder_p self){
    //按照shift matrix的内容，移位生成对应的数据。
    
    UINT encoding_number=0;
    for (encoding_number=0;encoding_number<self->n;encoding_number++){
        UINT ori_number=0;
        
        for (ori_number=0;ori_number<self->k;ori_number++){
            UINT ori_shift;
            
            VOID** shift_member_pos=malloc(sizeof(UINT *));
            //找到对应编码数据包encoding_number中原始数据包ori_number的移位量
            //获取对应ori_shift
            
            self->get_shift_member(self,encoding_number,ori_number,shift_member_pos);
            
            ori_shift=*(UINT *)(*shift_member_pos);
            //INFO_LOG("encoding_number=%d,ori_number=%d,shift=%d",encoding_number,ori_number,ori_shift);
            
            UINT data_bit=0;
            //需要进行需要对每个编码数据位进行操作
            VOID ** ori_data_start_pos=malloc(sizeof(UCHAR *));;
            self->get_ori_data_member(self,ori_number,0,ori_data_start_pos);
            
            VOID ** encoding_data_start_pos=malloc(sizeof(UCHAR *));;
            self->get_encoding_data_member(self,encoding_number,ori_shift,encoding_data_start_pos);
            //获取原始数据包和编码移位数据包对应的起始位置
            for (data_bit=0;data_bit<self->L;data_bit++){
                //每个编码数据包的每个位
                *(UCHAR *)(*encoding_data_start_pos+data_bit)^=*(UCHAR *)(*ori_data_start_pos+data_bit);
            }//end for data_bit
            free(shift_member_pos);
            free(ori_data_start_pos);
            free(encoding_data_start_pos);
        }//end for oridata
        
    }//end for encoding_number



    return RETURN_SUCCESS;
}


#define test_encoding 0
#if test_encoding
INT main(){
    encoder_p test_encoder_p=(encoder_p)malloc(sizeof(encoder));
    init_encoder(test_encoder_p,4,5,10,5);
    //INFO_LOG("finished encodier");
    test_encoder_p->generate_ori_data(test_encoder_p);
    //INFO_LOG("generate oridata");
    test_encoder_p->generate_shift(test_encoder_p);
    test_encoder_p->show_ori_data(test_encoder_p);
    test_encoder_p->show_shift(test_encoder_p);
    test_encoder_p->encode(test_encoder_p);
    test_encoder_p->show_encoding_data(test_encoder_p);
    

    destroy_encoder(test_encoder_p);
    return RETURN_SUCCESS;
}

#endif