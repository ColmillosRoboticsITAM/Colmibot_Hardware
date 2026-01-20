/* 08_robot_velocity_control
Envío de la velocidad lineal y angular de cada motor y PID*/

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
// Variables para el cálculo de la diferencia de pulsos
volatile long L_pulses = 0; 
volatile long R_pulses = 0; 

// Variables para el cálculo de la diferencia de pulsos
volatile long current_L_pulses = 0; 
volatile long current_R_pulses = 0; 

// Parámetros del robot
#define PPR 480       // Pulsos por revolución tomando en cuenta sólo el flanco de subida (16*30)
#define RADIO_RUEDA 0.03 // Metros 
#define SEPARACION_RUEDAS 0.2 // Metros

// Variables para el cálculo del tiempo
volatile unsigned long previousTime = 0; 
volatile unsigned long currentTime = 0; 
volatile float deltaTime  = 0;  // deltaTime = currentTime - previousTime

const unsigned long sampleTime = 100; // Intervalo de tiempo para calcular la velocidad (en milisegundos)

// Variables PID
float errorAnteriorIzq = 0;
float integralIzq = 0;
float errorAnteriorDer = 0;
float integralDer = 0;
#define KP 0.5   // Ganancia Proporcional
#define KI 0.1   // Ganancia Integral
#define KD 0.05  // Ganancia Derivativa

// Variables para el Cálculo de Velocidad 
float L_angVel_rps = 0.0;
float R_angVel_rps = 0.0;

float L_angVel_rad_s = 0.0;
float R_angVel_rad_s = 0.0;


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
// Calcular velocidades angulares de cada rueda (rad/s)
  float L_tangVel_setpoint = linealVel - (angularVel * SEPARACION_RUEDAS / 2.0);  
  float R_tangVel_setpoint = linealVel + (angularVel * SEPARACION_RUEDAS / 2.0);  

// Convertir velocidad tangencial a velocidad angular de la rueda (rad/s)
  L_angVel_rad_s_setpoint = L_tangVel_setpoint / RADIO_RUEDA;
  R_angVel_rad_s_setpoint = R_tangVel_setpoint / RADIO_RUEDA;

// Calcular velocidades angulares actuales de las ruedas (rad/s)
  measureVel();
  /*espacio para pwm




  */

  
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


void measureVel(){
  currentTime = millis();

  // Calcular la velocidad angular cada sampleTime (100 ms)
  if (currentTime - previousTime >= sampleTime) {
    deltaTime = currentTime - previousTime;
    previousTime = currentTime;   // Actualiza el tiempo de la última lectura
    current_L_pulses = L_pulses;
    current_R_pulses = R_pulses;

    // Calcular las revoluciones por segundo del MOTOR
    L_angVel_rps = ((float)current_L_pulses) / (0.1*PPR);
    R_angVel_rps = ((float)current_R_pulses) / (0.1*PPR);

    // Convertir revoluciones por segundo a radianes por segundo
    L_angVel_rad_s = L_angVel_rps * 2 * PI;
    R_angVel_rad_s = R_angVel_rps * 2 * PI;

    L_pulses = 0;                 // Reinicia el contador del motor izquierdo para el próximo intervalo
    R_pulses = 0;                 // Reinicia el contador del motor derecho para el próximo intervalo
  }
}


void printValues(){
  Serial.print("Tiempo: ");
  Serial.print(currentTime);
  Serial.print(" ms | Delta_t: ");
  Serial.print(deltaTime);
  Serial.print(" | Pulsos en intervalo: ");
  Serial.print(current_L_pulses);
  Serial.print(" -  ");
  Serial.print(current_R_pulses);
  Serial.print(" | Velocidad angular: ");
  Serial.print(L_angVel_rad_s, 2); // 2 decimales
  Serial.print(" rad/s - ");
  Serial.print(R_angVel_rad_s, 2); // 2 decimales
  Serial.print(" rad/s | ");
  Serial.print(L_angVel_rps, 2); // 2 decimales
  Serial.print(" - ");
  Serial.print(R_angVel_rps, 2); // 2 decimales
  Serial.println(" rev/s");
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
  moveRobot(0.3, 0.0);  // Mover recto hacia adelante 
  printValues();
  Serial.println("Forward");
  //delay(2000);         // Esperar 2 segundos

  /*moveRobot(-1.0, 0);  // Mover recto hacia atrás 
  printValues();
  Serial.println("Back");
  delay(2000);         // Esperar 2 segundos

  moveRobot(0.0, 1.0); // Girar hacia la izquierda
  printValues();
  Serial.println("Left");
  delay(2000);         // Esperar 2 segundos

  moveRobot(0.0, -1.0);  // Girar hacia la derecha
  printValues();
  Serial.println("Right");
  delay(2000);         // Esperar 2 segundos

  moveRobot(0, 0);    // Detenido
  printValues();
  Serial.println("Stop");
  delay(2000);         // Esperar 2 segundos*/
}

