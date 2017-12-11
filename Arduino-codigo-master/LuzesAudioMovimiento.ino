
//**************************************************************************************
//**************************************************************************************


#include <Servo.h>

// Frecuencias de las notas (se mandarán al zumbador o buzzer)
#define DO4  262
#define RE4  294
#define MI4  330
#define FA4  349
#define FAS4  370
#define SOL4  392
#define LA4  440
#define LAS4  466
#define SI4  494
#define DO5  523
#define RE5  587

// Pins de I/O (definiciones de entrada y salida)
#define SPKPIN        11  //********
#define TRANSPOSEPIN  3
#define SHUFFLEPIN    4
#define SPEEDPIN      1
#define LED1          12 //********
#define LED2          13 //********
#define LED3          6
#define LED4          5

// Estado de los LEDs (actuan como flags para alternar entre encendidos y apagados de los leds)
boolean led1State = HIGH;
boolean led2State = LOW;
boolean led3State = HIGH;
boolean led4State = LOW;

// Duracion inicial de la nota
int noteDuration = 500;
// Delay para tocar la siguiente nota
int noteDelay = noteDuration + 50;

// Valores para shuffle de la duracion de notas
int staccato = 0;
int shuffle = 0;
int contador= 1;

// Variables para hacer transpose a las notas
int transposedNote;
int transpose = 0;

// Array de notas para We Wish You A Merry Christmas (esta es la canción, podemos modificar notas para modificar la melodía)
int notes[54] = {
  DO4, FA4, FA4, SOL4, FA4, MI4, RE4, RE4, RE4, SOL4, SOL4, LA4, SOL4, FA4, MI4, DO4, DO4,
  LA4, LA4, LAS4, LA4, SOL4, FA4, RE4, DO4, DO4, RE4, SOL4, MI4, FA4, DO4, FA4, FA4, FA4,
  MI4, MI4, FA4, MI4, RE4, DO4, SOL4, LA4, SOL4, SOL4, FA4, FA4, DO5, DO4, DO4, DO4,
  RE4, SOL4, MI4, FA4
};

// Entre mas alto el valor mas corta la nota (modificar para modificar duración de las notas)
int durations[54] = {
  2, 2, 3, 3, 3, 3, 2, 2, 2, 2, 3, 3, 3, 3, 2, 2, 2,
  2, 3, 3, 3, 3, 2, 2, 3, 3, 2, 2, 2, 1, 2, 2, 2, 2,
  1, 2, 2, 2, 2, 1, 2, 2, 3, 3, 3, 3, 2, 2, 3, 3,
  2, 2, 2, 1
};

//Creamos dos servos
Servo Servo1;
Servo Servo2;

int poserv1=0;
int poserv2=0;
int diez=0;
int veinte=0;

//Entramos en la función principal
void setup() {
  // Inicializar los pins como OUTPUT
  pinMode(SPKPIN, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
 
//Configuramos pines de los servos (cambiar valores si queremos utilizar otro pinout)
  Servo1.attach(8);   //********
  Servo2.attach(8);   //********

  poserv1=map(poserv1, 0, 1023, 0 ,179);
  poserv2=map(poserv2, 0, 1023, 0 ,179);

  // Inicializar el puerto serial para imprimir el valor de las notas y las frases de TodoElectronica21
  // en el monitor serial
  Serial.begin(9600);
}

void loop() {
  // hacemos un for loop de todas las notas en el array notes
  for (int i=0; i < sizeof(notes) / sizeof(int); i++) {
    // Para implementar el shuffle hay que poner un potenciometro en
    // el pin definido como SHUFFLEPIN (4) 
    // Esto no lo usamos en nuestro vídeo de Navidad, pero se puede utilizar
    /*
    shuffle = analogRead(SHUFFLEPIN);
     staccato = random(10, map(shuffle, 0, 1023, 10, 200));
     */

    // Se lee el valor del potenciometro en TRANSPOSEPIN
    // que nos da un numero del 0 al 1023
    // con la funcion map() lo mapeamos de 1 a 5 para hacerlo más sencillo
    transpose = map(analogRead(TRANSPOSEPIN), 0, 1023, 1, 5);

    // Se guarda en la variable transposedNote
    // la frecuencia de la nota actual mas
    // la nota actual multiplicado por el valor de transpose
    transposedNote = notes[i] + (notes[i] * transpose);

    // Imprimimos en el monitor serial la frecuencia que se
    // esta tocando por el buzzer
    Serial.println(transposedNote);

    // leemos el valor del potenciometro en el SPEEDPIN
    // y lo mapeamos de 800 a 100 para hacerlo más sencillo (no lo utilizamos, pero existe la posibilidad).
    noteDuration = map(analogRead(SPEEDPIN), 0, 1023, 800, 100);

    // Dividimos el valor de noteDuration entre la duracion de la nota actual
    // y le sumamos el staccato (shuffle)
    noteDelay = (noteDuration / durations[i]) + staccato;

    // Enviamos al buzzer conectado al SPKPIN, la frecuencia de la nota
    // por una duracion de noteDuration entre la duracion de la nota actual
    // menos el valor de staccato
    tone(SPKPIN, transposedNote, (noteDuration / durations[i]) - staccato);
    // detenemos el programa con la funcion delay() por la duracion
    // guardada en noteDelay (milisegundos: 1000 = 1 segundo)
    delay(noteDelay);
    // Le decimos al buzzer que deje de sonar
    noTone(SPKPIN);

    // Enviamos los valores guardados en las variables
    // ledState a los LEDs conectados a los pins definidos en
    // LED1..., LED4 - Con estas sentencias variamos los leds de nuestro árbol de navidad
    digitalWrite(LED1, led1State);
    digitalWrite(LED2, led2State);
    digitalWrite(LED3, led3State);
    digitalWrite(LED4, led4State);

    // Invertimos los valores de las variables guardadas en
    // cada una de estas variables
    // (si ledState era LOW, ahora es HIGH)
    // En Arduino LOW = false, HIGH = true - Con esto hacemos que apaguen y enciendan

    led1State = !led1State;
    led2State = !led2State;
    led3State = !led3State;
    led4State = !led4State;

   //Configuramos condiciones para el movimiento de los brazos de nuestro Santa Claus
    if(poserv1==0)
      {
          diez=10;
      }
      
    if(poserv2==0)
      {
          veinte=5;
      }
      
    if(poserv1==170)
       {
          diez=-10;
       }
       
    if(poserv2==160)
       {
          veinte=-5;
       }

    //Movemos los brazos de nuestro Santa
    Servo1.write(poserv1);
    Servo2.write(poserv2);
    poserv1=poserv1+diez;
    poserv2=poserv2+veinte;

  }
  // Terminamos el for loop!

  // Esperamos para repetir todo el programa
  // la cantidad de noteDelay * 1 (milisegundos)
  delay(noteDelay * 1);
  Serial.println("");
  Serial.println("s");
  Serial.println("");

  //Forzamos a que el programa se ejecute dos veces antes parar
  if(contador!=2){
    contador++;
  }

  //Imprimimos mensaje final y esperamos 10 minutos (600000 milisegundos)
  else{
    Serial.println("Prueba!!!");
    Serial.println("");
    delay(600000);
    contador=0;
  }
  //Terminamos nuestro programa
  }
  
