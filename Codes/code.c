#include <rtl.h> 
#include<stdio.h>
#include<stdlib.h>
#include<lpc21xx.h>
#include"./lcd1.h"
#include"./led_buzzer.h"
__task void task1(void);
__task void task2(void);
__task void task3(void);

void display(char *msg);
unsigned char receive();



OS_TID tsk1, tsk2,tsk3;
 	unsigned int i;
	unsigned char mg;
int main (void)

.{
  os_sys_init (task1);
}
 __task void task1(void)
 {
	
	 tsk1= os_tsk_self();
 os_tsk_prio_self(2);
	tsk2 = os_tsk_create (task2, 0);
 while(1)
 {
	 serial();
 display("PRESS 1 TO REPORT A VOILATION   ///  \n");
 mg = receive();
	 if(mg == '1')
	 {
		 led_blink();
		 serial();
		 display("voilation detected  ////  \n");
	 os_tsk_prio(tsk2, 3);}
 }}
 
 __task void task2(void)
 {
 tsk3 = os_tsk_create (task3, 0);
 while(1)
 {
	 serial();
 display("ENTER 1-OVER SPEED 2-DRINK AND DRIVE 3-NO HELMET AND SEND PHOTO PROOF   ///  \n");
 mg = receive();
		 if(mg == '1')
		 {
			 display("OUTPUT :: OVER SPEED   ///  \n");
	}
	 else if(mg == '2')
	 {
		  display("OUTPUT :: DRINK AND DRIVE   ///  \n");
	 }
	 else if(mg == '3')
	 {
		 display("OUTPUT :: NO HELMET   ///  \n");
	 }
	 else{}
		 buzzer();
 if(mg=='1' || mg=='2' || mg=='3'){
 os_tsk_prio(tsk3,4);
 }
 }}
 
 __task void task3(void)
 {
 while(1)
 {
 lcd("PHOTOS RECEIVED",15);
  lcd("BY POLICE",9);
  lcd("OWNER FOUND",11);
  lcd("MESSAGE SENT",12);
os_tsk_prio(tsk2,0);
	 os_tsk_prio_self(0);
 }}

 unsigned char receive()
{
	while(!(U0LSR & 0x01));
return (U0RBR);
}
 void display(char *msg)
{
	unsigned int i=0;
	for(i=0;msg[i]!=0;i++)
   {	
     U0THR = msg[i];
while(!(U0LSR & 0x20));			 
   }
}

