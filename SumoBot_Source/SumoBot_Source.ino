/*============================================================ - LIBRARIES & PRE-PROCESSOR INSTRUCTIONS - ============================================================*/
//LIBRARIES
#include <AccelStepper.h>
#include "BluetoothSerial.h"

//PRE-PROCESSORS INSTRUCTIONS
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

/*============================================================ - CONSTS DECLARATION - ============================================================*/
const int stepsPerRevolution = 200;

//LOOPDELAY - INDICA EL RETRASO QUE TENDRÁ EL MÉTODO VOID-LOOP
int LOOPDELAY = 50;

//S_MONITOR_BAUDRATE - INDICA LOS BAUDIOS DEL MONITOR SERIAL
int S_MONITOR_BAUDRATE = 9600;

//DOWN_LIMIT_DISTANCE - INDICA LA EL MÍNIMO DE DISTANCIA PARA SER CONSIDERADA VÁLIDA (MEDIDO EN CM)
float DOWN_LIMIT_DISTANCE = 0;

//UP_LIMIT_DISTANCE - INDICA LA EL MAXIMO DE DISTANCIA PARA SER CONSIDERADA VÁLIDA (MEDIDO EN CM)
float UP_LIMIT_DISTANCE = 45;

/*============================================================ - PIN DECLARATIONS - ============================================================*/

//BLUETOOTH INSTANTIATION
BluetoothSerial SerialBT;

/*============================== - LEDS - ==============================*/

//TRYLED - LED DE PRUEBAS
int tryled = 2;

/*=============== - INFRARED SENSORS - ===============*/

//1ST INFRARED (IR1) - PRIMER INFRARROJO
int IR1 = 23;

//2ND INFRARED (IR2) - SEGUNDO INFRARROJO
int IR2 = 4;

//3RD INFRARED (IR3) - TERCER INFRARROJO
int IR3 = 13;

//4TH INFRARED (IR4) - CUARTO INFRARROJO
int IR4 = 34;

/*=============== - ULTRASONIC SENSORS - ===============*/

//1ST ULTRASONIC (US1) - PRIMER ULTRASÓNICO
int US1_TRIGG = 32;
int US1_ECHO = 35;

//2ND ULTRASONIC (US2) - SEGUNDO ULTRASÓNICO
//int US2_TRIGG = 21;
int US2_TRIGG = 21;
int US2_ECHO = 19; // 22

//3RD ULTRASONIC (US3) - TERCER ULTRASÓNICO
int US3_TRIGG = 33;
int US3_ECHO = 25;

/*============================== - MOTORS - ==============================*/

// CONEXIONES DEL MOTOR IZQUIERDO
const int AIn1_IZQ = 26;
const int AIn2_IZQ = 27;
const int BIn1_IZQ = 14;
const int BIn2_IZQ = 12;

// CONEXIONES DEL MOTOR DERECHO
const int AIn1_DER = 22;
const int AIn2_DER = 5;
const int BIn1_DER = 17;
const int BIn2_DER = 16;

const int NORM_SPD = 300;
const int SLOW_SPD = 270;

AccelStepper STEPPER_LEFT(AccelStepper::FULL4WIRE, AIn1_IZQ, AIn2_IZQ, BIn1_IZQ, BIn2_IZQ);  // PARA CONTROLAR EL MOTOR IZQUIERDO
AccelStepper STEPPER_RIGHT(AccelStepper::FULL4WIRE, AIn1_DER, AIn2_DER, BIn1_DER, BIn2_DER);  // PARA CONTROLAR EL MOTOR DERECHO

/*============================================================ - SETUP - ============================================================*/
void setup() {

  //INITIALIZING LED
  pinMode(tryled, OUTPUT);

  //INITIALIZING IR-SENSORS - DIGITAL INPUT
  //IR1
  pinMode(IR1, INPUT);

  //IR2
  pinMode(IR2, INPUT);

  //IR3
  pinMode(IR3, INPUT);

  //IR4
  pinMode(IR4, INPUT);

  //INITIALIZING US-SENSORS - DIGITAL INPUT-OUTPUT
  //US1
  //TRIGGER
  pinMode(US1_TRIGG, OUTPUT);
  //ECHO
  pinMode(US1_ECHO, INPUT);

  //US2
  //TRIGGER
  pinMode(US2_TRIGG, OUTPUT);
  //ECHO
  pinMode(US2_ECHO, INPUT);

  //US3
  //TRIGGER
  pinMode(US3_TRIGG, OUTPUT);
  //ECHO
  pinMode(US3_ECHO, INPUT);

  pinMode(AIn1_IZQ, OUTPUT);
  pinMode(AIn2_IZQ, OUTPUT);
  pinMode(BIn1_IZQ, OUTPUT);
  pinMode(BIn2_IZQ, OUTPUT);

  pinMode(AIn1_DER, OUTPUT);
  pinMode(AIn2_DER, OUTPUT);
  pinMode(BIn1_DER, OUTPUT);
  pinMode(BIn2_DER, OUTPUT);
  
  // ACELERACIÓN, VELOCIDAD Y VELOCIDAD MAXIMA DEL MOTOR IZQUIERDO
  STEPPER_LEFT.setAcceleration(150);
  STEPPER_LEFT.setMaxSpeed(500);
  STEPPER_LEFT.setSpeed(NORM_SPD);
  
  // ACELERACIÓN, VELOCIDAD Y VELOCIDAD MAXIMA DEL MOTOR DERECHO  
  STEPPER_RIGHT.setAcceleration(150);
  STEPPER_RIGHT.setMaxSpeed(500);
  STEPPER_RIGHT.setSpeed(NORM_SPD);

  //INITIALIZING SERIAL MONITOR
  Serial.begin(S_MONITOR_BAUDRATE);
  SerialBT.begin("Ragnar-SumoBot"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

/*============================================================ - FUNCTIONS - ============================================================*/

/*============================== - INFRARED SENSORS - ==============================*/

/*
  SENSE_SINGLE_IR(INT IRPIN) - SENSA EL SENSOR DECLARADO EN IRPIN Y RETORNA
  TRUE SI SE ENCUENTRA DENTRO DEL ÁREA VÁLIDA (COLOR NEGRO)
  FALSE SI SE ENCUENTRA FUERA DEL ÁREA VÁLIDA (CUALQUIER COLOR DIFERENTE A NEGRO)
*/
bool sense_single_ir(int irpin) {
  if (digitalRead(irpin) == LOW) {
    return true;
  }
  else {
    return false;
  }
}

/*
  SENSE_ALL_IR() - SENSA TODOS LOS INFRARROJOS Y RETORNA
  TRUE SI TODOS SE ENCUENTRAN DENTRO DEL ÁREA VÁLIDA (COLOR NEGRO)
  FALSE SI AL MENOS UNO DE LOS SENSORES SE ENCUENTRA FUERA DEL ÁREA VÁLIDA (CUALQUIER OTRO COLOR DIFERENTE A NEGRO)
*/
bool sense_all_ir() {
  //REVISA TODOS LOS INFRARROJOS
  if (sense_single_ir(IR1) == true && sense_single_ir(IR2) == true && sense_single_ir(IR3) == true && sense_single_ir(IR4) == true) {
    Serial.println("En el area");
    return true;
  }
  else {
    Serial.println("Fuera el area");
    return false;
  }
}

int find_out_ir() {
  //STATE_IR - AYUDAN A SABER EL ESTADO DE LOS INFRARROJOS Y A RETORNAR CUALES ESTÁN O NO EN EL ÁREA VÁLIDA
  bool state_ir1 = sense_single_ir(IR1);
  bool state_ir2 = sense_single_ir(IR2);
  bool state_ir3 = sense_single_ir(IR3);
  bool state_ir4 = sense_single_ir(IR4);

  if (state_ir1 == false && state_ir2 == false && state_ir3 == false && state_ir4 == false) {
    //TODOS LOS SENSORES SE ENCUENTRAN FUERA DEL ÁREA VÁLIDA - HEMOS PERDIDO
    Serial.println("Hemos perdido");
    return 1234;
  }

  if (state_ir1 == false && state_ir2 == false && state_ir3 == false && state_ir4 == true) {
    //SENSORES 1, 2 Y 3 FUERA DEL ÁREA - SENSOR DELANTERO IZQUIERDO EN EL ÁREA
    return 123;
  }

  if (state_ir1 == false && state_ir2 == false && state_ir3 == true && state_ir4 == false) {
    //SENSORES 1, 2 Y 4 FUERA DEL ÁREA - SENSOR TRASERO IZQUIERDO EN EL ÁREA
    return 124;
  }

  if (state_ir1 == false && state_ir2 == false && state_ir3 == true && state_ir4 == true) {
    //SENSORES 1 Y 2 FUERA DEL ÁREA - SENSORES IZQUIERDOS EN EL ÁREA
    return 12;
  }

  if (state_ir1 == false && state_ir2 == true && state_ir3 == false && state_ir4 == false) {
    //SENSORES 1, 3 Y 4 FUERA DEL ÁREA - SENSOR TRASERO DERECHO EN EL ÁREA
    return 134;
  }

  if (state_ir1 == false && state_ir2 == true && state_ir3 == true && state_ir4 == false) {
    //SENSORES 1 Y 4 FUERA DEL ÁREA - SENSORES TRASEROS EN EL ÁREA
    return 14;
  }

  if (state_ir1 == false && state_ir2 == true && state_ir3 == true && state_ir4 == true) {
    //SENSOR 1 FUERA DEL ÁREA - SENSORES TRASEROS Y DELANTERO IZQUIERDO DENTRO DEL ÁREA
    return 1;
  }

  if (state_ir1 == true && state_ir2 == false && state_ir3 == false && state_ir4 == false) {
    //SENSORES 2, 3 Y 4 FUERA DEL ÁREA - SENSOR DELANTERO DERECHO DENTRO DEL ÁREA
    return 234;
  }

  if (state_ir1 == true && state_ir2 == false && state_ir3 == false && state_ir4 == true) {
    //SENSORES 2 Y 3 FUERA DEL ÁREA - SENSORES DELANTEROS DENTRO DEL ÁREA
    return 23;
  }

  if (state_ir1 == true && state_ir2 == false && state_ir3 == true && state_ir4 == true) {
    //SENSOR 2 FUERA DEL ÁREA - SENSORES DELANTEROS Y TRASERO IZQUIERDO DENTRO DEL ÁREA
    return 2;
  }

  if (state_ir1 == true && state_ir2 == true && state_ir3 == false && state_ir4 == false) {
    //SENSORES 3 Y 4 FUERA DEL ÁREA - SENSORES DERECHOS DENTRO DEL ÁREA
    return 34;
  }

  if (state_ir1 == true && state_ir2 == true && state_ir3 == false && state_ir4 == true) {
    //SENSOR 3 FUERA DEL ÁREA - SENSORES DELANTEROS Y TRASERO DERECHO DENTRO DEL ÁREA
    return 3;
  }

  if (state_ir1 == true && state_ir2 == true && state_ir3 == true && state_ir4 == false) {
    //SENSOR 4 FUERA DEL ÁREA - SENSORES TRASEROS Y DELANTERO DERECHO DENTRO DEL ÁREA
    return 4;
  }

  if (state_ir1 == true && state_ir2 == true && state_ir3 == true && state_ir4 == true) {
    //TODOS LOS SENSORES SE ENCUENTRAN DENTRO DEL ÁREA VÁLIDA
    return 0;
  }
}

/*============================== - ULTRA SONIC SENSORS - ==============================*/

/*
  SENSE_SINGLE_US(INT US_PIN_OUT, INT US_PIN_IN) - SENSA EL SENSOR DECLARADO
  US_PIN_TRIGG (TRIGGER) - PIN PARA ENVIAR LA SEÑAL SÓNICA
  US_PIN_ECHO (ECHO) - PIN PARA RECIBIR LA SEÑAL SÓNICA
  SE RETORNA LA DISTANCIA OBTENIDA EN LA MEDICIÓN
*/
float sense_single_us(int us_pin_trigg, int us_pin_echo) {

  //Declarando variables auxiliares
  float duracion;
  float distancia_cm;

  //Enviando señal ultrasónica
  digitalWrite(us_pin_trigg, LOW);
  delayMicroseconds(4);
  digitalWrite(us_pin_trigg, HIGH);
  delayMicroseconds(10);
  digitalWrite(us_pin_trigg, LOW);

  //Recibiendo señal
  duracion = pulseIn(us_pin_echo, HIGH);

  //De tiempo a distancia
  distancia_cm = ((duracion / 2) * 0.0344);

  return distancia_cm;
}

/*
  VALIDATE_US_DISTANCE(FLOAT DISTANCE) - VALIDA QUE LA DISTANCIA SENSADA
  SEA VÁLIDA (ESTÉ EN EL RANGO VÁLIDO)
  RETORNA TRUE SI CUNMPLE LA CONDICIÓN Y FALSE SI NO LA CUMPLE
*/
bool validate_us_distance(float distance) {
  if (distance >= DOWN_LIMIT_DISTANCE && distance <= UP_LIMIT_DISTANCE) {
    return true;
  }
  else {
    return false;
  }
}

/*
  SENSE_ALL_US() - SENSA TODOS LOS SENSORES ULTRASÓNICOS
  TOMA LA DECISION INDICADA PARA QUE SEA EJECUTADA POR LOS MOTORES
*/
void sense_all_us() {
  //Distancias
  float distance_US_1 = sense_single_us(US1_TRIGG, US1_ECHO);
  float distance_US_2 = sense_single_us(US2_TRIGG, US2_ECHO);
  float distance_US_3 = sense_single_us(US3_TRIGG, US3_ECHO);

  //calculando cual es el sensor que más cerca tiene un objeto
  if (distance_US_1 < distance_US_2) {
    if (distance_US_1 < distance_US_3) {
      //El sensor ultrasónico 1 es el que tiene al objeto más cercano
      if (validate_us_distance(distance_US_1) == true) {
        //Avanzar hacia adelante
        Serial.print("Avanzar hacia adelante - Distancia objeto: ");
        Serial.print(distance_US_1);
        Serial.println(" cm");
        moving(-200,200,NORM_SPD,NORM_SPD);
      }
      else {
        //Avanzar un poco hacia adelante
        Serial.println("Distancia no válida - Avanzar un poco hacia adelante ");
      }
    }
    else {
      //El sensor ultrasónico 3 es el que tiene al objeto más cercano
      if (validate_us_distance(distance_US_3) == true) {
        //Girar 90° a la izquierda
        Serial.print("Girar 90° a la izquierda - Distancia objeto: ");
        Serial.print(distance_US_3);
        Serial.println(" cm");
        moving(-150,-135,NORM_SPD,SLOW_SPD);
      }
      else {
        //Avanzar un poco hacia adelante
        Serial.println("Distancia no válida - Avanzar un poco hacia adelante ");
      }
    }
  }
  else {
    if (distance_US_2 < distance_US_3) {
      //El sensor ultrasónico 2 es el que tiene al objeto más cercano
      if (validate_us_distance(distance_US_2) == true) {
        //Girar 90° a la derecha
        Serial.print("Girar 90° a la derecha - Distancia objeto: ");
        Serial.print(distance_US_2);
        Serial.println(" cm");
        moving(135,150,SLOW_SPD,NORM_SPD);
      }
      else {
        //Avanzar un poco hacia adelante
        Serial.println("Distancia no válida - Avanzar un poco hacia adelante ");
      }      
    }
    else {
      //El sensor ultrasónico 3 es el que tiene al objeto más cercano
      if (validate_us_distance(distance_US_3) == true) {
        //Girar 90° a la izquierda
        Serial.print("Girar 90° a la izquierda - Distancia objeto: ");
        Serial.print(distance_US_3);
        Serial.println(" cm");
      }
      else {
        //Avanzar un poco hacia adelante
        Serial.println("Distancia no válida - Avanzar un poco hacia adelante ");
      }      
    }
  }
}

void moving(long L_STEP,long R_STEP,long L_SPD,long R_SPD) {
  STEPPER_LEFT.moveTo(L_STEP);
  STEPPER_RIGHT.moveTo(R_STEP);
  STEPPER_LEFT.setSpeed(L_SPD);
  STEPPER_RIGHT.setSpeed(R_SPD);
  while((STEPPER_LEFT.distanceToGo()> 0) && (STEPPER_RIGHT.distanceToGo()> 0)) {
    STEPPER_LEFT.run();
    STEPPER_RIGHT.run();
    Serial.println("corriendo");
    SerialBT.println("Corriendo");
  }
  Serial.println("deteniendo");
  SerialBT.println("deteniendo");

}

/*============================================================ - LOOP - ============================================================*/
void loop() {

  //VERIFICANDO QUE SE ESTÉ DENTRO DEL ÁREA
  if (sense_all_ir() == true) {
    //TODOS LOS SENSORES ESTÁN DENTRO DEL ÁREA
    //SE PROCEDE A SENSAR CON LOS SENSORES ULTRASÓNICOS
    Serial.println("Todo en orden... Se procede a sensar ultrasonicos...");
    sense_all_us();
  }
  else {
    //AL MENOS UNO DE LOS SENSORES NO ESTÁ DENTRO DEL ÁREA

    //SE BUSCA CUAL O CUALES SENSORES SE ENCUENTRAN FUERA DEL ÁREA
    int irs_out_of_area = find_out_ir();

    //SE SELECCIONA DE ACUERDO AL CASO
    switch (irs_out_of_area) {

      case 1234:
        //RECONOCIMIENTO DE PÉRDIDA
        Serial.println("Todos los infrarojos estan fuera - Hemos perdido.");
        SerialBT.println("Todos los infrarojos estan fuera - Hemos perdido.");
        break;

      case 123:
        //GIRAR UN POCO A LA IZQUIERDA YENDO HACIA ADELANTE Y RUEDAS TRASERA HACIA ADELANTE
        Serial.println("IR-D Dentro del area - Gira 45° adelante-izquierda y luego avanza.");
        SerialBT.println("IR-D Dentro del area - Gira 45° adelante-izquierda y luego avanza.");
        break;

      case 124:
        //GIRAR UN POCO A LA IZQUIERDA YENDO HACIA ATRÁS Y RUEDAS DELANTERAS HACIA ATRÁS
        Serial.println("IR-C Dentro del area - Gira 45° atras-izquierda y luego retrocede.");
        SerialBT.println("IR-C Dentro del area - Gira 45° atras-izquierda y luego retrocede.");
        //Se puede girar luego 180°?
        break;

      case 12:
        //GIRAR 90 GRADOS A LA IZQUIERDA YENDO HACIA ADELANTE Y LUEGO AVANZAR
        Serial.println("IR-C e IR-D Dentro del area - Gira 90° adelante-izquierda y luego avanza.");
        SerialBT.println("IR-C e IR-D Dentro del area - Gira 90° adelante-izquierda y luego avanza.");
        moving(-150,-135,NORM_SPD,SLOW_SPD);
        break;

      case 134:
        //GIRAR UN POCO HACIA LA DERECHA YENDO HACIA ATRAS Y LUEGO RUEDAS DELANTERAS HACIA ATRÁS
        Serial.println("IR-B Dentro del area - Gira 45° atras-derecha y luego retrocede.");
        SerialBT.println("IR-B Dentro del area - Gira 45° atras-derecha y luego retrocede.");
        //Se puede girar luego 180°?
        break;

      case 14:
        // RETROCEDER
        Serial.println("IR-B e IR-C Dentro del area - Retrocede y gira 180°.");
        SerialBT.println("IR-B e IR-C Dentro del area - Retrocede y gira 180°.");
        moving(100,-100,NORM_SPD,NORM_SPD);
        break;

      case 1:
        //GIRAR UN POCO HACIA LA IZQUIERDA YENDO HACIA ATRAS Y LUEGO DAR VUELTA PARA BUSCAR OPONENTE
        Serial.println("IR-B, IR-C e IR-D Dentro del area - Gira 45° atras-izquierda y gira 180°.");
        SerialBT.println("IR-B, IR-C e IR-D Dentro del area - Gira 45° atras-izquierda y gira 180°.");
        moving(100,-100,NORM_SPD,NORM_SPD);
        break;

      case 234:
        //GIRAR HACIA LA DERECHA YENDO HACIA ADELANTE Y LUEGO TRASERAS HACIA ADELANTE
        Serial.println("IR-A Dentro del area - Gira 45° adelante-derecha y avanza.");
        SerialBT.println("IR-A Dentro del area - Gira 45° adelante-derecha y avanza.");
        break;

      case 23:
        //AVANZAR
        Serial.println("IR-A e IR-D Dentro del area - Avanza.");
        SerialBT.println("IR-A e IR-D Dentro del area - Avanza.");
        moving(-100,100,NORM_SPD,NORM_SPD);
        break;

      case 2:
        //GIRAR HACIA LA IZQUIERDA YENDO HACIA ADELANTE
        Serial.println("IR-A, IR-C e IR-D Dentro del area - gira 45° adelante-izquierda y avanza.");
        SerialBT.println("IR-A, IR-C e IR-D Dentro del area - gira 45° adelante-izquierda y avanza.");
        moving(-100,100,NORM_SPD,NORM_SPD);
        break;

      case 34:
        //GIRAR 90 GRADOS A LA DERECHA YENDO HACIA ADELANTE Y LUEGO AVANZAR
        Serial.println("IR-A e IR-B Dentro del area - gira 90° adelante-derecha y avanza.");
        SerialBT.println("IR-A e IR-B Dentro del area - gira 90° adelante-derecha y avanza.");
        moving(135,150,SLOW_SPD,NORM_SPD);
        break;

      case 3:
        //GIRAR UN POCO HACIA LA DERECHA YENDO HACIA ADELANTE
        Serial.println("IR-A, IR-B e IR-D Dentro del area - gira 45° adelante-derecha y avanza.");
        SerialBT.println("IR-A, IR-B e IR-D Dentro del area - gira 45° adelante-derecha y avanza.");
        moving(-100,100,NORM_SPD,NORM_SPD);
        break;

      case 4:
        //GIRAR UN POCO HACIA LA DERECHA YENDO HACIA ATRAS
        Serial.println("IR-A, IR-B e IR-C Dentro del area - gira 45° atras-izquierda y gira 180°.");
        SerialBT.println("IR-A, IR-B e IR-C Dentro del area - gira 45° atras-izquierda y gira 180°.");
        moving(100,-100,NORM_SPD,NORM_SPD);
        break;

      default:
        //LOOP O BUSCAR OPONENTE
        Serial.println("Algo anda mal... Este mensaje no deberia estarse reproduciendo... ");
        SerialBT.println("Algo anda mal... Este mensaje no deberia estarse reproduciendo... ");
        break;
    }
  }

  //DELAYING LOOP
  delay(LOOPDELAY);
}
