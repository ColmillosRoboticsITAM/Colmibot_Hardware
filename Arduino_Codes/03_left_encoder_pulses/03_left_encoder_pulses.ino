/* 03_left_encoder_pulses
Contador de pulsos del encoder izquierdo y saber el sentido de giro*/

// ---------- Pines de conexión de los encoders -----------------
#define L_EncA 35  // Pin de interrupción de motor izquierdo Canal A
#define L_EncB 34   // Pin para verificar sentido de motor izquierdo Canal B


// --------------------- Variables -----------------------------
volatile long L_pulses = 0; 


// ---------------------- Interrupciones -----------------------------
void IRAM_ATTR L_PulsesCounter(){
  if (digitalRead(L_EncB) == HIGH){     // si B es HIGH, sentido horario
    L_pulses++ ;        // incrementa PULSES en 1
  }
  else {          // si B es LOW, sentido anti horario
    L_pulses-- ;        // decrementa el PULSES en 1
  }
}


// ---------------------- Funciones -----------------------------
void printValues(){
  Serial.print("PulsosIzquierda: ");
  Serial.println(L_pulses);
  delay(10);
}


// ----------------------- Setup -----------------------------
void setup() {
  pinMode(L_EncA, INPUT);
  pinMode(L_EncB, INPUT);

  attachInterrupt(L_EncA, L_PulsesCounter, RISING);
  
  Serial.begin(115200);
}


// ----------------------- Loop -----------------------------
void loop() {
  printValues();
}

