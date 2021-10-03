/* LEFEBVRE Vincent EISE 3 TP3
 * Contient les constantes
 * et fonctions de l'application
 */

#ifndef __APP_H__
#define __APP_H__
#define ADC_RESOLUTION 1024
#define LINE_NR_CHAR 16
#define STEP 8
#define PCLK 24e6
#define Fe 41920//Fr�quence d'echantillonnage du tableau, soit la vitesse d'interruption du timer 0
#define N 1024
//Pas besoin de racine carr�e car on d�sire juste comparer les modules
#define MOD_F(n) ((*n**n+I(n)*I(n) ))
#endif

