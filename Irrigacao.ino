#include <Wifi.h>

#include <Thingspeak.h>
const long CHANNEL = "2567533"
const char WRITE_API = "C0AESWMJU4GDDL0G";
const char READ_API = "D4IH5OS02S8CZTUD";



#define pinSensor A0
#define ledVermelho 9
#define ledAmarelo 7
#define bomba 12
const int limiteUmidade = 72; //umidade ideal do solo
const int tempoRega = 8; //tempo em segundos
int umidadeSolo = 0;


void setup() {
  Serial.begin(9600);
  pinMode (pinSensor, INPUT);
  pinMode (ledVermelho, OUTPUT);
  pinMode (ledAmarelo, OUTPUT);
  pinMode (bomba, OUTPUT);

  ThingSpeak.begin(cliente); //inicializar o thingspeak 
}

void loop() {
  for (int i=0; i < 5; i++){
    Serial.println("Umidade: ");
    umidadeSolo = analogRead(pinSensor); //faz a leitura do sensor de umidade do solo
    umidadeSolo = map(umidadeSolo, 1023, 0, 0, 100); //converte a variação do sensor de 0 a 1023 para 0 a 100
    //mostra a mensagem
    Serial.print(umidadeSolo); 
    Serial.println("% ");
    delay(3000); //espera 3segundos para fazer uma nova leitura
    }


if (umidadeSolo < limiteUmidade){
  if (umidadeSolo < 20){
    Serial.println("ALERTA: Solo extremamente seco!!");
    digitalWrite(ledVermelho, LOW);
    digitalWrite(bomba, LOW);
    delay(tempoRega*3000);
    digitalWrite(bomba, HIGH);
  } 
  else{
    Serial.println("Regando...");
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(bomba, HIGH);
    delay(tempoRega*1000);
    digitalWrite(bomba, LOW);
  }
}
if (umidadeSolo >= limiteUmidade){
  if(umidadeSolo > 80){
    Serial.println("ALERTA: Solo encharcado!!");
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(bomba, HIGH);
  }
  else {
    Serial.println("Solo umido");
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(bomba, HIGH);

  }
}
}





