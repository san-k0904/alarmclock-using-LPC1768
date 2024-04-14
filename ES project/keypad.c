#include <LPC17xx.h>
#include "lcd_disp.h" // use all the functions of lcd program
#include<time.h>
void scan(void);
unsigned char Msg1[13];//for displaying on lcd
unsigned char row, flag, key;
unsigned int k;
unsigned long int i, var1, temp, temp1, temp2, temp3;
unsigned char SCAN_CODE[16] = {0x11,0x21,0x41,0x81,
0x12,0x22,0x42,0x82,
0x14,0x24,0x44,0x84,
0x18,0x28,0x48,0x88};
unsigned char ASCII_CODE[16] = {'0','1','2','3',
'4','5','6','7',
'8','9','o','I',
' ',' ',' ',' '};


char alarm[5];
unsigned int current[4];
unsigned int hexsegment[10]={0x3F,0x06,0x5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F};

/*void sevenseg(){
	
	for(i=0;i<=3;i++){}
}
*/

void current_time(){
	unsigned int hour,min;
	time_t t = time(NULL);
	struct tm date=*localtime(&t);
	hour=date.tm_hour;
	min=date.tm_min;
	while(hour!=0){
		current[2]=hour%10;
		hour=hour/10;
		current[3]=hour;
		hour=hour/10;
	}
	while(min!=0){
		current[0]=min%10;
		min=min/10;
		current[1]=min;
		min=min/10;
	}
}


int timeDifferenceInSeconds() {
	char str_realTime[5];
	unsigned int z;
	int hours1,minutes1,hours2,minutes2,totalMinutes1,totalMinutes2,differenceInMinutes,differenceInSeconds;
	for (z=3 ;z>=0;z--)
		str_realTime[3-i]=current[i]+'0';
	
    hours1 = atoi(str_realTime);
    minutes1 = atoi(str_realTime + 3);

    // Extract hours and minutes from time2
    hours2 = atoi(alarm);
    minutes2 = atoi(alarm + 3);

    // Calculate total minutes for each time
    totalMinutes1 = hours1 * 60 + minutes1;
    totalMinutes2 = hours2 * 60 + minutes2;

    // If time2 is earlier than time1, adjust it to the next day
    if (totalMinutes2 < totalMinutes1) {
        totalMinutes2 += 24 * 60; // Add 24 hours
    }

    // Calculate the difference in minutes
    differenceInMinutes = totalMinutes2 - totalMinutes1;

    // Convert difference to seconds
    differenceInSeconds = differenceInMinutes * 60;

    return differenceInSeconds;
}
int main(void)
{
	unsigned int difference;
	LPC_GPIO2->FIODIR |= 0x00003C00; //made output P2.10 to P2.13 (rows)
	LPC_GPIO1->FIODIR &= 0xF87FFFFF; //made input P1.23 to P1.26(cols)
	initimer1();
	SystemInit();
	SystemCoreClockUpdate();
	lcd_init();
	lcd_comdata(0x80, 0);
	delay_lcd(800);
	clear_ports();
	delay_lcd(3200);
	lcd_puts(&Msg1[0]); //display the message
	while(1)
	{
		while(1)
		{
			for(row=1;row<5;row++)
			{
				if(row == 1)
					var1 = 0x00000400;
				else if(row == 2)
					var1 = 0x00000800;
				else if(row == 3)
					var1 = 0x00001000;
				else if(row == 4)
					var1 = 0x00002000;
				temp = var1;
				LPC_GPIO2->FIOCLR = 0x00003C00;
				LPC_GPIO2->FIOSET = var1;
				flag = 0;
				scan();
				if(flag == 1)
					break;
			} //end for
			if(flag == 1)
				break;
			} //2nd while(1)
			if(key!=0x44 && key!=0x84){
				alarm[k++]=key;
				for(i=0;i<16;i++) //get the ascii code for display
				{
					if(key == SCAN_CODE[i])
					{
						key = ASCII_CODE[i];
						break;
					} 
				}
				lcd_puts(&key);
			}
			else{
				if (key==0x44){
					//call function to get time difference
					difference = timeDifferenceInSeconds();
					inittimer0(difference);
					delay_clock();
				}
				else if(key==0x84){
					//interrupt function to stop alarm and make emr =1 to stop timer;
					//make k=0 in the interrupt function
				}
			}
	}//end while 1
}//end main
void scan(void){
	temp3 = LPC_GPIO1->FIOPIN;
	temp3 &= 0x07800000; 
	if(temp3 != 0x00000000)
	{
		flag = 1;
		temp3 >>= 19; 
		temp >>= 10; 
		key = temp3|temp;
		}
}


//buzzer
//interrupt