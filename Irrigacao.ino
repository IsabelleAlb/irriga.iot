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
  bombear(leituraSensor);
  delay(3000); //espera 3segundos para fazer uma nova leitura
  }


  int leituraSensor(){
    
    umidadeSolo = analogRead(pinSensor); //faz a leitura do sensor de umidade do solo
    umidadeSolo = map(umidadeSolo, 1023, 0, 0, 100); //converte a variação do sensor de 0 a 1023 para 0 a 100
    //mostra a mensagem
    Serial.println("Umidade: ");
    Serial.print(umidadeSolo); 
    Serial.println("% ");
    
  }

  void bombear (leituraSensor) {
    
if (umidadeSolo < limiteUmidade){
  Serial.println("Regando...");
  digitalWrite(ledVermelho, HIGH);
  digitalWrite(ledAmarelo, LOW);
  digitalWrite(bomba, HIGH);
  delay(15000);
  digitalWrite(bomba, LOW);
  }
else {
  Serial.println("Solo umido");
  digitalWrite(ledAmarelo, HIGH);
  digitalWrite(ledVermelho, LOW);
  digitalWrite(bomba, LOW);
  }
 
}
