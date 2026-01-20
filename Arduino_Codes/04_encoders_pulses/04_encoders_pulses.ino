/* 04_encoders_pulses
Contador de pulsos de ambos encoders y saber el sentido de giro*/

// ---------- Pines de conexión de los encoders -----------------
#define L_EncA 35  // Pin de interrupción de motor izquierdo Canal A
#define L_EncB 34   // Pin para verificar sentido de motor izquierdo Canal B
#define R_EncA 39   // Pin de interrupción de motor derecho Canal A
#define R_EncB 36   // Pin para verificar sentido de motor derecho Canal B


// --------------------- Variables -----------------------------
volatile long L_pulses = 0; 
volatile long R_pulses = 0; 


// ---------------------- Interrupciones -----------------------------
void IRAM_ATTR L_PulsesCounter(){
  if (digitalRead(L_EncB) == HIGH){     // si B es HIGH, sentido horario
    L_pulses++ ;        // incrementa PULSES en 1
  }
  else {          // si B es LOW, sentido anti horario
    L_pulses-- ;        // decrementa el PULSES en 1
  }
}

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
  Serial.print("PulsosIzquierda: ");
  Serial.print(L_pulses);
  Serial.print("  --  PulsosDerecha: ");
  Serial.println(R_pulses);
  delay(10);
}


// ----------------------- Setup -----------------------------
void setup() {
  pinMode(L_EncA, INPUT);
  pinMode(L_EncB, INPUT);
  pinMode(R_EncA, INPUT);
  pinMode(R_EncB, INPUT);


  attachInterrupt(L_EncA, L_PulsesCounter, RISING);
  attachInterrupt(R_EncA, R_PulsesCounter, RISING);
  
  Serial.begin(115200);
}


// ----------------------- Loop -----------------------------
void loop() {
  printValues();
}

