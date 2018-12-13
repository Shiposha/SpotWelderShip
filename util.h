#ifndef UTIL_H
#define UTIL_H

//прототипы функций 
void SetDef_EEPROM();
void pageInfo();
void pageMain();
void debugVarPrint();

//****************** MENU *******************
double IncDec[] = 
  { 
      0, // 0 
      1, // 1 boolean
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
  uint8_t Type;
  int Range;
  double Value;
  uint8_t EepromAdrr;
  void (*handler)(); //обработчик входа в меню (можем не иметь подменю но вызвать ф-цию)  
} MENU;


//Установки
MENU Reset[] =  
{
  {"Are You Shure?",  NULL,  NULL, NULL, NULL, NULL, NULL},//заголовок
  {"Yes",             NULL,  NULL, NULL,    0,    9, SetDef_EEPROM},//1
  {NULL}
};


//Установки температуры
MENU TempSetup[] =  
{
  {"<<<",             NULL,  NULL,NULL, NULL, NULL, NULL},//заголовок
  {"SetTemp",         NULL,  2,   300,  180,  10,   NULL},//1
  {"Temp corr",       NULL,  9,   5,    0,    14,   NULL},//2
  {"PID P",           NULL,  2,   250,  100,  18,   NULL},//3
  {"PID I",           NULL,  10,  100,  1,    22,   NULL},//4
  {"PID D",           NULL,  10,  100,  10,   24,   NULL},//5
  {"PID W",           NULL,  5,   10000,1500, 28,   NULL},//6
#ifdef Pid_Autotune  
  {"PID Auto",        NULL,  1,   1,    0,    32,   NULL},//7 
#endif    
  {NULL}
};


//Установки скорости моторов
MENU MotorSetup[] =  
{
  {"<<<",             NULL,  NULL,NULL, NULL, NULL, NULL},//заголовок
  {"Mtr Spd",         NULL,  2,   200,  50,   40,   NULL},//1
  {"Mtr Drct",        NULL,  1,   1,    0,    44,   NULL},//2
  {"Temp off",        NULL,  2,   100,  80,   48,   NULL},//3
  {NULL}
};

//Основное меню
MENU Switch[] =  
{
  {"<<<",           NULL,  NULL, NULL, NULL, NULL, NULL}, //заголовок 
  {"Heat Start",    NULL,  1,    1,    0,    1,    NULL},// 1
  {"Cool Start",    NULL,  1,    1,    0,    1,    NULL},// 2  
  {"Reverse",       NULL,  1,    1,    0,    2,    NULL},// 3
  {"Beep",          NULL,  1,    1,    0,    5,    NULL},// 4
  {"Factory reset", Reset, NULL, NULL, NULL, NULL, NULL},// 5
  {NULL}
};

//Информация
MENU Menu_Main[] =  
{
  {"",       NULL,       NULL, NULL, NULL, NULL, pageInfo}, 
  {"",       Switch,       NULL, NULL, NULL, NULL, pageMain}, 
 // {"",       Switch,     NULL, NULL, NULL, NULL, pageSwitch},
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