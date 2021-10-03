# ifndef __LCD_H__
#define  __LCD_H__

void i2c_write_command_lcd(char com);
void i2c_write_data_lcd(char data);
void init_LCD();
void display(char state,double * dB);
void init_RGB();
void welcome();
void results(unsigned char run,unsigned char* res,double * db,unsigned char age);
void color_RGB(int r, int g, int b);
void write_mult_lcd(char * data_to_send,int nb);
#endif
