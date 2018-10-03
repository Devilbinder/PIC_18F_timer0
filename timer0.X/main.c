#include <xc.h>
#include <p18f4520.h>
#include "conbits.h"
#include "stdint.h"
#include "stdbool.h"

void interrupt high_isr(void);
void interrupt low_priority low_isr(void);

uint8_t blink_count = 0;
uint8_t button_count = 0;
bool button_press = false;

void main(void) {
    
    OSCCONbits.IRCF = 0x07;
    OSCCONbits.SCS = 0x03;
    
    while(OSCCONbits.IOFS!=1);

    TRISBbits.RB7=0; 
    TRISBbits.RB6=0; 
    
    TRISBbits.RB1=1; 
    LATBbits.LB7 = 0; 
    LATBbits.LB6 = 0;
    
    T0CONbits.TMR0ON=0; 
    T0CONbits.T08BIT=1;
    T0CONbits.T0CS = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS=2; //1:8
    
    RCONbits.IPEN = 1;
    INTCONbits.T0IE=1;
    INTCON2bits.TMR0IP=1;
    INTCONbits.GIEH = 1; 
    INTCONbits.GIEL = 1;
    
    T0CONbits.TMR0ON=1;
 
    while(1){
        
        if(blink_count >= 200){
            LATBbits.LB7 = ~LATBbits.LB7;
            blink_count = 0;
        }
        if(!button_press){
            if(PORTBbits.RB1){
                button_press = true;
            }
        }
        if(button_press){
            if(button_count >= 80){
                if(PORTBbits.RB1){
                    LATBbits.LB6 = ~LATBbits.LB6;
                    button_press = false;
                    button_count = 0;
                }else{
                   button_press = false;
                   button_count = 0; 
                }
            }
        }
    } 
}

void interrupt high_isr(void){
    INTCONbits.GIEH = 0;
    
    if(INTCONbits.TMR0IF){
        blink_count++;
        if(button_press){
            button_count++;
        }
        INTCONbits.TMR0IF = 0;
    }
    
    INTCONbits.GIEH = 1;
}

void interrupt low_priority low_isr(void){
    INTCONbits.GIEH = 0;
    INTCONbits.GIEH = 1;
}

