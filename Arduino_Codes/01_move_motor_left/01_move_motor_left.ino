/* 01_move_motor_Left
 Movimiento de un sólo motor del robot Colmibot con movimientos básico*/

// ---------- Pines de conexión de los motores -----------------
#define LMot_dir 32     // Dir1 Cytron
#define LMot_pwm 33      // PWM1 Cytron


// --------------------- Variables -----------------------------



// ---------------------- Funciones -----------------------------
void moveMotor(float Vel) {
// Variables para calcular las señales PWM de cada motor
  int L_pwm = 0;

  L_pwm = Vel*100;        // Se multiplican los valores en m/s y rad/s por un factor de 100 cada uno para obtener el valor de la rueda izq
  L_pwm = constrain(L_pwm, -100, 100);    // Restringuiendo límites para rueda izquierda para no salturarla
  L_pwm = map(L_pwm,-100,100, -255, 255); // Se escala para tener el valor de PWM de la rueda izquierda
  
// Condiciones para establecer el giro del motor izquierdo
  if (L_pwm >= 0){
    // Movimiento sentido antihorario
    digitalWrite(LMot_dir,LOW);
  }
  
  else {
    // Movimiento sentido horario
    digitalWrite(LMot_dir,HIGH);
  }

  // Aplicar señal PWM al motor
  analogWrite(LMot_pwm, abs(L_pwm));
}



// ----------------------- Setup -----------------------------
void setup() {
  // Configuración de pines como salida o entrada
  pinMode(LMot_dir, OUTPUT);
  Serial.begin(115200);
}


// ----------------------- Loop -----------------------------
void loop() {
  // Ejemplo de uso de la función moverRobot
  moveMotor(0.5);  // Mover recto hacia adelante 
  Serial.println("Forward");
  delay(4000);         // Esperar 2 segundos
  moveMotor(-0.5);  // Mover recto hacia atrás 
  Serial.println("Back");
  delay(2000);         // Esperar 2 segundos
  moveMotor(0.0);    // Detenido
  Serial.println("Stop");
  delay(2000);         // Esperar 2 segundos
}

