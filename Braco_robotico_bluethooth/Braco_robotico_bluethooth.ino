
// Projeto Braço Robótico controlado por bluetooth 
// Autor: Ivancley Brito 
// 21/05/23
// Utilizado braço robótico fornecido pela Robocore
// Módulo Bluetooth - HR-05 

#include <SoftwareSerial.h>
#include <Servo.h>

#define SERVO_BASE 5     // branco
#define SERVO_ELEVADOR 6 //verde
#define SERVO_FRENTE 9   // amarelo
#define SERVO_GARRA 10   //laranja
#define DELAY 1000

// Configs Bluetooth
const int pinoRX = 2; 
const int pinoTX = 3;
SoftwareSerial bluetooth(pinoRX, pinoTX);

Servo servo_base; 
Servo servo_elevador;
Servo servo_frente; 
Servo servo_garra;

int servo_base_valor; 
int servo_elevador_valor;
int servo_frente_valor; 
int servo_garra_valor;

int posicao_base     = 80;
int posicao_elevador = 120;
int posicao_frente   = 90;
int posicao_garra    = 80;

String bbase_mais    = "bbmais";
String bbase_menos   = "bbmenos";
String bgarra_mais   = "bgmais";
String bgarra_menos  = "bgmenos";
String bsobe_mais    = "bsmais";
String bsobe_menos   = "bsmenos";
String bfrente_mais  = "bfmais";
String bfrente_menos = "bfmenos";

int salto = 5;

int dadoBluetooth = 0;

char resp;
String text; 

void setup(){
  // Definindo Servos
  servo_base.attach(SERVO_BASE); 
  servo_elevador.attach(SERVO_ELEVADOR); 
  servo_frente.attach(SERVO_FRENTE); 
  servo_garra.attach(SERVO_GARRA); 
  // Posicionando Servos
  servo_base.write(posicao_base); 
  servo_elevador.write(posicao_elevador); 
  servo_frente.write(posicao_frente); 
  servo_garra.write(posicao_garra); 
  // Definido para comunicação com o terminal 
  Serial.begin(19200);
  // Definido para comunicação com o Módulo Bluetooth 
  bluetooth.begin(9600);
} 

void loop(){
  // Enquanto estiver recebendo dados do bluetooth faça a concatenação 
  while(bluetooth.available()){
    resp = bluetooth.read();
    if (resp == '\n' || resp == '\r') {
      break;
    }
    text += resp;
  }
  // Exibi no termina 
  if(text!=""){
    exibir_recebido(text);
  }
  // Confere valores recebidos
  if(text == bbase_mais)    { posicao_base     += salto; }
  if(text == bbase_menos)   { posicao_base     -= salto; }
  if(text == bgarra_mais)   { posicao_garra    += salto; }
  if(text == bgarra_menos)  { posicao_garra    -= salto; }
  if(text == bsobe_mais)    { posicao_elevador += salto; }
  if(text == bsobe_menos)   { posicao_elevador -= salto; }
  if(text == bfrente_mais)  { posicao_frente   += salto; }
  if(text == bfrente_menos) { posicao_frente   -= salto; }
  
  atualizarServos(); 
  delay(DELAY);
}
void exibir_recebido(String texto_recebido){
    Serial.println(" ");
    Serial.println("texto recebido");
    Serial.println(texto_recebido);
    Serial.println(" ");
}

void atualizarServos(){
  servo_base.write(posicao_base); 
  servo_elevador.write(posicao_elevador); 
  servo_frente.write(posicao_frente); 
  servo_garra.write(posicao_garra); 
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