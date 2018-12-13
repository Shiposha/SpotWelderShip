#ifndef INIT_H
#define INIT_H

//Pins
#define BUZZER_PIN         11 //пин матюкалки

LiquidCrystal lcd(4,5,6,7,8,9); // rs, en, d4, d5, d6, d7

Button Select_Button(12);
Button Up_Button(10); 
Button Down_Button(13);

//Menu
struct Menu_Struct * CurrentMenu = Menu_Main; //начальное меню

//soft reboot Arduino
void(* reboot) (void) = 0; 

bool BeepState = false; //флаг - матюгальника

#endif