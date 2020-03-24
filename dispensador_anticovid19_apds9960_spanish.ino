/**######################################################
  #            *              *                    *    #
  #   *          Dispernsador_covid19.ino      *       *#
  #        Usando sensor de gestos APDS9960             #
  #      *        y micro servos SG90.                  #
  #            *      *                    *            #
  #               Copyright: Limonaso  *           *    #
  #     *        *          *                 *         #
  ####################################################**/

#include "Adafruit_APDS9960.h"                 //incluyo biblioteca para usar el sensor de gestos
#include <Servo.h>                              //incluyo la biblioteca de servos para facilitar su uso

Adafruit_APDS9960 apds;                       //creando un objeto para nombrar al sensor
int pos = 1500;                              //valor posicion de ambos servos
//int movimiento_abajo = 0;                  //variables para almacenar el estado de la lectura de movimiento de cada gesto
//int movimiento_arriba = 0;                   inhabilito el eje y debido al tipo de proyecto al que va dirigido este codigo
int movimineto_izquierda = 0;
int movimiento_derecha = 0;

Servo jabon;                                    //crea un objeto para controlar el servo
Servo jabon2;

void setup()                                    //funcion de pre-configuracion al inicio, solo se ejecuta 1 vez
{
  jabon.attach(9, 1000, 2000);                           //vincular pin 9 a servo 1, valor minimo 1000 microsegs de giro hasta 2000 microsegs(0-180Âº)
  jabon2.attach(10, 1000, 2000);                         //vincular pin10 a servo 2, valor minimo 1000 microsegs de giro hasta 2000 microsegs(0-180Âº)
  Serial.begin(115200);                           //Inicio puerto de comunicaciones a velocidad de datos - 115200 baudios
  jabon.write(pos);                             //posicion inicial central para el servo 1, valor escala 0-180 grados basandose entre 1000microseg y 2000microseg
  jabon2.write(pos);                            //posicion inicial central para el servo 2, valor escala 0-180 grados b..
  if (!apds.begin())                            //si no se detecta sensor, mostrar fallo por seral com
    Serial.println("Fallo al inicializar el dispositivo! Por favor checkea las conexiones.");
  apds.enableProximity(true);                   //habilitar deteccion de proximidad
  apds.enableGesture(true);                     //habilitar deteccion de gestos
}

void loop()                                      //funcion ciclica en la que se centran las tareas que realizara el arduino en bucle, se ejecuta infinitamente
{
  uint8_t gesture = apds.readGesture();          //variable que integra la lectura del sensor y almacena su valor de lectura
  //if (gesture == APDS9960_DOWN)                
  //if (gesture == APDS9960_UP)                  
  if (gesture == APDS9960_LEFT)   movimiento_izquierda = 1;  //cambiar estado de variable de movimiento hacia la izquierda a 1 si detecta lectura concreta
  if (gesture == APDS9960_RIGHT)  movimiento_derecha = 1;    //cambiar estado de variable de movimiento hacia la derecha a 1 si detecta lectura concreta

  if (movimiento_izquierda||movimiento_derecha == 1)         //si se detecta interaccion lateral de la mano sobre el sensor, activar mecanismo
  {
    for (pos = 1500; pos <= 2000; pos += 500)                //valor pos va de 90 a 180 grados en un paso de 500 microsegs
    {
      jabon.writeMicroseconds(pos);                                //posiciona servo segun el valor de pos
      jabon2.writeMicroseconds(pos);                               //posiciona servo2 segun el valor de pos
      delay(15);                                            //espera 15 milisegundos para que el servo vaya a la posicion
    }
    movimiento_izquierda == 0;                              //devuelve el estado inicial a la variables de estado para que entren en else
    movimiento_derecha == 0;                                  y los servos vuelvan a la posicion inicial
  }

  else                                                       //si no se detecta interaccion con el sensor, volver al estado inicial
  {
    for (pos = 2000; pos >= 1500; pos -= 500)               //valor pos va de 180 a 0 grados en un paso de 500 microsegs
    {
      jabon.writeMicroseconds(pos);                               //posiciona servo1 segun valor de pos
      jabon2.writeMicroseconds(pos);                              //posiciona servo2 segun valor de pos
      delay(15);                                      //espera 15 milisegundos para que el servo2 vaya a la posicion
    }
  }

  delay(250);                                    //dar un cuarto de segundo de pausa
}

