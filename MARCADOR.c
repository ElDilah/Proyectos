
#include <MARCADOR_BALONCESTO.h>


////////////////////////////////
#define LCD_ENABLE_PIN PIN_C6 //
#define LCD_RS_PIN PIN_C4     //
#define LCD_RW_PIN PIN_C5     //
#define LCD_DATA4 PIN_C0      //  PINES DE CONTROL Y DATOS PARA EL LCD
#define LCD_DATA5 PIN_C1      //
#define LCD_DATA6 PIN_C2      //
#define LCD_DATA7 PIN_C3      //
////////////////////////////////

//////////////////////////                                           
#BIT salida1 = PORTA.4  //
#BIT salida2 = PORTB.4  //
#BIT salida3 = PORTB.5  //  
#BIT salida4 = PORTB.7  //
                        //    VARIABLES PARA CONTROLAR EL TECLADO MATRICIAL   
#bit IOCA0 = IOCA.0     //
#bit IOCA1 = IOCA.1     //
#bit IOCA2 = IOCA.2     //
#bit IOCA3 = IOCA.3     //
//////////////////////////



#bit RBIAF=0x0B.0





#bit Flag_Timer1 = 0x0C.0   //FLAG DESBORDAMIENTO TIMER1//


int Estado[2];   //Variable que almacena el estado activo actual y anterior//

int Puntos_local = 0;  //Variable que contabiliza los puntos del equipo local//
int Puntos_visitante = 0;  //Variable que contabiliza los puntos del equipo visitante//

int Faltas_local = 0;  //Variable que contabiliza las faltas del equipo local//
int Faltas_visitante = 0;  //Variable que contabiliza las faltas del equipo visitante//

int Tiempo_muerto_local = 0;  //Variable que contabiliza el número de tiempos muertos del equipo local//
int Tiempo_muerto_visitante = 0;  //Variable que contabiliza el número de tiempos muertos del equipo visitante//

int Cuarto_de_juego = 1;  //Variable que almacena el cuarto de juego del partido//
/*
//Variables que almacenan el valor actual de tiempo del marcador//  
int dec_min = 0; 
int un_min = 0;
int dec_seg = 0;
int un_seg = 0;
int deci_seg = 0;
int cent_seg = 0;

//Variables auxiliares que almacenan el valor actual de tiempo del marcador (Usado para el estado TIEMPO_MUERTO)//
int dec_min_aux;  
int un_min_aux;
int dec_seg_aux;
int un_seg_aux;
int deci_seg_aux;
int cent_seg_aux;

*/
//
int n_cuenta=0;
int minutos =10;
int segundos=00;
int decimas=99;
//
void Muestra_lcd(void)
{
   // se borra lo que habia previamente en lcd 
//    lcd_clr_line(1);
//    lcd_clr_line(2);
 //  se pone lo que debe marcar
    
    
    lcd_gotoxy(3,1);
    printf(lcd_putc,"L");
   
// valor de tiempo 
   lcd_gotoxy(7,1);
   if (minutos>0){
     printf(lcd_putc,"%d:%d",minutos,segundos);
      }else
      {
    printf(lcd_putc,"%d:%d",segundos,decimas); 
     }
      // 
   lcd_gotoxy(15,1);
   printf(lcd_putc,"V");
      
      //segunda fila 
      lcd_gotoxy(3,2);
      printf(lcd_putc,"%d",Puntos_local);
      //
      lcd_gotoxy(9,2);
      printf(lcd_putc,"%d",Cuarto_de_juego);
      //
      lcd_gotoxy(15,2);
      printf(lcd_putc,"%d",Puntos_visitante);
   } //funcion que se llamara para mostrar automaticamente en el LCD 





int Ticks_tiempo;   //Variable auxiliar para temporizar con el TIMER1//
long int Precarga;  //Variable que almacena la precarga a cargar en el TIMER 1 (debe ser formato long ya que es mayor que 256)//

int Cont_parpadeo = 1;   //Contador para realizar efecto de parpadeo//

char texto[10]; //Variable auxiliar para mostrar texto en el LCD en función de la situación//

char tecla_pulsada='L';  //Variable auxiliar para localizar la tecla pulsada//

//////////////////////////////
//ESTADOS MÁQUINA DE ESTADOS//
//////////////////////////////

#define INICIO 0 
#define PRIMER_CUARTO 1
#define SEGUNDO_CUARTO 2 
#define TERCER_CUARTO 3
#define CUARTO_CUARTO 4
#define PRORROGA 5 
#define PARADO 6 
#define DESCANSO 7  
#define TIEMPO_MUERTO 8 

///////////////////////////////////////////////////////////////////////////////
int n =0;
#INT_RA
void  RA_isr(void)   //PROGRAMA TRATAMIENTO DE LA INTERRUPCIÓN POR CAMBIO DE ESTADO EN EL PUERTO A (para el teclado matricial)//
{
    n++;
     
    delay_ms(10);  // DELAY PARA MINIMIZAR EFECTO DE REBOTES //

    
       salida1 = 0;
       salida2 = 1;
       salida3 = 1;
       salida4 = 1;
       
       if(input(pin_a0)==0)   //Si el PIN_A0 está a 0 la tecla pulsada será la '0'//
          tecla_pulsada='0';
       if(input(pin_a1)==0)   //Si el PIN_A0 está a 0 la tecla pulsada será la '1'//
          tecla_pulsada='1';
       if(input(pin_a2)==0)   //Si el PIN_A0 está a 0 la tecla pulsada será la '2'//
          tecla_pulsada='2';
       if(input(pin_a3)==0)   //Si el PIN_A0 está a 0 la tecla pulsada será la '3'//
          tecla_pulsada='3';
                  
       salida1 = 1;
       salida2 = 0;
       salida3 = 1;
       salida4 = 1;
       if(input(pin_a0)==0)    //Si el PIN_A0 está a 0 la tecla pulsada será la '4'//
          tecla_pulsada='4';
       if(input(pin_a1)==0)    //Si el PIN_A0 está a 0 la tecla pulsada será la '5'//
          tecla_pulsada='5';
       if(input(pin_a2)==0)    //Si el PIN_A0 está a 0 la tecla pulsada será la '6'//
          tecla_pulsada='6';
       if(input(pin_a3)==0)    //Si el PIN_A0 está a 0 la tecla pulsada será la '7'// 
          tecla_pulsada='7';       
           
       salida1 = 1;
       salida2 = 1;
       salida3 = 0;
       salida4 = 1;
       if(input(pin_a0)==0)     //Si el PIN_A0 está a 0 la tecla pulsada será la '8'// 
          tecla_pulsada='8';
       if(input(pin_a1)==0)     //Si el PIN_A0 está a 0 la tecla pulsada será la '9'// 
          tecla_pulsada='9';
       if(input(pin_a2)==0)     //Si el PIN_A0 está a 0 la tecla pulsada será la 'A'// 
          tecla_pulsada='A';
       if(input(pin_a3)==0)     //Si el PIN_A0 está a 0 la tecla pulsada será la 'B'// 
          tecla_pulsada='B';
                  
       salida1 = 1;
       salida2 = 1;
       salida3 = 1;
       salida4 = 0;
        
       if(input(pin_a0)==0)      //Si el PIN_A0 está a 0 la tecla pulsada será la 'C'// 
          tecla_pulsada='C';
          if(input(pin_a1)==0)   //Si el PIN_A0 está a 0 la tecla pulsada será la 'D'// 
          tecla_pulsada='D';
       if(input(pin_a2)==0)      //Si el PIN_A0 está a 0 la tecla pulsada será la 'E'// 
          tecla_pulsada='E';
       if(input(pin_a3)==0)      //Si el PIN_A0 está a 0 la tecla pulsada será la 'F'// 
          tecla_pulsada='F'; 
        
                    
       salida1 = 0;
       salida2 = 0;
       salida3 = 0;
       salida4 = 0;

       
       
        RBIAF=0;   
}
