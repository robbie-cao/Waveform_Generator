/*_____________________________________________________________________________________________________________________

        Author:         Jesse Nolan
_____________________________________________________________________________________________________________________*/
#include <compiler_defs.h>
#include <C8051F120_defs.h>     // SFR declarations
#include "Assign2.h"

// function prototypes

void Play_Note(char note);
void push_button();
void push_buttonP3();

// global variables

unsigned char DAClevel = 0x00;
int i = 1;
int j;
int poly_count = 0;
// used to pass frequency values to interrupt vector
int freq1 = 0;
int freq2 = 0;
int freq3 = 0;
int freq4 = 0;
int freq5 = 0;
int freq6 = 0;
int freq7 = 0;
//  keeps track of system states
int State = 1;
int pulse_count = 0;
int count = 0;
int counts = 0;
int counts_reached;

//  stores debounced button values
unsigned char button;
unsigned char buttonP3;

//  used for phasor accumulator in sine generator
unsigned short int phi1 = 0;
unsigned short int phi1HB;
unsigned short int phi2 = 0;
unsigned short int phi2HB;
unsigned short int phi3 = 0;
unsigned short int phi3HB;
unsigned short int phi4 = 0;
unsigned short int phi4HB;
unsigned short int phi5 = 0;
unsigned short int phi5HB;
unsigned short int phi6 = 0;
unsigned short int phi6HB;
unsigned short int phi7 = 0;
unsigned short int phi7HB;

//  used for button debouncing
unsigned char P1temp;
unsigned int temp;

// keeps track of volume level and state
unsigned int volume_level = 9;
unsigned int volume_count = 7;

//     temporary variables for polyphonics
unsigned int tempH;
unsigned int tempL;

unsigned int LED_state = 0;

//    keeps track of LED output for voume levels
unsigned int Vol_LED[16] =
{0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0};

//  Sin Lookup Table
code unsigned int sin[256] = 
{
0x0833, 0x0865, 0x0897, 0x08C9, 0x08FB, 0x092D, 0x095F, 0x0990, 0x09C1, 0x09F2, 0x0A23, 0x0A53, 0x0A83, 
0x0AB2, 0x0AE2, 0x0B10, 0x0B3E, 0x0B6C, 0x0B99, 0x0BC6, 0x0BF2, 0x0C1D, 0x0C48, 0x0C72, 0x0C9C, 0x0CC4, 
0x0CEC, 0x0D14, 0x0D3A, 0x0D60, 0x0D85, 0x0DA9, 0x0DCC, 0x0DEE, 0x0E0F, 0x0E30, 0x0E4F, 0x0E6D, 0x0E8B, 
0x0EA7, 0x0EC3, 0x0EDD, 0x0EF6, 0x0F0F, 0x0F26, 0x0F3C, 0x0F51, 0x0F65, 0x0F77, 0x0F89, 0x0F99, 0x0FA8, 
0x0FB6, 0x0FC3, 0x0FCF, 0x0FD9, 0x0FE2, 0x0FEA, 0x0FF1, 0x0FF7, 0x0FFB, 0x0FFE, 0x0FFF, 0x0FFF, 0x0FFF, 
0x0FFE, 0x0FFB, 0x0FF7, 0x0FF1, 0x0FEA, 0x0FE2, 0x0FD9, 0x0FCF, 0x0FC3, 0x0FB6, 0x0FA8, 0x0F99, 0x0F89, 
0x0F77, 0x0F65, 0x0F51, 0x0F3C, 0x0F26, 0x0F0F, 0x0EF6, 0x0EDD, 0x0EC3, 0x0EA7, 0x0E8B, 0x0E6D, 0x0E4F, 
0x0E30, 0x0E0F, 0x0DEE, 0x0DCC, 0x0DA9, 0x0D85, 0x0D60, 0x0D3A, 0x0D14, 0x0CEC, 0x0CC4, 0x0C9C, 0x0C72, 
0x0C48, 0x0C1D, 0x0BF2, 0x0BC6, 0x0B99, 0x0B6C, 0x0B3E, 0x0B10, 0x0AE2, 0x0AB2, 0x0A83, 0x0A53, 0x0A23, 
0x09F2, 0x09C1, 0x0990, 0x095F, 0x092D, 0x08FB, 0x08C9, 0x0897, 0x0865, 0x0833, 0x0801, 0x07CE, 0x079C, 
0x076A, 0x0738, 0x0706, 0x06D4, 0x06A2, 0x0671, 0x0640, 0x060F, 0x05DE, 0x05AE, 0x057E, 0x054F, 0x051F, 
0x04F1, 0x04C3, 0x0495, 0x0468, 0x043B, 0x040F, 0x03E4, 0x03B9, 0x038F, 0x0365, 0x033D, 0x0315, 0x02ED, 
0x02C7, 0x02A1, 0x027C, 0x0258, 0x0235, 0x0213, 0x01F2, 0x01D1, 0x01B2, 0x0194, 0x0176, 0x015A, 0x013E, 
0x0124, 0x010B, 0x00F2, 0x00DB, 0x00C5, 0x00B0, 0x009C, 0x008A, 0x0078, 0x0068, 0x0059, 0x004B, 0x003E, 
0x0032, 0x0028, 0x001F, 0x0017, 0x0010, 0x000A, 0x0006, 0x0003, 0x0001, 0x0001, 0x0001, 0x0003, 0x0006, 
0x000A, 0x0010, 0x0017, 0x001F, 0x0028, 0x0032, 0x003E, 0x004B, 0x0059, 0x0068, 0x0078, 0x008A, 0x009C, 
0x00B0, 0x00C5, 0x00DB, 0x00F2, 0x010B, 0x0124, 0x013E, 0x015A, 0x0176, 0x0194, 0x01B2, 0x01D1, 0x01F2, 
0x0213, 0x0235, 0x0258, 0x027C, 0x02A1, 0x02C7, 0x02ED, 0x0315, 0x033D, 0x0365, 0x038F, 0x03B9, 0x03E4, 
0x040F, 0x043B, 0x0468, 0x0495, 0x04C3, 0x04F1, 0x051F, 0x054F, 0x057E, 0x05AE, 0x05DE, 0x060F, 0x0640, 
0x0671, 0x06A2, 0x06D4, 0x0706, 0x0738, 0x076A, 0x079C, 0x07CE, 0x0800

};

/*--------------------------------------------------------------------------------------------------------------------
        Function:         Main

        Description:      Main routine

--------------------------------------------------------------------------------------------------------------------*/
void main(void)
{
    // initialising
	General_Init();
	Timer_Init();
	Voltage_Reference_Init();
	DAC_Init();
	Interrupts_Init();
		
    //  main loop
	while(1)
	{	
		//  If a button is pressed on port 3, debounce button
		if (~P3 > 0)
		{
			push_buttonP3();

			//if MPB is pressed, change the system state and dont exit the loop until it is let go
			if (buttonP3)
			{	
								
				State  = State + 1;
				if (State == 4)
				{
					State = 1;
				}
				while(~MPB);
			}
		}
		
		/*------------------------------------------------------------------------------
				PLAY STATE
		--------------------------------------------------------------*/
		if (State == 1)			
		{
            // initialise variables
			P2 = 0;
			poly_count = 0;
			TR0 = 0;

            // Checks which buttons were pressed and assigns the number of buttons and the frequencies of those buttons to the
            //  interrupt vector
			if ((P1&0x01)&&(~P1&0x02))
			{
				Play_Note('C');
				P2 = P2 | 0x02;
				poly_count++;
			}
			if ((P1&0x01)&&(~P1&0x04))
			{
				Play_Note('D');
				P2 = P2 | 0x04;
				poly_count++;
			}
			if ((P1&0x01)&&(~P1&0x08))
			{
				Play_Note('E');
				P2 = P2 | 0x08;
				poly_count++;
			}
			if ((P1&0x01)&&(~P1&0x10))
			{
				Play_Note('F');
				P2 = P2 | 0x10;
				poly_count++;
			}
			if ((P1&0x01)&&(~P1&0x20))
			{
				Play_Note('G');
				P2 = P2 | 0x20;
				poly_count++;
			}
			if ((P1&0x01)&&(~P1&0x40))
			{
				Play_Note('A');
				P2 = P2 | 0x40;
				poly_count++;
			}
			if ((P1&0x01)&&(~P1&0x80))
			{
				Play_Note('B');
				P2 = P2 | 0x80;
				poly_count++;
			}
			if ((~P1&0x01)&&(~P1&0x02))
			{
				Play_Note('c');
				P2 = P2 | 0x02 | 0x01;
				poly_count++;
			}
			if ((~P1&0x01)&&(~P1&0x04))
			{
				Play_Note('d');
				P2 = P2 | 0x04| 0x01;
				poly_count++;
			}
			if ((~P1&0x01)&&(~P1&0x08))
			{
				Play_Note('E');
				P2 = P2 | 0x08| 0x01;
				poly_count++;
			}
			if ((~P1&0x01)&&(~P1&0x10))
			{
				Play_Note('f');
				P2 = P2 | 0x10| 0x01;;
				poly_count++;
			}
			if ((~P1&0x01)&&(~P1&0x20))
			{
				Play_Note('g');
				P2 = P2 | 0x20| 0x01;
				poly_count++;
			}
			if ((~P1&0x01)&&(~P1&0x40))
			{
				Play_Note('a');
				P2 = P2 | 0x40| 0x01;
				poly_count++;
			}
			if ((~P1&0x01)&&(~P1&0x80))
			{
				Play_Note('B');
				P2 = P2 | 0x80 | 0x01;
				poly_count++;
			}

            // Keeps playing the same notes until one or more is let go
			P1temp = P1;
			while (P1 == P1temp)
			{
				push_buttonP3();
                
                // if the state change button is pressed, change states
				if (buttonP3)
				{
					P1temp++;
					State = 2;
					while (~MPB);
				}
			}
            
            // reinitialising variables
			TR2 = 0;
			P2 = 0;
			poly_count = 0;
			freq1 = 0;
			freq2 = 0;
			freq3 = 0;
			freq4 = 0;
			freq5 = 0;
			freq6 = 0;
			freq7 = 0;
			phi1 = 0;
			phi2 = 0;
			phi3 = 0;
			phi4 = 0;
			phi5 = 0;
			phi6 = 0;
			phi7 = 0;
							
		} 
		/*------------------------------------------------------------------------------
				VOLUME STATE
		----------------------------------------------------------------------*/
		else if (State == 3)	//If the volume state is selected
		{	
			//init variables
			TR0 = 1;	
			freq1 = 0;	
			button = 0xFF;
			
			//if button is pushed
			if (~P1 > 0)
			{
				push_button();			
			}
			// if volume is not min level, increase volume level (decrease volume)
			if(volume_level<16)
			{
				if (~button & 0x02)
				{
					volume_level++;
					volume_count--;
					while(~PB2);
				}
			}
			// if volume is not max laval, increase volume level (increase volume)
			if(volume_level>1)
			{
				if (~button & 0x01)
				{
					volume_level--;
					volume_count++;
					while(~PB1);
				}		
			}	
			// if volume is min, turn dac off
			if(volume_level==16)
			{
				SFRPAGE = DAC0_PAGE;
				DAC0CN = 0x00;	
			} 
			else if (volume_level<16)
			{
				SFRPAGE = DAC0_PAGE;
				DAC0CN = 0x80;
			}
		// sets LED states for volume display
		LED_state = Vol_LED[volume_count]|LED_state;
			
		P2 = LED_state;

		LED_state = LED_state&0x01;

		}		

		/*  Theremin */
		
		else if (State == 2)
		{
			P2 = 0xFF;			
			//sets timer 2 
			SFRPAGE   = TMR2_PAGE;
			TR2 = 1;
			SFRPAGE   = TIMER01_PAGE;

			TH1 = 0xFA;
			
			ET2 = 0;			
			// send pulse for ultrasonic
			TR1 = 1;		//timer 1 on
			while(~TF1);
			TF1 = 0;
			P0_2 = 1;		
			while(~TF1);
			TF1 = 0;

			P0_2 = 0;
			while (~TF1);
			TF1 = 0;
			P0_2 = 1;		
			while(~TF1);
			TF1 = 0;

			P0_2 = 0;
			while (~TF1);
			TF1 = 0;
			P0_2 = 1;		
			while(~TF1);
			TF1 = 0;
			P0_2 = 0;
	
			ET2 = 1;				

			/* Mask time */
			TH1 = 0x53;
			while(~TF1);
			TF1 = 0;
			
			
			TH1 = 0xF6;   // 10 system clocks until overflow
							
			while (count < 51)		//while time elapsed is less than 51*10 system clock cycles (~1ms)
			{
				if (TF1)
				{
					count++;
					TF1 = 0;

					if (P0_3 == 1 && ~counts_reached)
					{
						counts = count;
						counts_reached=1;
					}
				}
			
			}	
			//if no RX was detected, default counts to count
			if (counts_reached = 0)
			{
				counts = count;
			}
			//init variables		
			counts_reached = 0;		
			count = 0;		
			// sets frequency as a scalar of the tiem difference for TX and RX
			freq1 = counts*200;
			
			TH1 = 0x00;
			TR1 = 1;
			for(j=0;j<20;j++)
			{
				while(~TF1);
				TF1 = 0;
			}
			
			TR1 = 0;
			SFRPAGE   = TMR2_PAGE;
			TR2 = 0;
			SFRPAGE   = TIMER01_PAGE;
		}	
	}
}

/*--------------------------------------------------------------------------------------------------------------------
        Functions
--------------------------------------------------------------------------------------------------------------------*/
void General_Init()
{
	WDTCN = 0xde;
	WDTCN = 0xad;
	SFRPAGE = CONFIG_PAGE;
	P0MDOUT = 0x00;		// set P0.0 to be input for timer enable bit
	P1MDOUT = 0x00; 	// Set port 1 to input (automatically 0 at reset)
	P2MDOUT = 0xff;		// Need to make pushpull outputs to drive LEDs properly
	OSCICN    = 0x83;

	XBR2 = 0x40;	//turns on crossbar
	//XBR1 = 0x20;	//routes timer 2 enable bit to port pin
	P2 = 0;

	P0_2 = 1;
}

void Timer_Init()
{
	SFRPAGE   = TMR2_PAGE;
    TMR2CF    = 0x08;
	//RCAP2H = 0xFD;
	//RCAP2L = 0x13;

	RCAP2H = 0xFD;
	RCAP2L = 0x0A;

	SFRPAGE   = TIMER01_PAGE;
   	TMOD      = 0x21;
    CKCON     = 0x02;
    TL0       = 0x9E;
    TH0       = 0x38;
	TL1		  = 0xAA;
	TH1		  = 0xFE;

    SFRPAGE   = TMR3_PAGE;
    TMR3CF    = 0x08;

}

void Voltage_Reference_Init()
{
	SFRPAGE   = ADC0_PAGE;
	REF0CN    = 0x03; // Turn on internal bandgap reference and output buffer to get 2.4V reference (pg 107)
}

void DAC_Init()
{
	SFRPAGE = DAC0_PAGE;
	DAC0CN 	= 0x80;
  
}

void Interrupts_Init()
{
	IE        = 0xA2;  // Global enable interrupt + timer 2 interrupt
	EIE2      = 0x01;
}

/*--------------------------------------------------------------------------------------------------------------------
        Function:         Timer0_ISR

        Description:     used to pulse 1Hz wave

--------------------------------------------------------------------------------------------------------------------*/
void Timer0_ISR (void) interrupt 1
{
	if (pulse_count == 5)
	{
		
		LED_state = LED_state ^ 0x01;
		//P2 = LED_state;
		pulse_count = 0; 
	}

	pulse_count ++;

	return;
}
/*--------------------------------------------------------------------------------------------------------------------
        Function:         Timer2_ISR

        Description:     Produces a sine wave based on the frequencies of individual inputs and the number of inputs

--------------------------------------------------------------------------------------------------------------------*/

void Timer2_ISR (void) interrupt 5
{
	// extracts the high bytes from the phasor variables
	phi1HB = phi1 & 0xFF00;
	phi1HB = phi1HB>>8;

	phi2HB = phi2 & 0xFF00;
	phi2HB = phi2HB>>8;

	phi3HB = phi3 & 0xFF00;
	phi3HB = phi3HB>>8;

	phi4HB = phi4 & 0xFF00;
	phi4HB = phi4HB>>8;

	phi5HB = phi5 & 0xFF00;
	phi5HB = phi5HB>>8;
	
	phi6HB = phi6 & 0xFF00;
	phi6HB = phi6HB>>8;

	phi7HB = phi7 & 0xFF00;
	phi7HB = phi7HB>>8;

	// adds together multiple sine waves based on the number of inputs
	temp = sin[(unsigned char)phi1HB];
	
	if (poly_count>1)
	{
		temp = temp + sin[(unsigned char)phi2HB];
	}
	if (poly_count>2)
	{
		temp = temp + sin[(unsigned char)phi3HB];
	}
	if (poly_count>3)
	{
		temp = temp + sin[(unsigned char)phi4HB];
	}
	if (poly_count>4)
	{
		temp = temp + sin[(unsigned char)phi5HB];
	}
	if (poly_count>5)
	{
		temp = temp + sin[(unsigned char)phi6HB];
	}
	if (poly_count>6)
	{
		temp = temp + sin[(unsigned char)phi7HB];
	}

	// scales the output based on the volume control
	temp = temp/(poly_count*volume_level);
	
	// splits the sine value into high and low bytes
	tempL = temp & 0x00FF;
	tempH = temp & 0xFF00;
	tempH = tempH>>8;

	// sets the dac to the high and low bytes
	DAC0L = (unsigned char)tempL;
	DAC0H = (unsigned char)tempH;

	// increments the phasor variables based on the frequency
	phi1 = phi1 + (freq1*2);

	phi2 = phi2 + (freq2*2);

	phi3 = phi3 + (freq3*2);

	phi4 = phi4 + (freq4*2);

	phi5 = phi5 + (freq5*2);

	phi6 = phi6 + (freq6*2);

	phi7 = phi7 + (freq7*2);


	TF2 = 0;        // Reset Interrupt							      // Restore SFR page
	return;
}

/*--------------------------------------------------------------------------------------------------------------------
        Function:         Push button port 3

        Description:     debounces the port 3 buttons

--------------------------------------------------------------------------------------------------------------------*/
void push_buttonP3(){

	unsigned char test1, test2;
		
		//read first value
		test1 = P3;
		//wait
		TL1		  = 0xAA;
		TH1		  = 0xFE;
		TR1 = 1;
		while(~TF1);
		TF1 = 0;
		TR1 = 0;
		//read second value
		test2 = P3;
		// if they are the same, return the debounecd port value
		if (test1 == test2)
		{		
				buttonP3 = test2;
				buttonP3 = ~buttonP3;
				buttonP3 = buttonP3 & 0x80;
		}

		return;	
}

/*--------------------------------------------------------------------------------------------------------------------
        Function:         push button port 1

        Description:     debounces the port 1 buttons

--------------------------------------------------------------------------------------------------------------------*/
void push_button(){

	unsigned char test1, test2;
		
		// same as previous function
		test1 = P1;
	
		TL1		  = 0xAA;
		TH1		  = 0xFE;

		TR1 = 1;
		while(~TF1);
		TR1 = 0;
		TF1 = 0;
		
		test2 = P1;

		if (test1 == test2)
		{		
				button = test2;
		}

		return;	
}

/*--------------------------------------------------------------------------------------------------------------------
        Function:         Play note

        Description:    assigns different notes to different frequency variables based on what notes were passed
						when the button was pressed.
--------------------------------------------------------------------------------------------------------------------*/
void Play_Note(char note)
{
	TR2 = 1;		//turns the timer on
	
	switch(note)
	{
		// if the note C is played
		case('C'):
			switch(poly_count)
			{	
				// set the freq1 to the note C
				case(0):	
				freq1 = 262;
				break;
				// if 1 note has been pressed before hand, set the second freq2 to C
				case(1):
				freq2 = 262;
				break;
				//etc...
				case(2):
				freq3 = 262;
				break;
				case(3):
				freq4 = 262;
				break;
				case(4):
				freq5 = 262;
				break;
				case(5):
				freq3 = 262;
				break;
				case(6):
				freq4 = 262;
				break;
				case(7):
				freq5 = 262;
				break;
			}
		break;

		case('D'):
			switch(poly_count)
			{
				case(0):	
				freq1 = 294;
				break;
				case(1):
				freq2 = 294;
				break;
				case(2):
				freq3 = 294;
				break;
				case(3):
				freq4 = 294;
				break;
				case(4):
				freq5 = 294;
				break;
				case(5):
				freq3 = 294;
				break;
				case(6):
				freq4 = 294;
				break;
				case(7):
				freq5 = 294;
				break;
			}
		break;

		case('E'):
			switch(poly_count)
			{
				case(0):	
				freq1 = 330;
				break;
				case(1):
				freq2 = 330;
				break;
				case(2):
				freq3 = 330;
				break;
				case(3):
				freq4 = 330;
				break;
				case(4):
				freq5 = 330;
				break;
				case(5):
				freq3 = 330;
				break;
				case(6):
				freq4 = 330;
				break;
				case(7):
				freq5 = 330;
				break;
			}
		break;

		case('F'):
			switch(poly_count)
			{
				case(0):	
				freq1 = 349;
				break;
				case(1):
				freq2 = 349;
				break;
				case(2):
				freq3 = 349;
				break;
				case(3):
				freq4 = 349;
				break;
				case(4):
				freq5 = 349;
				break;
				case(5):
				freq3 = 349;
				break;
				case(6):
				freq4 = 349;
				break;
				case(7):
				freq5 = 349;
				break;
			}
		break;

		case('G'):
			switch(poly_count)
			{
				case(0):	
				freq1 = 392;
				break;
				case(1):
				freq2 = 392;
				break;
				case(2):
				freq3 = 392;
				break;
				case(3):
				freq4 = 392;
				break;
				case(4):
				freq5 = 392;
				break;
				case(5):
				freq3 = 392;
				break;
				case(6):
				freq4 = 392;
				break;
				case(7):
				freq5 = 392;
				break;
			}
		break;

		case('A'):
			switch(poly_count)
			{
				case(0):	
				freq1 = 440;
				break;
				case(1):
				freq2 = 440;
				break;
				case(2):
				freq3 = 440;
				break;
				case(3):
				freq4 = 440;
				break;
				case(4):
				freq5 = 440;
				break;
				case(5):
				freq3 = 440;
				break;
				case(6):
				freq4 = 440;
				break;
				case(7):
				freq5 = 440;
				break;
			}
		break;

		case('B'):
			switch(poly_count)
			{
				case(0):	
				freq1 = 494;
				break;
				case(1):
				freq2 = 494;
				break;
				case(2):
				freq3 = 494;
				break;
				case(3):
				freq4 = 494;
				break;
				case(4):
				freq5 = 494;
				break;
				case(5):
				freq3 = 494;
				break;
				case(6):
				freq4 = 494;
				break;
				case(7):
				freq5 = 494;
				break;
			}
		break;

		case('c'):
		switch(poly_count)
			{
				case(0):	
				freq1 = 277;
				break;
				case(1):
				freq2 = 277;
				break;
				case(2):
				freq3 = 277;
				break;
				case(3):
				freq4 = 277;
				break;
				case(4):
				freq5 = 277;
				break;
				case(5):
				freq3 = 277;
				break;
				case(6):
				freq4 = 277;
				break;
				case(7):
				freq5 = 277;
				break;
			}
		break;

		case('d'):
			switch(poly_count)
			{
				case(0):	
				freq1 = 311;
				break;
				case(1):
				freq2 = 311;
				break;
				case(2):
				freq3 = 311;
				break;
				case(3):
				freq4 = 311;
				break;
				case(4):
				freq5 = 311;
				break;
				case(5):
				freq3 = 311;
				break;
				case(6):
				freq4 = 311;
				break;
				case(7):
				freq5 = 311;
				break;
			}
		break;

		case('f'):
			switch(poly_count)
			{
				case(0):	
				freq1 = 370;
				break;
				case(1):
				freq2 = 370;
				break;
				case(2):
				freq3 = 370;
				break;
				case(3):
				freq4 = 370;
				break;
				case(4):
				freq5 = 370;
				break;
				case(5):
				freq3 = 370;
				break;
				case(6):
				freq4 = 370;
				break;
				case(7):
				freq5 = 370;
				break;
			}
		break;

		case('g'):
			switch(poly_count)
			{
				case(0):	
				freq1 = 415;
				break;
				case(1):
				freq2 = 415;
				break;
				case(2):
				freq3 = 415;
				break;
				case(3):
				freq4 = 415;
				break;
				case(4):
				freq5 = 415;
				break;
				case(5):
				freq3 = 415;
				break;
				case(6):
				freq4 = 415;
				break;
				case(7):
				freq5 = 415;
				break;
			}
		break;

		case('a'):
			switch(poly_count)
			{
				case(0):	
				freq1 = 466;
				break;
				case(1):
				freq2 = 466;
				break;
				case(2):
				freq3 = 466;
				break;
				case(3):
				freq4 = 466;
				break;
				case(4):
				freq5 = 466;
				break;
				case(5):
				freq3 = 466;
				break;
				case(6):
				freq4 = 466;
				break;
				case(7):
				freq5 = 466;
				break;
			}
		break;
	}

	return;

}
