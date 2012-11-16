//*******************************************************************************
//*       Programmname: Trinity          DOS Name:trinity2_00_45K22.c             *   
//*       Release: 1.1                   Datum: 21.12.2011                        *
//*       Org. Autor: STRI               Firma: HTL                              *
//*       Bearb. Autor: PITD             Firma: HTL                              *
//*******************************************************************************
//
//*******************************************************************************
//*  Bemerkungen:                                                                 *
//*  Compiler Release: MPLAB 8.73 CCS Compiler PCH (PCM und PCB 4.127)            *
//*                                                                               *
//*******************************************************************************
//*******************************************************************************
//*      Pinbelegung:                                                *
//*      RA0 = AN0 Analogeingang 0 VU links                               *
//*      RA1 = AN1 Analogeingang 1 VU recht                               *
//*      RA2 = AN2 Analogeingang 2 Bass                                  *
//*      RA3 = AN3 Analogeingang 3 Mitten                               *
//*      RA4 = CS\ PCM2705                                           *
//*      RA5 = AN4 Analogeingang 4 Höhen                                 *
//*      RA6 = NC                                                *
//*      RA7 = SSPND\ Powerdown für Audio                               *
//*      RB0 = SCLK SPI Clock PCM 2705                                  *
//*      RB1 = MOSI SPI Daten                                        *
//*      RB2 = Taste - activ low                                       *
//*      RB3 = Taste + active low                                     *
//*      RB4 = Taste Down activ low                                     *
//*      RB5 = Taste Up activ low                                     *
//*      RB6 = Taste OK activ low (nicht verwendet) & Programm Clock PGC          *
//*      RB7 = Taste Enter activ low (nicht verwendet) & Programm Data PGD       *
//*      RC0 = VR0 VU Meter rechts unterste LED                            *
//*      RC1 = VR1 VU Meter rechts                                      *
//*      RC2 = VR2 VU Meter rechts                                      *
//*      RC3 = VR3 VU Meter rechts                                      *
//*      RC4 = VL7 VU Meter rechts oberste LED                           *
//*      RC5 = VL6 VU Meter rechts                                      *
//*      RC6 = VL5 VU Meter rechts                                      *
//*      RC7 = VL4 VU Meter rechts                                      *
//*      RD0 = VR4 VU Meter links                                     *
//*      RD1 = VR5 VU Meter links                                     *
//*      RD2 = VR6 VU Meter links                                     *
//*      RD3 = VR7 VU Meter links oberste LED                            *
//*      RD4 = VL3 VU Meter links                                     *
//*      RD5 = VL2 VU Meter links                                     *
//*      RD6 = VL1 VU Meter links                                     *
//*      RD7 = VL0 VU Meter links unterste LED                            *
//*      RE0 = LED Bass                                              *
//*      RE1 = LED Mitten                                           *
//*      RE2 = LED Höhen                                              *
//*******************************************************************************
//

//###############################################################################
//#                                                                               #
//# Diese Mustervorlage soll unverändert unter dem Namen: Muster.c bleiben        #
//# Erstellt für PIC 18FF45Kxx                                 STRASSER  2011.    #
//# Verwendeter Schriftfont: Courier New, Fett, 8-14, Textmode CCS Standard       #
//# Tabstops 4, Nicht mit Leerzeichen auffüllen, Auto intend on                   #
//# PIC REGISTER in Großbuchstaben z.B: GIE                                       #
//# Variablen groß / klein z.B: int Count_B (B=Byte(8),L(16),D(32),F,C(8),b(1))   #
//# Konstante groß / klein z.B: const int Delay1_K = 255; oder:                 #
//#   define Delay1_K 55 (Dezimal, 0x37 Hex,055 Octal).                              #
//#   Ausnahme: Maschinenkonstante: TRIS_INIT, OPTION_INIT                          #
//# Befehle immmer klein z.B:  for (Count_B=1;Count_B=DELAY;Count_B++)            #
//# Unterscheidung bit und Byte durch _b bzw. _B am Ende                          #
//# I/O groß, Portdefinitionen durch _p bzw. _P (p..Pin, P..Port) am Ende         #
//#   z.B: LED_p, SENSOR_p, SEGMENT_P                                           #
//# Labels immer Groß & klein, ohne Einrücken am Anfang, allein in einer Zeile    #
//#                                                                               #
//###############################################################################
//
//                    ** Grunddefinitionen Include Dateien**                                                                    
#include    <18F45K22.h>             // Zuordnung Prozessorregister
//#device    ICD=TRUE               // ICD2 Implementation
#device    adc=10                  // A/D Wandler auf 10 Bit
#case                            // Compiler ist Case sensitive 
#use       fast_io(a)               // TRIS Register wird selbst
#use       fast_io(b)                // gesetzt
#use      fast_io(c)               // 
#use      fast_io(d)               // 
#use      fast_io(e)               // 
#fuses       NOPROTECT,NOWDT,NOBROWNOUT,PUT,INTRC_IO,NOLVP,NOMCLR,NOPBADEN,NOPLLEN
#use       delay (clock=8000000)      // Verzögerungsroutinen (notwendig f. RS232)

//                        ** Datenrichtung **
#define         TRISA_INIT      0b10101111    // Datenrichtung Register RA
#define         TRISB_INIT      0b11111100    // Datenrichtung Register RB
#define         TRISC_INIT      0b00000000   // Datenregister Register RC
#define         TRISD_INIT      0b00000000   //
#define         TRISE_INIT      0b00000000   //

//                              ** Konstantendefinitionen **
const int Zimax_K =            10;         // maximale Anzahl Ziffernvorrat
const long Timerstartwert_K =   100;       // Timer wird auf 10ms eingestellt
const int Preload_K   =         5;         // Korrekturwert für Prescaler

                              
//                        ** Datentabellen **

   
//                        ** Variablendefinition **

short      Dimm_b =    0;            // Flag für Analogumschaltung RA0/1

int         
         Hsec_B =         0,
         Sec_B =            0,
         Min_B =            0,
         Hour_B =         0,
         Presc_B =         Preload_K,
         Cout_B =         0,
         Dout_B =         0,
         Down_B =         0;      // Wird im Interrupt decrementiert
long      Ana_L =            0;

//                  ** Arraydefinitionen **
long   Adc_L [5];                                 
                  

//                        ** Portdefinitionen **
#define      Vul_p            PIN_A0      // Eingang Aussteuerung links
#define      Vur_p            PIN_A1      // Eingang Aussteuerung rechts
#define      Bass_p            PIN_A2      // Eingang Bass
#define      Mid_p            PIN_A3      // Eingang Mitten
#define      Treble_p         PIN_A5      // Eingang Höhen
#define      Cs_p            PIN_A4      // Chipselect PCM2705
#define      Sspnd_p            PIN_A7      // Powerdown low aktiv
#define      Sclk_p            PIN_B0      // Serielle Clock
#define    Mosi_p            PIN_B1      // Serielle Daten
#define      Minus_p            PIN_B2      // Taste Minus
#define    Plus_p            PIN_B3      // Taste Plus
#define      Down_p            PIN_B4      // Taste Down
#define    Up_p            PIN_B5      // Taste Up
#define      Ledb_p            PIN_E0      // Bass LED
#define      Ledm_p            PIN_E1      // Mid LED
#define      Ledt_p            PIN_E2      // Treble LED



//                        ** Globale Includedateien mit Port IO **



//                        ** Interrupts **
#INT_TIMER0
void timer0_int()
{
   set_timer0 (Timerstartwert_K);          // Timer0 Interr. alle 1/100 sek
   --Down_B; 
   if (-- Presc_B == 0)
   {
      Presc_B = Preload_K;
      if (++Hsec_B == 100)
      {
         Hsec_B = 0;
         if (++Sec_B == 60)
         {
            Sec_B = 0;
            if (++Min_B == 60)
            {
               Min_B = 0;
               if (++Hour_B == 24)
               {
                  Hour_B = 0;
               }
            }
         }
      }
   }
}

//                        ** Functions **

void Portout (int Li_B,int Re_B)
{
   int Out_B = 0, I_B;
   
   for(I_B=0;I_B <=3;++I_B) Out_B  |= bit_test (Li_B,(3-I_B)) << I_B; // Bit0-3 auf 7-4
   swap (Out_B);
   swap (Re_B);
   for(I_B=0;I_B <=3;++I_B) Out_B  |= bit_test (Re_B,(I_B)) << I_B;
   Out_B ^= 255;
   output_d (Out_B);
   Out_B = 0;
   swap (Li_B);
   for(I_B=0;I_B <=3;++I_B) Out_B  |= bit_test (Li_B,(3-I_B)) << I_B;
   swap (Out_B);
   swap (Re_B);
   for(I_B=0;I_B <=3;++I_B) Out_B  |= bit_test (Re_B,(I_B)) << I_B;
   Out_B ^= 255;
   output_c (Out_B);
}

int Makevu (long Ana_L)
{
   int I_B;
   
   if (Ana_L >400)   I_B = 255;
   else if (Ana_L >220)I_B = 127;
   else if (Ana_L >140)I_B = 63;
   else if (Ana_L >80)I_B = 31;
   else if (Ana_L >40)I_B = 15;
   else if (Ana_L >20)I_B = 7;
   else if (Ana_L >12)I_B = 3;
   else if (Ana_L >8)I_B = 1;
   else I_B=0;
   return (I_B);
}

void SendSPI(int16 Data)
{
   output_low(Sclk_p);
   output_low(Cs_p);
   delay_ms (1);
   
   int i,u;
   
   for (i=16; i>0; i--)
   {   // msb first
      u=i-1;
      output_low(Sclk_p);
      delay_ms (1);
      (Data & (1<<u)) ? (output_high(Mosi_p)) : (output_low(Mosi_p));
      delay_ms (1);
      output_high(Sclk_p);
      delay_ms (1);
   }
   
   output_high(Mosi_p);
   delay_ms (10);
   output_high(Cs_p);
   return;
}


//                        ** Grundinitialisierung **
void coldstart () 
{   
    setup_adc_ports(sAN0|sAN1|sAN2|sAN3|sAN4|VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL|ADC_TAD_MUL_0);
   setup_oscillator(OSC_8MHZ|OSC_INTRC);
   setup_comparator(NC_NC_NC_NC);
   output_a (0b00001000);
   output_b (0);
   output_c (0);
   output_d (0);
   output_e (0);
   set_tris_a (TRISA_INIT);               // Datenrichtung Port A
   set_tris_b (TRISB_INIT);               // Datenrichtung Port B
   set_tris_c (TRISC_INIT);
   set_tris_d (TRISD_INIT);
   set_tris_e (TRISE_INIT);
   port_b_pullups(TRUE);
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_32|RTCC_8_BIT);
                                    // Timer0 intern, Takt 20.00/4/64 = 78.125 KHz
                                       // Interrupt alle 256/15.625 = 3.2768 ms (305Hz)
                                       // Korrekturwert für 10 ms: 156 Timerclicks
                                       // -> Timer wird auf 256-156=100 vorgestellt 
      set_timer0 (Timerstartwert_K);            // Timerwert auf Startwert setzen
      enable_interrupts(INT_TIMER0);
   setup_timer_1(T1_DISABLED);               // Nur Timer0 Interrupt
   delay_ms (200);
}



void LEDTest1()
{
   int I_B, li, re;
   
   output_low (Ledb_p);
   output_low (Ledm_p);
   output_low (Ledt_p);
   
   /*
      255 = 1111 1111
      127 = 0111 1111
   */
   
   li = 1;
   re = 0;
   for (I_B = 0; I_B < 8; I_B++){
      Portout(li, re);
      delay_ms (100);
      li *= 2;
   }
   re = 128;
   for (I_B = 0; I_B < 9; I_B++){
      Portout(li, re);
      delay_ms (100);
      re /= 2;
   }
   
   //delay_ms (500);
   
   output_high (Ledb_p);
   output_high (Ledm_p);
   output_high (Ledt_p);  
   
   delay_ms (500);
   
   for(I_B = 0; I_B < 3; I_B++){
      Portout(255,255);
      output_low (Ledb_p);
      output_low (Ledm_p);
      output_low (Ledt_p);
      
      delay_ms(100);
      
      Portout(0,0);
      output_high (Ledb_p);
      output_high (Ledm_p);
      output_high (Ledt_p);
      
      delay_ms(100);
   }
}
void LEDTest2()
{
   int I_B, li, re;

   
   for(I_B = 0; I_B < 3; I_B++){
      output_low (Ledb_p);
      output_low (Ledm_p);
      output_low (Ledt_p);
      
      /*
         255 = 1111 1111
         127 = 0111 1111
          85 = 0101 0101
         170 = 1010 1010
      */
     
      li = 170;
      re = 85;
      
      Portout(li, re);
      delay_ms (200);
      
      output_high (Ledb_p);
      output_high (Ledm_p);
      output_high (Ledt_p);
      
      li = 85;
      re = 170;
      
      Portout(li, re);
      delay_ms (200);
   }
}
void LEDTest3()
{
   int I_B, li, re;
   
   for(I_B = 0; I_B < 2; I_B++){
      output_low (Ledb_p);
      output_high (Ledm_p);
      output_low (Ledt_p);
      
      li = 1;
      re = 1;
      
      for (I_B = 0; I_B < 8; I_B++){
         Portout(li, re);
         delay_ms (200);
         li = (li * 2) + 1 ;
         re = (re * 2) + 1 ;
      }
      
      output_high (Ledb_p);
      output_low (Ledm_p);
      output_high (Ledt_p);
      
      
      for (I_B = 0; I_B < 8; I_B++){
         Portout(li, re);
         delay_ms (200);
         li = (int)(li / 2);
         re = (int)(re / 2);
      }
   
   }    
}
void LEDTest4()
{
   int I_B, I_B2, value;
   
   for(I_B2 = 0; I_B2 < 2; I_B2++){
      value = 1;
      for (I_B = 0; I_B < 8; I_B++){
         if (I_B % 2 == 0){
            Portout(value, 0);
            output_low (Ledb_p);
            output_high (Ledm_p);
            output_high (Ledt_p);
            delay_ms (200);
         }
         else{
            Portout(0, value);
            output_high (Ledb_p);
            output_high (Ledm_p);
            output_low (Ledt_p);
            delay_ms (200);
         }
         value = (value*2) + 1;
      }
   }
}
void LEDTest5()
{
   int I_B, I_B2, top, bottom;
   for(I_B2 = 0; I_B2 < 2; I_B2++){
      top = 192;
      bottom = 3;
      for (I_B = 0; I_B < 6; I_B++){
         if (I_B % 2 == 0){
            Portout(top, bottom);
            output_low (Ledb_p);
            output_low (Ledm_p);
            output_low (Ledt_p);
            delay_ms (200);
         }
         else{
            Portout(bottom, top);
            output_high (Ledb_p);
            output_high (Ledm_p);
            output_high (Ledt_p);
            delay_ms (200);
         }
         top = top/2;
         bottom = bottom *2;
      }
   }
}
void LEDTest6()
{
   int I_B, top, bottom, teiler;
   top = 128;
   bottom = 1;
   teiler = 2;

   output_high (Ledb_p);
   output_high (Ledm_p);
   output_high (Ledt_p);
   
   for (I_B = 0; I_B < 4; I_B++)
   {
      Portout(top+bottom, top+bottom);

      top = top + top/teiler;
      bottom = bottom*2 + 1;
      teiler = teiler*2 + 2;
      
      delay_ms(250);
   }
   for (I_B = 0; I_B < 3; I_B++)
   {
         output_low (Ledb_p);
         output_low (Ledm_p);
         output_low (Ledt_p);
         
         delay_ms(150);
         
         output_high (Ledb_p);
         output_high (Ledm_p);
         output_high (Ledt_p);
         
         delay_ms(150);
   }
   for (I_B = 0; I_B < 4; I_B++)
   {
      Portout(top+bottom, top+bottom);

      top = top*2;
      bottom = bottom/2;      
      
      delay_ms(250);
   }
}
void checkKeys()
{

   //      Tasten
   if(input(Minus_p)==0)
      SendSPI(0x0004);
   
   if(input(Plus_p)==0)
      SendSPI(0x0002);

   if(input(Up_p)==0) 
      SendSPI(0x0008);
      
   if(input(Down_p)==0)
      SendSPI(0x0010);
      
   SendSPI(0x0000); 

}

//                        ** Hauptprogramm **

void main () 
{
   int R_B,L_B, R_VU, L_VU, avg_R=0, avg_L=0;
   
   coldstart();                        // Grundinitialisierung
      enable_interrupts(GLOBAL);
      
   set_adc_channel(0);
   delay_ms (1);
   avg_L = read_adc();
   
   set_adc_channel(1);
   delay_ms (1);
   avg_R = read_adc();


   while (TRUE)                         // Beginn Hauptprogramm
   {
      
        //      VU-Meter
      set_adc_channel(0);
      delay_ms (1);
      L_VU = read_adc();
      
      set_adc_channel(1);
      delay_ms (1);
      R_VU = read_adc();
      
      avg_L = (avg_L + L_VU) /2;
      avg_R = (avg_R + R_VU) /2;
      
      if((avg_L + avg_R) >= 3){
 
         //      VU-Meter - Portout
         L_B = Makevu (L_VU);
         R_B = Makevu (R_VU);
         Portout (L_B,R_B);
         
         //      Bass, mid, treble
           
      
         set_adc_channel(2);
         delay_ms (1);
         if (read_adc() > 560) output_low (Ledb_p);
         else output_high (Ledb_p);
         
         set_adc_channel(3);
         delay_ms (1);
         if (read_adc() > 570) output_low (Ledm_p);
         else output_high (Ledm_p);
         
         set_adc_channel(4);
         delay_ms (1);
         if (read_adc() > 550) output_low (Ledt_p);
         else output_high (Ledt_p);
      }
      else {
         LEDTest1();
         checkKeys();
         LEDTest2();
         checkKeys();
         LEDTest3();
         checkKeys();
         LEDTest4();
         checkKeys();
         LEDTest6();
         checkKeys();
         LEDTest5();
      }
      checkKeys();
   } 
}

// Programmende
