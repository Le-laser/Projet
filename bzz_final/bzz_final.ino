#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
#include <EEPROM.h>            // read and write from flash memory


//constantes
#define LONGTRAME 3 //Nombre de cases dans la trame
#define POSCARACSPE 0 //Position du caractère spécial dans la trame
#define POSITIONX 1 //Position de la valeur X dans la trame
#define POSITIONY 2 //Position de la valeur X dans la trame
#define CARACSPE 35 //Caractère spécial signifiant le début de la trame 35 en ASCII = #
#define seuil_gris 50  // nv de gris max accepté
#define saut_de_pixels 1 // tous les combien de pixels on vérifie
#define HEIGHT 320    // dépend du frame_size utilisé
#define WIDTH 240
#define LEN HEIGHT*WIDTH

#define EEPROM_SIZE 1

// Pin definition for CAMERA_MODEL_AI_THINKER
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

camera_fb_t *fb = NULL;

// les initialisations de variables au niveau du traitement d'image
bool tab_pix_noir[HEIGHT][WIDTH/8] = {0}; // tab contenant tous les pixels // à remettre à 0 au début de la fonction dans lequel on lancera le traitement
/* masquage :
 *  type bool : on prend un espace de 8 bits et utilise un seul bit par octet : 0000 0000 = False; 0000 0001 = True
 *  On utilise cette propriété pour affecter une valeur à chacun des bits de cet octet
 *  écriture :
 *    tab[y][x] = 1 équivaut à  *tab[y*WIDTH + x] = 1 et devient
 *    *tab[(y*WIDTH + x)/8] += 2^(7-((y*WIDTH + x)%8)) => *tab[(pos_y++*WIDTH + pos_x)/8] & 1<<(7-((pos_y++*WIDTH + pos_x)%8))
 *  lecture : 
 *    (*tab[(y*WIDTH + x)/8] & 1<<(7-((y*WIDTH + x)%8))) == True
*/
uint16_t recurs_taches(uint16_t, uint16_t, uint16_t);
bool img_trait();
void EnvoiTrame();

uint16_t x = 0, y = 0;
uint16_t taille_tache, taille_tache_max = 0,  x_tache, y_tache;
uint16_t pictureNumber = 0;
uint8_t centre = 0;
uint16_t iTabTrame[LONGTRAME];
camera_config_t config;

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  iTabTrame[POSCARACSPE]= CARACSPE;
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  //Serial.println();

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
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE; 

config.frame_size = FRAMESIZE_QVGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
config.jpeg_quality = 20;
config.fb_count = 2;
  
 /* FRAMESIZE_UXGA (1600 x 1200)
    FRAMESIZE_QVGA (320 x 240)
    FRAMESIZE_CIF (352 x 288)
    FRAMESIZE_VGA (640 x 480)
    FRAMESIZE_SVGA (800 x 600)
    FRAMESIZE_XGA (1024 x 768)
    FRAMESIZE_SXGA (1280 x 1024)
*/  
// Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  
  sensor_t * s = esp_camera_sensor_get();
  //Serial.println("Starting SD Card");
  if(!SD_MMC.begin()){
    Serial.println("SD Card Mount Failed");
    return;
  }
  
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD Card attached");
    return;
  }
}

uint16_t recurs_taches(uint16_t pos_x, uint16_t pos_y, uint16_t N){
  
    *tab_pix_noir[(pos_y*WIDTH + pos_x)/8] += 1<<(7-((pos_y*WIDTH + pos_x)%8));
    N++;
    
    if(fb -> buf[++pos_x + WIDTH * pos_y] < seuil_gris && (*tab_pix_noir[(pos_y*WIDTH + ++pos_x)/8] & 1<<(7-((pos_y * WIDTH + ++pos_x)%8))) == false && ++pos_x < WIDTH){
        N = recurs_taches(pos_x++, pos_y, N);
    }
    if(fb -> buf[--pos_x + WIDTH * pos_y] < seuil_gris && (*tab_pix_noir[(pos_y*WIDTH + --pos_x)/8] & 1<<(7-((pos_y * WIDTH + --pos_x)%8))) == false && --pos_x >= 0){
        N = recurs_taches(pos_x--, pos_y, N);
    }
    if(fb -> buf[pos_x + WIDTH * (++pos_y)] < seuil_gris && (*tab_pix_noir[(++pos_y*WIDTH + pos_x)/8] & 1<<(7-((++pos_y*WIDTH + pos_x)%8))) == false && ++pos_y < HEIGHT){
        N = recurs_taches(pos_x, pos_y++, N);
    }
    if(fb -> buf[pos_x + WIDTH * (--pos_y)] < seuil_gris && (*tab_pix_noir[(--pos_y*WIDTH + pos_x)/8] & 1<<(7-((--pos_y*WIDTH + pos_x)%8))) == false && --pos_y >= 0){
        N = recurs_taches(pos_x, pos_y--, N);
    }
    return N;
}

bool img_trait(){  // Take Picture with Camera
  fb = esp_camera_fb_get();
  
  if(!fb) {
    Serial.println("Camera capture failed");
    return false;
  }
   // ------------------------------------ Traitement de l'image ------------------------------------ //

  for(y = 0; y < HEIGHT; y += saut_de_pixels){ // on analyse tous les saut_de_pixels pixels, en x et y
    for(x = 0; x < WIDTH; x += saut_de_pixels){         
      if (fb -> buf[x + WIDTH * y] < seuil_gris && (*tab_pix_noir[(y*WIDTH + x)/8] & 1<<(7-((y*WIDTH + x)%8))) == false){ // si en-dessous du niveau de gris et false dans le tab alors on lance l'algo
        taille_tache = recurs_taches(x, y, 0);
        if(taille_tache > taille_tache_max ){
          taille_tache_max = taille_tache;
          centre = std::sqrt(taille_tache)/2;
          x_tache = x + centre;  // à peu près le milieu de la tache si on part du principe que c'est un carré
          y_tache = y + centre;
        }
      }
    }
  }
  // ---------------------------------- Fin traitement de l'image ---------------------------------- //

  iTabTrame[POSITIONX]= x_tache;
  iTabTrame[POSITIONY]= y_tache;
  EnvoiTrame();

  // initialize EEPROM with predefined size
  EEPROM.begin(EEPROM_SIZE);
  pictureNumber = EEPROM.read(0) + 1;

  // Path where new picture will be saved in SD Card
  String path = "/picture" + String(pictureNumber) +".jpg";

  fs::FS &fs = SD_MMC; 
  
  File file = fs.open(path.c_str(), FILE_WRITE);
  if(!file){
    return false;
  } 
  else {
    file.write(fb->buf, fb->len); // payload (image), payload length
    Serial.printf("Saved file to path: %s\n", path.c_str());
    EEPROM.write(0, pictureNumber);
    EEPROM.commit();
  }
  file.close();
  esp_camera_fb_return(fb);
  
  return true;
}

void EnvoiTrame()
{
  Serial.print("Envoi");
  for(int i= 0; i<LONGTRAME; i++)
  { 
    Serial.write(iTabTrame[i]);
  }
}

void loop() {
  //lecture de l'uart
}
