#include "mbed.h"
#include "app.h"
#include "ctype.h"
#include <stdio.h>

#define addr 0x7C
#define addr_RGB 0xC4

I2C i2c(P0_27,P0_28);
  
 void i2c_write_command_lcd(char com){
     
		 char cmd[2];
     cmd[0] = 0x80;
     cmd[1] = com;
     i2c.write(addr, cmd, 2);
}
 
 void i2c_write_data_lcd(char data){
    
    char cmd[2];
    cmd[0] = 0x40;
    cmd[1] = data;    
    i2c.write(addr, cmd, 2);  
}
 //facilite l'envoi de commande sur le LCD
void write_mult_lcd(char * data_to_send,int nb){
    for(int i = 0; i< nb; i++){
        i2c_write_data_lcd(data_to_send[i]);
    }
}
 
 /* Retourne le nombre de chiffre dans
* une chaîne pour la formater adéquatement pour 
* l'écran LCD
* 0 : aucune chiffre ne situe dans la chaîne
*/
int get_nb_digit(char * str, int size){
    for(int i =0; i<size;i++){
        if (!isdigit(str[i])
					&& (str[i] != ',' )
				  && (str[i] != '.' ))
            return i;
    }
   return 0;
}
 // Facilite la création des commandes
 char** set_cmds(int nb){
     char ** cmd =(char**) malloc(sizeof(char*)*nb);
     
     for(int i = 0 ; i < nb ; i ++){
        cmd[i] = (char*)malloc(sizeof(char)*2);
     }
     return cmd;
}
 // Facilite la destruction des commandes
void destruct_cmds( char ** to_destruct,int nb){

     for(int i = 0 ; i < nb ; i ++){
        free(to_destruct[i]); 
     }
     free(to_destruct);
}
void color_RGB(int r, int g, int b){
   
    char ** color = set_cmds(3) ;
    color[0][0] = 0x04;
    color[1][0] = 0x03;
    color[2][0] = 0x02;
    
    if(r == 1)
         color[0][1] = 0xff;
    else
        color[0][1] = 0x00;
        
    if(g == 1)
        color[1][1] = 0xff;
    else
        color[1][1] = 0x00;
        
    if(b == 1)
        color[2][1] = 0xff;
    else
        color[2][1] = 0x00;
    
        i2c.write(addr_RGB, color[0], 2);
        i2c.write(addr_RGB, color[1], 2);
        i2c.write(addr_RGB, color[2], 2);
        
    destruct_cmds(color,3);
}  

 void init_RGB(){
     
     char** cmd = set_cmds(2) ;   
	   cmd[0][0] = 0x0;
     cmd[0][1] = 0x00;
     /* Accès au registre LEDOUT */
     cmd[1][0] = 0x08;
     /* Chaque LED peut avoir sa luminosité
      contrôlée en accédant à son registre MLI*/
     cmd[1][1] = 0xAA;
     i2c.write(addr_RGB, cmd[0], 2);
	   i2c.write(addr_RGB, cmd[1], 2);
     destruct_cmds(cmd,2);
} 
 void init_LCD(){
   /*On réinitialise l'écran */
     i2c_write_command_lcd(0x80);
     
     /* Autorisation d'écriture sur deux lignes*/
     i2c_write_command_lcd(0x28);
     
     /*Configuration de l'écran */
     i2c_write_command_lcd(0x0C);
    
    /* Adresse de la CGRAM*/
     i2c_write_command_lcd(0x01);
          
     /*Adresse de la CGRAM*/
     i2c_write_command_lcd(0x06);
}
 



void display(){
	char msg_1[LINE_NR_CHAR +1] = "Appuyez sur le  "
		, msg_2[LINE_NR_CHAR +1] = "bouton ( > 1s ) " ;
	//On reset l'écran
	i2c_write_command_lcd(0x80);
	write_mult_lcd(msg_1,LINE_NR_CHAR );
	//On affiche sur la deuxième ligne
	i2c_write_command_lcd(0xC0); 
	color_RGB(1, 0,0);
	printf(msg_2,"bouton ( > 1s ) ");
	write_mult_lcd(msg_2,LINE_NR_CHAR );
}
