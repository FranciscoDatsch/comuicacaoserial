const int ledPin = 2;

void setup(){
  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, LOW);

  Serial.begin(115200);

  Serial.println("Digite on para ligar ou off para desligar o LED:");
}

void loop(){
  // Aguarda até que haja dados disponíveis no buffer serial
  while (!Serial.available()) {
    // Pequeno atraso para evitar ociosa repetição do loop
    delay(10);
  }

  String comando = Serial.readStringUntil('\n');
  comando.trim();
  
  if (comando == "on"){
    digitalWrite(ledPin, HIGH);
    Serial.print("LED ligado.\n");
  }
  else if(comando == "off"){
    digitalWrite(ledPin, LOW);
    Serial.print("Led desligado.\n");
  }
  else{
    Serial.print("Comando inválido, tente novamente.\n");
  }
  
  Serial.println("Digite on para ligar ou off para desligar o LED:");
}