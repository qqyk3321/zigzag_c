/*******************************************************************
 * @Descripttion   : ���룬i��j��Ϊ�˺������ж�Ӧ��ȫ��ʹ��i��ʾ�������ݰ���š�
 *                      j��ʾԭʼ���ݰ���š�
 * @version        : qqyk_01
 * @Author         : qqyk
 * @Date           : 2022-05-25 09:44
 * @LastEditTime   : 2022-05-26 10:52
 * @Copyright (c) 2022 by qqyk/Zigzag, All Rights Reserved. 
*******************************************************************/
#include "decoder.h"
#include "include.h"
/************������������*************/



static INT show(decoder_p self);//��ӡ���󣬴�ӡshift��������ɵľ��󡣴�ӡphi��

static INT decode(decoder_p self,zigzag_method method);

/*************decodeʹ�õĺ���************/
static INT __ori_zigzag_decode(decoder_p self);
static INT __greedy_zigzag_decode(decoder_p self);
static INT __zigzag_finished_decode(decoder_p self);//�����ж��Ƿ����ж�����˽���//�Ƿ���Ҫһ��ѭ�����нṹ��ʵ��������顣
static INT __zigzag_j_finished_decode(decoder_p self,UINT j);//�����ж��Ƿ񵥸�����˽��롣//�����ж��Ƿ�����˵������롣
/**********������õ��Ĺ�������***********/
static INT __recover(decoder_p self,UINT i,UINT j,UINT length);
static INT __get_encoding_message_pos(decoder_p self,UINT i,UINT j,VOID ** pos);
static INT __update_decoding_message(decoder_p self,UINT j,VOID **pos,UINT length);
static INT __updata_encoding_message(decoder_p self,UINT i,UINT j,VOID **pos ,UINT length);

static INT __update_R(decoder_p self,UINT i,UINT j,UINT length);
/*********ori_zigzag��ط�������*****/




     
//��ʼ�����
static INT __load_encoding_message_from_encoder(decoder_p self,encoder_p encoder_instance);


/********ʵ�����ͷ�************/   
INT init_decoder_from_encoder(decoder_p self,int n, int k,int L,int shift_max,encoder_p encoder_instance){
    //��encoder_instance �����벢��ֵ��
    self->n=n;
    self->k=k;
    self->L=L;
    self->shift_max=shift_max;
    //������ֵ
    self->decode=decode;
    self->show=show;
    //��Ҫ������Ƭ�ռ�
    //����ori_data�ռ�
    self->ori_data_matrix_p=(ori_data_p)malloc(sizeof(ori_data));
    init_ori_data(self->ori_data_matrix_p,k,L);
    //memset(self->ori_data_matrix_p,0,sizeof(UCHAR)*k*L);
    //����phi�ռ�
    //self->phi_p=(UINT *)malloc(sizeof(UINT)*k);
    self->phi_p=(matrix_p)malloc(sizeof(matrix));
    init_matrix(self->phi_p,k,1,"uint");

    __load_encoding_message_from_encoder(self,encoder_instance);
    return RETURN_SUCCESS;
    
} 
INT destroy_decoder(decoder_p self){
    //��ʱ������
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

/***********���ⲿ��******************/
static INT decode(decoder_p self,zigzag_method method){
    //����ѡ��
    //�����ʼ��
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
    //����������i��ʾ�������ݰ���ţ�j��ʾԭʼ���ݰ����
    /* ������İɻ���~
    *   ��Ҫע����ǣ����￼���˽���ʧ�ܵ�����������ж����б������ݰ��������ڿɽ�����λ��             
    *    matrix�������ɵ�ʱ���Ѿ���ʽ������
    *    �����phiʵ��������û��˼����ȫ�����ﶨ��Ϊk*1�ľ���         
    */ 
   
   
    UINT i;//��ʾ�������ݰ���ţ�������һ��(�е�ʱ����index_encoding)
    //UINT j;//��ʾԭʼ���ݰ���ţ�������һ��(�е�ʱ����index_source)
    bit_decoded if_decoded=DECODED;//����˵���Ƿ��б���������ݰ��������롣���bit_decoded��Ϊ0����û��ҪŬ��~
    while(__zigzag_finished_decode(self)==UNFINISHED && if_decoded==DECODED){
        if_decoded=UNDECODED;
        for(i=0;i<self->k;i++){
            //��β��־���������������ô�ܲ����Ѿ��ж���ϵ�message���в�����
            UINT min_1_index_source;//��С��
            UINT min_2_index_source;//�ڶ�С��
            self->shift_matirx_p->shift_row_min(self->shift_matirx_p,i,&min_1_index_source,&min_2_index_source);
            //

            //�ж���С������Ԫ�صĴ�С��
            VOID ** min_1_index_source_pos=malloc(sizeof(UCHAR *));
            VOID ** min_2_index_source_pos=malloc(sizeof(UCHAR *));
            self->shift_matirx_p->get_shift_member(self->shift_matirx_p,i,min_1_index_source,min_1_index_source_pos);
            self->shift_matirx_p->get_shift_member(self->shift_matirx_p,i,min_2_index_source,min_2_index_source_pos);
            
            //message_decodeble decodeble;
            //��Ҫ�ж��ǲ��Ǹ���С��λ��Ӧ��ԭʼ���ݰ��Ѿ��ָ���ȫ������Ѿ��ָ���ȫҲ��������Ҫ�ָ���
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
                //������̷Ŵ�����
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
    //����������i��ʾ�������ݰ���ţ�j��ʾԭʼ���ݰ����
    /* ������İɻ���~
    *   ��Ҫע����ǣ����￼���˽���ʧ�ܵ�����������ж����б������ݰ��������ڿɽ�����λ��             
    *    matrix�������ɵ�ʱ���Ѿ���ʽ������
    *    �����phiʵ��������û��˼����ȫ�����ﶨ��Ϊk*1�ľ���          
    */ 
  
   
    UINT i;//��ʾ�������ݰ���ţ�������һ��(�е�ʱ����index_encoding)
    //UINT j;//��ʾԭʼ���ݰ���ţ�������һ��(�е�ʱ����index_source)
    bit_decoded if_decoded=DECODED;//����˵���Ƿ��б���������ݰ��������롣���bit_decoded��Ϊ0����û��ҪŬ��~
    while(__zigzag_finished_decode(self)==UNFINISHED && if_decoded==DECODED){
        if_decoded=UNDECODED;
        for(i=0;i<self->k;i++){
            //��β��־���������������ô�ܲ����Ѿ��ж���ϵ�message���в�����
            UINT min_1_index_source;//��С��
            UINT min_2_index_source;//�ڶ�С��
            self->shift_matirx_p->shift_row_min(self->shift_matirx_p,i,&min_1_index_source,&min_2_index_source);
            //

            //�ж���С������Ԫ�صĴ�С��
            VOID ** min_1_index_source_pos=malloc(sizeof(UCHAR *));
            VOID ** min_2_index_source_pos=malloc(sizeof(UCHAR *));
            self->shift_matirx_p->get_shift_member(self->shift_matirx_p,i,min_1_index_source,min_1_index_source_pos);
            self->shift_matirx_p->get_shift_member(self->shift_matirx_p,i,min_2_index_source,min_2_index_source_pos);
            
            //message_decodeble decodeble;
            //��Ҫ�ж��ǲ��Ǹ���С��λ��Ӧ��ԭʼ���ݰ��Ѿ��ָ���ȫ������Ѿ��ָ���ȫҲ��������Ҫ�ָ���
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
                //������̷Ŵ�����
                //��ͬ��ORI�ĵط���������Ҫ�ж�length�Ĵ�С��
                //length�Ĵ�СΪ min((min2-min1)��L-phi[min_pos])
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
 * @brief  : �ж��Ƿ�������еĽ���
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




/*****************__recover  ���ʵ��*********************/
static INT __recover(decoder_p self,UINT i,UINT j,UINT length){
    VOID **encoding_message_pos=malloc(sizeof(UCHAR *));//ָ��ĳһ�����ݵ�ָ�룬

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

    /***���phi_pos***/
    self->phi_p->get_member(self->phi_p,j,0,phi_pos);
    
    //self->ori_data_matrix_p
    //���ori_data_matrix��Ӧ��λ�á�
    self->ori_data_matrix_p->get_ori_data_member(self->ori_data_matrix_p,j,*(UINT*)*phi_pos,ori_data_pos);
    //��encoding_message�����Ӧ�����ݣ��ͳ��� ������ori_data_pos�С�
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
            //��ȡ R_(index,j)��ƫ����Ϣ��
            self->shift_matirx_p->get_shift_member(self->shift_matirx_p,index,j,shift_pos);
            self->decoding_data_matrix_p->get_member(self->decoding_data_matrix_p,index,*(UINT*)*shift_pos,encoding_other_message_pos);
            //��ȡ ��Ӧλ����Ϣ��Ȼ���Ӧ���
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
    //��������ʹ�õĶ�����
    //shift �ĵ�i�м�1.ʵ���ϲ�û��ʹ�õ�j��
    self->shift_matirx_p->shift_col_add(self->shift_matirx_p,j,length);
    return RETURN_SUCCESS;
}

/***********showʵ��*****************/
static INT show(decoder_p self){
    //��ӡshift����
    self->shift_matirx_p->show_shift(self->shift_matirx_p);
    //��ӡphi��
    self->phi_p->show(self->phi_p,"phi");
    //��ӡencoding_data_matrix
    self->decoding_data_matrix_p->show(self->decoding_data_matrix_p,"decoding_message");
    //��ӡori_data
    self->ori_data_matrix_p->show_ori_data(self->ori_data_matrix_p);
    
    return RETURN_SUCCESS;
}
static INT __load_encoding_message_from_encoder(decoder_p self,encoder_p encoder_instance){
    self->shift_matirx_p=encoder_instance->shift_p;
    self->decoding_data_matrix_p=encoder_instance->encoding_data_p;
    printf("shift_matrix%d,%d\ndecoding_matrix%d,%d\n",self->shift_matirx_p->k,self->shift_matirx_p->n,self->decoding_data_matrix_p->col_size,self->decoding_data_matrix_p->row_size);
    return RETURN_SUCCESS;
}
