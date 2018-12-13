#include <Arduino.h>

#define Debug
//#define Set_EEPROM
// #define Pid_Autotune

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0])) //для вычесления размеров структур

#include <math.h>
#include <EEPROM.h>
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

  lcd.createChar(2, menuselect); //рисует треугольник выбора меню

  lcd.begin(16, 2);  

  pinMode(BUZZER_PIN, OUTPUT);    // пикалка
  digitalWrite(BUZZER_PIN, LOW);  //выключаем

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
    
    // if (millis() - previousMillis_Temp > interval_Temp) 
    // {     
    //     previousMillis_Temp = millis();           
    //     tempRead(analogRead(THERMISTOR_PIN)); //считыванием температуру   
    //     if (CurrentMenu==Menu_Main && CurrentMenu[0].handler) CurrentMenu[MenuNowPos].handler(); //рисуем меню
    // }   
    
    //флаги температуры и матюгальника
    // if (ActualTempC >= TempSetup[1].Value) 
    // {
    //   TempReacheState = true;
    //   if (!BeepState) Beep(3,10);
    //   BeepState = true;
    // }
    // else if (ActualTempC < MotorSetup[3].Value) 
    // {
    //   TempReacheState = false;
    //   if (BeepState) Beep(4,10);
    //   BeepState = false;
    // }

}

