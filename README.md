# home-automation-solution
Home Automation Solution uses esp12e wifi module to control the home appliance with any device having wifi card and access to chrome browser. 

Hardware needed:
1) Esp-12E wifi module shown in pdf file.
2) Arduino any module for serail programming of esp-12e as shown in jpg image file connection for esp-01 and esp-12e are same.
3) 4-channel relay module.
4) 2A 5V charger.
5) Jumper wires.
6) AMS1117-3.3v regulator
7) 10uF 16V capacitor
8) 104 capacitor
7) Four 230Ohm resistance for taking input from switches.

Connections:
1) Create a 5v dc to 3.3v dc converter as describe in hqdefault.jpg file.
2) Get two connection from 5V charger one for relay module vc and ground pin & other for 3.3v converter made in first step.

3) Connections for serial programming:
  i) Connect arduino and esp module as describe in ss.jpg file. Note the wifi module pins of esp-01 shown in image and attach it to corresponding esp-12e pins. like IO2 to GPIO2 & IO0 to GPIO0 and others.
  ii) Set arduino ide as specified in the image.
  iii) Upload the Wifi4Device.ino sketch to esp module.
  iv) After successfully uploading the sketch only remove the ground wire from GPIO0 to verify the sketch is completely uploaded.
   v) After few minutes later remove all the connections. 

4) Attach the four resistance to input pins as define in sketch Wifi4Device.ino file.
5) Common other end of all the resistance with the gnd pin of the module.
6) Attach jumper wires in between resistance & module pins.
7) Get two connection from 3.3v converter one for wifi module & other one for taking input from switches.
8) Taking input from switches means 3.3v common to all switches & attach the each jumper wires of step 6 individual to each switch pin other than common pin. In case of spst switch there is only two pins. Thus one is used for common and other one for jumper wire.
9) Attach the four jumper wires to output pins as define in sketch Wifi4Device.ino file.
10) Attach the other end of jumper wires to relay module.
