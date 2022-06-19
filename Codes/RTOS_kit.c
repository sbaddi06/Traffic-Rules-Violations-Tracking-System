#include <RTL.h> 
#include<stdio.h>
#include<stdlib.h>
#include<lpc21xx.h>
#include<string.h>

void delay(int time)
{
	unsigned int i;
	for(i=0;i<time;i++);
}

//#include"./delay.h"
#include"./string.h"
//#include"./stepper_dc.h"
#include"./led_buzzer.h"
#include"./lcd.h"

__task void task1(void);
__task void task2(void);
__task void task3(void);
void init_serial (void);

void led_blink(void);
void buzzer(void);
//void stepper_dry(void);
//void stepper_wet(void);
//void DC_motor(void);
void init(void);
void print_message(char*);
void clear_disp(void);
void move_cursor(void);

unsigned  int c[]={0x30,0x30,0x20,0x20,0x28,0x01,0x06,0x0e,0x80};
unsigned char i,j;
char arr[10];

char arr1[10],arr2[10],arr3[20];
char *ptr,var1[10];

OS_TID tsk1, tsk2,tsk3;
OS_SEM semaphore1,semaphore2;

__task void task1 (void) 
{
 
	char ch;
	OS_RESULT ret;
	os_tsk_prio_self(2);
	tsk2 = os_tsk_create (task2, 0);

	
  while (1) 
	{
   ret = os_sem_wait (semaphore1, 0x0f);
   if (ret == OS_R_SEM)
	 {
		 
		//TODO Task
		
		//Take the input for classification of waste
		clear_disp();
		move_cursor();
		print_message("ENTER  ///  ");
		cmd(0xC0);
		print_message(" 1-OVER SPEED");
		 cmd(0xC0);
		print_message(" 2-DRINK&DRIVE");
		cmd(0x80);
		print_message(" 3-NO HELMET");
		cmd(0xC0);
		print_message(" SEND PHOTO PROOF");
		 
		while(!(U0LSR & 0x01));
		ch = U0RBR;
		sprintf(arr,"%c Selected",ch);
		while(!(U0LSR &0x20));
		U0THR=ch;
		clear_disp();
		move_cursor();
		print_message(arr);
		 buzzer();
		//Print message and Start Stepper motor based on input 'ch'
		switch(ch)
		{
			case '1':
							clear_disp();
							move_cursor();
							print_message("OVER SPEED ");
							cmd(0xC0);
							print_message("PHOTOS RECEIVED");
							//stepper_dry();
							delay(50000);
							clear_disp();
							move_cursor();
							print_message("BY POLICE");
							cmd(0xC0);
							print_message("OWNER FOUND");
							//DC_motor();
							break;
			case '2':
							clear_disp();
							move_cursor();
							print_message("DRINK AND DRIVE");
							cmd(0xC0);
							print_message("PHOTOS RECEIVED");
							//stepper_dry(); 
							delay(50000);
							clear_disp();
							move_cursor();
							print_message(" BY POLICE");
							cmd(0xC0);
							print_message("OWNER FOUND");
							//DC_motor();
							break;
			case '3':
							clear_disp();
							move_cursor();
							print_message("NO HELMET ");
							cmd(0xC0);
							print_message("PHOTOS RECEIVED");
							//stepper_dry();
							delay(50000);
							clear_disp();
							move_cursor();
							print_message(" BY POLICE");
							cmd(0xC0);
							print_message(" OWNER FOUND");
							//DC_motor();
							
							break;
			default:
						break;
		}

      os_sem_send (semaphore2);
    }
  }
}

__task void task3 (void) 
{
 
//	char ch;
	OS_RESULT ret;
	os_tsk_prio_self(2);
	tsk2 = os_tsk_create (task2, 0);

	
  while (1) 
	{
   ret = os_sem_wait (semaphore2, 0x0f);
   if (ret == OS_R_SEM)
	 {
		 
		//TODO Task
		
		//Start DC motor
		DC_motor();
		clear_disp();
		move_cursor();
		print_message(" Tracking vehicle");

		//Buzzer [End]
		buzzer();
		clear_disp();
		move_cursor();
		print_message(" Transportation");
		cmd(0xC0);
		print_message(" Done.");

    }
  }
}

__task void task2 (void) 
{
	char x;
	os_sem_init (semaphore2, 1);
  while (1) 
  {
		
		//TODO Task
		
		//Take input from user
		clear_disp();
		move_cursor();
		print_message("Enter 1");
		
		while(!(U0LSR & 0x01));
		x = U0RBR;
		//x = x - 48;
		sprintf(arr,"%c Selected",x);
		clear_disp();
		move_cursor();
		print_message(arr);
		
		//Acknowledge the user with message and ledvvm
		clear_disp();
		move_cursor();
		print_message("Violation detected..");
		led_blink();
		delay(1000000);
		delay(1000000);
		
		/* Return a token back to a semaphore. */
		os_sem_send (semaphore1);
	}
}

int main (void)
 {
   init();
	 
  /* Initialize the Semaphore before the first use */
  os_sem_init (semaphore1, 1);
  os_sys_init (task1);
}

void print_message(char* mess)
{
	  int i,charLen = strlen(mess);
		for(i=0;i<charLen;i++)
		{
			while(!(U0LSR &0x20));
			U0THR=mess[i];
		}
	
		for(j=0;j<charLen;j++)
		{		
			data(mess[j]);
			delay(1000000);
		}  
}

void init()
{
	IODIR0 = 0xF00F4CFC; // Stepper, LED, Buzzer, DC, config as output
	//added LCD FC
	
	//For UART communication
	PINSEL0=0x00000005;
	U0LCR=0x83;
	U0DLL=0x61;
	U0LCR=0x03;
	
	
	//Init LCD commands
	for(i=0;i<9;i++)
	{
	cmd(c[i]);
	delay(10000);
	}
	
	cmd(0x80);
	delay(10000);
}

void move_cursor()
{
	 cmd(0x80);
		delay(10000);
	 delay(65000);
		delay(65000);
		delay(65000);
		delay(65000);
		
}

void clear_disp()
{
	cmd(0x01);
	delay(65000);
		delay(65000);
		delay(65000);
}
