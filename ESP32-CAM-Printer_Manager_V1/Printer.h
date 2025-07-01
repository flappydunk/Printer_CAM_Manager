

/*
Rerap Marlin command functions
commands from prontoface

Home :   G28 (home ALL axis), G92 E0 (Set Position) , M114 (Get Current Position)
Home X : G28 X0, M114

Move Z 10 : G91 (set Relative positioning) , G0 Z10 F101 (raid move), G90 (Set Absolute positioning) , M114

Extrude : G91, G1 E34.0 F93 (Linear move 34 mm), G90

SD print : M21 (init SD), M20 (List SD card)

Begin file list*
ATOMIC~1.GCO 4177323*
MODIFI~1.GCO 10268559*
RAD250~1.GCO 5522336*
End file list*ok*

Off : M84 (not used),M104 S0 (extruder temp),M140 S0 (bed Temp), M107 (Fan off), M81 (ATX power off)

M105 (get extruder Temp)
*/



void sendRoutineCommand(){
  
  if (!SD_list && SD_GET){
    //PrinterPort.println("M20 L"); 
    printSerial("M20 L",true);
    SD_GET = false;
    return;
  }

  //If we do not get an end of files message abort after 3 attempts
  if (SD_list){
    SD_error_count++;
    if (SD_error_count > 2){ //an SD error has occurred
      SD_list = false; //cancels the collection of file data.
      SD_error_count = 0;
      purgeConsole();   
    }  
     return; //do not continue if a file list is being received
   } 
  
  int nextMcmd_max = 4;
   
  purgeConsole(); //remove any old received data
   
  if(nextMcmd == 0){
    //PrinterPort.println("M105"); //temperature
    printSerial("M105",true); 
  }
  else if(nextMcmd == 1){
    //PrinterPort.println("M31"); //elapse time
    printSerial("M31",true); 
  }
  else if(nextMcmd == 2){
    //PrinterPort.println("M27 C");  //file
    printSerial("M27 C",true);  
  }
  else if(nextMcmd == 3){
    //PrinterPort.println("M27");  //progress
    printSerial("M27",true);  
  }
     
  nextMcmd++;
  if(nextMcmd > nextMcmd_max){   
    nextMcmd = 0;
  }
  
}

int reprap_handler(char variable[], char V1[]){
//specific to REPRAP, arduino Mega, Marlin
  char V2[32];
  char V3[32];
  char V4[32];
  String Send;

  if(!strcmp(V1, "pFan")) {   
    pFan_enabled = !pFan_enabled; //toggle
    if (pFan_enabled){
      //PrinterPort.println("M106");
      printSerial("M106",true);
    }else{
      //PrinterPort.println("M107");
      printSerial("M107",true);
    }
  }
  else if(!strcmp(V1, "Pause")) {   
    //PrinterPort.println("M1 ESP_PAUSED");//Pause
    printSerial("M1 ESP_PAUSED",true);
  }
  else if(!strcmp(V1, "Extr_temp_off")) {   
    //PrinterPort.println(get_comma(variable,2));
    Send = "M104 S0" ;
    //PrinterPort.println(Send);
    printSerial(Send,true); 
  }
  else if(!strcmp(V1, "Extr_temp")) {  
    //PrinterPort.println(get_comma(variable,2));
    getValue(variable, sep , 2 , V2);
    Send = "M104 S" + String(V2);
    //PrinterPort.println(Send);
    printSerial(Send,true);  
  }
  else if(!strcmp(V1, "Bed_off")) {   
    //PrinterPort.println(get_comma(variable,2));
    Send = "M140 S0" ;
    //PrinterPort.println(Send);
    printSerial(Send,true); 
  }
  else if(!strcmp(V1, "Bed_on")) {   
    //PrinterPort.println(get_comma(variable,2));
    getValue(variable, sep , 2 , V2);
    Send = "M140 S" + String(V2);
    //PrinterPort.println(Send);
    printSerial(Send,true); 
    
  }
  else if(!strcmp(V1, "Fil_For")) {   
    //PrinterPort.println(get_comma(variable,2));
    getValue(variable, sep , 2 , V2);
    getValue(variable, sep , 3 , V3); 
    //PrinterPort.println("G91");
    printSerial("G91",true);
    Send = "G1 E" + String(V2) + " F" + String(V3);
    //PrinterPort.println(Send);
    printSerial(Send,true); 
    //PrinterPort.println("G90");
    printSerial("G90",true);
    
  }
  else if(!strcmp(V1, "Fil_Bak")) {   
    //PrinterPort.println(get_comma(variable,2));
    getValue(variable, sep , 2 , V2);
    getValue(variable, sep , 3 , V3);
    //PrinterPort.println("G91");
    printSerial("G91",true);
    Send = "G1 E-" + String(V2) + " F" + String(V3);
    printSerial(Send,true);
    //PrinterPort.println(Send); 
    printSerial("G90",true);
    //PrinterPort.println("G90");
    
  }
  else if(!strcmp(V1, "Home")) {   
    //PrinterPort.println(get_comma(variable,2));
    getValue(variable, sep , 2 , V2);
    if(!strcmp(V1, "All")){
      Send = "G28";
    }
    else{
      Send = "G28 "+ String(V2);
    }   
    printSerial(Send,true); 
    //PrinterPort.println(Send);     
  }
  else if(!strcmp(V1, "Axis_m")) {  
    
    getValue(variable, sep , 2 , V2);
    getValue(variable, sep , 3 , V3); 
    getValue(variable, sep , 4 , V4);
    //PrinterPort.println("G91");
    printSerial("G91",true);
    if(!strcmp(V3, "-")){
      Send = "G1 " + String(V2) + String('-') + String(V4); 
    }
    else{
      Send = "G1 " + String(V2) + String(V4);
    } 
    printSerial(Send,true); 
    //PrinterPort.println(Send);
    printSerial("G90",true); 
    //PrinterPort.println("G90"); 
   
  } 
  else if(!strcmp(V1, "cmd")) {  
    getValue(variable, sep , 2 , V2);
    String command = String(V2);
    command.replace("%20"," ");
    //PrinterPort.println(command);
    printSerial(command,true); 
  }
  else if(!strcmp(V1, "SD_List")) { 
    SD_GET = true; //schedule next poll for SD data
  }
  else if(!strcmp(V1, "Print")) {  
    getValue(variable, sep , 2 , V2);
    Send = "M23 " + String(V2);
    //PrinterPort.println(Send);
    printSerial(Send,true); 
    //PrinterPort.println("M24");
    printSerial("M24",true); 
  }
  if(!strcmp(V1, "Log_on")) {   
    log_enabled = true; 
  }
  if(!strcmp(V1, "Log_off")) {   
    log_enabled = false; 
  }
  else {
    return -1;
  } 
  return 0; 
}
