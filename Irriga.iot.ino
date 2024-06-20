#include <WiFi.h>
#include <ThingSpeak.h>

//id e senha da rede
const char* ssid = "Marluce";
const char* password =  "Wifi8826";
WiFiClient client;


//chaves de conexão do thingspeak
const long CHANNEL = 2567533;
const char *WRITE_API = "C0AESWMJU4GDDL0G";
const char *READ_API = "D4IH5OS02S8CZTUD";
int intervaloTempo = 15000; // intervalo de tempo para escrever no Thing Speak write
unsigned long tempoAnterior;


//pinos, variáveis e constantes do módulo 
#define sensorUmidade 32
#define ledVermelho 19
#define ledAmarelo 18
#define bomba 5
const int limiteUmidade = 40; //umidade ideal do solo
int umidadeSolo = 0;
bool acionar = 0;


void setup() {

  pinMode (sensorUmidade, INPUT);
  pinMode (ledVermelho, OUTPUT);
  pinMode (ledAmarelo, OUTPUT);
  pinMode (bomba, OUTPUT);

  Serial.begin(9600);
  
  if (WiFi.status() !=  WL_CONNECTED) {
    Serial.print("Conectando à rede ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      Serial.println("Conectando...");
      delay(1000);
  }  
  Serial.println("Conexão bem sucedida!");
 }
  tempoAnterior = 0;
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client); //inicializa o ThingSpeak 

}

void leituraSensor (){
    
    umidadeSolo = analogRead(sensorUmidade); //faz a leitura do sensor de umidade do solo
    umidadeSolo = map(umidadeSolo, 4095, 0, 0, 100); //converte a variação do sensor de 0 a 1023 para 0 a 100
    //mostra a mensagem
    Serial.println("Umidade: ");
    Serial.print(umidadeSolo); 
    Serial.println("% ");

}

void bombear(){
  if (umidadeSolo < limiteUmidade){
    Serial.println("Regando...");
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(bomba, LOW);
    delay(8000);
    digitalWrite(bomba,HIGH);
    acionar = 1;
  }
  else{
    Serial.println("Umidade ideal!");
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(bomba, LOW);
  }
}


void loop() { 

pinMode(bomba, HIGH);  
  if ((millis() - tempoAnterior > intervaloTempo)){
    leituraSensor();
    bombear();
    ThingSpeak.setField(1, umidadeSolo);
    if (acionar == 1){
      ThingSpeak.setField(2, acionar);
      acionar = 0;
    }
    tempoAnterior = millis();
    int x = ThingSpeak.writeFields(CHANNEL, WRITE_API);
    if (x == 200) {
      Serial.println("Update realizado!");
    }
    else {
      Serial.println("Problema no canal - erro HTTP" + String (x));
    }
  }
  delay(10000);
}
