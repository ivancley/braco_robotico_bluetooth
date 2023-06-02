// Projeto Braço Robótico controlado por bluetooth 
// Autor: Ivancley Brito 
// 29/05/23
// Utilizado braço robótico fornecido pela Robocore
// Módulo Bluetooth - HR-05 
// Módulo controle Servos - PCA9685


/////////////////////////////// Configurações referente ao PCA9685
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Apenas declarando
void writeServos(int posicao, int tempo);
void beginServos();

void mover(uint8_t Servo, int origen, int destino);

// Código referente aos servos
uint8_t SERVO_GARRA  = 0; 
uint8_t SERVO_BASE   = 4;
uint8_t SERVO_FRENTE = 8;
uint8_t SERVO_ALTURA = 12;
// posições iniciais
int posicao_base     = 80;
int posicao_elevador = 120;
int posicao_frente   = 90;
int posicao_garra    = 80;

// definindo os valores que podem ser recebidos
String bbase_mais    = "bbmais";
String bbase_menos   = "bbmenos";
String bgarra_mais   = "bgmais";
String bgarra_menos  = "bgmenos";
String bsobe_mais    = "bsmais";
String bsobe_menos   = "bsmenos";
String bfrente_mais  = "bfmais";
String bfrente_menos = "bfmenos";

#define SALTO 5;

///////////////////////////////// Configurações referente ao Bluetooth
#include <SoftwareSerial.h>
const int pinoRX = 2; 
const int pinoTX = 3;
SoftwareSerial bluetooth(pinoRX, pinoTX);

String text; 
char resp;

void setup() {
  bluetooth.begin(9600);
  Serial.begin(9600);
  beginServos();
  delay(300);

  writeServos(SERVO_BASE, posicao_base);
  delay(300);
  writeServos(SERVO_ALTURA, posicao_elevador);
  delay(300);
  writeServos(SERVO_FRENTE, posicao_frente);
  delay(300);
  writeServos(SERVO_GARRA, posicao_garra);
  delay(300);
}


void loop() {

  while(bluetooth.available()){
    resp = bluetooth.read();
    if (resp == '\n' || resp == '\r') {
      break;
    }
    text += resp;
  }
  if(text!=""){
    exibir_recebido(text);
  }

  // Confere valores recebidos
  if(text == bbase_mais)    { posicao_base     += SALTO; }
  if(text == bbase_menos)   { posicao_base     -= SALTO; }
  if(text == bgarra_mais)   { posicao_garra    = 5; }
  if(text == bgarra_menos)  { posicao_garra    = 80; }
  if(text == bsobe_mais)    { posicao_elevador += SALTO; }
  if(text == bsobe_menos)   { posicao_elevador -= SALTO; }
  if(text == bfrente_mais)  { posicao_frente   += SALTO; }
  if(text == bfrente_menos) { posicao_frente   -= SALTO; }
  
  atualizarServos(); 

}


void exibir_recebido(String texto_recebido){
    Serial.println(" ");
    Serial.println(texto_recebido);
    Serial.println(" ");
}

void atualizarServos(){
  writeServos(SERVO_BASE, posicao_base);
  //delay(50);
  writeServos(SERVO_ALTURA, posicao_elevador);
  //delay(50);
  writeServos(SERVO_FRENTE, posicao_frente);
  //delay(50);
  writeServos(SERVO_GARRA, posicao_garra);
  //delay(50);
  exibir_valor("Base", posicao_base);
  exibir_valor("Frente", posicao_frente);
  exibir_valor("Elevador", posicao_elevador);
  exibir_valor("Garra", posicao_garra);
  
  // é necessário zerar o valor para pegar a proxima palavra 
  text="";
}

void exibir_valor(String texto_recebido, int valor){
  Serial.print(texto_recebido);
  Serial.print(":");
  Serial.print(valor);
  Serial.println("");
}
// IMPLEMENTO DE FUNÇÕES
void writeServos(int nServo, int posicao) {
#define SERVOMIN  205 // VALOR PARA UM PULSO MAIOR QUE 1 mS
#define SERVOMAX  409 // VALOR PARA UM PULSO MENOR QUE 2 mS

  int pos = map ( posicao , 0 , 180 , SERVOMIN, SERVOMAX);
  pwm.setPWM(nServo , 0, pos);
  delay(25);
}

void beginServos() {
#define Frequencia 50 // VALOR DA FREQUENCIA DO SERVO 

  pwm.begin(); // INICIA O OBJETO PWM
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(Frequencia); // DEFINE A FREQUENCIA DE TRABALHO DO SERVO
}