#define inicio 0
#define cuenta 1
#define tiempo 2
#define parada 3 
estado=inicio;
int min=1;
int seg=0;
signed double deci=99;
int marcador_vis=0;
int marcador_loc=0;
#INT_TIMER1
void  TIMER1_isr(void) 
{   deci--
if (deci<=0){
    seg=seg-1;
    deci=99;  
}
if {

}
    set_timer1();
}

void main()
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
    while(true)
    {switch(estado)
    {
        case(inicio):
            disable_interrupts(INT_TIMER1);
        break;
        case(cuenta):
            enable_interrupts(INT_TIMER1);
            while(estado==cuenta){
                // muestra LCD tiempo 
                if (min>0){}
                lcd_gotoxy(1,1);
                printf(lcd_putc,"%d     %d:%d     %d",marcador_loc,min,marcador_vis);
                }
                if {min<=0}{
                    lcd_gotoxy(1,1);
                printf(lcd_putc,"%d     %d:%d     %d",marcador_loc,seg,deci,marcador_vis);
                }
            
        break;
        case(tiempo);
        
        break;
        case(parada):

        break;
    }
    }



    