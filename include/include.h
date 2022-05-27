/*
 * @Author: taikongren0723@163.com taikongren0723@163.com
 * @Date: 2022-05-21 16:41:36
 * @LastEditors: taikongren0723@163.com taikongren0723@163.com
 * @LastEditTime: 2022-05-22 20:57:28
 * @FilePath: /zigzag_c/inculde.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __INCLUDE_H__ 
#define __INCLUDE_H__
//******************************公共include*************************/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <libconfig.h>
#include <string.h>
#include <unistd.h>


/****************************数据类型预定�?*************************/

typedef  int INT;
typedef  char CHAR;
typedef  unsigned char UCHAR;
typedef  unsigned int UINT;
typedef  void VOID;
/**************************print 封装�?********************************/
#define INFO_SWITCH 1
/*封装printf 方便后期整改和消�?*/
#if INFO_SWITCH
    #define INFO_LOG(format, ...) \
    do{ \
        printf("\r\n"); \
        printf("FUNCTION: %s, LINE: %05d: "format,__FUNCTION__,__LINE__,##__VA_ARGS__); \
        printf("\r\n"); \
    } while (0)
#else
    #define INFO_LOG(format, ...)
#endif



/*************************error封装�?**********************************/
#define INFO_ERROR 1
/*封装printf 方便后期错误打印消除*/
#if INFO_ERROR
    #define INFO_ERR(format, ...) \
    do{ \
        printf("\r\n"); \
        printf("FUNCTION: %s, LINE: %05d: "format,__FUNCTION__,__LINE__,##__VA_ARGS__); \
        printf("\r\n"); \
    } while (0)
#else
    #define INFO_ERR(format, ...)
#endif	
/*************************枚举类型************************************/
enum RETURN_VALUE
{
    RETURN_SUCCESS = 0,       /* 返回成功 */
	RETURN_FAILED = -1,		      /* 返回错误 */
	
};

/*********??????*********************/
#define MIN(a,b) ((a)<(b)?(a):(b))


#endif