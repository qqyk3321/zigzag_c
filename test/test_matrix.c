/*
 * @Author: taikongren0723@163.com taikongren0723@163.com
 * @Date: 2022-05-22 19:11:32
 * @LastEditors: taikongren0723@163.com taikongren0723@163.com
 * @LastEditTime: 2022-05-22 21:54:20
 * @FilePath: /zigzag_c/test_matrix.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "include.h"
#include "matrix.h"
int main(){
    matrix_p mm;
    mm=(matrix_p)malloc(sizeof(matrix));
    init_matrix(mm,10,10,"uint");
    mm->random_generate(mm,300);
    mm->show(mm,"title");
    void ** pos=malloc(sizeof(UINT *));
    
   
    UINT row;
    UINT *pos_1=(UINT*)malloc(sizeof(UINT));
    UINT *pos_2=(UINT*)malloc(sizeof(UINT));
    for (row=0;row<mm->row_size;row++){
        mm->row_min(mm,row,pos_1,pos_2);

        //INFO_LOG("row_min_finished");
        mm->get_member(mm,row,*pos_1,pos);
        printf("row:%d,col:%d is min =%d\n,",row,*pos_1,*((UINT *)*pos));
        mm->get_member(mm,row,*pos_2,pos);
        printf("row:%d,col=%d is min_2 =%d\n,",row,*pos_2,*((UINT *)*pos));
    }
    destroy_matrix(mm);
    matrix_p tt;
    tt=(matrix_p)malloc(sizeof(matrix));
    init_matrix(tt,10,10,"uchar");
    tt->random_generate(tt,200);
    tt->show(tt,"title");
    
    for (row=0;row<tt->row_size;row++){
        tt->row_min(tt,row,pos_1,pos_2);

        //INFO_LOG("row_min_finished");
        tt->get_member(tt,row,*pos_1,pos);
        printf("row:%d,col:%d is min =%02x\n,",row,*pos_1,*((UCHAR *)*pos));
        tt->get_member(tt,row,*pos_2,pos);
        printf("row:%d,col=%d is min_2 =%02x\n,",row,*pos_2,*((UCHAR *)*pos));
    }
    destroy_matrix(tt);
    return 0;
}