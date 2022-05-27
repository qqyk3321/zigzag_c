/*******************************************************************
 * @Descripttion   : 解码，i和j是为了和论文中对应，全部使用i表示编码数据包序号。
 *                      j表示原始数据包序号。
 * @version        : qqyk_01
 * @Author         : qqyk
 * @Date           : 2022-05-25 09:44
 * @LastEditTime   : 2022-05-26 10:52
 * @Copyright (c) 2022 by qqyk/Zigzag, All Rights Reserved. 
*******************************************************************/
#include "decoder.h"
#include "include.h"
/************方法类型声明*************/



static INT show(decoder_p self);//打印矩阵，打印shift，解码完成的矩阵。打印phi。

static INT decode(decoder_p self,zigzag_method method);

/*************decode使用的函数************/
static INT __ori_zigzag_decode(decoder_p self);
static INT __greedy_zigzag_decode(decoder_p self);
static INT __zigzag_finished_decode(decoder_p self);//用于判断是否所有都完成了解码//是否需要一个循环队列结构来实现这个事情。
static INT __zigzag_j_finished_decode(decoder_p self,UINT j);//用于判断是否单个完成了解码。//用于判断是否完成了单个解码。
/**********解码会用到的公共方法***********/
static INT __recover(decoder_p self,UINT i,UINT j,UINT length);
static INT __get_encoding_message_pos(decoder_p self,UINT i,UINT j,VOID ** pos);
static INT __update_decoding_message(decoder_p self,UINT j,VOID **pos,UINT length);
static INT __updata_encoding_message(decoder_p self,UINT i,UINT j,VOID **pos ,UINT length);

static INT __update_R(decoder_p self,UINT i,UINT j,UINT length);
/*********ori_zigzag相关方法声明*****/




     
//初始化相关
static INT __load_encoding_message_from_encoder(decoder_p self,encoder_p encoder_instance);


/********实例和释放************/   
INT init_decoder_from_encoder(decoder_p self,int n, int k,int L,int shift_max,encoder_p encoder_instance){
    //从encoder_instance 中引入并赋值。
    self->n=n;
    self->k=k;
    self->L=L;
    self->shift_max=shift_max;
    //方法赋值
    self->decode=decode;
    self->show=show;
    //需要申请两片空间
    //申请ori_data空间
    self->ori_data_matrix_p=(ori_data_p)malloc(sizeof(ori_data));
    init_ori_data(self->ori_data_matrix_p,k,L);
    //memset(self->ori_data_matrix_p,0,sizeof(UCHAR)*k*L);
    //申请phi空间
    //self->phi_p=(UINT *)malloc(sizeof(UINT)*k);
    self->phi_p=(matrix_p)malloc(sizeof(matrix));
    init_matrix(self->phi_p,k,1,"uint");

    __load_encoding_message_from_encoder(self,encoder_instance);
    return RETURN_SUCCESS;
    
} 
INT destroy_decoder(decoder_p self){
    //暂时不操作
    return RETURN_SUCCESS;
}
decode_status check_decoding_correct(decoder_p decoder_isntance,encoder_p encoder_instance){
    INT success;
    success=memcmp(decoder_isntance->ori_data_matrix_p->ori_data_matrix_p->p,encoder_instance->ori_data_p->ori_data_matrix_p->p,sizeof(UCHAR)*encoder_instance->k*encoder_instance->L);
    if(success==0){
        return DECODE_SUCCESS;
    }
    else{
        return DECODE_UNSUCCESS;
    }
}

/***********主题部分******************/
static INT decode(decoder_p self,zigzag_method method){
    //方法选择
    //必须初始化
    memset(self->ori_data_matrix_p->ori_data_matrix_p->p,0,sizeof(UCHAR)*self->k*self->L);
    
    memset(self->phi_p->p,0,sizeof(UINT)*self->k);
    switch (method)
    {
    case ORI_ZIGZAG:
        return __ori_zigzag_decode(self);
        break;
    case GREEDY_ZIGZAG:
        return __greedy_zigzag_decode(self);
        break;
    
    default:
        INFO_ERR("illegal method");
        sleep(5);
        return RETURN_SUCCESS;
    }

    return RETURN_SUCCESS;
}
static INT __ori_zigzag_decode(decoder_p self){
    //方法描述：i表示编码数据包序号，j表示原始数据包序号
    /* 详见论文吧还是~
    *   需要注意的是，这里考虑了解码失败的情况，就是判断所有编码数据包都不存在可解数据位。             
    *    matrix对象，生成的时候已经格式化过。
    *    申请的phi实际上由于没有思考周全，这里定义为k*1的矩阵。         
    */ 
   
   
    UINT i;//表示编码数据包序号，和论文一致(有的时候用index_encoding)
    //UINT j;//表示原始数据包序号，和论文一致(有的时候用index_source)
    bit_decoded if_decoded=DECODED;//用于说明是否有编码编码数据包发生解码。如果bit_decoded不为0，就没必要努力~
    while(__zigzag_finished_decode(self)==UNFINISHED && if_decoded==DECODED){
        if_decoded=UNDECODED;
        for(i=0;i<self->k;i++){
            //结尾标志如何制作？就是怎么能不对已经判断完毕的message进行操作。
            UINT min_1_index_source;//最小的
            UINT min_2_index_source;//第二小的
            self->shift_matirx_p->shift_row_min(self->shift_matirx_p,i,&min_1_index_source,&min_2_index_source);
            //

            //判断最小的两个元素的大小。
            VOID ** min_1_index_source_pos=malloc(sizeof(UCHAR *));
            VOID ** min_2_index_source_pos=malloc(sizeof(UCHAR *));
            self->shift_matirx_p->get_shift_member(self->shift_matirx_p,i,min_1_index_source,min_1_index_source_pos);
            self->shift_matirx_p->get_shift_member(self->shift_matirx_p,i,min_2_index_source,min_2_index_source_pos);
            
            //message_decodeble decodeble;
            //需要判断是不是该最小的位对应的原始数据包已经恢复完全，如果已经恢复完全也不会再需要恢复。
            VOID **min_1_phi_pos=malloc(sizeof(UINT *));
            self->phi_p->get_member(self->phi_p,min_1_index_source,0,min_1_phi_pos);
            //printf("number of coded message%d\n",i);
            //printf("min_1=%d,min_2=%d\n",*(UINT*)*min_1_index_source_pos,*(UINT*)*min_2_index_source_pos);
            //printf("what is shift phi %d\n",*(UINT*)*min_1_phi_pos);

            if(*(UINT*)*min_1_index_source_pos==*(UINT*)*min_2_index_source_pos ||*(UINT*)*min_1_phi_pos >=self->L){
                //decodeble=UNDECODEBALE;;
                if(*(UINT*)*min_1_phi_pos>=self->L){
                    __update_R(self,i,min_1_index_source,1);
                }
                ;
            }
            else{
                //decodeble=DECODEBALE;
                if_decoded=DECODED;
                //解码过程放大到这里
                __recover(self,i,min_1_index_source,1);
                __update_R(self,i,min_1_index_source,1);
            }
            
            free(min_1_phi_pos);
            free(min_1_index_source_pos);
            free(min_2_index_source_pos);
            
        }
    }
    return __zigzag_finished_decode(self);        
}


static INT __greedy_zigzag_decode(decoder_p self){
    //方法描述：i表示编码数据包序号，j表示原始数据包序号
    /* 详见论文吧还是~
    *   需要注意的是，这里考虑了解码失败的情况，就是判断所有编码数据包都不存在可解数据位。             
    *    matrix对象，生成的时候已经格式化过。
    *    申请的phi实际上由于没有思考周全，这里定义为k*1的矩阵。          
    */ 
  
   
    UINT i;//表示编码数据包序号，和论文一致(有的时候用index_encoding)
    //UINT j;//表示原始数据包序号，和论文一致(有的时候用index_source)
    bit_decoded if_decoded=DECODED;//用于说明是否有编码编码数据包发生解码。如果bit_decoded不为0，就没必要努力~
    while(__zigzag_finished_decode(self)==UNFINISHED && if_decoded==DECODED){
        if_decoded=UNDECODED;
        for(i=0;i<self->k;i++){
            //结尾标志如何制作？就是怎么能不对已经判断完毕的message进行操作。
            UINT min_1_index_source;//最小的
            UINT min_2_index_source;//第二小的
            self->shift_matirx_p->shift_row_min(self->shift_matirx_p,i,&min_1_index_source,&min_2_index_source);
            //

            //判断最小的两个元素的大小。
            VOID ** min_1_index_source_pos=malloc(sizeof(UCHAR *));
            VOID ** min_2_index_source_pos=malloc(sizeof(UCHAR *));
            self->shift_matirx_p->get_shift_member(self->shift_matirx_p,i,min_1_index_source,min_1_index_source_pos);
            self->shift_matirx_p->get_shift_member(self->shift_matirx_p,i,min_2_index_source,min_2_index_source_pos);
            
            //message_decodeble decodeble;
            //需要判断是不是该最小的位对应的原始数据包已经恢复完全，如果已经恢复完全也不会再需要恢复。
            VOID **min_1_phi_pos=malloc(sizeof(UINT *));
            self->phi_p->get_member(self->phi_p,min_1_index_source,0,min_1_phi_pos);
            //printf("number of coded message%d\n",i);
            //printf("min_1=%d,min_2=%d\n",*(UINT*)*min_1_index_source_pos,*(UINT*)*min_2_index_source_pos);
            //printf("what is shift phi %d\n",*(UINT*)*min_1_phi_pos);

            if(*(UINT*)*min_1_index_source_pos==*(UINT*)*min_2_index_source_pos ||*(UINT*)*min_1_phi_pos >=self->L){
                //decodeble=UNDECODEBALE;;
                if(*(UINT*)*min_1_phi_pos>=self->L){
                    __update_R(self,i,min_1_index_source,1);
                }
                ;
            }
            else{
                //decodeble=DECODEBALE;
                if_decoded=DECODED;
                //解码过程放大到这里
                //不同于ORI的地方，这里需要判断length的大小。
                //length的大小为 min((min2-min1)，L-phi[min_pos])
                UINT length;
                length=MIN((*(UINT*)*min_2_index_source_pos-*(UINT*)*min_1_index_source_pos),(self->L-*(UINT*)*min_1_phi_pos ));
                __recover(self,i,min_1_index_source,length);
                __update_R(self,i,min_1_index_source,length);
            }
            
            free(min_1_phi_pos);
            free(min_1_index_source_pos);
            free(min_2_index_source_pos);
            
        }
    }
    return __zigzag_finished_decode(self);        
}
/*******************************************************************
 * @Author: qqyk
 * @version: qqyk_01
 * @Date: 2022-05-25 10:37
 * @LastEditTime: Do not Edit
 * @brief  : 判断是否完成所有的解码
 * @return  {*}
 * @param {decoder_p} self
*******************************************************************/
static INT __zigzag_finished_decode(decoder_p self){
    UINT index_source;
    for(index_source=0;index_source<self->k;index_source++){
        if(__zigzag_j_finished_decode(self,index_source)==UNFINISHED){
            return UNFINISHED;
        }
    }

    return FINISHED;
    
}
static INT __zigzag_j_finished_decode(decoder_p self,UINT j){

    VOID ** phi_pos=malloc(sizeof(UINT *));
    self->phi_p->get_member(self->phi_p,j,0,phi_pos);
    //INFO_LOG("phi[%d]=%d ,self->L=%d\n",j,*(UINT*)*phi_pos,self->L);
    if(*(UINT*)*phi_pos<self->L){
        return UNFINISHED;
    }
    
    free(phi_pos);
    return FINISHED;
    
}




/*****************__recover  相关实体*********************/
static INT __recover(decoder_p self,UINT i,UINT j,UINT length){
    VOID **encoding_message_pos=malloc(sizeof(UCHAR *));//指向某一段数据的指针，

    __get_encoding_message_pos(self,i,j,encoding_message_pos);
    __update_decoding_message(self,j,encoding_message_pos,length);
    __updata_encoding_message(self,i,j,encoding_message_pos,length);
    free(encoding_message_pos);
    return RETURN_SUCCESS;
}
static INT __get_encoding_message_pos(decoder_p self,UINT i,UINT j,VOID ** encoding_message_pos){
    VOID ** shift_pos=malloc(sizeof(UINT *));
    self->shift_matirx_p->get_shift_member(self->shift_matirx_p,i,j,shift_pos);
    self->decoding_data_matrix_p->get_member(self->decoding_data_matrix_p,i,*(UINT*)*shift_pos,encoding_message_pos);
    free(shift_pos);
    return RETURN_SUCCESS;
}
static INT __update_decoding_message(decoder_p self,UINT j,VOID **encoding_message_pos,UINT length){
    
    VOID ** ori_data_pos=malloc(sizeof(UCHAR *));
    VOID ** phi_pos=malloc(sizeof(UINT *));

    /***获得phi_pos***/
    self->phi_p->get_member(self->phi_p,j,0,phi_pos);
    
    //self->ori_data_matrix_p
    //获得ori_data_matrix对应的位置。
    self->ori_data_matrix_p->get_ori_data_member(self->ori_data_matrix_p,j,*(UINT*)*phi_pos,ori_data_pos);
    //把encoding_message里面对应的内容，和长度 拷贝到ori_data_pos中。
    memcpy((UCHAR *)*ori_data_pos,(UCHAR *)*encoding_message_pos,sizeof(UCHAR)*length);
    *(UINT*)*phi_pos+=1;
    free(ori_data_pos);
    free(phi_pos);
    return RETURN_SUCCESS;
}
static INT __updata_encoding_message(decoder_p self,UINT i,UINT j,VOID **encoding_message_pos ,UINT length){
    UINT index;
    VOID ** encoding_other_message_pos=malloc(sizeof(UCHAR *));
    UINT now_length;
    for(index=0;index<self->n;index++){
        if (index==i){
            continue;
        }
        else{
            VOID ** shift_pos=malloc(sizeof(UINT *));
            //获取 R_(index,j)的偏移信息。
            self->shift_matirx_p->get_shift_member(self->shift_matirx_p,index,j,shift_pos);
            self->decoding_data_matrix_p->get_member(self->decoding_data_matrix_p,index,*(UINT*)*shift_pos,encoding_other_message_pos);
            //读取 对应位置信息，然后对应异或
            for(now_length=0;now_length<length;now_length++){
                *(UCHAR*)*(encoding_other_message_pos+now_length)^=*(UCHAR*)*(encoding_message_pos+now_length);
            }
            free(shift_pos);
        }

    }
    
    memset((UCHAR*)*encoding_message_pos,0,length);
    free(encoding_other_message_pos);
    return RETURN_SUCCESS;
}
static INT __update_R(decoder_p self,UINT i,UINT j,UINT length){
    //更新这里使用的东西。
    //shift 的第i行加1.实际上并没有使用到j。
    self->shift_matirx_p->shift_col_add(self->shift_matirx_p,j,length);
    return RETURN_SUCCESS;
}

/***********show实体*****************/
static INT show(decoder_p self){
    //打印shift矩阵
    self->shift_matirx_p->show_shift(self->shift_matirx_p);
    //打印phi。
    self->phi_p->show(self->phi_p,"phi");
    //打印encoding_data_matrix
    self->decoding_data_matrix_p->show(self->decoding_data_matrix_p,"decoding_message");
    //打印ori_data
    self->ori_data_matrix_p->show_ori_data(self->ori_data_matrix_p);
    
    return RETURN_SUCCESS;
}
static INT __load_encoding_message_from_encoder(decoder_p self,encoder_p encoder_instance){
    self->shift_matirx_p=encoder_instance->shift_p;
    self->decoding_data_matrix_p=encoder_instance->encoding_data_p;
    printf("shift_matrix%d,%d\ndecoding_matrix%d,%d\n",self->shift_matirx_p->k,self->shift_matirx_p->n,self->decoding_data_matrix_p->col_size,self->decoding_data_matrix_p->row_size);
    return RETURN_SUCCESS;
}
