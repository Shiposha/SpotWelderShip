#ifndef INIT_H
#define INIT_H

//Pins
#define MOTOR_CLK_PIN A2      
#define MOTOR_DIR_PIN A3
#define MOTOR_EN_PIN A1
#define HEAT_PIN           A0 //пин нагревателя
#define HEAT_RELLAY_PIN    2 //пин реле нагревателя
#define ZERO_CROSS_PIN     3
#define BUZZER_PIN         11 //пин матюкалки
#define THERMISTOR_PIN     A6 //пин термистора

LiquidCrystal lcd(4,5,6,7,8,9); // rs, en, d4, d5, d6, d7

//MOtors
HCMotor HCMotor;

Button Select_Button(12);
Button Up_Button(10); 
Button Down_Button(13);
 
//PID
double ActualTempC;
double Output;

//PID Autotune
#ifdef Pid_Autotune
  byte ATuneModeRemember=2;
  double aTuneStep=50, aTuneNoise=1, aTuneStartValue=100;
  unsigned int aTuneLookBack=20;
#endif

//Specify the links and initial tuning parameters
PID myPID(&ActualTempC, &Output, &TempSetup[1].Value, TempSetup[3].Value, TempSetup[4].Value, TempSetup[5].Value, DIRECT);

#ifdef Pid_Autotune 
  PID_ATune aTune(&ActualTempC, &Output);
#endif

unsigned long windowStartTime; //for PID

//Termistor
#define TEMPTABLE_LEN (sizeof(ThermistorTempTable)/sizeof(*ThermistorTempTable))
#define PGM_RD_W(x)   (short)pgm_read_word(&x)

//переменные для опроса температуры
long previousMillis_Temp;
long interval_Temp = 500; // интервал опроса датчика температуры

//Menu
struct Menu_Struct * CurrentMenu = Menu_Main; //начальное меню

//soft reboot Arduino
void(* reboot) (void) = 0; 

bool TempReacheState = false; //флаг - температура достигла утановочного значения
bool BeepState = false; //флаг - матюгальника

#endif