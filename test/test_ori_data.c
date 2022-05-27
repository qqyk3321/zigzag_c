/*******************************************************************
 * @Descripttion   : 
 * @version        : qqyk_01
 * @Author         : qqyk
 * @Date           : 2022-05-18 13:45
 * @LastEditTime   : 2022-05-18 15:41
 * @Copyright (c) 2022 by qqyk/X1000, All Rights Reserved. 
*******************************************************************/
#include "need_include.h"
#include "struct.h"
int main()
{
    
  
    
   
    //ori_data * ori_data_p=(ori_data *)malloc(sizeof(ori_data));
    struct ORI_DATA message;
    
    init_ori_data(&message,3,3);
  
    message.generate_ori_data(&message);
    message.print_ori_data(&message);
    message.close_ori_data(&message);
    return 0;
}