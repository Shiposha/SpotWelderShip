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


//****************** EEPROM & FLAGS *******************

//сохрание значений в EEPROM
void UpdateEeprom() 
{
  if (CurrentMenu[MenuNowPos].Type == 1) CurrentMenu[MenuNowPos].Value =  ((bool)CurrentMenu[MenuNowPos].Value % 2);
  EEPROM.put(CurrentMenu[MenuNowPos].EepromAdrr, CurrentMenu[MenuNowPos].Value);    
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

void Menu_Beep()
{
  if (CurrentMenu==Menu_Main && CurrentMenu[0].handler) CurrentMenu[MenuNowPos].handler();
  else printMenu();   
  if (WelderSetup[4].Value) Beep(0,10); //матюгальник      
}

void Buttons_Scan()
{
  if (Select_Button.pressed()) {keySelect(); Menu_Beep();}
  if (Up_Button.pressed()) {keyUp(); Menu_Beep();}
  if (Down_Button.pressed()) {keyDown(); Menu_Beep();}
}

#endif