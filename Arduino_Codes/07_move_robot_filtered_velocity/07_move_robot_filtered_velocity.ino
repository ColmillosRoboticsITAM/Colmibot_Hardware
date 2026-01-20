/* 08_move_robot_filtered_velocity
Movimiento motores y lectura de la velocidad angular con filtro pasa bajas*/

// ---------- Pines de conexión de los encoders -----------------
#define L_EncA 35  // Pin de interrupción izquierda Canal A
#define L_EncB 34   // Pin para verificar sentido de rueda izquierda Canal B
#define R_EncA 39   // Pin de interrupción derecha Canal A
#define R_EncB 36   // Pin para verificar sentido de rueda derecha Canal B

// ---------- Pines de conexión de los motores -----------------
#define LMot_dir 32     // Dir1 Cytron
#define LMot_pwm 33      // PWM1 Cytron
#define RMot_dir 25      // Dir2 Cytron 
#define RMot_pwm 26     // PWM2 Cytron

int inf = 0;
int sup = 4;
// --------------------- Variables -----------------------------
volatile long L_pulses = 0; 
volatile long R_pulses = 0; 

// Variables para el cálculo de la diferencia de pulsos
volatile long current_L_pulses = 0; 
volatile long current_R_pulses = 0; 

#define PPR 480       // Pulsos por revolución tomando en cuenta sólo el flanco de subida (16*30)
const unsigned long sampleTime = 100; // Intervalo de tiempo para calcular la velocidad (en milisegundos)

// Variables para el cálculo del tiempo
volatile unsigned long previousTime = 0; 
volatile unsigned long currentTime = 0; 
volatile float deltaTime  = 0;  // deltaTime = currentTime - previousTime

// Variables para el Cálculo de Velocidad 
float L_ang_Velocity_rev_s = 0.0;
float R_ang_Velocity_rev_s = 0.0;

// Variables para el cálculo de la diferencia de pulsos
float L_vel_Filt = 0;
float L_vel_Prev = 0;
float R_vel_Filt = 0;
float R_vel_Prev = 0;

float L_ang_Velocity_rad_s = 0.0;
float R_ang_Velocity_rad_s = 0.0;


// ---------------------- Funciones -----------------------------
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

  L_pwm = linealVel*50 - angularVel*50;        // Se multiplican los valores en m/s y rad/s por un factor de 100 cada uno para obtener el valor de la rueda izq
  L_pwm = constrain(L_pwm, -150, 150);    // Restringuiendo límites para rueda izquierda para no salturarla
  L_pwm = map(L_pwm,-150,150, -255, 255); // Se escala para tener el valor de PWM de la rueda izquierda

  R_pwm = linealVel*50 + angularVel*50;          // Se multiplican los valores en m/s y rad/s por un factor de 100 cada uno para obtener el valor de la rueda der
  R_pwm = constrain(R_pwm, -150, 150);        // Restringuiendo límites para rueda derecha para no salturarla
  R_pwm = map(R_pwm,-150,150, -255, 255);     // Se escala para tener el calor de PWM de la rueda derecha
  
// Condiciones para establecer los 4 movimientos del robot
  if (L_pwm > 10 && R_pwm > 10){
    // Movimiento recto al frente
    digitalWrite(LMot_dir,LOW);
    digitalWrite(RMot_dir,LOW);
  }
  
  else if (L_pwm < -10 && R_pwm < -10){
    // Movimiento recto atrás
    digitalWrite(LMot_dir,HIGH);
    digitalWrite(RMot_dir,HIGH);
  }

  else if (L_pwm < -10 && R_pwm > 10){
    // Giro a la izquierda
    digitalWrite(LMot_dir,HIGH);
    digitalWrite(RMot_dir,LOW);
  } 
  else if (L_pwm > 10 && R_pwm < -10){
    // Giro a la derecha
    digitalWrite(LMot_dir,LOW);
    digitalWrite(RMot_dir,HIGH);
  }

  // Aplicar señales PWM a los motores
  analogWrite(LMot_pwm, abs(L_pwm));
  analogWrite(RMot_pwm, abs(R_pwm));
  measureSpeed();
}


void measureSpeed(){
  currentTime = millis();

  // Calcular la velocidad angular cada sampleTime
  if (currentTime - previousTime >= sampleTime) {
    current_L_pulses = L_pulses;
    current_R_pulses = R_pulses;
    L_pulses = 0;                 // Reinicia el contador de la rueda izquierda para el próximo intervalo
    R_pulses = 0;                 // Reinicia el contador de la rueda izquierda para el próximo intervalo

    deltaTime = currentTime - previousTime;
    previousTime = currentTime;   // Actualiza el tiempo de la última lectura

    // Calcular las revoluciones por segundo del MOTOR
    L_ang_Velocity_rev_s = ((float)current_L_pulses)*10 / PPR;
    R_ang_Velocity_rev_s = ((float)current_R_pulses)*10 / PPR;

    // Filtro pasa-baja con 25 Hz de corte
    L_vel_Filt = 0.854*L_vel_Filt + 0.0728*L_ang_Velocity_rev_s + 0.0728*L_vel_Prev;
    L_vel_Prev = L_ang_Velocity_rev_s;


    // Convertir revoluciones por segundo a radianes por segundo
    L_ang_Velocity_rad_s = L_ang_Velocity_rev_s * 2 * PI;
    R_ang_Velocity_rad_s = R_ang_Velocity_rev_s * 2 * PI;
  }
}


void printValues(){
  /*Serial.print("Tiempo: ");
  Serial.print(currentTime);
  Serial.print(" ms | Delta_t: ");
  Serial.print(deltaTime);
  Serial.print(" | Pulsos en intervalo: ");
  Serial.print(current_L_pulses);
  Serial.print(" -  ");
  Serial.print(current_R_pulses);
  Serial.print(" | Velocidad angular: ");
  Serial.print(L_ang_Velocity_rad_s, 2); // 2 decimales
  Serial.print(" rad/s - ");
  Serial.print(R_ang_Velocity_rad_s, 2); // 2 decimales
  Serial.print(" rad/s | ");*/
  Serial.print(L_ang_Velocity_rev_s, 2); // 2 decimales
  Serial.print(" - ");
  Serial.print(L_vel_Filt, 2); // 2 decimales
  //Serial.print(" rev/s");
  Serial.print(" - ");
  Serial.print(inf);
  Serial.print(" - ");
  Serial.println(sup);
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
  moveRobot(3.0, 0.0);  // Mover recto hacia adelante 
  printValues();
  
  /*Serial.println("Forward");
  delay(2000);         // Esperar 2 segundos

  moveRobot(-1.0, 0);  // Mover recto hacia atrás 
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

