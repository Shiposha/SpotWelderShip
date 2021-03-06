#ifndef UTIL_H
#define UTIL_H

//прототипы функций 
void SetDef_EEPROM();
void pageInfo();
void pageMain();
void printMenu();
void debugVarPrint();

//****************** MENU *******************
double IncDec[] =  //it is for Type from Menu_Struct 
  { 
      0, // 0 
      1, // 1
      1, // 2 
     10, // 3
     60, // 4 
    100, // 5 
    0.1, // 6
   0.01, // 7
  -0.01, // 8
   -0.1, // 9
     -1, // 10
  };  

uint8_t MenuNowPos = 0; //текущий пункт меню
uint8_t MenuDrawPos = 0; //выделенный пункт
uint8_t MenuDrawCount = 2; //сколько меню на экране
uint8_t MenuDrawI = 0;//для вывода пунктов меню на экран
uint8_t MenuEdit = 0; //если не ноль то редактируемое поле
uint8_t MenuSize = 0; //количество пунктов в меню и загрузка из EEPROM

typedef struct Menu_Struct 
{
  char *Name; //имя меню
  struct Menu_Struct *submenu; //подменю данного элемента
  uint8_t Type;  //increment decrement form IncDec Structure
  int Range;     // 
  double Value;  //default value
  uint8_t EepromAdrr;
  void (*handler)(); //обработчик входа в меню (можем не иметь подменю но вызвать ф-цию)  
} MENU;


//Factory setting
MENU Reset[] =  
{
  {"Are You Shure?",  NULL,  NULL, NULL, NULL, NULL, NULL},//заголовок
  {"Yes",             NULL,  NULL, NULL,    0,    9, SetDef_EEPROM},//1
  {NULL}
};


//Установки температуры
MENU WelderSetup[] =  
{
  {"<<<",              NULL,  NULL, NULL, NULL, NULL, NULL},//заголовок
  {"Pulse duration:",  NULL,  2,    300,  180,  10,   NULL},//1
  {"Pulses number:",   NULL,  9,    5,    0,    14,   NULL},//2
  {"Shift time:",      NULL,  2,    250,  100,  18,   NULL},//3
  {"Beep",             NULL,  1,    1,    0,    5,    NULL},//4
  {"Factory reset",    Reset, NULL, NULL, NULL, NULL, NULL},//5
  {NULL}
};

//Информация
MENU Menu_Main[] =  
{
  {"",       NULL,        NULL, NULL, NULL, NULL, pageInfo}, 
  {"",       WelderSetup, NULL, NULL, NULL, NULL, pageMain}, 
  {NULL}
};

///Символы для LCD
//символ выбранного меню
byte menuselect[8] = 
{
  0b10000,
  0b11000,
  0b11100,
  0b11110,
  0b11100,
  0b11000,
  0b10000,
  0b00000
};

#endif