1. Load the code into NodeMCU. The SSID and password is supposed to be the home wifi.
2. Connect the relay to the NodeMCU, and the applicance. And turn on the circuit.
3. Start the server app on some device that's connected to your wifi network. Get this device's IP address (I found the value from my Wifi router's app)
4. Now set the switch value from any device in your wifi network.
   1. Toggle switch - visit `http://192.168.x.y:3000/toggle`. Fill in `x` and `y` to match server device's IP address.
   2. Get current switch value - visit `http://192.168.x.y:3000/toggle`.
