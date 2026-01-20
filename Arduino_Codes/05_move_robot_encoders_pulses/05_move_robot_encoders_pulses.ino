/* 05_move_robot_encoders_pulses
Movimiento motores y lectura de los pulsos de los encoders*/

// ---------- Pines de conexión de los encoders -----------------
#define L_EncA 35   // Pin de interrupción de motor izquierdo Canal A
#define L_EncB 34   // Pin para verificar sentido de motor izquierdo Canal B
#define R_EncA 39   // Pin de interrupción de motor derecho Canal A
#define R_EncB 36   // Pin para verificar sentido de motor derecho Canal B

// ---------- Pines de conexión de los motores -----------------
#define LMot_dir 32     // Dir1 Cytron
#define LMot_pwm 33     // PWM1 Cytron
#define RMot_dir 25     // Dir2 Cytron 
#define RMot_pwm 26     // PWM2 Cytron


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
void moveRobot(float linealVel, float angularVel){
// Variables para calcular las señales PWM de cada motor
  int L_pwm = 0;
  int R_pwm = 0;

  L_pwm = linealVel*100 - angularVel*100;        // Se multiplican los valores en m/s y rad/s por un factor de 100 cada uno para obtener el valor de la rueda izq
  L_pwm = constrain(L_pwm, -100, 100);    // Restringuiendo límites para rueda izquierda para no salturarla
  L_pwm = map(L_pwm,-100,100, -255, 255); // Se escala para tener el valor de PWM de la rueda izquierda

  R_pwm = linealVel*100 + angularVel*100;          // Se multiplican los valores en m/s y rad/s por un factor de 100 cada uno para obtener el valor de la rueda der
  R_pwm = constrain(R_pwm, -100, 100);        // Restringuiendo límites para rueda derecha para no salturarla
  R_pwm = map(R_pwm,-100,100, -255, 255);     // Se escala para tener el calor de PWM de la rueda derecha
  
// Condiciones para establecer el giro del motor izquierdo
  if (L_pwm >= 0){
    // Movimiento sentido antihorario
    digitalWrite(LMot_dir,LOW);
  }
  
  else {
    // Movimiento sentido horario
    digitalWrite(LMot_dir,HIGH);
  }
  
  // Condiciones para establecer el giro del motor derecho
  if (R_pwm >= 0){
    // Movimiento sentido horario
    digitalWrite(RMot_dir,LOW);
  }
  
  else {
    // Movimiento sentido horario
    digitalWrite(RMot_dir,HIGH);
  }

  // Aplicar señales PWM a los motores
  analogWrite(LMot_pwm, abs(L_pwm));
  analogWrite(RMot_pwm, abs(R_pwm));
}


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

  pinMode(LMot_dir, OUTPUT);
  pinMode(RMot_dir, OUTPUT);

  attachInterrupt(L_EncA, L_PulsesCounter, RISING);
  attachInterrupt(R_EncA, R_PulsesCounter, RISING);
  
  Serial.begin(115200);
}


// ----------------------- Loop -----------------------------
void loop() {
  moveRobot(0.5, 0.0);  // Mover recto hacia adelante 
  printValues();
  Serial.println("Forward");
  delay(2000);         // Esperar 2 segundos

  moveRobot(-0.5, 0);  // Mover recto hacia atrás 
  printValues();
  Serial.println("Back");
  delay(2000);         // Esperar 2 segundos

  moveRobot(0.0, 0.5); // Girar hacia la izquierda
  printValues();
  Serial.println("Left");
  delay(2000);         // Esperar 2 segundos

  moveRobot(0.0, -0.5);  // Girar hacia la derecha
  printValues();
  Serial.println("Right");
  delay(2000);         // Esperar 2 segundos

  moveRobot(0, 0);    // Detenido
  printValues();
  Serial.println("Stop");
  delay(2000);         // Esperar 2 segundos
}

