#ifndef FUNC_H
#define FUNC_H

//вычесляем размер дочернего массива
void CountSizeMenu(struct Menu_Struct* Size_Submenu) 
{ 
  CurrentMenu = Size_Submenu; //сщхраняем текущее меню меню
  MenuSize = 0;

  while (Size_Submenu->Name)
  {   
    ++Size_Submenu;
    ++MenuSize;  
  }    
  MenuSize -=1;
}


//****************** TEMPERATURE *******************

// опрос датчика и получения сведений о температур

static float tempRead(int raw) {
    byte i;
 
    for (i = 1; i < TEMPTABLE_LEN; i++)
    {
        if (PGM_RD_W(ThermistorTempTable[i][0]) > raw)
        {
            ActualTempC = PGM_RD_W(ThermistorTempTable[i - 1][1]) +
                (raw - PGM_RD_W(ThermistorTempTable[i - 1][0])) *
                (float)(PGM_RD_W(ThermistorTempTable[i][1]) - PGM_RD_W(ThermistorTempTable[i - 1][1])) /
                (float)(PGM_RD_W(ThermistorTempTable[i][0]) - PGM_RD_W(ThermistorTempTable[i - 1][0]));
            break;
        }
    }
 
    // Overflow: Set to last value in the table
    if (i == TEMPTABLE_LEN) ActualTempC = PGM_RD_W(ThermistorTempTable[i - 1][1]);
}


//****************** PID *******************

void HEAT_PID()
{
myPID.SetSampleTime(TempSetup[6].Value);

#ifdef Pid_Autotune  
  void AutoTuneHelper(boolean start)
  {
    if(start)
      ATuneModeRemember = myPID.GetMode();
    else
      myPID.SetMode(ATuneModeRemember);
  }

    if (PIDSetup[5].Value)
      {
        byte val = (aTune.Runtime());
        if (val!=0)
        {
          PIDSetup[5].Value = false;
        }
        if(!PIDSetup[5].Value)
        { //we're done, set the tuning parameters
          TempSetup[3].Value = aTune.GetKp();
          TempSetup[4].Value = aTune.GetKi();
          TempSetup[5].Value = aTune.GetKd();
          myPID.SetTunings(TempSetup[3].Value,TempSetup[4].Value,TempSetup[5].Value);
          AutoTuneHelper(false);
        }
      }
    else myPID.Compute();  
#else      
    myPID.SetTunings(TempSetup[3].Value, TempSetup[4].Value, TempSetup[5].Value);   
    myPID.Compute();
#endif    

  if(Output < millis() - windowStartTime) digitalWrite(HEAT_PIN, LOW);
  else digitalWrite(HEAT_PIN, HIGH);

  if(millis() - windowStartTime>TempSetup[6].Value)
  { //time to shift the Relay Window
    windowStartTime += TempSetup[6].Value;
  } 
}

#ifdef Pid_Autotune  

  void changeAutoTune()
  {
   if(!PIDSetup[5].Value)
    {
      //Set the output to the desired starting frequency.
      Output=aTuneStartValue;
      aTune.SetNoiseBand(aTuneNoise);
      aTune.SetOutputStep(aTuneStep);
      aTune.SetLookbackSec((int)aTuneLookBack);
      AutoTuneHelper(true);
      PIDSetup[5].Value = true;
    }
    else
    { //cancel autotune
      aTune.Cancel();
      PIDSetup[5].Value = false;
      AutoTuneHelper(false);
    }
  }

#endif

//****************** EEPROM & FLAGS *******************

//сохрание значений в EEPROM
void UpdateEeprom() 
{
  if (CurrentMenu[MenuNowPos].Type == 1) CurrentMenu[MenuNowPos].Value =  ((bool)CurrentMenu[MenuNowPos].Value % 2);
  EEPROM.put(CurrentMenu[MenuNowPos].EepromAdrr, CurrentMenu[MenuNowPos].Value);    
}

//set Switches
void Set_Switch(byte Size, bool off)
{
  for(byte i=1; i<Size; i++)
  {
    if (Switch[i].Type) Switch[i].Value = off;
  }
}

//set Value
void setValue(byte setVal_size, struct Menu_Struct* setVal)
{
  for(byte i=1; i<setVal_size; i++)
  {
    EEPROM.get(setVal[i].EepromAdrr, setVal[i].Value);        
  }
}

//SetDef_EEPROM
void SetDef_EEPROM()
{ 
  EEPROM.write(9, 1); //флаг заводских установок           
  reboot(); //reboot Arduino   
}

//set EEPROM 
void setEEPROM(byte setRom_size, struct Menu_Struct* setRom)
{
  for(byte i=1; i<setRom_size; i++)
  { 
    EEPROM.put(setRom[i].EepromAdrr, setRom[i].Value);  
  }
}

//****************** MOTOR *******************
void Motor()
{
   if (Switch[3].Value) HCMotor.Direction(0, REVERSE);
   else HCMotor.Direction(0, FORWARD);
  
  if (Switch[1].Value || Switch[2].Value) {digitalWrite(MOTOR_EN_PIN, LOW); HCMotor.DutyCycle(0, MotorSetup[1].Value);} 
  else if (ActualTempC > MotorSetup[3].Value) {digitalWrite(MOTOR_EN_PIN, LOW); HCMotor.DutyCycle(0, MotorSetup[1].Value);}
  else {digitalWrite(MOTOR_EN_PIN, HIGH                    ); HCMotor.DutyCycle(0, 0);}
}

//****************** HEAT *******************
void Heater()
{
  if (Switch[1].Value) {digitalWrite(HEAT_RELLAY_PIN, HIGH); HEAT_PID();} //управляет нагревом
  else {digitalWrite(HEAT_RELLAY_PIN, LOW); digitalWrite(HEAT_PIN, LOW);} //вырубает нагрев
}

//****************** SOUND *******************
void Beep(byte NumBeep, int Period)
{
  for (byte i = 0; i < NumBeep; i++)
  {
      digitalWrite (BUZZER_PIN, HIGH);
      delay (Period);
      digitalWrite(BUZZER_PIN, LOW);
      delay(75);
  }
}


//****************** KEYS *******************

double Increment(double trand)
{
      if (trand > 0 && CurrentMenu[MenuNowPos].Value <=  CurrentMenu[MenuNowPos].Range-0.1)       
         CurrentMenu[MenuNowPos].Value += IncDec[CurrentMenu[MenuNowPos].Type];         
      else if (trand < 0 && CurrentMenu[MenuNowPos].Value <= CurrentMenu[MenuNowPos].Range-0.1)       
         CurrentMenu[MenuNowPos].Value -= IncDec[CurrentMenu[MenuNowPos].Type];            
}
double Decrement(double trand)
{
      if (trand > 0 && CurrentMenu[MenuNowPos].Value > 0.1)       
         CurrentMenu[MenuNowPos].Value -= IncDec[CurrentMenu[MenuNowPos].Type]; 
                  
      else if (trand < 0 && CurrentMenu[MenuNowPos].Value >= -CurrentMenu[MenuNowPos].Range+0.1)       
         CurrentMenu[MenuNowPos].Value += IncDec[CurrentMenu[MenuNowPos].Type]; 
}

//кнопка "вверх"
void keyUp() 
{
  if (MenuEdit == 0)
  {
    if (MenuNowPos > 0)
    {
      --MenuNowPos;
      if (MenuDrawPos) --MenuDrawPos;
    }
  }
  //находимся в режиме редактирования
  else  Increment(IncDec[CurrentMenu[MenuNowPos].Type]);
}

//кнопка "вниз"
void keyDown() 
{
  if (MenuEdit == 0)
  {
    if (MenuNowPos < MenuSize)
    {
      ++MenuNowPos;
      //если в край экрана не упираемся в край экрана то сдвигаем позицию на 1 пункт вниз  
      if ((MenuDrawPos < MenuDrawCount - 1) && (MenuDrawPos < MenuSize)) ++MenuDrawPos;     
    }
  }
  //находимся в режиме редактирования
  else Decrement(IncDec[CurrentMenu[MenuNowPos].Type]);
}

//кнопка "Выбор"
void keySelect() {
  if (MenuEdit == 0)
  {
      if (CurrentMenu[MenuNowPos].submenu && MenuNowPos)
      {
        CurrentMenu[MenuNowPos].submenu->submenu = CurrentMenu; //сохраняем адрес текущего массива в 0м адресе дочернего массива
        CurrentMenu[MenuNowPos].submenu->Range = MenuNowPos; //сохраняем текущую позицию курсора
        CurrentMenu[MenuNowPos].submenu->Value = MenuDrawPos; //сохраняем текущую позицию меню 
        CountSizeMenu(CurrentMenu[MenuNowPos].submenu); //размер меню
        MenuNowPos = 0; //0 - название меню, 1 - 1й пункт меню
        MenuDrawPos = 0; //ставим курсор на 1й пунка


      }
      else if (CurrentMenu[MenuNowPos].submenu && !MenuNowPos)
      {
        MenuNowPos = CurrentMenu[0].Range; //востанавливаем позицию меню
        MenuDrawPos = CurrentMenu[0].Value; //востанавливаем позицию курсора
        CountSizeMenu(CurrentMenu[0].submenu); //размер меню          
        
      }
      else if (CurrentMenu[MenuNowPos].handler) CurrentMenu[MenuNowPos].handler();
      
      else if (CurrentMenu[MenuNowPos].Type == 1) CurrentMenu[MenuNowPos].Value = !CurrentMenu[MenuNowPos].Value;

      else if (CurrentMenu[MenuNowPos].Type) MenuEdit = 1;
  }
  else 
  {
    //обновление значание
    UpdateEeprom();
    MenuEdit = 0;    
  }
}

//print "0" func
void spaceprint(byte value) 
{
  if(value<10)lcd.print("0");
}

const char* OnOff(bool value) 
{
  return value ? "on" : "off";
}

//MENU          
void printMenu()
  {  
    lcd.clear();
    //цикл для вывода пунктов меню на экран
    for (MenuDrawI = 0; MenuDrawI < MenuDrawCount; MenuDrawI++)
    {  
      lcd.setCursor(1, MenuDrawI);      
      lcd.print(CurrentMenu[MenuNowPos - MenuDrawPos + MenuDrawI].Name);      
          //Если пункт меню бинарный
          if (CurrentMenu[MenuNowPos - MenuDrawPos + MenuDrawI].Type == 1)
          {
            lcd.setCursor(13, MenuDrawI);
            lcd.print(OnOff(CurrentMenu[MenuNowPos - MenuDrawPos + MenuDrawI].Value));
          }
          //Если пункт меню int
          else if (CurrentMenu[MenuNowPos - MenuDrawPos + MenuDrawI].Type > 1 && 
                   CurrentMenu[MenuNowPos - MenuDrawPos + MenuDrawI].Type <= 5) 
          {
            lcd.setCursor(11, MenuDrawI);
            lcd.print((int)CurrentMenu[MenuNowPos - MenuDrawPos + MenuDrawI].Value);     
          }          
          //Если пункт меню float
          else if (CurrentMenu[MenuNowPos - MenuDrawPos + MenuDrawI].Type > 5) 
          { 
            lcd.setCursor(11, MenuDrawI);       
            if(fabs(CurrentMenu[MenuNowPos - MenuDrawPos + MenuDrawI].Value) < 0.01)
              lcd.print((int)CurrentMenu[MenuNowPos - MenuDrawPos + MenuDrawI].Value);
            else 
              lcd.print(CurrentMenu[MenuNowPos - MenuDrawPos + MenuDrawI].Value,1);
          }
     }
      //рисуем треугольник выделенного меню
      lcd.setCursor(0, MenuDrawPos);
      if (MenuEdit == 1) lcd.blink();
      else {lcd.write(byte(2)); lcd.noBlink();}     
  }


void Menu_Beep()
{
  if (CurrentMenu==Menu_Main && CurrentMenu[0].handler) CurrentMenu[MenuNowPos].handler();
  else printMenu();   
  if (Switch[4].Value) Beep(0,10); //матюгальник      
}

void Buttons()
{
  if (Select_Button.pressed()) {keySelect(); Menu_Beep();}
  if (Up_Button.pressed()) {keyUp(); Menu_Beep();}
  if (Down_Button.pressed()) {keyDown(); Menu_Beep();}
}

// ВЫВОД ИНФОРМАЦИИ НА LCD
void pageInfo()
    {  
      lcd.clear();
      lcd.print("Laminator Ship");        
      lcd.setCursor(0, 1);      
      lcd.print("v1.0");
    }

void pageMain()
    {  
      lcd.clear();
      lcd.write(byte(0));
      lcd.print((int)ActualTempC); 
      lcd.print("/"); 
      lcd.print((int)TempSetup[1].Value);     
      lcd.write(byte(1));
      
      lcd.setCursor(0, 1); 

      if (Switch[1].Value && TempReacheState) lcd.print("Ready...");       
      else if (Switch[1].Value) lcd.print("Hearting...");  
      else if (!Switch[1].Value && ActualTempC > MotorSetup[3].Value) lcd.print("Cooling...");
      else if (Switch[2].Value) lcd.print("Cool Laminate...");
      else lcd.print("Standby...");

        
//отображаем символы
      lcd.setCursor(13, 0);      
      if (Switch[1].Value) lcd.write(byte(3));     
      else lcd.print("H");      

      if (Switch[1].Value || Switch[2].Value || ActualTempC > MotorSetup[3].Value) lcd.write(byte(4));
      else lcd.print("M");
      
      if (Switch[3].Value) lcd.print("<");
      else lcd.print(">");            

      lcd.setCursor(13, 0); 
      if (Switch[1].Value && ActualTempC < TempSetup[1].Value) lcd.blink();
      else lcd.noBlink();    
    }

void pagePID()
    {  
      lcd.clear();
      lcd.print("Kp "); 
      lcd.print((int)TempSetup[3].Value);  
      lcd.setCursor(8, 0);
      lcd.print("Ki ");  
      lcd.print(TempSetup[4].Value,1); 
      lcd.setCursor(0, 1);
      lcd.print("Kd ");
      lcd.print(TempSetup[5].Value,1);
      lcd.setCursor(8, 1);
      lcd.print("Ws ");  
      lcd.print((int)TempSetup[6].Value);
    }

void pageMotor()
    { 
      lcd.clear(); 
      lcd.print("Spd ");
      lcd.print(MotorSetup[1].Value, 0); 
      lcd.setCursor(8, 0);
      lcd.print("Drct ");
      lcd.print(MotorSetup[2].Value, 0);       
      lcd.setCursor(0, 1);     
      lcd.print("Temp off ");
      lcd.print(MotorSetup[3].Value, 0); 
    }


  #ifdef Debug
  void SerialRead () 
  {
    switch (Serial.read()) 
    {      
      case 'w':
        keyUp();
        debugVarPrint();      
        break;
      case 's':
        keyDown();
        debugVarPrint();
        break;
      case 'a':
        keySelect();
        debugVarPrint();
        break;
    }
  }
  #endif

#endif