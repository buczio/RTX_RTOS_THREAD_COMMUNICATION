//message queue with complex data (memorypool)
#include <cmsis_os.h>
#include "Board_LED.h"

typedef struct {
    uint8_t canData[8];
} message_t;
 
osPoolDef(mpool, 16, message_t);																		//define memory pool
osPoolId  mpool;
 
osMessageQDef(queue, 16, message_t);																//define message queue
osMessageQId  queue;
 
void producer_thread (void const *args) {
    message_t *message;																							//define pointer for memory pool buffer
		uint8_t index,testData = 0x55;
    while (1) 
			{
			
			if(testData == 0xAA)
			{
				testData = 0x55;
			}
			else
			{
				testData = 0xAA;
			}
      message = (message_t*)osPoolAlloc(mpool);											
			for(index =0;index<8;index++)
			{
        message->canData[index] = testData;													
			}
       osMessagePut(queue, (uint32_t)message, osWaitForever);				//Post pointer to memory pool buffer
       osDelay(1000);
    }
}

void consumer_thread (void const *args)
{
	
	while (1) {
        osEvent evt = osMessageGet(queue, osWaitForever);						//wait for message to arrive
				if (evt.status == osEventMessage) {													//check we have received a message
					  message_t *message = (message_t*)evt.value.p;						//read the pointer to memory pool buffer
						LED_SetOut((uint32_t)message->canData[0]);							//write the data to the LED's
				    osPoolFree(mpool, message);															//Release the buffer
        }
    }
}
 
 
 
osThreadDef(producer_thread, osPriorityNormal,1,0);
osThreadDef(consumer_thread, osPriorityNormal,1,0);

int main (void) {
	
		osKernelInitialize ();                    					
	
		LED_Initialize();
    mpool = osPoolCreate(osPool(mpool));
    queue = osMessageCreate(osMessageQ(queue), NULL);
    
    osThreadCreate(osThread(producer_thread), NULL);
    osThreadCreate(osThread(consumer_thread), NULL);
		
		osKernelStart();
    
}
