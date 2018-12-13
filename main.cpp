#include <Arduino.h>

#define Debug
//#define Set_EEPROM
// #define Pid_Autotune

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0])) //для вычесления размеров структур

#include <math.h>
#include <PID_v1.h>
  #ifdef Pid_Autotune  
    #include <PID_AutoTune_v0.h>
  #endif
#include <EEPROM.h>
#include "HCMotor.h"
#include <Button.h>
#include <LiquidCrystal.h>


#include "util.h" //загружаем меню 
#include "init.h" //пины
#include "func.h"
#include "debug.h"

void setup() {
#ifdef Debug
    Serial.begin(115200);
#endif

Select_Button.begin();
Up_Button.begin();
Down_Button.begin();
    
  lcd.createChar(0, grad); //рисует градусник
  lcd.createChar(1, degreesymbol); //рисует Цельсий
  lcd.createChar(2, menuselect); //рисует треугольник выбора меню
  lcd.createChar(3, H_invert); //рисует символ включеноого нагревателя
  lcd.createChar(4, E_invert); //рисует символ включеноого мотора

  lcd.begin(16, 2);  

  pinMode(BUZZER_PIN, OUTPUT);    // пикалка
  digitalWrite(BUZZER_PIN, LOW);  //выключаем

  pinMode (HEAT_PIN, OUTPUT);  //нагреватель
  digitalWrite(HEAT_PIN, LOW); //выключаем

  
  
  //analogReference(EXTERNAL); //внешний референс от 3.3V для кнопок
  
  // Initialise motor library
  HCMotor.Init();  
  HCMotor.attach(0, STEPPER, MOTOR_CLK_PIN, MOTOR_DIR_PIN);
  HCMotor.Steps(0,CONTINUOUS);
  HCMotor.DutyCycle(0, 0); //выключаем мотор
  
  pinMode(MOTOR_EN_PIN, OUTPUT);
  pinMode(HEAT_RELLAY_PIN, OUTPUT);
  digitalWrite(MOTOR_EN_PIN, HIGH); 
  digitalWrite(HEAT_RELLAY_PIN, LOW); 

//PID
//Setpoint = 240; 
  windowStartTime = millis();    
  myPID.SetOutputLimits(0, TempSetup[6].Value); //tell the PID to range between 0 and the full window size    
  myPID.SetMode(AUTOMATIC); //turn the PID on

#ifdef Pid_Autotune 
//Autotune Pid
    if(PIDSetup[5].Value)
  {
    PIDSetup[5].Value=false;
    changeAutoTune();
    PIDSetup[5].Value=true;
  }
#endif 
 
if (byte a = EEPROM.read(9))
{
  //clear EEPROM
  for ( int i = 0 ; i < EEPROM.length() ; i++ )
  {
   EEPROM.write(i, 0); 
  }
  //запись данных в EEPROM
  #define SETEEPROM(submenu) setEEPROM(ARRAY_SIZE(submenu), submenu)
  SETEEPROM(TempSetup);
  SETEEPROM(MotorSetup);
  EEPROM.write(9,0); //сбрасываем флаг ресета
  lcd.clear(); 
  lcd.print("Factory reset");
  lcd.setCursor(0, 1); 
  lcd.print("Sucessfully");
  delay(2000);
}
else 
{
  //Value restore
  #define SETVALUE(submenu) setValue(ARRAY_SIZE(submenu), submenu)
  SETVALUE(TempSetup); //загружаем данные из EEPROM в Menu.Value
  SETVALUE(MotorSetup);
}

Set_Switch (ARRAY_SIZE(Switch), 0); //переключатели в Off  

//вычесляем размер основного меню
CountSizeMenu(Menu_Main);
MenuNowPos = 1;

//рисуем экран  
pageInfo(); //заставка
delay(2000);
pageMain(); //основной экран
}

void loop() 
{     
  #ifdef Debug
    SerialRead(); //вместо кнопок (временно)
  #endif  

     
     Buttons(); //buttons check
     Motor(); //управление моторами
     Heater(); //управление нагревом

     
    if (millis() - previousMillis_Temp > interval_Temp) 
    {     
        previousMillis_Temp = millis();           
        tempRead(analogRead(THERMISTOR_PIN)); //считыванием температуру   
        if (CurrentMenu==Menu_Main && CurrentMenu[0].handler) CurrentMenu[MenuNowPos].handler(); //рисуем меню
    }   
    
    //флаги температуры и матюгальника
    if (ActualTempC >= TempSetup[1].Value) 
    {
      TempReacheState = true;
      if (!BeepState) Beep(3,10);
      BeepState = true;
    }
    else if (ActualTempC < MotorSetup[3].Value) 
    {
      TempReacheState = false;
      if (BeepState) Beep(4,10);
      BeepState = false;
    }

}

