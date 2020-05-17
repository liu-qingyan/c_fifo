/** 
 * @brief       fifo 功能的实现,模仿Linux kfifo
 * @param
 * @return
 * @note        https://www.cnblogs.com/wangguchangqing/p/6070286.html
 * @author      Liu
 * @version     v0.0.1 
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "fifo.h"

#define FIFO_MIN(a,b) ((a)<(b)?(a):(b))
#define FIFO_MAX(a,b) ((a)>(b)?(a):(b))

#define BUF_SIZE_MASK(size)                     (size - 1)
#define CALC_POSITION_IN_FIFO(index,size)       (index & BUF_SIZE_MASK(size))
#define IS_POWER_OF_2(size)                     ((size & (size - 1)) == 0)

//#define _OPEN_FIFO_LOG_
#ifdef  _OPEN_FIFO_LOG_
    #define FIFO_LOG(format, arg...)   printf(format, ##arg)
#else
    #define FIFO_LOG(format, ...)
#endif

/** 
 * @brief       fifo初始化
 * @param       *fifo:      fifo结构体指针
 *              buf_size:   fifo数组大小,必须为2的幂次
 *              buff:       fifo数组
 * @return      0: init success
 *              1: init failed
 */
int8_t  fifo_init(fifo_t* fifo, uint32_t buf_size, uint8_t* buff)
{
    if(IS_POWER_OF_2(buf_size) && buf_size > 0)
    {
        fifo->lock = un_lock;
        fifo->pdata = buff;
        fifo->fifo_size = buf_size;
        fifo->read_index = 0;
        fifo->write_index = 0;
        FIFO_LOG("fifo init success! fifo size is %d\n",fifo->fifo_size);
        return 0;
    }
    else
    {
        FIFO_LOG("fifo init failed! fifo size must be a power of 2 and not equal to 0\n");
        return -1;
    }
}

/** 
 * @brief       计算fifo已使用空间
 * @param       *fifo:  fifo结构体指针
 * @return      fifo已使用空间大小
 */
uint32_t fifo_used_size(fifo_t* fifo)
{
    return (fifo->write_index - fifo->read_index);
}

/** 
 * @brief       计算fifo剩余空间
 * @param       *fifo:  fifo结构体指针
 * @return      fifo剩余空间大小
 */
uint32_t fifo_remain_size(fifo_t* fifo)
{
    return (fifo->fifo_size - fifo_used_size(fifo));
}

/** 
 * @brief       计算fifo是否为空
 * @param       *fifo:  fifo结构体指针
 * @return      0:  fifo非空
 *              1:  fifo为空
 */
uint8_t fifo_is_empty(fifo_t* fifo)
{
    return (fifo->write_index == fifo->read_index);
}

/** 
 * @brief       计算fifo是否为满
 * @param       *fifo:  fifo结构体指针
 * @return      0:  fifo非满
 *              1:  fifo为满
 */
uint8_t fifo_is_full(fifo_t* fifo)
{
    return (fifo->fifo_size == fifo_used_size(fifo));
}

/** 
 * @brief       将指定长度数据推入fifo
 * @param       *fifo:      fifo结构体指针
 *              *p_data:    准备推入fifo的数据起始地址
 *              *len:       准备推入fifo的数据长度
 * @return      push_len:   实际推入fifo的长度
 *              -1:         数据推入fifo失败
 */
int32_t  push_data_to_fifo(fifo_t* fifo, uint8_t* p_data, uint32_t len)
{
    if(fifo->lock == un_lock && p_data != NULL && !fifo_is_full(fifo))
    {
        fifo->lock = locked;

        uint32_t push_len = FIFO_MIN(fifo_remain_size(fifo),len);
        uint32_t l = FIFO_MIN(push_len,(fifo->fifo_size - CALC_POSITION_IN_FIFO(fifo->write_index,fifo->fifo_size)));
        memcpy(fifo->pdata + CALC_POSITION_IN_FIFO(fifo->write_index,fifo->fifo_size),p_data,l);
        memcpy(fifo->pdata,p_data + l,push_len - l);

        fifo->write_index += push_len;
        fifo->lock = un_lock;
        FIFO_LOG("ready to push len is %d,actual push len is %d\n",len,push_len);
        return push_len;
    }
    else
    {   
        FIFO_LOG("push data failed!\n");
        return -1;
    }
}

/** 
 * @brief       从fifo中取出指定长度数据
 * @param       *fifo:      fifo结构体指针
 *              *p_data:    用于储存从fifo中读出数据的起始地址
 *              *len:       准备从fifo中读出的数据长度
 * @return      pull_len:   实际从fifo中读出的长度
 *              -1:         从fifo读取数据失败
 */
int32_t pull_data_from_fifo(fifo_t* fifo, uint8_t* p_data, uint32_t len)
{
    if(fifo->lock == un_lock && p_data != NULL && !fifo_is_empty(fifo))
    {
        fifo->lock = locked;

        uint32_t pull_len = FIFO_MIN(fifo_used_size(fifo),len);
        uint32_t l = FIFO_MIN(pull_len,(fifo->fifo_size - CALC_POSITION_IN_FIFO(fifo->read_index,fifo->fifo_size)));
        memcpy(p_data,fifo->pdata + CALC_POSITION_IN_FIFO(fifo->read_index,fifo->fifo_size),l);
        memcpy(p_data + l,fifo->pdata,pull_len - l);
        
        fifo->read_index += pull_len;
        fifo->lock = un_lock;
        FIFO_LOG("ready to pull len is %d,actual pull len is %d\n",len,pull_len);
        return pull_len;
    }
    else
    {
        FIFO_LOG("pull data failed!\n");
        return -1;
    }
}

