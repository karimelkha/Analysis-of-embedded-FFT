/******************************************************************************
			PROJET 	S6
	EL-KHARROUBI 	GRAVES 	LEFEBVRE 	NEZET
*******************************************************************************/
#include "lpc17xx.h"
#include "system_lpc17xx.h"
#include "LCD.h"
#include "fft.h"
#include "stdlib.h"
#include "mbed.h"
#include "app.h"

int n = 0;
unsigned int data0;
short tab_ech[N];
char adc_over =0;

extern "C" void TIMER0_IRQHandler(){			//Routine d'interruption du Timer 0
		
	if((LPC_TIM0->IR & 0x01)){
		if(!adc_over){
			if(n < N){			//Rentre la valeur de data0 dans le tableau
					tab_ech[n++] = data0;
			}else{
				n = 0;
				adc_over |= 0x1;
			}
		}
	}
	LPC_TIM0->IR = 1;		//Acquitement de l'interruption

}


extern "C" void init_TIMER0(){				//Initialisation du Timer 1
		//Valeur du Match 0 pour le Timer 1
	LPC_TIM0->MR0 = (int)(PCLK/(Fe) - 1);
  NVIC_EnableIRQ(TIMER0_IRQn);
	LPC_TIM0->MCR = 0x3;			//Activation de l'interruption et reset sur MR0
  LPC_TIM0->TCR = 0x1;				//Activation du Timer
}

	
void ADC_init(void){
	/*Enable CLOCK into ADC controller*/
	LPC_SC->PCONP |= (1<<12);					//Permet d'alimenter l'ADC (par défaul il est pas alimenté)
	LPC_PINCON->PINSEL1 |= (1<<14) | (1<<16) | (1<<18);		// P0.23 en mode AD0.0  (Soit le port A0 sur lequel on doit brancher le module micro Grove)
	LPC_ADC->ADCR = (0x07<<0)|					// SEL=1, permet de sélectionner uniquement le channel 0
									(1<<8)|			 			// ADC_CLK < 13 MHz,ADC_CLK = FPCLK / (CLKDIV+1), ici CLKDIV = 1
									(1<<16)|					// BURST = 0, no BURST, software controlled
									(0<<17)|					/* CLKS = 0, 11 clocks/10 bits */
									(1<<21)|					/* PDN = 1, normal operation */
									(0<<24)|					// On laisse le start a zéro, on comencera à la fin
									(0<<27);					/* EDGE = 0 (CAP/MAT singal falling trigger A/D conversion) */
	LPC_ADC->ADINTEN = 0x01;		//Autorise les interruptions sur le channel 0 (uniquement)
	NVIC_EnableIRQ(ADC_IRQn);		//Autorise l'interruption de ce périphérique
	LPC_ADC->ADCR |= (1<<24);		//Commence une conversion immédiatement
}

extern "C" void ADC_IRQHandler (void){

	data0 = (LPC_ADC->ADDR0>>4) & 0xFFF; 	//Masque pour ne garder que le resultat sur 12 bit
	LPC_ADC->ADCR |= (1<<24);		//Commence une conversion immédiatement
	//NVIC_ClearPendingIRQ(ADC_IRQn);
}

int main(void){
	
	float * twi ;
	
	float  tf[N*2] = {0}; 
	float tmp,mx_amp,mx_f,sn_f;
	int i;
	char buf[LINE_NR_CHAR],q = (Fe/N )>>1;;
	init_LCD();
	init_RGB();
	color_RGB(1,1,1);
	ADC_init();	//Initialisation du convertisseur analogique/numérique et début de la conversion
	init_TIMER0();	//Initialisation du timer et début de l'echantillonnage
	
	LPC_TIM1->PR = (int)(PCLK/1e6-1); // Initialisation du prescaler à la ms
	
	mx_f = 100;
	if(N<2048)
		twi = get_twiddles(N);
	while(1){
		if(adc_over){
			//Démarrage de la mesure
			LPC_TIM1->TCR = 0x1;	
			if(N<2048)
				fftf_rdx4 (tab_ech,tf, N, twi);
			else{

				fftf_rdx2 (tab_ech,tf, N);
			}
			
			mx_f  = 0;
			mx_amp = 0;
		
			for(i = 2; i < N; i+=2){
                tmp = sqrtf(*(tf+i)**(tf+i) + I(tf+i)*I(tf+i));
                if(tmp > mx_amp){
                    mx_f = i;
                    mx_amp = tmp;
                }				
            }
			i2c_write_command_lcd(0x80);
						
			sprintf(buf,"Harm 1=%6.0f Hz", mx_f*q 	);	
			write_mult_lcd(buf, LINE_NR_CHAR);
			
			/* Une harmonique secondaire est multiple	*
			 * de l'harmonique principale							*
			 * On détermine un seuil arbitraire afin  *
			 *	d'éviter des perturbations liées			*
			 * 	au bruit															*/
		/*	mx_amp = 1024;
			sn_f = 0;
			for(i=mx_f*2;i<N;i+=mx_f){
					tmp = MOD_F(tf+i);
                if(tmp > mx_amp){
                    sn_f = i ;
                    mx_amp = tmp;
                }		
			} 		
				sprintf(buf,"Harm 2=%6.0f Hz", sn_f);	
						*/
					
			i2c_write_command_lcd(0xC0); 	
			
			sprintf(buf,"t =%10d %cs", (LPC_TIM1->TC ),228);				
			write_mult_lcd(buf, LINE_NR_CHAR);
					
			//	write_mult_lcd(buf, LINE_NR_CHAR);
			
				adc_over = 0;
				//Fin de la mesure et reset
				LPC_TIM1->TCR = 0x2;	
		}
	}
}

