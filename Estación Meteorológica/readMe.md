Introducción:
El proyecto final tiene como propósito la medición de datos atmosféricos para su muestreo al usuario mediante una página asociada al prototipo. Esto con el fin de poder advertir a los usuarios de las condiciones atmosféricas del ambiente en cualquier momento y lugar. En este informe se mostrará el proceso por el que pasamos los integrantes y el proyecto hasta su culminación.

Primera Idea:
La primera idea del proyecto consistió en una impresora que pudiera escribir y hacer dibujos con un fibrón sobre una hoja de papel A4, esta idea fue rápidamente reemplazada por la estación meteorológica por falta de presupuesto.
Desde un principio la estación meteorológica tuvo como objetivo la medición de la temperatura, la humedad y la velocidad y dirección del viento. Estos datos serían mostrados en un display lcd, pero luego decidimos mostrarlos en una aplicación web diseñada en el stack HTML, CSS, JavaScript donde se mostrarán los datos actualizados en tiempo real.

Objetivo general:
El objetivo general del proyecto es desarrollar un módulo robótico similar a una estación meteorológica para la medición de la temperatura, humedad, velocidad del viento y dirección del viento de un ambiente específico, cuyos datos fueran mostrados en una aplicación web que se pudiera ver en cualquier momento con los datos actualizados en tiempo real.

Objetivo específico:
Como grupo nuestro objetivo fue la realización de un modelo robótico que cumpliera con los requisitos establecidos por los profesores, donde se incluyeran los conocimientos adquiridos en términos de robótica con arduino, sumado a la adquisición de nuevos conocimientos como los son las conexiones mediante protocolo mqtt o la comunicación bilateral vía WebSocket.


Diseño:
El diseño del proyecto consiste en una placa ESP32 donde están conectados los sensores para la medición de los datos. En un principio la placa seleccionada había sido una arduino mega para la comodidad de las conexiones, pero finalmente nos decantamos por la placa ESP32 por la facilidad que tiene para las conexiones vía wifi.
Los datos son medidos por un sensor de temperatura/humedad y un anemómetro de diseño propio. El primer sensor mide como dice su nombre la temperatura y la humedad, el anemómetro es un diseño casero donde mediante el voltaje generado por un motor de escobilla medimos la velocidad del viento, para poder generar este voltaje se le coloca un aspa impresa en 3D en la punta del motor. Para la medición de la dirección del viento se utilizan sensores touch que mediante una planeta que se mueve a la vez que el aspa de la velocidad del viento, activa el sensor touch correspondiente a la dirección de donde proviene el viento.
Los elementos de medición de viento están colocados en sus correspondientes contenedores. En el caso de los sensores touch están dispuestos en una placa redonda de 120 mm de diámetro con hendiduras donde están pegados los sensores, en el centro de placa se encuentra el aspa con la paleta que activa los sensores. En el caso del motor de escobilla está colocado dentro de un tubo que lo mantiene elevado para el correcto funcionamiento del aspa.
Finalmente todos los elementos están colocados en una caja de cartón.

Elementos utilizados:

Placa ESP32: placa controladora utilizada para las conexiones de los sensores y demás elementos electrónicos, además de ser utilizada para el envío de datos al servidor mediante wifi.

Sensor de temperatura/humedad: sensor utilizado para la medición de la temperatura y humedad ambiente.

Motor de escobilla: componente utilizado para la medición de la velocidad del viento mediante un aspa impresa en 3D.

Sensores touch: sensores utilizados para la medición de la dirección del viento.

Insumos impresos en 3D: base para colocar los sensores touch, caja de los sensores touch, aspa del anemómetro; aspa, palo y paleta para medición de la dirección del viento.

Código Esp32:
El prototipo del proyecto es muy simple, puesto que solo consiste de los sensores conectados a la placa Esp32.
El código de las conexiones tampoco es de gran complejidad ya que los sensores tienen un código estándar para su funcionamiento.
En el caso del anemómetro cómo utilizamos un motor de escobilla con un aspa en la punta, solo obtenemos el dato del voltaje que genera con el movimiento de las aspas. Para poder convertir ese dato de voltaje a la velocidad del viento se utilizó la fórmula 37,5*X+10 donde X es el dato del voltaje.
Para la conexión wifi del esp-32, se utilizó la librería WiFi.h.
De esta podemos crear un objeto de la clase WebClient.
Para inicializar la conexión wifi utilizamos el método WiFi.begin(), se le da como parámetro el ssid y el password de la red wifi a la que se conectará.
Inmediatamente después, se abre un while que se romperá si la conexión wifi fue establecida correctamente.
Para el cliente MQTT se utiliza la librería PubSubClient.h.
De esta creamos un objeto PubSubClient, que tomará como parámetro el objeto wifi creado anteriormente.
Para la conexión con el broker se utiliza el método .connected() para verificar si la conexión está establecida o no. En caso de no estarlo, se utiliza el método .connect, que toma como parámetros un nombre con el que se conectará el dispositivo al broker, el usuario del broker y la contraseña del mismo.
Para enviar los datos, se utiliza el método .publish, que toma como primer parámetro el tópico en el que se enviará el mensaje al broker, y como segundo parámetro, el mensaje, transformado en String.
Se envían los cuatro datos por separado, en cuatro instancias distintas, antes de cada envío se verifica la conexión a la red wifi.


Código página:

Para el maquetado de la página se utilizó HTML5, sumado a la librería de Bootstrap, de donde se sacaron los templates para el diseño de la página, a dichos templates se les modificó el diseño mediante los parámetros Sass(framework de CSS).
Para las pequeñas modificaciones de diseño se utilizó CSS3, un lenguaje de diseño y estética para dar estilo a una maqueta de la página.
Para la lógica y la funcionalidad del frontend se utilizó el lenguaje de programación 
JavaScript. Dentro de este se utilizaron dos protocolos:

MQTT:
Este protocolo funciona bajo la lógica de Subscriber/Publisher. Un publisher envía un mensaje bajo un topic y este es almacenado en un broker.
El broker recibe los mensajes enviados por los publisher bajo un topic específico y los envía a los subscribers de ese topic.
El publisher y el subscriber se encuentran en el cliente, dentro de este cliente el publisher, en nuestro caso la placa Esp32 8266 Wroom, publica los datos del ambiente y los publica bajo un tema con su subtema correspondiente.
Por otro lado el subscriber, en nuestro caso el cliente NodeJS, se subscribe a un topic para recibir los datos enviados por el publisher bajo ese topic. Para esto se utilizó la librería MQTT.JS, para crear el cliente y subscribirse al tema. Este subscribe recibe los datos y modifica el DOM (document object model). Para que los datos que llegan se vean reflejados en la página en tiempo real se utilizó el protocolo WebSocket.

WebSocket:
El protocolo websocket es un protocolo similar al http, pero en este caso el servidor también puede hacer peticiones a la vez que el cliente.
El mensaje que envía la palanca esp32 es recibido y se genera un mensaje websocket que actualiza la página html vía DOM.

Dificultades:
A la hora del desarrollo del proyecto nos topamos con diversas dificultades que fuimos sorteando a lo largo del proceso de desarrollo. Estas dificultades comenzaron siendo simples como que la computadora no detecta el módulo ESP32, problema el cual fue solucionado fácilmente mediante la instalación de los drivers correspondientes. Luego tuvimos dificultades con las conexiones entre el módulo y la página de muestreo de los datos, esto debido a nuestra falta de conocimiento del tema, el principal problema que tuvimos con respecto a la conexión fue la incompatibilidad de algunas librerías entre sí, pero esto pudo ser solucionado rápidamente. Otro de los problemas fue que no sabíamos cómo hacer para que la página recibiera los datos en tiempo real, esto se soluciono mediante la implementación del protocolo websocket explicado anteriormente. Por último la última dificultad que tuvimos fue que la página no recibe bien los datos de tipo entero.

Posibles mejoras:
Algunas de las mejoras que podría recibir el proyecto es la implementación de nuevos sensores que agreguen nuevas mediciones de datos atmosféricos como la radiación o la presión atmosférica.
Otra de las mejoras en caso de que sea utilizado en espacios cerrados sería la conexión con dispositivos como el aire acondicionado para que el usuario pueda configurar la temperatura del ambiente a su gusto.

Conclusión:
Como conclusiones de este proyecto podemos asegurar que la tecnología de arduino y la electrónica tiene un gran potencial para que personas como nosotros los estudiantes puedan desarrollar dispositivos de gran aporte social y tecnológico.
En cuanto a nuestro proyecto en sí, concluimos en que fue una gran experiencia de donde sacamos valiosos conocimientos en términos de conexiones entre dispositivos y softwares. Además el prototipo puede ser de gran ayuda para poder indicar datos de los ambientes que se encuentran en nuestra universidad como lo son las aulas o la cantina.
