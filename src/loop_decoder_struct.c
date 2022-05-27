/*******************************************************************
 * @Author: qqyk
 * @version: qqyk_01
 * @Date: 2022-05-26 13:05
 * @LastEditTime: Do not Edit
 * @brief  : 
 * @return  {*}
*******************************************************************/
#include "loop_decoder_struct.h"

/******node方法声明**************/
static INT  set(queue_node_p self,UINT i,UINT j);
static INT  link_head(queue_node_p self,queue_node_p pre);
static INT  link_tail(queue_node_p self,queue_node_p tail);
static INT  break_pre(queue_node_p self);
static INT  break_tail(queue_node_p self);
static is_head check_head(queue_node_p self);
static is_tail check_tail(queue_node_p self);
/***************queue方法声明***************/
static INT append(queue_p self,UINT i,UINT j);
static INT remove(queue_p self,queue_node_p node_2_remove);
static is_empty empty(queue_p self);
static INT __free_head(queue_p self);
static INT __free_tail(queue_p self);
/***************seq_set方法声明*****************/
static is_in num_is_in(seq_set_p self,UINT number);
static is_empty set_is_empty(seq_set_p self);
static INT pop(seq_set_p self,UINT number);
static INT add(seq_set_p self,UINT number);

/***********queue_node_p初始化和释放********/
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
    //free 自身。
    free(self);

}
/********queue_p 初始化和释放***********/
INT init_queue(queue_p self){
    //创建head。创建tail。把head和tail连上
    queue_node_p head=(queue_node_p)malloc(sizeof(queue_node));
    queue_node_p tail=(queue_node_p)malloc(sizeof(queue_node));
    //初始化head 和tail
    init_node(head);
    init_node(tail);
    //连接head和tail
    head->link_tail(head,tail);

    return RETURN_SUCCESS;
}
INT destroy_queue(queue_p self){
    //释放所有的node。
    while(self->is_empty(self)==EMPTY){
        self->remove(self,self->head->next);
    }
    __free_head(self);
    __free_tail(self);
    return RETURN_SUCCESS;
}
/*********************seq_set初始化和释放***************/
INT init_seq_set(seq_set_p self){
    //flag初始化为0
    memset(self->flag,0,sizeof(self->flag));
    self->count=0;
    //方法挂载
    self->pop=pop;
    self->add=add;
    self->num_is_in=num_is_in;
    self->set_is_empty=set_is_empty;
    

}
INT destroy_seq_set(seq_set_p self){}

/***********queue_node 方法主体部分********/
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
    //断开自身连接和下一个的连接。
    self->next->pre=NULL;
    self->next=NULL;
    return RETURN_SUCCESS;
}
static is_head check_head(queue_node_p self){
     if(self->pre==NULL && self->next!=NULL){
        return HEAD;
    }
    //is tail 的定义为，pre指向

    return UHEAD;
}
static is_tail check_tail(queue_node_p self){
    if(self->pre!=NULL && self->next==NULL){
        return TAIL;
    }
    //is tail 的定义为，pre指向

    return UTAIL;
}



/***********queue方法主体*****************/
static INT append(queue_p self,UINT i,UINT j){

    queue_node_p node_2_insert=(queue_node_p)malloc((sizeof(queue_node)));
    init_node(node_2_insert);
    node_2_insert->set(node_2_insert,i,j);


    queue_node_p last_node_p=NULL;
    last_node_p=self->tail->pre;
    //断开tail的pre
    self->tail->break_pre(self->tail); 
    //就是连接node_2_insert 的pre和last_node_p
    node_2_insert->link_head(node_2_insert,last_node_p);
    //连接node_2_insert 的tail和self->tail
    node_2_insert->link_tail(node_2_insert,self->tail);
    return RETURN_SUCCESS;

}
static INT remove(queue_p self,queue_node_p node_2_remove){
    //从某一个位置剪开。
    queue_node_p node_2_remove_pre=NULL;
    queue_node_p node_2_remove_next=NULL;
    //
    node_2_remove_next=node_2_remove->next;
    node_2_remove_pre=node_2_remove_pre;
    //断开当前的pre和tail，
    node_2_remove->break_pre(node_2_remove);
    node_2_remove->break_tail(node_2_remove);
    //连接node_2_remove_pre和node_2_remove_next
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
    //主要是self->head=NULL;
    self->head=NULL;
}
static INT __free_tail(queue_p self){
    //主要是self->tail=NULL;
    destroy_node(self->tail);
    self->tail=NULL;
}
/******seq_set主题部分*******/
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
    //判断对应的数据位为1
    return __num_is_in(self,number);
}
static is_empty set_is_empty(seq_set_p self){
    if(self->count==0){
        return EMPTY;
    }
    return UEMPTY;
}
static INT pop(seq_set_p self,UINT number){
    //判断对应位置是不是1，0就不做任何改变
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
    //判断对应位置是不是1，1就不做任何改变，
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


