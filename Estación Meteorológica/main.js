var client = mqtt.connect('ws://test.mosquitto.org:8081/mqtt');
let maxTemp = 0, maxWindSpeed = 0, lastWindSpeed = 0, lastWindDirection = 0, temp = 0;
let firstWindData = true;

/* Function para conectarse al broker MQTT */
function EventoConectar() {
  console.log("Conectado a MQTT");
  client.subscribe("KRN/#", function (err) {
    if (!err) {
      console.log("Suscripto con exito");
    }
  });
}

/* Function que calcula la sensacion termica y retorna un float */
function windChill(temperature, windspeed) {
  if (temperature < 10 || windspeed < 4.8) {
      return "No se puede calcular la sensación térmica en estas condiciones";
  } else {
    const windChill = 13.12 + 0.6215 * temperature - 11.37 * Math.pow(windspeed, 0.16) + 0.3965 * temperature * Math.pow(windspeed, 0.16);
    return windChill;
  }
}

/* Function que modifica el valor correspondiente a la direccion del viento en el DOM, el message recibido es un int entre 1 y 8, 
segun que message llegue correspondera un valor u otro */
function showDirection(message, id) {
  if (message == 1) {
    document.getElementById(id).innerHTML = "Norte";
  } else if (message == 2) {
    document.getElementById(id).innerHTML = "Noreste";
  } else if (message == 3) {
    document.getElementById(id).innerHTML = "Este";
  } else if (message == 4) {
    document.getElementById(id).innerHTML = "Sureste";
  } else if (message == 5) {
    document.getElementById(id).innerHTML = "Sur";
  } else if (message == 6) {
    document.getElementById(id).innerHTML = "Suroeste";
  } else if (message == 7) {
    document.getElementById(id).innerHTML = "Oeste";
  } else if (message == 8) {
    document.getElementById(id).innerHTML = "Noroeste";
  } 
}

/* Function que modifica el valor correspondiente tomando dos parametros, el topic del mensaje entrante, y el mensaje como tal.
Se compara el topic entrante, para modificar con el mensaje entrante el objeto del DOM que corresponda */
function EventoMensaje(topic, message) {
  EventoConectar();
  if (topic == "KRN/temp") {

    message = parseInt(message);
    if (message > maxTemp) {
      maxTemp = message;
      message = message.toString();
      document.getElementById('maxTemp').innerHTML = maxTemp.toString() + "°C";
    }

    message = message.toString();

    document.getElementById('temperature').innerHTML = message + "°C";


  } else if (topic == "KRN/windDirection"){

    message = parseInt(message);

    if (firstWindData) {

      lastWindDirection = message;

      showDirection(message, 'windDirection');
      showDirection(lastWindDirection, 'lastWindDirection');

      firstWindData = false;

    } else {
      
      showDirection(message, 'windDirection');
      showDirection(lastWindDirection, 'lastWindDirection');

      lastWindDirection = message;

    }
    

  } else if (topic == "KRN/windSpeed") {

    message = parseInt(message);
    if (message > maxWindSpeed) {
      maxWindSpeed = message;
      message = message.toString();
      document.getElementById('maxWindSpeed').innerHTML = maxWindSpeed + "km/h";
    }

    message = message.toString();

    windSpeed = message;
    document.getElementById('windSpeed').innerHTML = message + "km/h";


  } else if (topic == "KRN/humidity") {

    document.getElementById('humidity').innerHTML = message + "%";

    document.getElementById('windChill').innerHTML = windChill();

    document.getElementById('sensacionTermica').innerHTML = sensacion;
 
  }
  console.log(topic + " - " + message.toString());
}

/* Llamados a las funciones */
client.on("connect", EventoConectar);
client.on("message", EventoMensaje);    

console.log("Empezando a conectar");