/*******************************************************************
 * @Descripttion   : 
 * @version        : qqyk_01
 * @Author         : qqyk
 * @Date           : 2022-05-18 15:54
 * @LastEditTime   : 2022-05-18 15:56
 * @Copyright (c) 2022 by qqyk/X1000, All Rights Reserved. 
*******************************************************************/
#include "need_include.h"
#include "struct.h"
int main()
{
    
  
    
   
    //ori_data * ori_data_p=(ori_data *)malloc(sizeof(ori_data));
    struct SHIFT_MATRIX message;
    
    init_shift(&message,3,4,3);
  
    message.generate_shift(&message);
    message.print_shift(&message);
    message.close_shift(&message);
    return 0;
}