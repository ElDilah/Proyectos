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
    lcd_clr_line(1);
    lcd_clr_line(2);
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
   delay_ms(100);
   } //funcion que se llamara para mostrar automaticamente en el LCD 





int Ticks_tiempo;   //Variable auxiliar para temporizar con el TIMER1//
long int Precarga;  //Variable que almacena la precarga a cargar en el TIMER 1 (debe ser formato long ya que es mayor que 256)//

int Cont_parpadeo = 1;   //Contador para realizar efecto de parpadeo//

char texto[10]; //Variable auxiliar para mostrar texto en el LCD en función de la situación//

char tecla_pulsada='X';  //Variable auxiliar para localizar la tecla pulsada//

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
    lcd_gotoxy(14,2);
    printf(lcd_putc,"%3d",n);
    
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
        
        
                     lcd_gotoxy(1,1);
            printf(lcd_putc,"%c",tecla_pulsada);
            delay_ms(1000);
       
       
       
        RBIAF=0;   
}

///////////////////////////////////////////////////////////////////////////////

#SEPARATE
void Temporizacion_TIMER1 (int Ticks_tiempo, long int Precarga)  //Función que temporiza los tiempos necesarios para la cuenta regresiva//
{
    int contador = 1;
    while(contador <= Ticks_tiempo)
    {
        set_timer1(Precarga);
        while (Flag_Timer1 == 0)
        {
    
        }
        Flag_Timer1 = 0;
        contador = contador + 1;
    }
}

///////////////////////////////////////////////////////////////////////////////

#SEPARATE
void Parpadeo_TIMEOUT ()   //Función que hace el efecto de parpadeo del valor que tenga guardada la variable "texto"//
{
   /*
    //EFECTO PARPADEO DE DESCANSO//
                   
    if(Cont_parpadeo > 3)
    {
        Cont_parpadeo = 1;
    }
    while(Cont_parpadeo <= 3)
    { 
        Precarga = 40536;
        Ticks_tiempo = 5;
               
        lcd_gotoxy(6,2);
        printf(lcd_putc,"        "); 
        Temporizacion_TIMER1(Ticks_tiempo, Precarga);
        lcd_gotoxy(6,2);
        printf(lcd_putc,"%s",texto);
        Temporizacion_TIMER1(Ticks_tiempo, Precarga);
                  
        Cont_parpadeo = Cont_parpadeo + 1;
    }
    //FIN EFECTO PARPADEO//
    */
}

///////////////////////////////////////////////////////////////////////////////

#SEPARATE
void TEMPORIZACION_TIEMPO_MARCADOR()    //Función que realiza una cuenta regresiva desde los valores precargados en las variables del tiempo del marcador hasta 0//
{
   //MIENTRAS EL ESTADO ACTIVO NO SEA NI TIEMPO MUERTO NI PARADO NI Y MIENTRAS EL CONTADOR NO HAYA LLEGADO A 0 DEBERÁ DECREMENTARSE//
//    while ((Estado[0]!=PARADO)&&(Estado[0]!=TIEMPO_MUERTO)&&(dec_min + un_min + dec_seg + un_seg + deci_seg + cent_seg) != 0) 
//    {
//         if((un_min == 0)&&(dec_min == 0))  //Si el tiempo está por debajo del último minuto el marcador mostrará segundos, decimas y centésimas//
//               {
//                    Precarga = 63036;   //Precarga para que el timer cuente 1 centésima//
//                    Ticks_tiempo = 1;   //Valor que indica el número de veces que contará el TIMER1//
//                    Temporizacion_TIMER1(Ticks_tiempo, Precarga);  //Llamo a la función de temporización//
//                    
//                    
//                    if((un_seg == 0)&&(deci_seg == 0)&&(cent_seg == 0))  //Si las unidades,decimas y centesimas de segundo son cero, recargo valores y descuento una decena de segundo//
//                   {
//                       cent_seg = 9;
//                       deci_seg = 9;
//                       un_seg = 9;
//                       dec_seg = dec_seg - 1;
//                   }
//                   else  //Si no han pasado 10 segundos compruebo si ha pasado uno, si es así recargo valores y descuento un segundo//
//                   {
//                       if((deci_seg == 0)&&(cent_seg == 0))
//                       {
//                          deci_seg = 9;
//                          cent_seg = 9;
//                          un_seg = un_seg - 1;
//                       }
//                       else  //Si no ha pasado un segundo compruebo si ha pasado una décima, si es así recargo valores y descuento una decima//
//                       {
//                          if(cent_seg == 0)
//                          {
//                             cent_seg = 9;
//                             deci_seg = deci_seg - 1;
//                          }
//                          else  //Finalmente no ha ocurrido nada de lo anterior significa que ha pasado una centésima
//                          {
//                             cent_seg = cent_seg - 1;   
//                          }
//                       }
//                   }
//                   
//                   //ACTUALIZO LCD//
//                                         
//                    lcd_gotoxy(7,1);
//                    printf(lcd_putc,"%d%d:%d%d",dec_seg,un_seg,deci_seg,cent_seg);
//                                       
//               }
//               else  //Si el tiempo no está por debajo del último minuto el marcador mostrará minutos y segundos//
//               {
//                   Precarga = 40556;     //Precargo el valor para que el TIMER1 cuente una décima//
//                   Ticks_tiempo = 10;    //Establezco el número de veces que el TIMER1 temporizará una décima de segundo//
//                   Temporizacion_TIMER1(Ticks_tiempo, Precarga);  //Con los valores indicados la función temporizará un segundo//
//                   
//                                      
//                   if((un_min == 0)&&(dec_seg == 0)&&(un_seg == 0))  //Si las unidades de minuto, y los segundos son 0, recargo valores y decremento en uno las decenas de minuto//
//                   {
//                       un_min = 9;
//                       dec_seg = 5;
//                       un_seg = 9;
//                       dec_min = dec_min - 1;
//                   }
//                   else  //Si han pasado 60 segundos, recargo valores y decremento las unidades de minuto//
//                   {
//                       if((dec_seg == 0)&&(un_seg == 0))
//                       {
//                          dec_seg = 5;
//                          un_seg = 9;
//                          un_min = un_min - 1;
//                       }
//                       else   //Si han pasado 10 segundos decremento en uno las decenas de segundo//
//                       {
//                          if(un_seg == 0)
//                          {
//                             un_seg = 9;
//                             dec_seg = dec_seg - 1;
//                          }
//                          else   //Finalmente si no ha ocurrido nada de lo anterior decremento en uno los segundos//
//                          {
//                             un_seg = un_seg - 1;   
//                          }
//                       
//                        }
//                   } 
//                   
//                   if((dec_min == 0) && (un_min == 0))  //Si estamos en el útimo minuto muestro segundos,décimas y centésimas//
//                   {
//                       deci_seg = 9;
//                       cent_seg = 9;
//                       lcd_gotoxy(7,1);
//                       printf(lcd_putc,"%d%d:%d%d",dec_seg,un_seg,deci_seg,cent_seg);
//                   }
//                   else   //Si no estamos en el último minuto muestro minutos y segundos//
//                   {
//                       lcd_gotoxy(7,1);
//                       printf(lcd_putc,"%d%d:%d%d",dec_min,un_min,dec_seg,un_seg);
//                   }
//                   
//               }     
//    }
    
}
#INT_TIMER1
void  TIMER1_isr(void) 
{
n_cuenta++;  //incremento la cuenta de veces interrumpido 
   if (n_cuenta>3)
   {
      decimas--;
      if(decimas<1)
	 segundos--;
	 decimas=99;
      if(segundos<1)
	 minutos--;
	 segundos=59;
      }
   set_timer1(0xB1E0);
}
 
///////////////////////////////////////////////////////////////////////////////

void main()
{
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);      //262,144 ms overflow
   
   Estado[0]=0;

   lcd_init();   //INICIALIZO LCD//
      setup_adc_ports(NO_ANALOGS);   //ESTABLEZCO QUE LAS ENTRADAS SERÁN ENTRADAS DIGITALES Y NO ANALOGICAS//
   //Max_init(2);
   //enable_interrupts(INT_TIMER1);
   enable_interrupts(INT_RA);   //HABILITO INTERRUPCIONES POR CAMBIO DE ESTADO DEL PUERTO A//
   enable_interrupts(GLOBAL);   //HABILITO MÁSCARA GLOBAL DE INTERRUPCIONES//
   

   
   //DEFINO LOS PINES DE LOS PUERTOS A Y B QUE SERÁN ENTRADAS//
   
   TRISA = 0b001111;
   
   TRISB = 0b00000000;
   
   PORTA = 0b001111;
   PORTB = 0b00000000;
   
   IOCA0 = 1;
   IOCA1 = 1;
   IOCA2 = 1;
   IOCA3 = 1;

  
///////////////////////////////////////////////////////////////////////////////

   ///////////////////////////////
   //BUCLE DE EJECUCIÓN CONTINUA//
   ///////////////////////////////
   
   while(TRUE)   
   {
             
       switch(Estado[0])
       {
           case(INICIO):
               
               if(Estado[1]==DESCANSO)  //Si ya ha habido un partido previo se resetean ambas lineas del lcd//
               { 
		 Muestra_lcd();
               }
          
          //Max_print(24,2);
          //A continuación cargo los valores iniciales del marcador y los muestro en el LCD//
             
               break;
            
           case PRIMER_CUARTO:
               
               if(Estado[1] != TIEMPO_MUERTO)  //Si el estado anterior no es TIEMPO MUERTO (es decir, es la primer vez que se entra// 
               {                               //al estado), se inicializan los valores de tiempos muertos de ambos equipos//
                   Tiempo_muerto_local = 2;
                   Tiempo_muerto_visitante = 2;
               }
               
               
               Muestra_lcd();   //INICIALIZO LA TEMPORIZACIÓN//
               
               if((Estado[0] != PARADO)||(Estado[0] != TIEMPO_MUERTO))  //Si el estado activo es PARADO o TIEMPO MUERTO salimos del estado PRIMER CUARTO//
               {
                   Estado[1]=Estado[0];  //Desplazo tabla y establezco como estado activo el estado DESCANSO//
                   Estado[0] = DESCANSO;
               }
               
               break;
               
           case SEGUNDO_CUARTO:             
              
              //ACTUALIZO LCD//
              
              lcd_gotoxy(6,2);
              printf(lcd_putc,"        ");
               
               Muestra_lcd();  
               
               if((Estado[0] != PARADO)||(Estado[0] != TIEMPO_MUERTO))  //Si el estado activo es PARADO o TIEMPO MUERTO salimos del estado SEGUNDO CUARTO//
               {
                   Estado[1]=Estado[0];  //Desplazo tabla y establezco como estado activo DESCANSO//
                   Estado[0] = DESCANSO;
               }
               
               break;
       }
            /*   
           case TERCER_CUARTO:
               
              if(Estado[1] != TIEMPO_MUERTO)    //Si el estado anterior no es TIEMPO MUERTO recargo valores de tiempos muerto (solo se cumple la primera vez que se entra al estado TERCER CUARTO//
               {
                   Tiempo_muerto_local = 3;
                   Tiempo_muerto_visitante = 3;
               }
              
              //ACTUALIZO LCD//
              
              lcd_gotoxy(6,2);
              printf(lcd_putc,"        "); 
              lcd_gotoxy(9,2);
              printf(lcd_putc,"%d",Cuarto_de_juego);
               
               TEMPORIZACION_TIEMPO_MARCADOR();   //INICIO TEMPORIZACIÓN TERCER CUARTO//
               
               if((Estado[0] == PARADO)||(Estado[0] == TIEMPO_MUERTO))   //Si el estado activo es PARADO o TIEMPO MUERTO salimos del estado TERCER CUARTO//
               {
                   break;
               }
               else   //Si el estado activo no es ni PARADO ni TIEMPO MUERTO significa que la temporización acabó y debe haber un descanso//
               {
                   Estado[1]=Estado[0];   //Desplazo tabla y establezco como estado activo DESCANSO//
                   Estado[0] = DESCANSO;
               }
               
               break;
               
           case CUARTO_CUARTO:
                                                                              /////////////////////////////////////////////////////////////////////////////////////////////////////////
              if((Tiempo_muerto_local == 2) && (dec_min == 0) && (un_min <= 2)) //SOLO SE EVALUARÁ EN EL CASO DE ESTAR POR DEBAJO DE 2 MIN Y HABER USADO UN TIEMPO MUERTO,             //   
              {                                                                 // EN ESE CASO SI EL VALOR ACTUAL DE TIEMPOS MUERTOS ES DE 2 SIGNIFICA QUE EL EQUIPO TENÍA TRES CUANDO //
                  if((un_min == 2) && (dec_seg == 0) && (un_seg == 0))          // QUEDABAN MENOS DE 2 MIN Y SE TENDRÁ QUE RESTAR UNO PARA QUE SOLO TENGAN DISPONIBLE UNO MÁS          //
                  {                                                             ///////////////////////////////////////////////////////////////////////////////////////////////////////// 
                      Tiempo_muerto_local = 1; //Si el partido está en el cuarto cuarto y faltan menos de 2 min para que acabe el cuarto el número máximo de tiempos muertos es de 2//
                  }
                  else if (un_min < 2)
                  {
                      Tiempo_muerto_local = 1; //Si el partido está en el cuarto cuarto y faltan menos de 2 min para que acabe el cuarto el número máximo de tiempos muertos es de 2//
                  }   
              }
              else if((Tiempo_muerto_visitante == 2) && (dec_min == 0) && (un_min <= 2)) //SE APLICA EL MISMO CONCEPTO QUE PARA EL EQUIPO LOCAL//
              {
                  if((un_min == 2) && (dec_seg == 0) && (un_seg == 0))
                  {
                      Tiempo_muerto_visitante = 1; //Si el partido está en el cuarto cuarto y faltan menos de 2 min para que acabe el cuarto el número máximo de tiempos muertos es de 2//
                  }
                  else if (un_min < 2)
                  {
                      Tiempo_muerto_visitante = 1; //Si el partido está en el cuarto cuarto y faltan menos de 2 min para que acabe el cuarto el número máximo de tiempos muertos es de 2//
                  }   
              }
              
              
              //ACTUALIZO LCD//
              
              lcd_gotoxy(6,2);
              printf(lcd_putc,"        "); 
              lcd_gotoxy(9,2);
              printf(lcd_putc,"%d",Cuarto_de_juego);
               
               TEMPORIZACION_TIEMPO_MARCADOR();  //INICIO TEMPORIZACIÓN CUARTO CUARTO//
               
               if((Estado[0] == PARADO)||(Estado[0] == TIEMPO_MUERTO))   //Si el estado activo es PARADO o TIEMPO MUERTO salimos del estado CUARTO CUARTO//
               {
                   break;
               }
               else   //Si el estado activo no es ni PARADO ni TIEMPO MUERTO significa que la temporización acabó y debe haber un descanso//
               {
                   Estado[1]=Estado[0];   //Desplazo tabla y establezco como estado activo DESCANSO//
                   Estado[0] = DESCANSO;
               }
               
               break;
               
           case (PARADO):  //Si el estado activo es PARADO no se debe realizar nada//
               
      
               break;
               
           case (DESCANSO):
               
               if((Cuarto_de_juego == 1)||(Cuarto_de_juego == 3))  //Si el cuarto es el primero o el tercero el descanso será corto (TIMEOUT)//
               {
                   texto = "TIMEOUT";   //Designo texto de efecto y llamo a la función que lo realiza//
                   Parpadeo_TIMEOUT();
                   
                   dec_min = 0;
                   un_min = 2;
                   TEMPORIZACION_TIEMPO_MARCADOR();   //INICIO TEMPORIZACIÓN DESCANSO//
                   
                   Cuarto_de_juego = Cuarto_de_juego + 1;  //Actualizo el cuarto de juego en el que estamos//
                   
                   //RESETEO VALORES//
                   Faltas_local = 0;
                   Faltas_visitante = 0;
              
                   un_min = 0;
                   dec_min = 1;
              
                   lcd_gotoxy(2,1);
                   printf(lcd_putc," "); //Reseteo posible bonus faltas local//
                   lcd_gotoxy(16,1);
                   printf(lcd_putc," "); //Reseteo posible bonus faltas visitante//
                   
                   Estado[1]=Estado[0];  //Desplazo tabla//
                   if(Cuarto_de_juego == 2)  //Si debemos pasar al segundo cuarto la variable "Cuarto_de_juego" en este punto valdrá 2//
                   {
                       Estado[1] = SEGUNDO_CUARTO;  //EL MARCADOR DEBERÁ PARARSE PARA DAR TIEMPO A LOS JUGADORES A COLOCARSE ANTES DE INICIAR EL CUARTO//
                       Estado[0] = PARADO;
                   }
                   else   // En caso de que debieramos pasar al cuarto cuarto la variable "Cuarto_de_juego" en este punto valdrá 4//
                   {
                       Estado[1] = CUARTO_CUARTO;   //EL MARCADOR DEBERÁ PARARSE PARA DAR TIEMPO A LOS JUGADORES A COLOCARSE ANTES DE INICIAR EL CUARTO//
                       Estado[0] = PARADO;
                   }
               }
               else if(Cuarto_de_juego == 2)  //En caso de que el cuarto de juego sea el segundo habrá descanso largo (HALFTIME)//
               {
                   texto = "HALFTIME";   //Designo texto de efecto y llamo a la función que lo realiza//
                   Parpadeo_TIMEOUT();
                   
                   dec_min = 1;
                   un_min = 5;
                   TEMPORIZACION_TIEMPO_MARCADOR();    //INICIO TEMPORIZACIÓN DESCANSO// 
                   
                   Cuarto_de_juego = Cuarto_de_juego + 1;   //Actualizo el cuarto de juego en el que estamos//
                   
                   //RESETEO VALORES// 
                   Faltas_local = 0;
                   Faltas_visitante = 0;
              
                   un_min = 0;
                   dec_min = 1;
              
                   lcd_gotoxy(2,1);
                   printf(lcd_putc," "); //Reseteo posible bonus faltas local//
                   lcd_gotoxy(16,1);
                   printf(lcd_putc," "); //Reseteo posible bonus faltas visitante//
                   
                   Estado[1]=Estado[0];  //Desplazo tabla//
                   
                   Estado[1]=TERCER_CUARTO;  //EL MARCADOR DEBERÁ PARARSE PARA DAR TIEMPO A LOS JUGADORES A COLOCARSE ANTES DE INICIAR EL CUARTO//
                   Estado[0] = PARADO;   
               }
               else if(Cuarto_de_juego == 4)   //Si el cuarto de juego es el cuarto habrá que comprobar si hay empate o no//
               {
                   if(Puntos_local == Puntos_visitante)  //Si hay empate habrá prorroga//
                   {
                       
                        Faltas_local = 0;
                        Faltas_visitante = 0;
              
                        un_min = 5;
                        dec_min = 0;
              
                        lcd_gotoxy(2,1);
                        printf(lcd_putc," "); //Reseteo posible bonus faltas local//
                        lcd_gotoxy(16,1);
                        printf(lcd_putc," "); //Reseteo posible bonus faltas visitante//
                   
                        texto = "OVERTIME";  //Designo texto de efecto y llamo a la función que lo realiza//
              
                        Parpadeo_TIMEOUT();
                        Estado[1]=Estado[0];  //Desplazo tabla//
                       
                        Estado[1]=PRORROGA;   //EL MARCADOR DEBERÁ PARARSE PARA DAR TIEMPO A LOS JUGADORES A COLOCARSE ANTES DE INICIAR EL CUARTO//
                        Estado[0]=PARADO;   
                   }
                   else   //En caso de que no hubiera empate deberemos volver a INICIO//
                   {     
                       Estado[1]=Estado[0];   //Desplazo tabla//
                       Estado[0] = INICIO;    //El estado activo deberá ser INICIO//
                   }                   
               }
                
                break; 
                
            case (PRORROGA):

               //ACTUALIZO LCD//
                
                lcd_gotoxy(6,2);
                printf(lcd_putc,"OVERTIME");
                
                if(Estado[1] != TIEMPO_MUERTO)  //Si no venimos de TIEMPO MUERTO (solo ocurre la primer vez que se entra al estado PRORROGA)//
               {                                //reseteamos los valores de tiempos muertos siguiendo las reglas FIBA para una prorroga//
                   Tiempo_muerto_local = 1;
                   Tiempo_muerto_visitante = 1;
               }
               
                            
                TEMPORIZACION_TIEMPO_MARCADOR();   //INICIO TEMPORIZACIÓN DE LA PRORROGA//
                
                if((Estado[0] == PARADO)||(Estado[0] == TIEMPO_MUERTO))  //Si el estado activo es PARADO o TIEMPO MUERTO salimos del estado PRORROGA//
                   {
                       break;
                   }
               else    //Si el estado activo no es ni PARADO ni TIEMPO MUERTO significa que la temporización acabó y debe haber un descanso//
                   {
                       Estado[1]=Estado[0];   //Desplazo tabla//
                       Estado[0] = DESCANSO;  //Actualizo estado activo//
                   }
                
                if(Puntos_local == Puntos_visitante)  //Si hay empate volvemos a DESCANSO//
                   {
                       Estado[1]=Estado[0];
                       Estado[0] = DESCANSO;
                   }
                   else   //Si no hay empate actualizo LCD, desplazo tabla y establezco como estado activo INICIO//
                   {
                       lcd_gotoxy(6,2);
                       printf(lcd_putc,"        "); 
                       
                       Estado[1]=Estado[0];
                       Estado[0] = INICIO;
                   }
           
                break;
                
           case TIEMPO_MUERTO:
               
                Estado[0]=Estado[1];   //Recupero valor de estado activo anterior//
                Estado[1]=TIEMPO_MUERTO;   //Guardo como valor de estado activo anterior TIEMPO MUERTO//
               
               //Guardo en variables temporales auxiliares los valores actuales de tiempos//
               
                dec_min_aux=dec_min;  
                un_min_aux=un_min;
                dec_seg_aux=dec_seg;
                un_seg_aux=un_seg;
                deci_seg_aux=deci_seg;
                cent_seg_aux=cent_seg;
                
                texto = "TIMEOUT";  //Designo texto de efecto y llamo a la función que lo realiza//
                Parpadeo_TIMEOUT();
                
                un_min = 1;    //Establezco el minuto de reglamentario de tiempo muerto//
                dec_min = 0;
                dec_seg = 0;
                un_seg = 0;
                deci_seg = 0;
                cent_seg = 0;
                 
                TEMPORIZACION_TIEMPO_MARCADOR();   //INICIO TEMPORIZACIÓN DE TIEMPO MUERTO//
                
                
                //Devuelvo valores de tiempos que había en el marcador//
                
                dec_min = dec_min_aux;   
                un_min = un_min_aux;
                dec_seg = dec_seg_aux;
                un_seg = un_seg_aux;
                deci_seg = deci_seg_aux;
                cent_seg = cent_seg_aux;
                                
                //ACTUALIZO LCD//
                
                lcd_gotoxy(6,2);
                printf(lcd_putc,"        "); 
                lcd_gotoxy(9,2);
                printf(lcd_putc,"%d",Cuarto_de_juego);
                
                
             break;
                
            }
       */
       
              delay_ms(1000);     
              lcd_gotoxy(1,2);
            printf(lcd_putc,"%c",tecla_pulsada);

 ///////////////////////////      TECLADO MATRICIAL ///////////////////////////////////  
 //      if (tecla_pulsada!='X'){
       switch (tecla_pulsada)
        {
            case '0':   // BOTÓN START/STOP //
                
                if(Estado[0] == INICIO)  //Si el estado activo es INICIO desplaza la tabla y pasa al primer cuarto//
                {
                    Estado[1] = Estado[0];
                    Estado[0] = PRIMER_CUARTO;
		   enable_interrupts(INT_TIMER1);
                }
		else if (estado ==PARADO)  {// SI ESTA PARADO Y SE PULSA MARCHA
		     Estado[0] = Estado[1];
		     Estado[1] = PARADO;
		     enable_interrupts(INT_TIMER1);
		}
		else{				// si esta en marcha y se pulsa paro 
		   disable_interrupts(INT_TIMER1);
		   Estado[1] = Estado[0];
		   Estado[0] = PARADO;
		   }
                tecla_pulsada = 'L';  //Reinicio la variable tecla_pulsada al valor conjunto vacío//
                
                break;
            
            case '1':  // BOTÓN CAMBIO DE POSESIÓN //
                
                // REINICIAR A 24 s LA POSESIÓN //
                
                //SE MANDA PULSO DURANTE 100 ms POR PIN_A5 AL PIC16F88//
                output_high(PIN_A5);  
                delay_ms(100);        
                output_low(PIN_A5);
                
                tecla_pulsada = 'L';  //Reinicio la variable tecla_pulsada al valor conjunto vacío//
                
                break;
            
            case '2':  // BOTÓN ARRANCAR/PARAR TIEMPO POSESIÓN //
                
                //SE MANDA PULSO DURANTE 100 ms POR PIN_B6 AL PIC16F88//
                output_high(PIN_B6);
                delay_ms(100);
                output_low(PIN_B6);
                
                tecla_pulsada = 'L';   //Reinicio la variable tecla_pulsada al valor conjunto vacío//
                
                break;
                
            case '3':  //BOTÓN REINICIO POSESIÓN A 14 s//
                
                //SE MANDA PULSO DURANTE 100 ms POR PIN_C7 AL PIC16F88//
                output_high(PIN_C7);
                delay_ms(100);
                output_low(PIN_C7);
                
                tecla_pulsada = 'L';   //Reinicio la variable tecla_pulsada al valor conjunto vacío//
                
                break;
                
            case '4': // BOTÓN ANOTACIÓN EQUIPO LOCAL //
                
                if(Estado[0] != INICIO)   //Si el estado activo es INICIO no hacer nada//
                {
                    Puntos_local ++;
		}
                tecla_pulsada = 'L';   //Reinicio la variable tecla_pulsada al valor conjunto vacío//
                
                break;
             
            case '5': // BOTÓN FALTAS LOCAL //
                
                Faltas_local ++;
	       if (Faltas_local>4){
                Estado[1]=Estado[0];  //Desplazo tabla y pongo de estado activo PARADO ya que el tiempo debe permanecer parado hasta que se reanude el partido y se de al botón START//
                Estado[0]=PARADO;
	       }
                tecla_pulsada = 'L';   //Reinicio la variable tecla_pulsada al valor conjunto vacío//
                
                break;
                
            case '6': // BOTÓN FALTAS VISITANTE //
                
                 Faltas_visitante ++;
                
              	       if (Faltas_local>4){
                Estado[1]=Estado[0];  //Desplazo tabla y pongo de estado activo PARADO ya que el tiempo debe permanecer parado hasta que se reanude el partido y se de al botón START//
                Estado[0]=PARADO;
	       }
                 tecla_pulsada = 'L';   //Reinicio la variable tecla_pulsada al valor conjunto vacío//
                 
                break;
                
            case '7': // BOTÓN ANOTACIÓN EQUIPO VISITANTE //
                
                if(Estado[0] != INICIO)   //Si el estado activo es INICIO no hacer nada//
                {
                    Puntos_visitante ++;
                }
                               
                tecla_pulsada = 'L';   //Reinicio la variable tecla_pulsada al valor conjunto vacío//
                
                break;
                
            case '8': // BOTÓN RESTA DE PUNTOS LOCAL //
                
                if(Puntos_local > 0)   //Si los puntos del equipo local son 0 no se debe restar (no hay puntos negativos)//
                {
                    
                   Puntos_local --;
                   /*
                   if(Puntos_local < 10)  //Si los puntos del equipo local son menores de 10, borrar casilla de su derecha//
                   {
                        lcd_gotoxy(4,2);
                        printf(lcd_putc," ");
                   }
                   
                   if(Puntos_local < 100) //Si los puntos del equipo local son menores de 100, borrar casilla de su derecha//
                   {
                        lcd_gotoxy(5,2);
                        printf(lcd_putc," ");
                   }
		   */ // se va a sustituir por un clear lcd
                }
                
                tecla_pulsada = 'L';   //Reinicio la variable tecla_pulsada al valor conjunto vacío//
                
                break;
                
            case '9': // BOTÓN TIEMPO MUERTO EQUIPO LOCAL //
                
                if (Tiempo_muerto_local != 0)  //Mientras el número de tiempos muertos del equipo local no sea 0 se restará uno al número//
                {                              //de tiempos muertos disponibles del equipo local se pasa al estado TIEMPO MUERTO//
                    Tiempo_muerto_local--;
                    
                    Estado[1]=Estado[0];  //Desplazo tabla//
                    Estado[0] = TIEMPO_MUERTO;  
                } 
                
                tecla_pulsada = 'L';   //Reinicio la variable tecla_pulsada al valor conjunto vacío//
                
                break;
                
            case 'A': // BOTÓN TIEMPO MUERTO EQUIPO VISITANTE //
                
                if (Tiempo_muerto_visitante != 0)  //Mientras el número de tiempos muertos del equipo visitante no sea 0 se restará uno al número//
                {                                  //de tiempos muertos disponibles del equipo local se pasa al estado TIEMPO MUERTO//
                    Tiempo_muerto_visitante--;
                    
                    Estado[1]=Estado[0];
                    Estado[0] = TIEMPO_MUERTO;
                } 
                
                tecla_pulsada = 'L';    //Reinicio la variable tecla_pulsada al valor conjunto vacío//
                
                break;
                
            case 'B': // BOTÓN RESTA DE PUNTOS VISITANTE //
                
                if(Puntos_visitante != 0)    //Si los puntos del equipo visitante son 0 no se debe restar (no hay puntos negativos)//
                {
                  
                    Puntos_visitante --;
                }
                              
                tecla_pulsada = 'L';    //Reinicio la variable tecla_pulsada al valor conjunto vacío//
                
                break;
                
        }
       
                     delay_ms(1000);     
              lcd_gotoxy(16,1);
            printf(lcd_putc,"%c",tecla_pulsada);
            
            
     }
    } 
