/*_____________________________________________________________________________________________________________________
        Project: Waveform Generator    
        Author: Jesse Nolan    
        Description: Generates sinusoidal waveforms in the chromatic scale with volume control and multi note functionality
        _______________________________________________________________________________________________________________________*/

#ifndef Assign2_h
#define Assign2_h

//--------------------------------------------------------------------------------------------------------------------
//                              Global Variables
//--------------------------------------------------------------------------------------------------------------------
sbit PB1 = P1 ^ 0;                           // Pushbutton PB1
sbit PB2 = P1 ^ 1;                           // Pushbutton PB2
sbit PB3 = P1 ^ 2;                           // Pushbutton PB3   
sbit PB4 = P1 ^ 3;                           // Pushbuttom PB4
sbit PB5 = P1 ^ 4;                           // Pushbutton PB5
sbit PB6 = P1 ^ 5;                           // Pushbutton PB6
sbit PB7 = P1 ^ 6;                           // Pushbutton PB7   
sbit PB8 = P1 ^ 7;                           // Pushbuttom PB8

sbit P0_0 = P0 ^ 0;                           // Pushbutton PB1
sbit P0_1 = P0 ^ 1;                           // Pushbutton PB2
sbit P0_2 = P0 ^ 2;                           // Pushbutton PB3   
sbit P0_3 = P0 ^ 3;                           // Pushbuttom PB4
sbit P0_4 = P0 ^ 4;                           // Pushbutton PB5
sbit P0_5 = P0 ^ 5;                           // Pushbutton PB6
sbit P0_6 = P0 ^ 6;                           // Pushbutton PB7   
sbit P0_7 = P0 ^ 7;                           // Pushbuttom PB8

sbit MPB = P3 ^ 7;													 // Pushbuttom MPB on 8051 development board

sbit LD1 = P2 ^ 0;                         // LED LD1   
sbit LD2 = P2 ^ 1;                         // LED LD2
sbit LD3 = P2 ^ 2;                         // LED LD3
sbit LD4 = P2 ^ 3;                         // LED LD4  
sbit LD5 = P2 ^ 4;                         // LED LD5   
sbit LD6 = P2 ^ 5;                         // LED LD6
sbit LD7 = P2 ^ 6;                         // LED LD7
sbit LD8 = P2 ^ 7;                         // LED LD8    

//--------------------------------------------------------------------------------------------------------------------
//                              Function prototypes
//--------------------------------------------------------------------------------------------------------------------
void main(void);
void General_Init(void);
void Timer_Init(void);
void Voltage_Reference_Init(void);
void DAC_Init(void);
void Interrupts_Init(void);

#endif    
