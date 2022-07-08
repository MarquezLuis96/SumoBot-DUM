/*============================================================ - CONSTS DECLARATION - ============================================================*/

//LOOPDELAY - INDICA EL RETRASO QUE TENDRÁ EL MÉTODO VOID-LOOP
int LOOPDELAY = 50;

//S-MONITOR_BAUDRATE - INDICA LOS BAUDIOS DEL MONITOR SERIAL
int S_MONITOR_BAUDRATE = 9600;

/*============================================================ - PIN DECLARATIONS - ============================================================*/

//

/*============================== - LEDS - ==============================*/

//TRYLED - LED DE PRUEBAS
int tryled = 2;

/*============================== - SENSORS - ==============================*/

//

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
int US2_TRIGG = 18;
int US2_ECHO = 19;

//3RD ULTRASONIC (US3) - TERCER ULTRASÓNICO
int US3_TRIGG = 33;
int US3_ECHO = 25;

/*============================== - MOTORS - ==============================*/

//

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

  //INITIALIZING SERIAL MONITOR
  Serial.begin(S_MONITOR_BAUDRATE);
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

/*============================== - INFRARED SENSORS - ==============================*/

/*
 SENSE_SINGLE_US(INT US_PIN_OUT, INT US_PIN_IN) - SENSA EL SENSOR DECLARADO
  US_PIN_OUT (TRIGGER) - PIN PARA ENVIAR LA SEÑAL SÓNICA
  US_PIN_IN (ECHO) - PIN PARA RECIBIR LA SEÑAL SÓNICA
  SE RETORNA LA DISTANCIA OBTENIDA EN LA MEDICIÓN
*/

/*============================================================ - LOOP - ============================================================*/
void loop() {

  //VERIFICANDO QUE SE ESTÉ DENTRO DEL ÁREA
  if (sense_all_ir() == true) {
    //TODOS LOS SENSORES ESTÁN DENTRO DEL ÁREA
    //SE PROCEDE A SENSAR CON LOS SENSORES ULTRASÓNICOS
    Serial.println("Todo en orden... Se procede a sensar ultrasonicos...");
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
        break;

      case 123:
        //GIRAR UN POCO A LA IZQUIERDA YENDO HACIA ADELANTE Y RUEDAS TRASERA HACIA ADELANTE
        Serial.println("IR-D Dentro del area - Gira 45° adelante-izquierda y luego avanza.");
        break;

      case 124:
        //GIRAR UN POCO A LA IZQUIERDA YENDO HACIA ATRÁS Y RUEDAS DELANTERAS HACIA ATRÁS
        Serial.println("IR-C Dentro del area - Gira 45° atras-izquierda y luego retrocede.");
        //Se puede girar luego 180°?
        break;

      case 12:
        //GIRAR 90 GRADOS A LA IZQUIERDA YENDO HACIA ADELANTE Y LUEGO AVANZAR
        Serial.println("IR-C e IR-D Dentro del area - Gira 90° adelante-izquierda y luego avanza.");
        break;

      case 134:
        //GIRAR UN POCO HACIA LA DERECHA YENDO HACIA ATRAS Y LUEGO RUEDAS DELANTERAS HACIA ATRÁS
        Serial.println("IR-B Dentro del area - Gira 45° atras-derecha y luego retrocede.");
        //Se puede girar luego 180°?
        break;

      case 14:
        // RETROCEDER
        Serial.println("IR-B e IR-C Dentro del area - Retrocede y gira 180°.");
        break;

      case 1:
        //GIRAR UN POCO HACIA LA IZQUIERDA YENDO HACIA ATRAS Y LUEGO DAR VUELTA PARA BUSCAR OPONENTE
        Serial.println("IR-B, IR-C e IR-D Dentro del area - Gira 45° atras-izquierda y gira 180°.");
        break;

      case 234:
        //GIRAR HACIA LA DERECHA YENDO HACIA ADELANTE Y LUEGO TRASERAS HACIA ADELANTE
        Serial.println("IR-A Dentro del area - Gira 45° adelante-derecha y avanza.");
        break;

      case 23:
        //AVANZAR
        Serial.println("IR-A e IR-D Dentro del area - Avanza.");
        break;

      case 2:
        //GIRAR HACIA LA IZQUIERDA YENDO HACIA ADELANTE
        Serial.println("IR-A, IR-C e IR-D Dentro del area - gira 45° adelante-izquierda y avanza.");
        break;

      case 34:
        //GIRAR 90 GRADOS A LA DERECHA YENDO HACIA ADELANTE Y LUEGO AVANZAR
        Serial.println("IR-A e IR-B Dentro del area - gira 90° adelante-derecha y avanza.");
        break;

      case 3:
        //GIRAR UN POCO HACIA LA DERECHA YENDO HACIA ADELANTE
        Serial.println("IR-A, IR-B e IR-D Dentro del area - gira 45° adelante-derecha y avanza.");
        break;

      case 4:
        //GIRAR UN POCO HACIA LA DERECHA YENDO HACIA ATRAS
        Serial.println("IR-A, IR-B e IR-C Dentro del area - gira 45° atras-izquierda y gira 180°.");
        break;

      default:
        //LOOP O BUSCAR OPONENTE
        Serial.println("Algo anda mal... Este mensaje no deberia estarse reproduciendo... ");
        break;
    }
  }

  //DELAYING LOOP
  delay(LOOPDELAY);
}
