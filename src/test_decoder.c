/*******************************************************************
 * @Descripttion   : 
 * @version        : qqyk_01
 * @Author         : qqyk
 * @Date           : 2022-05-25 15:18
 * @LastEditTime   : 2022-05-26 10:08
 * @Copyright (c) 2022 by qqyk/Zigzag, All Rights Reserved. 
*******************************************************************/
#include "include.h"
#include "decoder.h"
INT main(){


    
   
    //ori_data * ori_data_p=(ori_data *)malloc(sizeof(ori_data));
    encoder_p test_encoder_p=(encoder_p)malloc(sizeof(encoder));
    decoder_p test_decoder_p=(decoder_p)malloc(sizeof(decoder));
    UINT times_total=3;
    UINT k=2;
    UINT n=2;
    UINT L=20;
    UINT shift_max=3;
    init_encoder(test_encoder_p,k,n,L,shift_max);
    init_decoder_from_encoder(test_decoder_p,n,k,L,shift_max,test_encoder_p);
    UINT times=0;
    //使用clock_t 时间
    clock_t start_time;
    clock_t end_time;
    clock_t totol_time=0;
    //clock_t main_start_time=clock();
    
    double duration;
    
    //打开三个文件，文件名+尾缀为time()返回值。
    /*
    FILE * ori_data_f;
    FILE * encoding_data_f;
    FILE * shift_f;
    
    
    
    ori_data_f=fopen("ori_data","w");
    encoding_data_f=fopen("encoding_data","w");
    shift_f=fopen("shift","w");
    */
    //
    for (times=0;times<times_total;times++){
        start_time=clock();
        test_encoder_p->generate_shift(test_encoder_p);
        test_encoder_p->generate_ori_data(test_encoder_p);
        
        test_encoder_p->encode(test_encoder_p);
        end_time=clock();
        totol_time+=(end_time-start_time);
        //INFO_LOG("show_after_encoder");
        //show encoder
        //test_encoder_p->show_encoding_data(test_encoder_p);
        //test_encoder_p->show_ori_data(test_encoder_p);
        //test_encoder_p->show_shift(test_encoder_p);
        //all_finished finished;
        //INFO_LOG("show_befor_decoder");
        //test_decoder_p->show(test_decoder_p);
        if(test_decoder_p->decode(test_decoder_p,GREEDY_ZIGZAG)==FINISHED){
            INFO_LOG("finished_decoder");
            
            if(check_decoding_correct(test_decoder_p,test_encoder_p)==DECODE_SUCCESS){
                INFO_LOG("decoding suceess");
            }
            else{
                INFO_LOG("some thing is wrong decoding ");
                sleep(5);
                test_decoder_p->show(test_decoder_p);
                sleep(5);
            }
        }
        else{
            INFO_LOG("shift_matrix is not decodable");
        }
        
        printf("times have been finished %d\n",times);
        //message.print_encoding_data(&message);
        /*fwrite(message.ori_data->data_p,sizeof(unsigned char),message.k*message.L,ori_data_f);
        fflush(ori_data_f);
        fwrite(message.shift_matrix->shift_p,sizeof(unsigned char),message.k*message.n,shift_f);
        fflush(ori_data_f);
        fwrite(message.encoding_data_p,sizeof(unsigned char),message.n*(message.L+message.shift_max),encoding_data_f);
        fflush(ori_data_f);
        */
        
    }
    printf("duration_encoding is %f seconds\n",duration=(double)(totol_time)/CLOCKS_PER_SEC);
    
    //message.print_encoding_data(&message);
    destroy_encoder(test_encoder_p);
    
    //printf("duration is %f seconds\n",duration=(double)(clock()-main_start_time)/CLOCKS_PER_SEC);
    //关闭三个文件符
    
    return 0;
}
