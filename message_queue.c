
#include "MKL25Z4h"
#include "cmsis_os.h"

#include "Board_LED.h"

osMessageQId Q_LED;																		//define the message queue of
osMessageQDef (Q_LED,0x16,unsigned char);							//16 unsigned char elements
osEvent  result;


void led_Thread1 (void const *argument);																
void led_Thread2 (void const *argument);

osThreadDef(led_Thread1, osPriorityAboveNormal, 1, 0);
osThreadDef(led_Thread2, osPriorityNormal, 1, 0);

osThreadId T_led_Thread1;
osThreadId T_led_Thread2;
osThreadId	T_main; 



/*----------------------------------------------------------------------------
  Task 2 'ledOff': switches the LED off
 *---------------------------------------------------------------------------*/
void led_Thread1 (void const *argument) 
{
	for (;;) 
	{
		result = 	osMessageGet(Q_LED,osWaitForever);				//wait for a message to arrive
		LED_On(result.value.v);                          		// write the data to the LED's
	}
}
/*----------------------------------------------------------------------------
 switches the LED on
 *---------------------------------------------------------------------------*/
void led_Thread2 (void const *argument) 
{
	for (;;) 
	{
		osMessagePut(Q_LED,0x01,osWaitForever);             //Place a value in the message queue
		osDelay(100);																				//Wait 100 ms
		LED_Off(0x01);																			//switch off the LED
		osMessagePut(Q_LED,0x02,osWaitForever);							//Repeat for other bit patterns
		osDelay(100);
		LED_Off(0x02);
		osMessagePut(Q_LED,0x03,osWaitForever);
		osDelay(100);
		LED_Off(0x03);
		osMessagePut(Q_LED,0x04, osWaitForever);
		osDelay(100);
		LED_Off(0x04);
	}
}


int main (void) 
{
	osKernelInitialize ();                    						
	
	LED_Initialize ();
	Q_LED = osMessageCreate(osMessageQ(Q_LED),NULL);					//create the message queue
	T_led_Thread1 = osThreadCreate(osThread(led_Thread1), NULL);
	T_led_Thread2 =	osThreadCreate(osThread(led_Thread2), NULL);
	
	osKernelStart ();                         					
}
