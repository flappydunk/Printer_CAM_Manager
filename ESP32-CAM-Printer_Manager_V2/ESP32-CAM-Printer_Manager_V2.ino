
/************************************************************************
 For the complete instructions for the build of the Printer Manager
 goto:

  https://github.com/flappydunk/Printer_CAM_Manager/tree/main


******** For the camera part *************************************************************
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete instructions at https://RandomNerdTutorials.com/esp32-cam-projects-ebook/
******************************************************************************************

  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

//Arduino Board ESP32-CAM = AI Thinker ESP32-CAM

#include <HardwareSerial.h>
#include "esp_camera.h"
#include <WiFi.h>
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "fb_gfx.h"
#include "soc/soc.h"             // disable brownout problems
#include "soc/rtc_cntl_reg.h"    // disable brownout problems
#include "esp_http_server.h"
#include "Web_Page_tabs_graph_V1.h"

//version
char version[] = "ESP32-CAM-Printer_Manager_V2";

//function declarations
void getValue(char data[], char separator, int index , char seg[]);

//Hardware serial settings
HardwareSerial PrinterPort ( 1 );
int PrintSerial_Speed = 115200; 

//Printer Type
#define PRINTER_REPRAP //all Marlin commands are formatted for REPRAP

//ESP32 CAM
#define ledPin 4               //Flash Pin
#define SERIAL1_RXPIN 15       //Serial Pin for PrinterSerial  was 15
#define SERIAL1_TXPIN 14       //Serial Pin for PrinterSerial was 14



//Serial interface and timers for control of the printer
String elapsedt, progress, timestamp_sec;
String web_message;

//sd card support (Enable Long file name support in Marlin)
String SD_files;  //RAW file listing
String Last_SD_files;  //RAW file listing
boolean SD_GET;   //flag to send next SD command 
String File_Data; //seperated list of file data
boolean SD_list = false; //flag to indicate a file list is being processed.
int SD_error_count;

//routine commands
int nextMcmd; //routine Mcommands sender
long int poll_time = 2000;
long int next_poll_time;

//Printer reset timer (not tested)
boolean PTR_reset_req = false;  //flag for printer reset if fitted
long int reset_time = 100;
long int next_reset_time;

//logging
String rawReceived;
boolean log_enabled;

//ESP32 toggle flags
boolean light_enabled = false;
boolean pFan_enabled = false;
int vflip = 1; //default setting of camera
int hmirr = 0; //default setting of camera

//web messages value seperator
char sep = ',';


#include "Serial.h"
#include "Printer.h"


// Replace with your network credentials
const char* ssid = "SSID";
const char* password = "Password";

#define PART_BOUNDARY "123456789000000000000987654321"

#define CAMERA_MODEL_AI_THINKER
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WITHOUT_PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM_B
//#define CAMERA_MODEL_WROVER_KIT

#if defined(CAMERA_MODEL_WROVER_KIT)
  #define PWDN_GPIO_NUM    -1
  #define RESET_GPIO_NUM   -1
  #define XCLK_GPIO_NUM    21
  #define SIOD_GPIO_NUM    26
  #define SIOC_GPIO_NUM    27
  
  #define Y9_GPIO_NUM      35
  #define Y8_GPIO_NUM      34
  #define Y7_GPIO_NUM      39
  #define Y6_GPIO_NUM      36
  #define Y5_GPIO_NUM      19
  #define Y4_GPIO_NUM      18
  #define Y3_GPIO_NUM       5
  #define Y2_GPIO_NUM       4
  #define VSYNC_GPIO_NUM   25
  #define HREF_GPIO_NUM    23
  #define PCLK_GPIO_NUM    22

#elif defined(CAMERA_MODEL_M5STACK_PSRAM)
  #define PWDN_GPIO_NUM     -1
  #define RESET_GPIO_NUM    15
  #define XCLK_GPIO_NUM     27
  #define SIOD_GPIO_NUM     25
  #define SIOC_GPIO_NUM     23
  
  #define Y9_GPIO_NUM       19
  #define Y8_GPIO_NUM       36
  #define Y7_GPIO_NUM       18
  #define Y6_GPIO_NUM       39
  #define Y5_GPIO_NUM        5
  #define Y4_GPIO_NUM       34
  #define Y3_GPIO_NUM       35
  #define Y2_GPIO_NUM       32
  #define VSYNC_GPIO_NUM    22
  #define HREF_GPIO_NUM     26
  #define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_M5STACK_WITHOUT_PSRAM)
  #define PWDN_GPIO_NUM     -1
  #define RESET_GPIO_NUM    15
  #define XCLK_GPIO_NUM     27
  #define SIOD_GPIO_NUM     25
  #define SIOC_GPIO_NUM     23
  
  #define Y9_GPIO_NUM       19
  #define Y8_GPIO_NUM       36
  #define Y7_GPIO_NUM       18
  #define Y6_GPIO_NUM       39
  #define Y5_GPIO_NUM        5
  #define Y4_GPIO_NUM       34
  #define Y3_GPIO_NUM       35
  #define Y2_GPIO_NUM       17
  #define VSYNC_GPIO_NUM    22
  #define HREF_GPIO_NUM     26
  #define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_AI_THINKER)
  #define PWDN_GPIO_NUM     32
  #define RESET_GPIO_NUM    -1
  #define XCLK_GPIO_NUM      0
  #define SIOD_GPIO_NUM     26
  #define SIOC_GPIO_NUM     27
  
  #define Y9_GPIO_NUM       35
  #define Y8_GPIO_NUM       34
  #define Y7_GPIO_NUM       39
  #define Y6_GPIO_NUM       36
  #define Y5_GPIO_NUM       21
  #define Y4_GPIO_NUM       19
  #define Y3_GPIO_NUM       18
  #define Y2_GPIO_NUM        5
  #define VSYNC_GPIO_NUM    25
  #define HREF_GPIO_NUM     23
  #define PCLK_GPIO_NUM     22

#elif defined(CAMERA_MODEL_M5STACK_PSRAM_B)
  #define PWDN_GPIO_NUM     -1
  #define RESET_GPIO_NUM    15
  #define XCLK_GPIO_NUM     27
  #define SIOD_GPIO_NUM     22
  #define SIOC_GPIO_NUM     23
  
  #define Y9_GPIO_NUM       19
  #define Y8_GPIO_NUM       36
  #define Y7_GPIO_NUM       18
  #define Y6_GPIO_NUM       39
  #define Y5_GPIO_NUM        5
  #define Y4_GPIO_NUM       34
  #define Y3_GPIO_NUM       35
  #define Y2_GPIO_NUM       32
  #define VSYNC_GPIO_NUM    25
  #define HREF_GPIO_NUM     26
  #define PCLK_GPIO_NUM     21

#else
  #error "Camera model not selected"
#endif



static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

httpd_handle_t camera_httpd = NULL;
httpd_handle_t stream_httpd = NULL;

//opens the web page
static esp_err_t index_handler(httpd_req_t *req){
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}


//manages the stream
static esp_err_t stream_handler(httpd_req_t *req){
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  
  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if(res != ESP_OK){
    return res;
  }

  while(true){
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    } else {
      if(fb->width > 400){
        if(fb->format != PIXFORMAT_JPEG){
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if(!jpeg_converted){
            Serial.println("JPEG compression failed");
            res = ESP_FAIL;
          }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      }
    }
    
    if(res == ESP_OK){
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);

    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);

    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));

    }

   
    if(fb){
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if(_jpg_buf){
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if(res != ESP_OK){
      break;
    }
  }
  
  return res;
}

static esp_err_t cmd_handler(httpd_req_t *req){
  char*  buf;
  size_t buf_len;
  char variable[32] = {0,};
  char V1[32];
  
  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    buf = (char*)malloc(buf_len);
    if(!buf){
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
      if (httpd_query_key_value(buf, "go", variable, sizeof(variable)) == ESP_OK) {
      } else {
        free(buf);
        httpd_resp_send_404(req);
        return ESP_FAIL;
      }
    } else {
      free(buf);
      httpd_resp_send_404(req);
      return ESP_FAIL;
    }
    free(buf);
  } else {
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }

  sensor_t * s = esp_camera_sensor_get();
  //flip the camera vertically
  s->set_vflip(s, vflip);          // 0 = disable , 1 = enable
  // mirror effect
  s->set_hmirror(s, hmirr);          // 0 = disable , 1 = enable

  int res = 0;

    //Serial.println(variable);

  //retrieve first value from received string "variable"
  getValue(variable, sep , 1 , V1); //sep = ','

  //These are the standard actions for teh ESP32-CAM and a printer reset 
  if(!strcmp(V1, "values")) { //this is the routine update of the web page data
    httpd_resp_set_status(req, HTTPD_200);
    httpd_resp_set_type(req, "text/plain");
    
    if(log_enabled) {
      if(web_message.length() == 0){ //only send if no other message
        if(rawReceived.length() != 0){
          web_message = "L," + rawReceived + ",";
          rawReceived = "";
        }
      }
    }
    
    String send_message = web_message;
    //Serial.println(web_message);
    web_message = "";
    return httpd_resp_sendstr(req, send_message.c_str());
  }
  else if(!strcmp(V1, "CAM_Restart")) {   
    ESP.restart();
  }
  else if(!strcmp(V1, "light")) {
    light_enabled = !light_enabled; //toggle
    if (light_enabled){digitalWrite(ledPin, HIGH);}else{digitalWrite(ledPin, LOW);}
  }
  else if(!strcmp(V1, "vflip")) {
    if (vflip == 1){vflip = 0;}else{vflip = 1;}
  }
  else if(!strcmp(V1, "hmirr")) {
    if (hmirr == 1){hmirr = 0;}else{hmirr = 1;}
  }
  else {
    //printer specific
    #if defined(PRINTER_REPRAP)
      res = reprap_handler(variable, V1);
    #endif
  }

  if(res){
    return httpd_resp_send_500(req);
  }
  return httpd_resp_send(req, NULL, 0);
}



void startCameraServer(){
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;
  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
  };
  //all buttons
  httpd_uri_t cmd_uri = {
    .uri       = "/action",
    .method    = HTTP_GET,
    .handler   = cmd_handler,
    .user_ctx  = NULL
  };
  
  httpd_uri_t stream_uri = {
    .uri       = "/stream",
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
  };
  if (httpd_start(&camera_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(camera_httpd, &index_uri);
    httpd_register_uri_handler(camera_httpd, &cmd_uri);
  }
  config.server_port += 1;
  config.ctrl_port += 1;
  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(stream_httpd, &stream_uri);
  }
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  
  Serial.begin(115200);
  Serial.setDebugOutput(false); //wifi diagnostic output
  pinMode(ledPin, OUTPUT);
  Serial.println(version);
  
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; 
  Serial.print(F("FrameSize "));
  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;//0-63 lower number means higher quality
    config.fb_count = 2;
    Serial.println(F("VGA"));
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;//0-63 lower number means higher quality
    config.fb_count = 1;
    Serial.println(F("SVGA"));
  }
  
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  // Wi-Fi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.println(WiFi.localIP());
  
  // Start streaming web server
  startCameraServer();

  PrinterPort.begin(PrintSerial_Speed, SERIAL_8N1, SERIAL1_RXPIN, SERIAL1_TXPIN);
  PrinterPort.onReceive(UART_RX_IRQ);
  blink();
}

void loop() {
  debug_Console();
 if(next_poll_time < millis()){  
    sendRoutineCommand();
    next_poll_time = poll_time + millis();
  }
 //printer specific
  #if defined(PRINTER_REPRAP)
    Reprap_readSerial();
  #endif
}



void blink() //This blinks the onboard LED flash once you are connected to the Wifi.
{
  digitalWrite(ledPin, HIGH); // turn on the LED
  delay(50);                  // wait for half a second or 500 milliseconds
  digitalWrite(ledPin, LOW);  // turn off the LED
  delay(50);                  // wait for half a second or 500 milliseconds
  digitalWrite(ledPin, HIGH); // turn on the LED
  delay(50);                  // wait for half a second or 500 milliseconds
  digitalWrite(ledPin, LOW);  // turn off the LED
  delay(50);                  // wait for half a second or 500 milliseconds
}

//extract field from data stream
void getValue(char data[], char separator, int index , char seg[]){
    //separates the chosen field from a separated String object
    int found = 0;
    int maxIndex = strlen(data) - 1;
    int start = 0;
    int fin = 0;
    int last_sep = 0;
    //Serial.print(" maxIndex = ");
    //Serial.println(maxIndex);
   //eg  Extr_temp,T1,

    
    //search array for the correct seperator
    for (int i = 0; i < maxIndex+1; i++) {    
        if ((data[i] == separator) && (found < index)) {
            found++;         
            //Serial.print(" found index = ");
            //Serial.println(i);
            //check the array before the seperator
            if (found == index){
              fin = i-1; 
              if(index > 1){start = last_sep + 1;}       
            }
            last_sep = i;
        }      
    }

    if(found == 0){
      //no seperator found return full array
      fin = maxIndex;
    }

    //Serial.print(" start = ");
    //Serial.print(start);
    //Serial.print(" fin = ");
    //Serial.println(fin);

    //extract bytes from data array and create a c string
    if (found == index || found == 0){
      int k=0;
       for(int i=start;i<fin+1;i++){
          seg[k] = data[i];
          k++;
       }
       seg[k] =  '\0';
    }
    else{
      seg[0] =  '\0';
    }
}
