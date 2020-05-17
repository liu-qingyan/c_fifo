/** 
 * @brief       fifo 功能的实现,模仿Linux kfifo
 * @param
 * @return
 * @note        https://www.cnblogs.com/wangguchangqing/p/6070286.html
 * @author      Liu
 * @version     v0.0.1 
 */
#ifndef _FIFO_H_
#define _FIFO_H_

#include <stdint.h>

typedef enum {
    un_lock = 0,
    write_lock = 1,
    read_lock = 2,
    locked = 3,
}lock_t;

typedef struct{
    lock_t          lock;
    uint8_t*        pdata;
    uint32_t        fifo_size;
    uint32_t        write_index;
    uint32_t        read_index;
}fifo_t;

#endif