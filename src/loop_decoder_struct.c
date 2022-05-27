/*******************************************************************
 * @Author: qqyk
 * @version: qqyk_01
 * @Date: 2022-05-26 13:05
 * @LastEditTime: Do not Edit
 * @brief  : 
 * @return  {*}
*******************************************************************/
#include "loop_decoder_struct.h"

/******node��������**************/
static INT  set(queue_node_p self,UINT i,UINT j);
static INT  link_head(queue_node_p self,queue_node_p pre);
static INT  link_tail(queue_node_p self,queue_node_p tail);
static INT  break_pre(queue_node_p self);
static INT  break_tail(queue_node_p self);
static is_head check_head(queue_node_p self);
static is_tail check_tail(queue_node_p self);
/***************queue��������***************/
static INT append(queue_p self,UINT i,UINT j);
static INT remove(queue_p self,queue_node_p node_2_remove);
static is_empty empty(queue_p self);
static INT __free_head(queue_p self);
static INT __free_tail(queue_p self);
/***************seq_set��������*****************/
static is_in num_is_in(seq_set_p self,UINT number);
static is_empty set_is_empty(seq_set_p self);
static INT pop(seq_set_p self,UINT number);
static INT add(seq_set_p self,UINT number);

/***********queue_node_p��ʼ�����ͷ�********/
INT init_node(queue_node_p self){
    
    self->pre=NULL;
    self->next=NULL;
    
    self->break_pre=break_pre;
    self->break_tail=break_tail;
    self->check_head=check_head;
    self->check_tail=check_tail;
    self->link_head=link_head;
    self->link_tail=link_tail;
    self->set=set;

}
INT destroy_node(queue_node_p self){
    //free ����
    free(self);

}
/********queue_p ��ʼ�����ͷ�***********/
INT init_queue(queue_p self){
    //����head������tail����head��tail����
    queue_node_p head=(queue_node_p)malloc(sizeof(queue_node));
    queue_node_p tail=(queue_node_p)malloc(sizeof(queue_node));
    //��ʼ��head ��tail
    init_node(head);
    init_node(tail);
    //����head��tail
    head->link_tail(head,tail);

    return RETURN_SUCCESS;
}
INT destroy_queue(queue_p self){
    //�ͷ����е�node��
    while(self->is_empty(self)==EMPTY){
        self->remove(self,self->head->next);
    }
    __free_head(self);
    __free_tail(self);
    return RETURN_SUCCESS;
}
/*********************seq_set��ʼ�����ͷ�***************/
INT init_seq_set(seq_set_p self){
    //flag��ʼ��Ϊ0
    memset(self->flag,0,sizeof(self->flag));
    self->count=0;
    //��������
    self->pop=pop;
    self->add=add;
    self->num_is_in=num_is_in;
    self->set_is_empty=set_is_empty;
    

}
INT destroy_seq_set(seq_set_p self){}

/***********queue_node �������岿��********/
static INT  set(queue_node_p self,UINT i,UINT j){
    self->i=i;
    self->j=j;
    return RETURN_SUCCESS;
}
static INT  link_head(queue_node_p self,queue_node_p head){
    self->pre=head;
    head->next=self;

    return RETURN_SUCCESS;
}
static INT  link_tail(queue_node_p self,queue_node_p tail){
    self->next=tail;
    tail->pre=self;
    return RETURN_SUCCESS;
}
static INT  break_pre(queue_node_p self){
    self->pre->next=NULL;
    self->pre=NULL;
    return RETURN_SUCCESS;
}
static INT  break_tail(queue_node_p self){
    //�Ͽ��������Ӻ���һ�������ӡ�
    self->next->pre=NULL;
    self->next=NULL;
    return RETURN_SUCCESS;
}
static is_head check_head(queue_node_p self){
     if(self->pre==NULL && self->next!=NULL){
        return HEAD;
    }
    //is tail �Ķ���Ϊ��preָ��

    return UHEAD;
}
static is_tail check_tail(queue_node_p self){
    if(self->pre!=NULL && self->next==NULL){
        return TAIL;
    }
    //is tail �Ķ���Ϊ��preָ��

    return UTAIL;
}



/***********queue��������*****************/
static INT append(queue_p self,UINT i,UINT j){

    queue_node_p node_2_insert=(queue_node_p)malloc((sizeof(queue_node)));
    init_node(node_2_insert);
    node_2_insert->set(node_2_insert,i,j);


    queue_node_p last_node_p=NULL;
    last_node_p=self->tail->pre;
    //�Ͽ�tail��pre
    self->tail->break_pre(self->tail); 
    //��������node_2_insert ��pre��last_node_p
    node_2_insert->link_head(node_2_insert,last_node_p);
    //����node_2_insert ��tail��self->tail
    node_2_insert->link_tail(node_2_insert,self->tail);
    return RETURN_SUCCESS;

}
static INT remove(queue_p self,queue_node_p node_2_remove){
    //��ĳһ��λ�ü�����
    queue_node_p node_2_remove_pre=NULL;
    queue_node_p node_2_remove_next=NULL;
    //
    node_2_remove_next=node_2_remove->next;
    node_2_remove_pre=node_2_remove_pre;
    //�Ͽ���ǰ��pre��tail��
    node_2_remove->break_pre(node_2_remove);
    node_2_remove->break_tail(node_2_remove);
    //����node_2_remove_pre��node_2_remove_next
    node_2_remove_next->link_head(node_2_remove_next,node_2_remove_pre);

    destroy_node(node_2_remove);
    return RETURN_SUCCESS;


}
static is_empty empty(queue_p self){
    if(self->head->next==self->tail->pre){
        return EMPTY;
    }
    return UEMPTY;
}
static INT __free_head(queue_p self){
    destroy_node(self->head);
    //��Ҫ��self->head=NULL;
    self->head=NULL;
}
static INT __free_tail(queue_p self){
    //��Ҫ��self->tail=NULL;
    destroy_node(self->tail);
    self->tail=NULL;
}
/******seq_set���ⲿ��*******/
static is_in __num_is_in(seq_set_p self,UINT number){
    UINT* flag_pos=self->flag+(number/8);
    UINT bit_pos=number%8;
    if (*(flag_pos)&(1<<bit_pos)){
        return IN;
    }
    else{
        return UIN;
    }
}
static is_in num_is_in(seq_set_p self,UINT number){
    //�ж϶�Ӧ������λΪ1
    return __num_is_in(self,number);
}
static is_empty set_is_empty(seq_set_p self){
    if(self->count==0){
        return EMPTY;
    }
    return UEMPTY;
}
static INT pop(seq_set_p self,UINT number){
    //�ж϶�Ӧλ���ǲ���1��0�Ͳ����κθı�
    if(__num_is_in(self,number)==IN){
        UINT* flag_pos=self->flag+(number/8);
        UINT bit_pos=number%8;
        *(flag_pos)=*(flag_pos)&(~1<<bit_pos);
        self->count-=1;
        return RETURN_SUCCESS;
    }
    else{
        return RETURN_SUCCESS;
    }
}
static INT add(seq_set_p self,UINT number){
    //�ж϶�Ӧλ���ǲ���1��1�Ͳ����κθı䣬
    if(__num_is_in(self,number)==UIN){
        UINT* flag_pos=self->flag+(number/8);
        UINT bit_pos=number%8;
        *(flag_pos)=*(flag_pos)|1<<bit_pos;
        self->count+=1;
        return RETURN_SUCCESS;
    }
    else{

        return RETURN_SUCCESS;
    }
}


