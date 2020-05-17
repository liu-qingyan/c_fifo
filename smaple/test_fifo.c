#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "../fifo/fifo.h"

int main(void)
{
    fifo_t test_fifo;
    uint8_t buffer[64] = {0};

    fifo_init(&test_fifo,64,buffer);
    printf("fifo size = %d\n",test_fifo.fifo_size);

    for(uint8_t i = 0; i < 30; i++)
    {
        push_data_to_fifo(&test_fifo,&i,1);
    }
    printf("now use size is %d, remain size is %d\n",fifo_used_size(&test_fifo),fifo_remain_size(&test_fifo));
    
    uint8_t temp_data = 0;
    while(pull_data_from_fifo(&test_fifo,&temp_data,1) >= 0)
    {
        printf("read data is %d\n",temp_data);
    }

    for(uint8_t i = 30; i < 80; i++)
    {
        push_data_to_fifo(&test_fifo,&i,1);
    }
    printf("now use size is %d, remain size is %d\n",fifo_used_size(&test_fifo),fifo_remain_size(&test_fifo));

    while(pull_data_from_fifo(&test_fifo,&temp_data,1) >= 0)
    {
        printf("read data is %d\n",temp_data);
    }
    printf("now use size is %d, remain size is %d\n",fifo_used_size(&test_fifo),fifo_remain_size(&test_fifo));

    uint8_t temp[158] = {0,5,4,68,1,2,88,22,5,95,8,5,78,11,52,1,7,1};
    push_data_to_fifo(&test_fifo,temp,sizeof(temp));
    printf("now use size is %d, remain size is %d\n",fifo_used_size(&test_fifo),fifo_remain_size(&test_fifo));
    
    while(pull_data_from_fifo(&test_fifo,&temp_data,1) >= 0)
    {
        printf("read data is %d\n",temp_data);
    }
    printf("now use size is %d, remain size is %d\n",fifo_used_size(&test_fifo),fifo_remain_size(&test_fifo));
    
    return 0;
}