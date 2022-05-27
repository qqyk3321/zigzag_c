/*******************************************************************
 * @Descripttion   : 构建需要的结构，主要是原始数据
 * @version        : qqyk_01
 * @Author         : qqyk
 * @Date           : 2022-05-18 12:52
 * @LastEditTime   : 2022-05-19 13:09
 * @Copyright (c) 2022 by qqyk/X1000, All Rights Reserved. 
*******************************************************************/
//引用列表
#include "struct.h"
//声明列表
static int __init_ori_data(struct ORI_DATA * self,int k,int L);
static int __init_shift(struct SHIFT_MATRIX * self,int k,int n,int shift_max);//。 
static int __init_encoding(struct ENCODING *self,int k,int n,int L,int shift_max);
/*******************ORI_DATA 对象函数操作声明***********************/
void generate_ori_data(struct ORI_DATA *self);
int close_ori_data(struct ORI_DATA *self);
int print_ori_data(struct ORI_DATA *self);
/*******************ENCODING_MATRIX对象函数操作声明***********************/
void generate_shift(struct SHIFT_MATRIX *self);//随机产生移位矩阵
int print_shift(struct SHIFT_MATRIX *self);//打印数据
int close_shift(struct SHIFT_MATRIX *self);//关闭
/*******************ENCODING 对象函数操作声明***********************/
void encode(struct ENCODING *self);//利用SHIFT_MATRIX和原始数据包ORI_DATA生成n个编码数据包
int print_encoding_data(struct ENCODING *self);//打印数据


int close_encoding(struct ENCODING *self);
int generate_encoding_data(struct ENCODING *self);

//实体部分
static int __init_ori_data(struct ORI_DATA * self,int k,int L)
{
 
    self->k=k;
    self->L=L;
    self->close_ori_data=close_ori_data;
    self->generate_ori_data=generate_ori_data;
    self->print_ori_data=print_ori_data;
    unsigned char * data_p;
    data_p=(unsigned char *)malloc(k*L*sizeof(unsigned char));
    if(NULL == data_p)
    {
        printf("error __init\n");
        perror("malloc error\n");
    }
    self->data_p=data_p;
    //printf("end __init\n");
    
    return 0;
};
int init_ori_data(struct ORI_DATA *self,int k,int L)
{
    
    return __init_ori_data(self,k,L);
    
};

void generate_ori_data(struct ORI_DATA *self)
{
   
    //printf("start\n");
    int row;
    int col;
    int random_data;
    srand((unsigned)time(NULL));
    //printf("start\n");
    for (row=0;row<self->k;row++){
        for (col=0;col<self->L;col++){
            random_data=rand();
            random_data=(unsigned char)(random_data%256);
            *(self->data_p+row*self->L+col)=random_data;
            
        }
        
    }
   
    return ;
};
int close_ori_data(struct ORI_DATA *self)
{
    free(self->data_p);
    return 0;
};

int print_ori_data(struct ORI_DATA *self)
{
    int row;
    int col;
    printf("ori_data is :\n");
    printf("[");
    for (row=0;row<self->k;row++){
        for (col=0;col<self->L;col++){
            if (col==0){
                printf("%02x",*(self->data_p+row*self->L+col));
                continue;
            }
            
            printf(" %02x",*(self->data_p+row*self->L+col));
        }
        printf("\n");
    }
    printf("]\n");
    return 0;
};
/*************************有关SHIFT_MATRIX 对象******************************/
int init_shift(struct SHIFT_MATRIX * self,int k,int n,int shift_max)//。  
{
    return __init_shift(self,k,n,shift_max);//。  
    
}
static int __init_shift(struct SHIFT_MATRIX * self,int k,int n,int shift_max)
{
    self->k=k;
    self->n=n;
    self->shift_max=shift_max;
    self->generate_shift=generate_shift;
    self->close_shift=close_shift;
    self->print_shift=print_shift;
    unsigned char * shift_p;
    shift_p=(unsigned char *)malloc(k*n*sizeof(unsigned char));
    if(NULL == shift_p)
    {
        printf("error __init\n");
        perror("malloc error\n");
    }
    self->shift_p=shift_p;
    //printf("end __init\n");
    
    return 0;
}  
void generate_shift(struct SHIFT_MATRIX *self)//随机产生移位矩阵
{
    //printf("start\n");
    int row;
    int col;
    int random_data;
    srand((unsigned)time(NULL));
    //printf("start\n");
    for (row=0;row<self->k;row++){
        for (col=0;col<self->n;col++){
            random_data=rand();
            random_data=(unsigned char)(random_data%self->shift_max);
            *(self->shift_p+row*self->n+col)=random_data;
            
        }
        
    }
   
    return ;
}
int print_shift(struct SHIFT_MATRIX *self)//打印数据
{
    int row;
    int col;
    printf("shift_matrix is :\n");
    printf("[");
    for (row=0;row<self->k;row++){
        for (col=0;col<self->n;col++){
            if (col==0){
                printf("%02x",*(self->shift_p+row*self->n+col));
                continue;
            }
            
            printf(" %02x",*(self->shift_p+row*self->n+col));
        }
        printf("\n");
    }
    printf("]\n");
    return 0;
}
int close_shift(struct SHIFT_MATRIX *self)//关闭
{
    free(self->shift_p);
    return 0;
}

/*****************************有关ENCODING对象******************************/
static int __init_encoding(struct ENCODING *self,int k,int n,int L,int shift_max)
{
    //所有实例的初始化加上
    //加上需要使用的函数
    //初始化所需要的encoding unsinged char 然后还需要初始化为0（大小为n*（L+max））
    self->k=k;
    self->L=L;
    self->n=n;
    self->shift_max=shift_max;
    
    self->ori_data=(struct ORI_DATA *)malloc(sizeof(struct ORI_DATA));//申请空间free 在close中
    self->shift_matrix=(struct SHIFT_MATRIX *)malloc(sizeof(struct SHIFT_MATRIX));//申请空间 free在close中
    //实例化申请
    __init_ori_data(self->ori_data,k,L);//实例化
    __init_shift(self->shift_matrix,k,n,shift_max);//实例化
    //存储空间self->encodi
    unsigned char* encoding_data_p;
    encoding_data_p=(unsigned char *)malloc(n*(L+shift_max)*sizeof(unsigned char));
    if(NULL == encoding_data_p)
    {
        printf("error __init\n");
        perror("malloc error\n");
    }
    self->encoding_data_p=encoding_data_p;
    //printf("end __init\n");
    
    
    //对应的方法装载
    self->encode=encode;
    self->print_encoding_data=print_encoding_data;//打印数据//打印数据
    self->close_encoding=close_encoding;
    self->generate_encoding_data=generate_encoding_data;//初始化encoding data空间的所有数据位为空，重新generare shiftmatrix和重新generate oridata
    return 0;

    
}
int close_encoding(struct ENCODING *self){
    return 0;
}

int generate_encoding_data(struct ENCODING *self){
    self->ori_data->generate_ori_data(self->ori_data);
    self->shift_matrix->generate_shift(self->shift_matrix);
    memset(self->encoding_data_p,0,sizeof(unsigned char)*(self->L+self->shift_max)*self->n);
    return 0;
}
int init_encoding(struct ENCODING *self,int k,int n,int L,int shift_max)
{
    return __init_encoding(self,k,n,L,shift_max);
}
void encode(struct ENCODING *self)
{
    int encoding_number=0;
    for (encoding_number=0;encoding_number<self->n;encoding_number++){
        int ori_number=0;
        
        for (ori_number=0;ori_number<self->k;ori_number++){
            unsigned char ori_shift;
            //找到对应编码数据包encoding_number中原始数据包ori_number的移位量
            ori_shift=*(self->shift_matrix->shift_p+ori_number*self->n+encoding_number);
            
            
            int data_bit=0;
            //需要进行需要对每个编码数据位进行操作
            for (data_bit=0;data_bit<self->L;data_bit++){
                //每个编码数据包的每个位
                int encoding_data_shift=0;
                encoding_data_shift=data_bit+encoding_number*(self->L+self->shift_max);
                //找到对应原始数据位
                unsigned char ori_data_bit=0;
                ori_data_bit=*(self->ori_data->data_p+ori_number*self->L+data_bit);
                //修改对应位置，异或原始数据位数据
                *(self->encoding_data_p+encoding_data_shift+ori_shift)^=ori_data_bit;
            }//end for data_bit

        }//end for oridata
        
    }//end for encoding_number

    
    
}
int print_encoding_data(struct ENCODING *self)//打印数据
{
    //打印原始数据
    self->ori_data->print_ori_data(self->ori_data);
    //打印shift-matrix
    self->shift_matrix->print_shift(self->shift_matrix);
    //打印
    
    
    int row;
    int col;
    printf("encoding_data is :\n");
    printf("[");
    for (row=0;row<self->n;row++){
        for (col=0;col<(self->L+self->shift_max);col++){
            if (col==0){
                printf("%02x",*(self->encoding_data_p+row*(self->L+self->shift_max)+col));
                continue;
            }
            
            printf(" %02x",*(self->encoding_data_p+row*(self->L+self->shift_max)+col));
        }
        printf("\n");
    }
    printf("]\n");
    return 0;
}
