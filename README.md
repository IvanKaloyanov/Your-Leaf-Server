# Your-Leaf-Server
YourLeafServer is part of YourLeaf IoT platform. YourLeafServer is used to manage and monitor the system's hardware. The entire platform is comepletely free under 'The Unlicense' terms and conditions.

# Development

## Hardware
For our official prototype, we've used WeMos D1 as a main board with an addition of a few sensors, water pump and a heater. Here is the list of all components needed for our prototype:

1. 1x WeMos D1 (or Uno with esp8266 shield)
2. 1x DHT11
3. 1x SEN-13322 (or similar moisture sensor)
4. 1x Photoresistor
5. 3x Resistors 2.2kΩ
6. 1x Resistor 10kΩ
7. 3x TIP120 Transistors
8. 3x 1N4001 Diodes (or SB560)
9. 1x 12v Battery
10. 1x Water pump (5-12v)
11. 1x Heater (5-12v)
12. 1m LED strip (or 5-12v Bulb)
13. Wires and blank PCB (or breadboard)

## Circuit
The circuit is pretty straightforward when you have the components. Here is a simple diagram of the entire circuit:
![scdone](https://user-images.githubusercontent.com/16307530/50449220-8077ef00-092e-11e9-8890-ad1e2d899b37.png)

## Communication
The communication between the application and the server is done via REST on 4 main routes. Each route supports HTTP GET/POST methods for managing the YourLeaf system and obtaining certain information. For communication we use thread executors to get the data asynchronous over time. If you rework the YourLeaf platform and you are interested in a more frequent communication with may want to switch to Websockets. If you are interested in getting a public access to the system you have to do this towards your router settings. If you have trouble please doing so, please let me know.

## External Libraries & Dependencies
For our prototype we've used only 2 external libraries. DHT for easier adjustments to DHT11 Humidity & Temperature Sensor and ESP8266 for our Server, Communication and Authentication. You may easily drop the DHT and convert the signal yourself or incase you use Yun you may even drop both.

# Our Cause
Our cause is to encourage the growth of fresh veggies/plants at home. That's why in order to help people to do this easily we've made the YourLeaf platform. 
<b>For every single one of the first 100 people who implement our system and start to grow veggies at there homes we will plant a tree.</b>

# Twinck it
Modify, break, update and stay curious. Hit me up with your changes or questions and like awayse feel free to coppy and paste.


