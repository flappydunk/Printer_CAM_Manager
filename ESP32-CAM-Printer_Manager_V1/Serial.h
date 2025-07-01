//Serial interface

char ch;
String serialReceived;
boolean Serial_ready;




char waitCharConsole() {
  while (Serial.available() == 0);
  return (char)Serial.read();
}

char CharConsole() {
  if (Serial.available() > 0){
    return (char)Serial.read();
  }
  return (char)'\0';
}


//creates a delay while data is being received
String waitStringConsole() {
  String s = "";
  char chr;
  while (true) {
    chr = waitCharConsole();
    if (chr=='\n' || chr=='\r'){
      break;}
    else{
      s += chr;
    };
  }
  return s;
}

void debug_Console(){
  char c = CharConsole();
  if(c=='f'){ //
    Serial.println(Last_SD_files);
  }
  if(c=='s'){
   
  }
}


void UART_RX_IRQ(){
 //printer data collector
  while(PrinterPort.available()){
    ch = (char) PrinterPort.read();
    if(log_enabled){rawReceived += ch;} //for logging
    
    if(ch == 0xFF){Serial.println("FF");return;}
    //Serial.print("ch = ");
    //Serial.print(ch,HEX);
    //Serial.print(" ");
    //Serial.println(ch);
    if (ch=='\n' || ch=='\r'){
      serialReceived += ',';//replace with 
      Serial_ready = true;
     }
    else{
      serialReceived += ch; 
    }
  } 
  
}

void printSerial(String s, boolean endline){
  if(endline){
    PrinterPort.println(s);
    if(log_enabled) {rawReceived += "> " + s +"\r\n";} //any send commands also added
  }
  else{
    PrinterPort.print(s);
  }
}



void purgeConsole() {
  while (PrinterPort.available())
    PrinterPort.read();
}

//checksum calculator for String
int calcChecksum(String data){
  int checksum = 0;
  for (int i = 0; i < data.length(); i++){
    checksum += data[i];
  }
  return checksum;
}


//################ message decoded ##################################

  /*
Arduino Mega responses
M24 resume print
M25 Pause SD print


M27 
Not SD printing
SD printing byte 2134/235422
ok

M27 C
Current file: filena~1.gco Filenagotcha.gcode
ok

M27 S2 ; Report the SD card status every 2 seconds

M31
echo:Print time: 0s
ok

M105
M112 Full stop

- `T:` - Hotend (actual / target)
- `B:` - Bed (actual / target)
- `Tx:` - x Tool (actual / target)
- `@:` - Hotend power
- `B@:` - Bed power
- `P:` - PINDAv2 actual (only MK2.5/s and MK3/s)
- `A:` - Ambient actual (only MK3/s)
 
_Examples:_
 
ok T:201 B:117
ok T:201 /202 B:117 /120
ok T:201 /202 B:117 /120 C:49.3 /50
ok T:201 /202 T0:110 /110 T1:23 /0 B:117 /120 C:49.3 /50
ok T0:110 /110 T1:23 /0 B:117 /120
ok T:20.2 /0.0 B:19.1 /0.0 T0:20.2 /0.0 @:0 B@:0 P:19.8 A:26.4

M106,M107
ok

M20 L
Begin file list
...
End file list

//sd print e.g
M23 /musicg\~1/jingle.gco

 
 */

 //Unknown response:  T:26.82 /0.00 B:57.85 /60.00 @:0 B@:127 W:?,
 //                ok T:27.27 /0.00 B:31.67 /0.00 @:0 B@:0,

 String parse_rec(String s, String sfind ){
  //isolate sub string for each
  String output;
  int index_1,index_2,index_3,index_4;

  //find first value after sFind
  index_1 = s.indexOf(sfind);//first
  if(index_1 == -1){
    return "";
  }
  index_1 = index_1 + sfind.length(); 
  index_2 = s.indexOf(" ", index_1);//second
  output = s.substring(index_1, index_2);

  //find / value if it exists
  index_3 = s.indexOf("/", index_2);//third
  if(index_3 > -1){
    index_3 = index_3 + 1;
    index_4 = s.indexOf(" ", index_3);//fourth
    output = output + "," + s.substring(index_3, index_4);
  }
  /*
  Serial.print("index_1 = ");
  Serial.println(index_1);

  Serial.print("index_2 = ");
  Serial.println(index_2);

  Serial.print("index_3 = ");
  Serial.println(index_3);

  Serial.print("index_4 = ");
  Serial.println(index_4);
*/ 
  return output;
}

void Reprap_readSerial() {
    if(!Serial_ready){return;}
  
    String rMsg = serialReceived;
    if (SD_list){
      SD_files += serialReceived;
    }
      
    serialReceived = "";//clear buffer
    //Serial.print("rMsg = ");
    //Serial.println(rMsg); //from printer to normal serial
    boolean success = false;

    //index variables used to decode Strings
    int last_ind_1;
    int last_ind_2;
    int l;

    
    //Reprap / Marlin specific here
    if (rMsg.startsWith(",")){ //sometimes we can get a leading comma  
      rMsg.remove(0);  //remove the comma 
    }
    if (rMsg.startsWith(" ")){ //sometimes we can get a leading space 
      last_ind_1 = rMsg.indexOf(" ");
      rMsg.remove(0, last_ind_1+1); //remove the spaces
    }    
    if (rMsg.startsWith("ok")){ //remove leading OK
        //strip off any leading Ok
        last_ind_1 = rMsg.indexOf(" ");
        rMsg.remove(0, last_ind_1+1);
        //Serial.println(rMsg); //from printer to normal serial
    }
 
    if (rMsg.startsWith("T:")){ //temparature message
          timestamp_sec = String(millis()/1000);
          //M105 response ""
          //defines the string sent to the graphs and web page
          //select further ones if needed
          web_message = "";
          //find first value and /value if exists
          web_message = parse_rec(rMsg, "T:") + ",";
          //web_message = web_message + parse_rec(rMsg, "T0:") + ",";
          // web_message = web_message + parse_rec(rMsg, "T1:") + ",";
          //web_message = web_message + parse_rec(rMsg, "B@:") + ",";
          //web_message = web_message +parse_rec(rMsg, "@:");
          web_message = web_message + parse_rec(rMsg, "B:");
          
          
          //recd = ok T:22.50 /0.00 B:22.46 /0.00 @:0 B@:0*

           web_message = "T," + web_message + "," + timestamp_sec + ",";
          //Serial.print("sMsg = ");
          //Serial.println(web_message);
          success = true;         
      }
      if (rMsg.startsWith("echo:P")){
        //M31 response Print time "echo:Print time: 0s,ok,"  
        //Note: CR or LF replaced with *    
         
        last_ind_1 = rMsg.lastIndexOf(":");
        rMsg.remove(0, last_ind_1);//" 0s,ok,"
        rMsg.remove(0, 2);//0s,ok,"
        last_ind_2 = rMsg.indexOf(",");
        rMsg.remove(last_ind_2, 4);
        elapsedt = rMsg;
        web_message = "E," + elapsedt + ",";
        success = true;
      }
      if (rMsg.startsWith("Not SD")){
        progress = "Not Printing";
        web_message = "P," + progress + ",";
        success = true;
      }
      if (rMsg.startsWith("Cur")){//Current file: VALVEC~1.GCO valve case lid.gcode
        progress = rMsg;
        web_message = "F," + progress + ",";
        success = true;
      }
      if (rMsg.startsWith("SD print")){ //M27
        //SD printing byte 2134/235422,ok,
        last_ind_1 = rMsg.lastIndexOf("e");
        rMsg.remove(0, last_ind_1+2);//"2134/235422,ok,"
        last_ind_2 = rMsg.indexOf(",");
        l = rMsg.length();
        rMsg.remove(last_ind_2, l);   //"2134/235422"
        progress = rMsg; //String of the basic numbers if you prefer
        
        //decode percentage
        last_ind_2 = rMsg.indexOf("/");
        l = rMsg.length();
        String b1 = rMsg.substring(0,last_ind_2); //first part
        String b2 = rMsg.substring(last_ind_2+1,l); //second part
        //Serial.println(b1);
        //Serial.println(b2);
        char tempBuf[32];
        float r = (b1.toFloat()/b2.toFloat())*100;
        dtostrf(r,2,1,tempBuf);
        //Serial.println(tempBuf);
        
        web_message = "P,Progress: " + String(tempBuf) + "%,"; //percentage only
        //Serial.println(progress);
        success = true;
      }
      
      
      if (rMsg.startsWith("Begin file")){ 
        SD_list = true;
        SD_files = rMsg;
        //Serial.println("T");
      }
      if (rMsg.startsWith("End file")){            
        success = true;
        
        
        /*tidy up received string to just get files e.g
         * //NOTE: Marlin does not print the full longname??
        Begin file list
        ATOMIC~1.GCO 4177323 Atomic_U300_Printhead_blow
        KH-SLI~1.GCO 5522336 KH-Slice.008_wheel segment
        MODIFI~1.GCO 10268559 Modified_hot_end_Printhead
        TEST_5~1.GCO 172634 test_50x50_square.gcode
        TEST_2~1.GCO 172634 test_2_50x50_square.gcode
        TEST_3~1.GCO 172634 test_3_50x50_square .gcode
        End file list
        ok
        
 

        Last_SD_files = SD_files; //store temp for debug
        
        SD_checksum = calcChecksum(SD_files); //ESP32 does not like long serial data
        if (SD_checksum != last_SD_checksum){
           Serial.println("SD error");
          //retry
          last_SD_checksum = SD_checksum;
          SD_files = "";
          SD_retry = true;
          SD_list = false;
          return;
        }
        SD_retry = false; //cancel further commands
        
*/     //dont leave in as ESP can cause errors 
        //Serial.println(SD_files); //Raw file data

        
        int sd_length;
        int Start_ind = SD_files.indexOf(","); //Begin file list,
        SD_files.remove(0, Start_ind + 1); //removes first field and comma
        int end_ind = SD_files.indexOf("End file list");
        sd_length = SD_files.length();
        SD_files.remove(end_ind, sd_length); //removes comma and end of String 
        //Serial.println(SD_files); //Raw file data

          

        //seperate file data
        File_Data = "";
        int sf_ind;
        int size_ind;
        //ATOMIC~1.GCO 4177323 Atomic_U300_Printhead_blow,
        while(SD_files.indexOf(",") > 0){
          sf_ind = SD_files.indexOf(" ");//first 8.3 file (sf) short form(no spaces allowed in filename)
                   
          File_Data += SD_files.substring(0,sf_ind) + ","; //create comma seperated String
          SD_files.remove(0, sf_ind + 1); //remove short form

          size_ind = SD_files.indexOf(" ");//File size end
          File_Data += SD_files.substring(0,size_ind) + ","; //File size
          SD_files.remove(0, size_ind + 1); //remove file size including comma

          end_ind = SD_files.indexOf(","); //end of first record
          File_Data += SD_files.substring(0,end_ind) + ","; //long file string
  
          SD_files.remove(0, end_ind + 1); //remove first record and restart
          //Serial.println(SD_files);
        }
        //Serial.println(File_Data);
        SD_files = "";
        web_message = "S," + File_Data; //send SD file data
        Last_SD_files = web_message; //store temp for debug
        SD_list = false; 
        //Serial.println("F");   
      }
      if(!success){
          if (!SD_list) {Serial.println("Unknown response: " + rMsg);}
      }
      Serial_ready = false; 
}



  


  
  
  




  
