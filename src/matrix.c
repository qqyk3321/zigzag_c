/*
 * @Author: taikongren0723@163.com taikongren0723@163.com
 * @Date: 2022-05-21 16:19:33
 * @LastEditors: taikongren0723@163.com taikongren0723@163.com
 * @LastEditTime: 2022-05-22 21:51:43
 * @FilePath: /zigzag_c/matrix.c
 * @Description:矩阵使用实例主体部分
 */
#include "matrix.h"

/********方法声明********************/
static INT show( matrix_p self,CHAR * title);
//返回行最小值和第二小值所在位置
static INT row_min(matrix_p self,UINT row_number,UINT *pos_1,UINT *pos_2);
//随机产生所有数据
static INT random_generate(matrix_p self,UINT member_max); 
//返回第row_number,col_number对应的元素
static INT get_member(matrix_p self,UINT row_number,UINT col_number,VOID** pos);
//比较两个位置元素大小，判断是否交换
static INT __compare(matrix_p self,UINT row_number,UINT *pos_min,UINT * pos);
//交换两个位置的元素
static INT __swap(UINT *pos_1,UINT *pos_2);
//某一列增加
static INT col_add(matrix_p self,UINT col_number,UINT increase);
//某一行减
static INT row_subtraction(matrix_p self,UINT row_number,UINT decrease);
//矩阵标准化，定义为减去每一行中最小值，确保最小值为0
static INT norm(matrix_p self);
/********主体部分**********************/
INT init_matrix(matrix_p self,UINT row_size,UINT col_size,CHAR * type){
    /**确保self，已经被malloc了*/
    /**下面是对应的实例初始化***/
    self->row_size=row_size;
    self->col_size=col_size;
    /****判断所属数据类型*********/

    if(strcmp(type,"uchar")==0){
        self->type_size=sizeof(UCHAR);
    }
    else if(strcmp(type,"uint")==0){
        self->type_size=sizeof(UINT);
    }
    else{
        INFO_ERR("incorrect type");
    }



    //************分配对应的空间给p*************//
    void *p;
    p=malloc(self->type_size*row_size*col_size);
    if(p==NULL){
        INFO_ERR("malloc error");
    }
    memset(p,0,self->type_size*row_size*col_size);
    self->p=p;
    //***************方法赋值******************//
    self->show=show;
    //随机产生所有数据
    self->random_generate=random_generate;
    //返回行最小值和第二小值所在位置
    self->row_min=row_min;
    //返回第row_number,col_number对应的元素
    self->get_member=get_member;
    //某一列增加
    self->col_add=col_add;
    //某一行减
    self->row_subtraction=row_subtraction;
    //确保每一行中最小值为0；
    self->norm=norm;
    return RETURN_SUCCESS;
}

static INT show(matrix_p self,CHAR * title){
/*打印示例
* title=
* [[1 2 3 3]
* [2 3 4 5]]
*/
    
    printf ("%s=\n",title);
    UINT row;
    UINT col;




    printf("[");
    if (self->type_size==sizeof(UCHAR)){
        for (row=0;row<self->row_size;row++){
            printf("[");
            for (col=0;col<self->col_size;col++){
                if (col==0){
                    printf("%02x",*((UCHAR *)self->p+row*self->col_size+col));
                }
                else{
                    printf("  %02x",*((UCHAR *)self->p+row*self->col_size+col));
                }
                
            }
            if(row==self->row_size-1){
                ;
            }
            else{
                printf("]\n");
            }
        }


    }
    else{
        for (row=0;row<self->row_size;row++){
            printf("[");
            for (col=0;col<self->col_size;col++){
                if (col==0){
                    printf("%d",*((int*)self->p+row*self->col_size+col));
                }
                else{
                    printf("  %d",*((int*)self->p+row*self->col_size+col));
                }
                
            }
            if(row==self->row_size-1){
                ;
            }
            else{
                printf("]\n");
            }
        }

    }


    





    printf("]]\n");

    
    
    
    return RETURN_SUCCESS;

}


INT destroy_matrix(matrix_p self){
    free(self->p);
    free(self);
    return RETURN_SUCCESS;
}

/**
 * @description: 
 * @param {matrix} self
 * @param {UINT} row_number
 * @param {UINT} *pos_min
 * @param {UINT *} pos
 * @return {*}0:代表>=无需交换pos与pos_min  
 *         * 1 :表示需要交换，因为pos_min对应的元素大于pos对应的元素。
 */
static INT __compare(matrix_p self,UINT row_number,UINT *pos_min,UINT * pos){
    VOID ** data_pos_min=malloc(sizeof(UINT *));
    VOID ** data_pos=malloc(sizeof(UINT *));
    self->get_member(self,row_number,*pos_min,data_pos_min);
    self->get_member(self,row_number,*pos,data_pos);
    int change=0;
    if (self->type_size==sizeof(UCHAR)){
        if(*((UCHAR *)*data_pos_min)>*((UCHAR *)*data_pos)){//*((UCHAR *)*data_pos)
            __swap(pos_min,pos);
            change=1;
            
        }
        else{
            ;
        }

    }
    else{
        if(*((UINT *)*data_pos_min)>*((UINT *)*data_pos)){
            __swap(pos_min,pos);
            change=1;
        }
        else{
            ;
        }

    }
    free(data_pos_min);
    free(data_pos);
    return change;
    
    
}

//交换两个UNIT指针指向的内容。
static INT __swap(UINT *pos_1,UINT *pos_2){
    UINT pos=0;
    pos=*pos_1;
    *pos_1=*pos_2;
    *pos_2=pos;
    return RETURN_SUCCESS;
}




//返回行最小值和第二小值所在位置
/**
 * @description: 返回最小值
 * @param {matrix_p} self
 * @param {UINT} row_number
 * @param {UINT} *pos_1//表示最小的元素的列坐标的指针
 * @param {UINT} *pos_2//表示第二小的元素列坐标的指针。
 * @return {*}
 */
static INT row_min(matrix_p self,UINT row_number,UINT *pos_1,UINT *pos_2){
    if(self->col_size<2){
        INFO_ERR("there is only one element in the row");
    }    



    //INFO_LOG("START row_min");
    *pos_1=0;
    *pos_2=1;
    __compare(self,row_number,pos_1,pos_2);
    //INFO_LOG("compare pos_1,pos_2");
    UINT col=2;
    UINT * pos=(UINT *)malloc(sizeof(UINT));
    for (col=2;col<self->col_size;col++){
        *pos=col;
        //INFO_LOG("now col=%d",col);
        if(__compare(self,row_number,pos_1,pos)){
            //如果发生交换，没有比较直接交换就好。
            __swap(pos,pos_2);
        }
        else{
            __compare(self,row_number,pos_2,pos);
        }

    }
    
    free(pos);


    
    return RETURN_SUCCESS;
}
//随机产生所有数据
static INT random_generate(matrix_p self,UINT member_max){
    UINT row;
    UINT col;
    UINT random_data;
    srand((unsigned)time(NULL));
    //printf("start\n");
    
    if(self->type_size==sizeof(UCHAR)){
        for (row=0;row<self->row_size;row++){
            for (col=0;col<self->col_size;col++){
                random_data=rand();
                random_data=(UCHAR)(random_data%member_max);
                *((UCHAR *)self->p+row*self->col_size+col)=random_data;
                
            }
            
        }

    }
    else{
        for (row=0;row<self->row_size;row++){
            for (col=0;col<self->col_size;col++){
                random_data=rand();
                random_data=(UINT)(random_data%member_max);
                *((UINT *)self->p+row*self->col_size+col)=random_data;
                
            }
            
        }

    }
    

    
    
    return RETURN_SUCCESS;
}
//返回第row_number,col_number对应的元素
static INT get_member(matrix_p self,UINT row_number,UINT col_number,VOID** pos){
    if(row_number>=self->row_size || col_number>=self->col_size ){
        INFO_ERR(" row_number and col_number is illegal\n row_number=%d,row_size=%d\n col_number=%d,col_size=%d\n",row_number,self->row_size,col_number,self->col_size);
    }
    if(self->type_size==sizeof(UCHAR)){
        *pos=(UCHAR *)self->p+row_number*self->col_size+col_number;
    }
    else{
        *pos=(UINT *)self->p+row_number*self->col_size+col_number;
    }
    
    return RETURN_SUCCESS;
}

//对应列增加，过界的问题不考虑。并且仅支持int类型
static INT col_add(matrix_p self,UINT col_number,UINT increase){
    UINT row;
    if(col_number>=self->col_size){
        INFO_ERR("illegal col_number");
    }
    if(self->type_size==sizeof(UCHAR))
    {
        INFO_ERR("char type not supported");
        
    }
    for(row=0;row<self->row_size;row++){
        *((UINT*)self->p+row*self->col_size+col_number)+=increase;
    }
    return RETURN_SUCCESS;
    
}
static INT row_subtraction(matrix_p self,UINT row_number,UINT decrease){
    UINT col;
    if(row_number>=self->row_size){
        INFO_ERR("illegal row_number");
    }
    if(self->type_size==sizeof(UCHAR))
    {
        INFO_ERR("char type not supported");
        
    }
    UINT row_shift=row_number*self->col_size;
    for(col=0;col<self->col_size;col++){
        *((UINT*)self->p+row_shift+col)-=decrease;
    }
    return RETURN_SUCCESS;
}
static INT norm(matrix_p self){
    UINT row=0;
    void ** pos=malloc(sizeof(UINT *));
    UINT *pos_1=(UINT*)malloc(sizeof(UINT));
    UINT *pos_2=(UINT*)malloc(sizeof(UINT));
    for(row=0;row<self->row_size;row++){
        //check最小值，
        
        self->row_min(self,row,pos_1,pos_2);

        //INFO_LOG("row_min_finished");
        self->get_member(self,row,*pos_1,pos);
        //printf("row:%d,col:%d is min =%d\n,",row,*pos_1,*((UINT *)*pos));
        self->row_subtraction(self,row,*((UINT *)*pos));
        //该行decrease
        
    }
    free(pos);
    free(pos_1);
    free(pos_2);
    return RETURN_SUCCESS;
}
#define TEST_MATRIX 0
#if TEST_MATRIX

int main(){
    matrix_p mm;
    mm=(matrix_p)malloc(sizeof(matrix));
    init_matrix(mm,10,10,"uint");
    mm->random_generate(mm,300);
    mm->show(mm,"title");
    void ** pos=malloc(sizeof(UINT *));
    
   
    UINT row;
    UINT *pos_1=(UINT*)malloc(sizeof(UINT));
    UINT *pos_2=(UINT*)malloc(sizeof(UINT));
    mm->norm(mm);
    mm->show(mm,"title");
    destroy_matrix(mm);
    matrix_p tt;
    tt=(matrix_p)malloc(sizeof(matrix));
    init_matrix(tt,10,10,"uchar");
    tt->random_generate(tt,200);
    tt->show(tt,"title");
    
    for (row=0;row<tt->row_size;row++){
        tt->row_min(tt,row,pos_1,pos_2);

        //INFO_LOG("row_min_finished");
        tt->get_member(tt,row,*pos_1,pos);
        printf("row:%d,col:%d is min =%02x\n,",row,*pos_1,*((UCHAR *)*pos));
        tt->get_member(tt,row,*pos_2,pos);
        printf("row:%d,col=%d is min_2 =%02x\n,",row,*pos_2,*((UCHAR *)*pos));
    }
    destroy_matrix(tt);
    return 0;
}
#endif
