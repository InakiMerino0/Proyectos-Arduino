#include<DHTesp.h>
#include<WiFi.h>
#include<PubSubClient.h>
 
//WiFi and MQTT parameters
constchar* ssid = "A";
constchar* password = "12345678";
 
constchar* mqttServer = "gintea.ar";
#definemqtt_port1883;
constchar* mqtt_user = "asd";
constchar* mqtt_password = "123";
 
//MQTT topics
constchar* tempTopic = "KRN/temp";
constchar* humidityTopic = "KRN/humidity";
constchar* speedTopic = "KRN/windSpeed";
constchar* directionTopic = "KRN/windDirection";
 
//WiFi client, MQTT client
WiFiClient espClient;
PubSubClient client(espClient);
 
//DHT
int pinDHT = 34;
DHTesp dht;
 
//anemometer
float veloc1 = 0;
float v1 = 0;
 
//Wind direction
//sensores touch
#defineTTP113
#defineTTP212
#defineTTP314
#defineTTP427
#defineTTP526
#defineTTP625
#defineTTP733
#defineTTP832
 
float finalDirection = 0;
 
//Connect function
voidconnect(){​​​​​​​​
  if(!client.connected()){​​​​​​​​
    if(client.connect("ESP32Client", mqtt_user, mqtt_password)){​​​​​​​​
      Serial.println("Conexión al servidor MQTT exitosa");
    }​​​​​​​​else{​​​​​​​​
      Serial.println("Error en la conexión al servidor MQTT");
      delay(5000);
      return;
    }​​​​​​​​
  }​​​​​​​​
}​​​​​​​​
 
//Float publish functions
voidPublishInt(constchar*topic, floatmessage = 0){​​​​​​​​
if(client.publish(topic, String(message).c_str())){​​​​​​​​
Serial.println("Mensaje MQTT enviado con éxito");
}​​​​​​​​else{​​​​​​​​
Serial.println("Error al enviar el mensaje MQTT");
}​​​​​​​​
}​​​​​​​​
 
voidsetup(){​​​​​​​​
  //Serial setup
Serial.begin(115200);
 
  //Touch setup
pinMode(TTP1, INPUT);
pinMode(TTP2, INPUT);
pinMode(TTP3, INPUT);
pinMode(TTP4, INPUT);
pinMode(TTP5, INPUT);
pinMode(TTP6, INPUT);
pinMode(TTP7, INPUT);
pinMode(TTP8, INPUT);
 
  //DHT Setup
dht.setup(pinDHT, DHTesp::DHT11);
 
  //WiFi Setup
WiFi.begin(ssid, password);
 
while(WiFi.status() != WL_CONNECTED){​​​​​​​​
delay(2000);
Serial.println("Conectando a la red WiFi...");
}​​​​​​​​
client.setServer(mqttServer, 1883);
}​​​​​​​​
 
voidloop(){​​​​​​​​
delay(1000);
  TempAndHumidity data = dht.getTempAndHumidity();
float finalTemp = (data.temperature, 2);
float finalHumidity = (data.humidity, 1);
 
Serial.print("Temperatura: ");
Serial.print(data.temperature, 2);
Serial.print(", Humedad: ");
Serial.print(data.humidity, 1);
 
  //anemometer read
   v1 = analogRead(35);
float lectura=v1/1023*5.0;
  veloc1 = (37.5*lectura)+10;
if(lectura<0.1){​​​​​​​​
    veloc1=0;
}​​​​​​​​
Serial.println(veloc1);
delay(1000);
 

  //Direction read
if(digitalRead(TTP1) == HIGH){​​​​​​​​
    finalDirection = 1;
}​​​​​​​​elseif(digitalRead(TTP2) == HIGH){​​​​​​​​
    finalDirection = 2;
}​​​​​​​​elseif(digitalRead(TTP3) == HIGH){​​​​​​​​
    finalDirection = 3;
}​​​​​​​​elseif(digitalRead(TTP4) == HIGH){​​​​​​​​
    finalDirection = 4;
}​​​​​​​​elseif(digitalRead(TTP5) == HIGH){​​​​​​​​
    finalDirection = 5;
}​​​​​​​​elseif(digitalRead(TTP6) == HIGH){​​​​​​​​
    finalDirection = 6;
}​​​​​​​​elseif(digitalRead(TTP7) == HIGH){​​​​​​​​
    finalDirection = 7;
}​​​​​​​​elseif(digitalRead(TTP8) == HIGH){​​​​​​​​
    finalDirection = 8;
}​​​​​​​​
 
  //MQTT sv connection
connect();
  //MQTT Publish Temp
if(client.publish(tempTopic, String(data.temperature, 2).c_str())){​​​​​​​​
Serial.println("Mensaje MQTT enviado con éxito (temperatura)");
}​​​​​​​​else{​​​​​​​​
Serial.println("Error al enviar el mensaje MQTT");
}​​​​​​​​
delay(1000);
 
  //Send Humidity
  //MQTT sv connection
connect();
  //MQTT publish
if(client.publish(humidityTopic, String(data.humidity, 1).c_str())){​​​​​​​​
Serial.println("Mensaje MQTT enviado con éxito (humedad)");
}​​​​​​​​else{​​​​​​​​
Serial.println("Error al enviar el mensaje MQTT");
}​​​​​​​​


 
Serial.println(veloc1);
  //MQTT sv connection
connect();
  //MQTT Publish wind speed
PublishInt(speedTopic, veloc1);
 
Serial.println(finalDirection);
  //MQTT sv connection
connect();
  //MQTT Publish wind direction
PublishInt(directionTopic, finalDirection);
 
client.loop();
}​​​​​​​​