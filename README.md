# Progetto_IoT_Asincrona
 Descrizione:
 Facendo pressione sul bottone viene inviata una stringa (la prima città predefinita nello switch presente nel file .ino, aggiungetene quante ne volete,
 ma assicuratevi di mettere il nome in inglese) via MQTT al Broker di destinazione;
 Una volta arrivata la stringa su Node-red la rispedisce, con una chiamata http, alla Weather API (una API Free sul meteo https://openweathermap.org/api),
 una volta ricevuta la Response Node-red la snellisce e la invia via MQTT ad arduino;
 Quest'ultimo prendendo singolarmente il nome, la temperatura, l'umidità e la condizione atmosferica della città le printa sull'OLED.
 Ogni volta che si fa pressione sul bottone si cambia città
 
 arduino_secrets.h :
 Qui vanno inseriti SSID e Pass della vostra rete wifi
 
 

Hardware utilizzato per il progetto :
  - Arduino MKR Wifi 1010
  - Grove - Red LED Button
  - Grove - OLED Display 0.96" (SSD1315)
  
