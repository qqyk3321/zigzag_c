/*******************************************************************
 * @Descripttion   : 
 * @version        : qqyk_01
 * @Author         : qqyk
 * @Date           : 2022-05-23 12:50
 * @LastEditTime   : 2022-05-23 13:00
 * @Copyright (c) 2022 by qqyk/Zigzag, All Rights Reserved. 
*******************************************************************/
#include "include.h"
#include "matrix.h"
#include "ori_data.h"


//声明列表

 


/*******************ENCODING_MATRIX对象函数操作声明***********************/
static INT random_generate_ori_data(ori_data_p self);//随机产生移位矩阵
static INT show_ori_data(ori_data_p self);//打印数据
static INT get_ori_data_member(ori_data_p self,UINT row_number,UINT col_number,VOID ** pos);//获取某个元素
static INT ori_data_row_min(ori_data_p self,UINT row_number,UINT * pos_1,UINT * pos_2);//获取某一行中最小的元素的两个元素的位置。

    


//实体部分
/*************************有关ori_data_MATRIX 对象******************************/
INT init_ori_data(ori_data_p self,UINT k,UINT L)//。  
{
    self->k=k;
    self->L=L;
   
    self->random_generate_ori_data=random_generate_ori_data;

    self->show_ori_data=show_ori_data;
    self->get_ori_data_member=get_ori_data_member;
    self->ori_data_row_min=ori_data_row_min;
   
    
    matrix_p ori_data_matrix_p;
    ori_data_matrix_p=(matrix_p)malloc(sizeof(matrix));

    if(NULL == ori_data_matrix_p)
    {
        INFO_ERR("malloc error");
    }
    init_matrix(ori_data_matrix_p,k,L,"uchar");

    self->ori_data_matrix_p=ori_data_matrix_p;
    //printf("end __init\n");
    
    return RETURN_SUCCESS;
    
}
INT destroy_ori_data(ori_data_p self)//关闭
{
    free(self);
    destroy_matrix(self->ori_data_matrix_p);
    return RETURN_SUCCESS;
}


static INT random_generate_ori_data(ori_data_p self)//随机产生移位矩阵
{
    //printf("start\n");
    return self->ori_data_matrix_p->random_generate(self->ori_data_matrix_p,256);
    
}
static INT show_ori_data(ori_data_p self)//打印数据
{
    self->ori_data_matrix_p->show(self->ori_data_matrix_p,"ori_data_matrix");
    return RETURN_SUCCESS;
}
static INT get_ori_data_member(ori_data_p self,UINT row_number,UINT col_number,VOID ** pos){
    self->ori_data_matrix_p->get_member(self->ori_data_matrix_p,row_number,col_number,pos);
    return RETURN_SUCCESS;
}//获取某个元素
static INT ori_data_row_min(ori_data_p self,UINT row_number,UINT * pos_1,UINT * pos_2){
    self->ori_data_matrix_p->row_min(self->ori_data_matrix_p,row_number,pos_1,pos_2);
    return RETURN_SUCCESS;
}//获取某一行中最小的元素的两个元素的位置。

//#define TEST_ori_data 1
#ifdef TEST_ori_data
INT main(){
    ori_data_p test_ori_data_p=(ori_data_p)malloc(sizeof(ori_data));
    init_ori_data(test_ori_data_p,3,4);
    test_ori_data_p->random_generate_ori_data(test_ori_data_p);
    test_ori_data_p->show_ori_data(test_ori_data_p);
    
    

    
    void ** pos=malloc(sizeof(UCHAR *));
    
   
    UINT row;
    UINT *pos_1=(UINT*)malloc(sizeof(UINT));
    UINT *pos_2=(UINT*)malloc(sizeof(UINT));
    for (row=0;row<test_ori_data_p->k;row++){
        test_ori_data_p->ori_data_row_min(test_ori_data_p,row,pos_1,pos_2);

        //INFO_LOG("row_min_finished");
        test_ori_data_p->get_ori_data_member(test_ori_data_p,row,*pos_1,pos);
        printf("row:%d,col:%d is min =%02x\n,",row,*pos_1,*((UCHAR *)*pos));
        test_ori_data_p->get_ori_data_member(test_ori_data_p,row,*pos_2,pos);
        printf("row:%d,col=%d is min_2 =%02x\n,",row,*pos_2,*((UCHAR *)*pos));
    }

    destroy_ori_data(test_ori_data_p);
    return RETURN_SUCCESS;
}
#endif

