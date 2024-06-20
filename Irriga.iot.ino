#include <WiFi.h>
#include <ThingSpeak.h>

//id e senha da rede
const char* ssid = "Isa";
const char* password =  "isabelle123";
WiFiClient client;


//chaves de conexão do thingspeak
const long CHANNEL = "2567533";
const char *WRITE_API = "C0AESWMJU4GDDL0G";
const char *READ_API = "D4IH5OS02S8CZTUD";
int intervaloTempo = 15000; // intervalo de tempo para escrever no Thing Speak write
unsigned long tempoAnterior, tempoAgora;


//pinos, variáveis e constantes do módulo 
#define sensorUmidade 0 
#define ledVermelho 
#define ledAmarelo 
#define bomba 
const int limiteUmidade = 72; //umidade ideal do solo
int umidadeSolo = 0;
bool acionar = 0;


void setup() {

  pinMode (sensorUmidade, INPUT);
  pinMode (ledVermelho, OUTPUT);
  pinMode (ledAmarelo, OUTPUT);
  pinMode (bomba, OUTPUT);

  Serial.begin(9600);
  
  if (WiFi.status() !=  WL_CONNECTED) {
    Serial.print("Conectando à rede ")
    Serial.println(ssid)
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      Serial.println("Conectando...");
      delay(1000);
  }  
  Serial.println("Conexão bem sucedida!");
 }
  tempoAnterio = 0;
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client); //inicializa o ThingSpeak 

}

void leituraSensor (){
    
    umidadeSolo = analogRead(sensorUmidade); //faz a leitura do sensor de umidade do solo
    umidadeSolo = map(umidadeSolo, 1023, 0, 0, 100); //converte a variação do sensor de 0 a 1023 para 0 a 100
    //mostra a mensagem
    Serial.println("Umidade: ");
    Serial.print(umidadeSolo); 
    Serial.println("% ");

}

void bombear(){
  if (umidadeSolo < limiteUmidade){
    Serial.println("Regando...");
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(bomba, HIGH);
    acionar = 1;
  }
  else{
    Serial.println("Umidade ideal!");
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(bomba, LOW);
  }
}


void loop() { 

  tempoAgora = millis();
  if ((tempoAgora - tempoAnterior > intervaloTempo)){
    ThingSpeak.setField(1, umidadeSolo);
    ThingSpeak.setField(2, acionar);
    leituraSensor();
    bombear();
    acionar = 0;
  }
  int x = ThingSpeak.writeFields(CHANNEL, WRITE_API);
    if (x == 200) {
      Serial,println("Update realizado!");
  }
    else {
      Serial.println("Problema no canal - erro HTTP" + String (x));
  }
  

}
