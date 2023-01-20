//DECLARACIÓN DE LIBRERÍAS
#include <MyoBridge.h> //Para el control del brazalete Myo Armband
#include <SoftwareSerial.h> //Comunicación serie
#include <Servo.h> //Control de los servomotores

//*************************
//DECLARACIÓN DE VARIABLES
//*************************
SoftwareSerial bridgeSerial(2,3); //Conexión con MyoBridge

MyoBridge bridge(bridgeSerial); //Inicialización de los objetos de la API MyoBridge con la conexión serie

String POSE; //Pose de que detecta MyoBridge

int inicio = 1; //inicio de la conexión

int cerrar; //cerrar
int reposo;  //reposo
int fuerza; //fuerza
int fuerzaInicial1; //fuerza inicial 1
int fuerzaInicial2; //fuerza inicial 2
int fuerzaInicialFinal; //fuerza inicial final
int fuerzaI1Por; //Primer porcenaje de la fuerza inicial
int fuerzaI2Por; //Segundo porcentaje de la fuerza inicial
int fuerzaI3Por; //Tercer porcentaje de la fuerza inicial

//variables para controlar los servomotores
Servo servo1;
Servo servo2;
Servo servo3;

//Sensor de fuerza
String sensorFuerza1; 
String sensorFuerza2; 
String sensorFuerza3; 
String sensorFuerza4; 
//Leds de fuerza
const int ledVerde = 4;
const int ledAmarillo = 5;
const int ledRojo = 6;

//******************************************
//DECLARACIÓN DE FUNCIONES Y PROCEDIMIENTOS
//******************************************

//Procedimiento que guarda en la variable POSE, la pose realizada
//y muestra por pantalla dicha pose
void handlePoseData(MyoPoseData& data) {  
  //Convierte los datos obtenidos en una pose de la API 
  MyoPose pose;
  pose = (MyoPose)data.pose;
  //Guardamos la pose en la variable pose
  POSE = bridge.poseToString(pose);
  //Mostramos la pose detectada por pantalla
  Serial.println(bridge.poseToString(pose));  
}

//Procedimiento que realiza el movimiento inicial de los servomotores
void PoseInicial(int i){
  
  //Pose inicial
  if (i == 1) {
    //realizamos el movimiento inicial de los servomotores
    servo1.write(180);      
    delay(500);
    servo2.write(0);
    delay(500);
    servo3.write(0);
    delay(500);
    servo1.write(0);      
    delay(500);
    servo2.write(180);
    delay(500);
    servo3.write(180);
    //inicializamos las variables que vamos a utilizar 
    inicio = 0;
    cerrar = 1;
    reposo = 1;
    //Lectura inicial de los sensores de presión
    fuerzaInicial1 = analogRead(A0) + analogRead(A1) + analogRead(A2) + analogRead(A3); 
    delay(1000);
    fuerzaInicial2 = analogRead(A0) + analogRead(A1) + analogRead(A2) + analogRead(A3); 
    fuerzaInicialFinal = (fuerzaInicial1 + fuerzaInicial2)/2;
    fuerzaI1Por = fuerzaInicialFinal*0.001; //0.1% de la fuerza inicial
    fuerzaI2Por = fuerzaInicialFinal*0.002; //0.2% de la fuerza inicial
    fuerzaI3Por = fuerzaInicialFinal*0.003;//0.3% de la fuerza inicial    

    Serial.println(fuerzaInicialFinal);
    Serial.println(fuerzaI1Por);
    Serial.println(fuerzaI2Por);
    Serial.println(fuerzaI3Por);
  }
}

//Procedimiento que realiza los movimientos de los servomotores para
//realizar el movimiento de cierre de la mano
void CerrarMano(String pose){
  //Pose cerrar
    if ((pose.compareTo("MYO_POSE_FIST") == 0 && cerrar == 1) || (pose.compareTo("MYO_POSE_WAVE_IN") == 0 && cerrar == 1)){
     
      //cerramos del todo la mano
      servo1.write(180);      
      servo2.write(0);
      servo3.write(0);
      //Esperamos
      delay(500);
      
      //indicamos que la mano está cerrada y solo puede estar en reposo
      cerrar = 0;
      reposo = 1;       
      
      //Dentro del funcionamiento de la mano comprobamos la fuerza ejercida
      //para encender los leds correspondientes
      fuerza = analogRead(A0) + analogRead(A1) + analogRead(A2) + analogRead(A3) - fuerzaInicialFinal;     
      //Mostramos por pantalla la suma de la fuerza ejercida por todos los sensores de presión
      Serial.print("Suma de la fuerza ejercida por todos los sensores: ");
      Serial.println(fuerza);
      //Llamamos a la función que enciende los leds 
      EnciendeLEDs(fuerza); 
    }
}


//Procedimiento encargado de realizar los movimientos de los
//servomotores para poner la mano en posición de reposo
void ReposoMano(String pose){
  
   //Pose relajar  
   if (pose.compareTo("MYO_POSE_REST") == 0 && reposo == 1) {  
     
    //abrimos todos los servos
    servo1.write(0);
    servo2.write(180);
    servo3.write(180);
    
    //indicamos que estamos en reposo y que solo puede cerrar la mano
    reposo = 0;
    cerrar = 1;
    fuerza = 0; //al relajar no necesitamos medir la fuerza
    
    //Apagamos los leds de fuerza
    EnciendeLEDs(fuerza);    
    
    //Esperamos
    delay(1000);
    }    
}

//Función que enciende los LEDs correspondientes dependiendo de
//la variable de entrada f
void EnciendeLEDs(int f) {

  
  //Si es menor que el primer porcentaje de la fuerza inicial, los leds permanecerán apagados
  if (f < fuerzaI1Por) { 
    //enciende verde
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarillo, LOW);
    digitalWrite(ledRojo, LOW);  
  }
  //si está entre el primer y el segundo porcentaje de la fuerza inicial, se encenderá el led verde
  if (f >= fuerzaI1Por && f < fuerzaI2Por) {
    //enciende verde
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarillo, LOW);
    digitalWrite(ledRojo, LOW);      
  }
  //si está entre el segundo y el tercer porcentaje de la fuerza inicial, se encenderán los leds verde y amarillo
  if (f >= fuerzaI2Por && f < fuerzaI3Por) {
    //enciende verde y amarillo
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarillo, HIGH);
    digitalWrite(ledRojo, LOW);      
  }
  //Si es mayor que el tercer porcentaje de la fuerza inicial, se encenderán los tres leds.
  if (f >= fuerzaI3Por){
    //enciende verde, amarillo y rojo
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarillo, HIGH);
    digitalWrite(ledRojo, HIGH);  
  }  
}


//********************************************************
//Procedimiento que realiza la configuración:
//  - La conexión bluetooth con HM11
//  - Inicialización de procedimientos de la API
//  - La asignación de pines para los servomotores y leds
//********************************************************

void setup() {
  
  //Inicializa la conexión serie
  Serial.begin(115200);
  bridgeSerial.begin(115200);
  //La conexión bluetooth con HM11
  //Queda a la espera hasta que se realiza la conexión con Myo Armband
  Serial.println("Searching for Myo...");
  bridge.begin();
  Serial.println("connected!");

  //Inicialización de procedimientos de la API
  //configuración de la función que obtiene la pose
  bridge.setPoseEventCallBack(handlePoseData);
  //Endica que está activa la recepción de la pose
  bridge.enablePoseData();
  //Nos aseguramos que el brazalete Myo Armband está desbloqueado.
  bridge.unlockMyo();

  //La asignación de pines para los servomotores y leds
  //Indicamos los Pinnes para los servos
  servo1.attach(10); //índice y corazón
  servo2.attach(11); //anular y meñique
  servo3.attach(12); //pulgar
  
  //indicamos los pines para los leds de fuerza
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);
  pinMode(ledRojo, OUTPUT);  
}

//**********************
// Buble de repetición 
//**********************
void loop() {
  //Actualizamos la conexión a MyoBridge
  bridge.update();  

  //Comprobamos si es la pose inicial
  PoseInicial(inicio);
  
  //Comprobamos si la pose es cerrar la mano
  CerrarMano(POSE);

  //Comprobamos si la pose es reposo
  ReposoMano(POSE); 

}
