/*
 * File:   
 * Author: norberto
 *
 *  Test to verify start_AuK when 10 tasks (without defects) are correctly 
 * created and run. 
 * 
 */


// DSPIC33EP512GM604 Configuration Bit Settings

// 'C' source line config statements

// FICD
#pragma config ICS = PGD1               // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FPOR
#pragma config BOREN = OFF              //  (BOR is disabled)
#pragma config ALTI2C1 = OFF            // Alternate I2C1 pins (I2C1 mapped to SDA1/SCL1 pins)
#pragma config ALTI2C2 = OFF            // Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)
#pragma config WDTWIN = WIN25           // Watchdog Window Select bits (WDT Window is 25% of WDT period)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON              // PLL Lock Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FOSC
#pragma config POSCMD = XT              // Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config IOL1WAY = OFF            // Peripheral pin select configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSECMD           // Clock Switching Mode bits (Clock switching is enabled,Fail-safe Clock Monitor is disabled)

// FOSCSEL
#pragma config FNOSC = PRI              // Oscillator Source Selection (Primary Oscillator (XT, HS, EC))
#pragma config PWMLOCK = OFF            // PWM Lock Enable bit (PWM registers may be written without key sequence)
#pragma config IESO = ON                // Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source)

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GCP = OFF                // General Segment Code-Protect bit (General Segment Code protect is Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include "xc.h"
#include "AuK_v1_1_8.h"
#include <libpic30.h>
#include <stdio.h>



void init_clock_signal(void)
{
    RCONbits.SWDTEN = 0; // Disable Watchdog Timer

    // This chip is taged with I-Temp stamp (-I/SP) then the temperature range
    // is -40�C to 85�C which implies a maximum of 70 MIPS (Fosc = 140 MHz)
    // Chapter 33.1 DC characteristics in dsPIC33EP512GM604 manual.
    
    // We take a conservative configuration
    
    // Configure Oscillator to operate the device at 40 Mhz
    // Fosc = Fin*M/(N1*N2), Fcy = Fosc/2
    // Fosc = 7.3728*65/(2*2) = 119.808 MHz
    // Fcy = Fosc/2 = 59.904 MHz

    // Configure PLL prescaler, PLL postscaler and PLL divisor

    PLLFBDbits.PLLDIV = 63; // M = PLLDIV + 2 = 65 -> PLLDIV = 65 - 2 = 63
    CLKDIVbits.PLLPOST = 0; // N2 = 2 (Output/2)
    CLKDIVbits.PLLPRE = 0; // N1 = 2 (Input/2)
    
    // clock switching to incorporate PLL
    __builtin_write_OSCCONH(0x03); // Initiate Clock Switch to Primary
    __builtin_write_OSCCONL(0x01); // Start clock switching

    while (OSCCONbits.COSC != 0b011); // Wait for Clock switch to occur
    while (OSCCONbits.LOCK != 1) {}; // Wait for PLL to lock (If LOCK = 1 -> PLL start-up timer is satisfied)

}

void init_ports(void)
{
    /* All possible analog bits are configured as digital */
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;    
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB6 = 0;
    TRISBbits.TRISB7 = 0;
    TRISBbits.TRISB8 = 0;
    TRISBbits.TRISB9 = 0;
}

void init_uart(void)
{
     /* Specified pins for UART1 */
    
    /* RX RPI44 */
    RPINR18bits.U1RXR = 0b0101100; 
    TRISBbits.TRISB12 = 1;
    
    /* TX RP43 */
    RPOR4bits.RP43R = 0b000001;
    TRISBbits.TRISB11 = 0;
    
    
    /* Configure UART */
    U1MODEbits.USIDL = 1;   // Stop on idle mode
    U1MODEbits.IREN = 0;    // disable IrDA operation
    U1MODEbits.UEN = 0;     // Only RX and TX are used (non CTS, RTS and BCLK)
    U1MODEbits.WAKE = 0;    // Wake up on start bit is disabled
    U1MODEbits.LPBACK = 0;  // Loopback mode disabled
    U1MODEbits.ABAUD = 0;   // Baud rate measurement disabled
    U1MODEbits.URXINV = 0;  // Non polarity inversion. Idle state is 1
    U1MODEbits.BRGH = 0;    // High baude rate disabled
    U1MODEbits.PDSEL = 0;   // 8 bit data with no parity
    U1MODEbits.STSEL = 0;   // One stop bit.
    
    
    U1STAbits.URXISEL = 0;  // Interrupt on each character received
    
    // U1BRG = (Fcy/(16*Baud_rate) -1)
    //U1BRG = 3470;            // For 9600 bauds (119.808/2 MHz)/(16*9600) - 1
    U1BRG = 32; //for 115200 bauds
    
    /* Configure interrupts */
    IPC2bits.U1RXIP = 2;
    IFS0bits.U1RXIF= 0;
    IEC0bits.U1RXIE = 0;
    
    U1MODEbits.UARTEN = 1; // Enable UART operation
    U1STAbits.UTXEN = 1;    // Enable uart1 TX (must be done after UARTEN)
    
    /* It is needed to wait one transmision bit to ensure start bit detection 
     When 9600 Baud rate is selected it is necessary to wait 104 us */
    __delay32(383);
    
    //__C30_UART = 1; // Redirects printf output to UART1
}


void test_task()
{
    int my_id;
    
    my_id = give_me_my_id();
    
    while(1)
    {
        switch(my_id)
        {
            case 0: PORTBbits.RB0 = ~LATBbits.LATB0;
                    break;
            case 1: PORTBbits.RB1 = ~LATBbits.LATB1;
                    break;        
            case 2: PORTBbits.RB2 = ~LATBbits.LATB2;
                    break;
            case 3: PORTBbits.RB3 = ~LATBbits.LATB3;
                    break;
            case 4: PORTBbits.RB4 = ~LATBbits.LATB4;
                    break;
            case 5: PORTBbits.RB5 = ~LATBbits.LATB5;
                    break;
            case 6: PORTBbits.RB6 = ~LATBbits.LATB6;
                    break;
            case 7: PORTBbits.RB7 = ~LATBbits.LATB7;
                    break;
            case 8: PORTBbits.RB8 = ~LATBbits.LATB8;
                    break;
            case 9: PORTBbits.RB9 = ~LATBbits.LATB9;
                    break;
            default: break;
        }
    }
        
}


int t_id;

int main(void) 
{
    int x;
       
    init_clock_signal();
    init_ports();
    init_uart();
    
    SRbits.IPL = 0;
    
    x = init_AuK(59.904E+6, 0.01);
    
    if(x != 0)
        return(x);
    
    for(x = 0; x < 10; x++)
    {
        t_id = create_task(__builtin_tblpage(test_task),
                           __builtin_tbloffset(test_task), 50, 1);

        if(t_id == -1)
            return(t_id);
    }
        
    
    start_AuK();
    
    
    Sleep();
    return(0);
}

