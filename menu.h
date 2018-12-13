#ifndef MENU_H
#define MENU_H

//print "0" func
void spaceprint(byte value) 
{
  if(value<10)lcd.print("0");
}

//print on&off
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
      else {lcd.write(byte(0)); lcd.noBlink();}     
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
        
//отображаем символы
      // lcd.setCursor(13, 0);      
      // if (Switch[1].Value) lcd.write(byte(3));     
      // else lcd.print("H");      

      // if (Switch[1].Value || Switch[2].Value) lcd.write(byte(4));
      // else lcd.print("M");
      
      // if (Switch[3].Value) lcd.print("<");
      // else lcd.print(">");            
    }

#endif    