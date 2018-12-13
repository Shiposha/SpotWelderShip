#include <Arduino.h>

#define Debug
//#define Set_EEPROM
// #define Pid_Autotune

#include <EEPROM.h>
#include <Button.h>
#include <LiquidCrystal.h>

#include "util.h" //загружаем меню 
#include "init.h" //пины
#include "func.h"
#include "menu.h"
#include "debug.h"

void setup() {
#ifdef Debug
    Serial.begin(115200);
#endif

Select_Button.begin();
Up_Button.begin();
Down_Button.begin();

lcd.begin(16, 2);
lcd.createChar(0, menuselect); //рисует треугольник выбора меню
  
pinMode(BUZZER_PIN, OUTPUT);    // пикалка
digitalWrite(BUZZER_PIN, LOW);  //выключаем

if (EEPROM.read(9)) //9 - reset flag
{
  //clear EEPROM
  for ( int i = 0 ; i < EEPROM.length() ; i++ )
  {
   EEPROM.write(i, 0); 
  }
  //запись данных в EEPROM
  #define SETEEPROM(submenu) setEEPROM(ARRAY_SIZE(submenu), submenu)
  SETEEPROM(WelderSetup);
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
  SETVALUE(WelderSetup); //загружаем данные из EEPROM в Menu.Value
}

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
 
     Buttons_Scan(); //buttons check
    
    // if (millis() - previousMillis_Temp > interval_Temp) 
    // {     
    //     previousMillis_Temp = millis();           
    //     tempRead(analogRead(THERMISTOR_PIN)); //считыванием температуру   
        if (CurrentMenu==Menu_Main && CurrentMenu[0].handler) CurrentMenu[MenuNowPos].handler(); //рисуем меню
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

