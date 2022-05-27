/*******************************************************************
 * @Descripttion   : 
 * @version        : qqyk_01
 * @Author         : qqyk
 * @Date           : 2022-05-23 15:13
 * @LastEditTime   : 2022-05-23 15:48
 * @Copyright (c) 2022 by qqyk/Zigzag, All Rights Reserved. 
*******************************************************************/
#include "include.h"
#include "encoder.h"
INT main()
{
    
  
    
   
    //ori_data * ori_data_p=(ori_data *)malloc(sizeof(ori_data));
    encoder_p test_encoder_p=(encoder_p)malloc(sizeof(encoder));
    UINT times_total=1000;
    UINT k=50;
    UINT n=50;
    UINT L=1024;
    UINT shift_max=256;
    init_encoder(test_encoder_p,k,n,L,shift_max);
    
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