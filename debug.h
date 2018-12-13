#ifndef DEBUG_H
#define DEBUG_H

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


void debugVarPrint() 
{ 

/*
  Serial.print ("State[0].Value = ");
  Serial.println (State[0].Value);
  Serial.print ("State[1].Value = ");
  Serial.println (State[1].Value);
  Serial.print ("State[2].Value = ");
  Serial.println (State[2].Value);  
  Serial.print ("State[3].Value = ");
  Serial.println (State[3].Value);
  Serial.print ("State[4].Value = ");
  Serial.println (State[4].Value);  
  

  Serial.println ("==========VARIABLES===========");
  Serial.print ("EEPROM 10 = ");
  float a;
  EEPROM.get (10, a);
  Serial.println (a);
  
  
  Serial.print ("MenuNowPos = ");
  Serial.println (MenuNowPos);
  Serial.print("MenuSize = ");
  Serial.println(MenuSize);
  Serial.print("CurrentMenu[MenuNowPos].Type = ");
  Serial.println(CurrentMenu[MenuNowPos].Type);
  Serial.print("CurrentMenu[MenuNowPos].Range = ");
  Serial.println(CurrentMenu[MenuNowPos].Range);
  Serial.print("MenuValue[MenuNowPos] = ");
  Serial.println(CurrentMenu[MenuNowPos].Value);
  Serial.print("CurrentMenu[MenuNowPos].Name = ");
  Serial.println(CurrentMenu[MenuNowPos].Name);
  if (!CurrentMenu[MenuNowPos].handler) Serial.println("Handler = No");
  else Serial.println("Handler = YES ");
  Serial.print("MenuEdit = ");  
  Serial.println(MenuEdit);
  Serial.print("Select= ");  



  Serial.print("ActualTempC = ");  
  Serial.println(ActualTempC);
  Serial.print("SetTempC = ");  
  Serial.println(TempSetup[1].Value);
  Serial.print("Kp = ");  
  Serial.println(TempSetup[3].Value);
  Serial.print("Ki = ");  
  Serial.println(TempSetup[4].Value);
  Serial.print("Kd = ");  
  Serial.println(TempSetup[5].Value);
  Serial.print("WS = ");  
  Serial.println(TempSetup[6].Value);

  Serial.println ("=====================");  
  */
}

#endif