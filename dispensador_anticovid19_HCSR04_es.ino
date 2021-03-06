/**####################################################
#            *              *                    *    #
#   *           dispensador_anticovid19.ino      *   *#
#        Usando sensor de ultrasonidos HC-SR04        #
#      *        y micro servos SG90.                  #
#            *      *                    *            #
#               Copyright: Limonaso  *           *    #                        
#     *        *          *                 *         #
####################################################**/


#include <Servo.h>                              //incluyo la biblioteca de servos para facilitar su uso

const int EcoPin = 5;                          //declaro pin de emision de la onda sonica
const int DisparoPin = 6;                       //declaro pin de sensor de la onda sonica
int pos = 1500;                              //valor posicion de ambos servos

Servo jabon;                                    //crea un objeto para controlar el servo
Servo jabon2;

void setup()                                    //funcion de pre-configuracion al inicio, solo se ejecuta 1 vez
{
  pinMode(DisparoPin, OUTPUT);                 //Declaro pin del emisor como salida 
  pinMode(EcoPin, INPUT);                      //Declaro pin del sensor como entrada 
  jabon.attach(9,1000,2000);                             //vincular pin 9 a servo 1, valor minimo 1000 microsegs de giro hasta 2000 microsegs(0-180º)
  jabon2.attach(10,1000,2000);                           //vincular pin10 a servo 2, valor minimo 1000 microsegs de giro hasta 2000 microsegs(0-180º)
  Serial.begin(9600);                           //Inicio puerto de comunicaciones a velocidad de datos - 9600 baudios
  jabon.write(pos);                             //posicion inicial central para el servo 1, valor escala 0-180 grados basandose entre 1000microseg y 2000microseg
  jabon2.write(pos);                            //posicion inicial central para el servo 2, valor escala 0-180 grados b..
}

void loop()                                      //funcion cíclica en la que se centran las tareas que realizara el arduino en bucle, se ejecuta infinitamente
{
  int cm = ping(DisparoPin, EcoPin);             //creo valor entero cm y le vinculo el valor de la funcion externa ping nombrando pines del sensor ultrasonico 
  Serial.print("Distancia: ");                   //imprime por el puerto de comunicaciones serie el texto para encabezar el valor de cm
  Serial.println(cm);                            //imprime valor de distancia

  if (cm <= 6)                                   //si la distancia de la mano es menor o igual de 6cm apretar expendedor de jabon o alcohol
  {
        for (pos = 1500; pos <= 2000; pos += 500)                //valor pos va de 90 a 180 grados en un paso de 500 microsegs
      { 
        jabon.writeMicroseconds(pos);                                //posiciona servo segun el valor de pos
        jabon2.writeMicroseconds(pos);                               //posiciona servo2 segun el valor de pos
        delay(15);                                            //espera 15 milisegundos para que el servo vaya a la posicion
      } 
  }
  
  else                                                       //si no se detecta la proximidad, volver al estado inicial
  {
        for (pos = 2000; pos >= 1500; pos -= 500)               //valor pos va de 180 a 0 grados en un paso de 500 microsegs
      { 
        jabon.writeMicroseconds(pos);                               //posiciona servo1 segun valor de pos
        jabon2.writeMicroseconds(pos);                              //posiciona servo2 segun valor de pos
        delay(15);                                      //espera 15 milisegundos para que el servo2 vaya a la posicion
      }
  }
                            
  delay(250);                                    //dar un cuarto de segundo de pausa de interrupcion de refresco
}

int ping(int DisparoPin, int EcoPin)            //creo el entero ping que constara del tiempo de la emision y recepcion de la onda para determinar la distancia en cm 
{
  long duracion, distanciaCm;                   //creo dos valores largos para introducir las mediciones de la duracion de la onda y el resultado de la distancia
  
  digitalWrite(DisparoPin, LOW);                 //para generar un pulso limpio pongo a LOW 4 microsegundos
  delayMicroseconds(4);
  digitalWrite(DisparoPin, HIGH);                //genero disparo de 10 microsegundos
  delayMicroseconds(10);
  digitalWrite(DisparoPin, LOW);                 //apago emisor de pulso sonico
  
  duracion = pulseIn(EcoPin, HIGH);              //medimos el tiempo entre pulsos, en microsegundos
  
  distanciaCm = duracion * 10 / 292/ 2;             //convertimos a distancia, en cm
  return distanciaCm;                               //vuelve arriba hasta el valor distanciaCm para repetir el proceso y refrescar valor
}
