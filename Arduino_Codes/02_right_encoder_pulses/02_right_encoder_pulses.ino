/* 02_right_encoder_pulses
Contador de pulsos del encoder derecho y saber el sentido de giro*/

// ---------- Pines de conexión de los encoders -----------------
#define R_EncA 39   // Pin de interrupción de motor derecho Canal A
#define R_EncB 36   // Pin para verificar sentido de motor derecho Canal B


// --------------------- Variables -----------------------------
volatile long R_pulses = 0; 


// ---------------------- Interrupciones -----------------------------
void IRAM_ATTR R_PulsesCounter(){
  if (digitalRead(R_EncB) == HIGH){     // si B es HIGH, sentido horario
    R_pulses-- ;        // incrementa PULSES en 1
  }
  else {          // si B es LOW, sentido anti horario
    R_pulses++ ;        // decrementa el PULSES en 1
  }
}


// ---------------------- Funciones -----------------------------
void printValues(){
  Serial.print("PulsosDerecha: ");
  Serial.println(R_pulses);
  delay(10);
}


// ----------------------- Setup -----------------------------
void setup() {
  pinMode(R_EncA, INPUT);
  pinMode(R_EncB, INPUT);

  attachInterrupt(R_EncA, R_PulsesCounter, RISING);
  
  Serial.begin(115200);
}


// ----------------------- Loop -----------------------------
void loop() {
  printValues();
}

