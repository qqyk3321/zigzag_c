/*******************************************************************
 * @Descripttion   : shift���������Ҫ���ݴ洢ʹ��matrix��,ÿ��Ԫ����int�͡�
 * @version        : qqyk_01
 * @Author         : qqyk
 * @Date           : 2022-05-23 10:29
 * @LastEditTime   : 2022-05-23 16:17
 * @Copyright (c) 2022 by qqyk/Zigzag, All Rights Reserved. 
*******************************************************************/
#include "shift.h"
#include "include.h"

//�����б�

 


/*******************ENCODING_MATRIX��������������***********************/
static INT random_generate_shift(shift_p self);//���������λ����
static INT show_shift(shift_p self);//��ӡ����
static INT get_shift_member(shift_p self,UINT row_number,UINT col_number,VOID ** pos);//��ȡĳ��Ԫ��
static INT shift_row_min(shift_p self,UINT row_number,UINT * pos_1,UINT * pos_2);//��ȡĳһ������С��Ԫ�ص�����Ԫ�ص�λ�á�
static INT shift_col_add(shift_p self,UINT col_number,UINT increase);//��ȡ�м�increase
    


//ʵ�岿��
/*************************�й�SHIFT_MATRIX ����******************************/
INT init_shift(shift_p self,UINT k,UINT n,UINT shift_max)//��  
{
    self->k=k;
    self->n=n;
    self->shift_max=shift_max;
    self->random_generate_shift=random_generate_shift;

    self->show_shift=show_shift;
    self->get_shift_member=get_shift_member;
    self->shift_row_min=shift_row_min;
    self->shift_col_add=shift_col_add;
    
    matrix_p shift_matrix_p;
    shift_matrix_p=(matrix_p)malloc(sizeof(matrix));

    if(NULL == shift_matrix_p)
    {
        INFO_ERR("malloc error");
    }
    init_matrix(shift_matrix_p,n,k,"uint");

    self->shift_matrix_p=shift_matrix_p;
    //printf("end __init\n");
    
    return RETURN_SUCCESS;
    
}
INT destroy_shift(shift_p self)//�ر�
{
    free(self);
    destroy_matrix(self->shift_matrix_p);
    return RETURN_SUCCESS;
}


static INT random_generate_shift(shift_p self)//���������λ����
{
    //printf("start\n");
    //INFO_LOG("in random_generate_shift");
    
    self->shift_matrix_p->random_generate(self->shift_matrix_p,self->shift_max);
    self->shift_matrix_p->norm(self->shift_matrix_p);
    //����룬����ȷ��ÿһ������Сֵ��Ϊ0.
    return RETURN_SUCCESS;
    
}
static INT show_shift(shift_p self)//��ӡ����
{
    self->shift_matrix_p->show(self->shift_matrix_p,"shift_matrix");
    return RETURN_SUCCESS;
}
static INT get_shift_member(shift_p self,UINT row_number,UINT col_number,VOID ** pos){
    self->shift_matrix_p->get_member(self->shift_matrix_p,row_number,col_number,pos);
    return RETURN_SUCCESS;
}//��ȡĳ��Ԫ��
static INT shift_row_min(shift_p self,UINT row_number,UINT * pos_1,UINT * pos_2){
    self->shift_matrix_p->row_min(self->shift_matrix_p,row_number,pos_1,pos_2);
    return RETURN_SUCCESS;
}//��ȡĳһ������С��Ԫ�ص�����Ԫ�ص�λ�á�
static INT shift_col_add(shift_p self,UINT col_number,UINT increase){
    self->shift_matrix_p->col_add(self->shift_matrix_p,col_number,increase);
    return RETURN_SUCCESS;
}//��ȡ�м�increase
//#define TEST_SHIFT 0
#ifdef TEST_SHIFT
INT main(){
    shift_p test_shift_p=(shift_p)malloc(sizeof(shift));
    init_shift(test_shift_p,3,4,20);
    test_shift_p->random_generate_shift(test_shift_p);
    test_shift_p->show_shift(test_shift_p);
    test_shift_p->shift_col_add(test_shift_p,1,100);
    test_shift_p->show_shift(test_shift_p);
    

    
    void ** pos=malloc(sizeof(UINT *));
    
   
    UINT row;
    UINT *pos_1=(UINT*)malloc(sizeof(UINT));
    UINT *pos_2=(UINT*)malloc(sizeof(UINT));
    for (row=0;row<test_shift_p->n;row++){
        test_shift_p->shift_row_min(test_shift_p,row,pos_1,pos_2);

        //INFO_LOG("row_min_finished");
        test_shift_p->get_shift_member(test_shift_p,row,*pos_1,pos);
        printf("row:%d,col:%d is min =%d\n,",row,*pos_1,*((UINT *)*pos));
        test_shift_p->get_shift_member(test_shift_p,row,*pos_2,pos);
        printf("row:%d,col=%d is min_2 =%d\n,",row,*pos_2,*((UINT *)*pos));
    }

    destroy_shift(test_shift_p);
    return RETURN_SUCCESS;
}
#endif



