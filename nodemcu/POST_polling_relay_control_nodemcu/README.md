## Instructions

1. Load the code into NodeMCU. The SSID and password is supposed to be the home wifi.
2. Connect the relay to the NodeMCU, and the applicance. And turn on the circuit.
3. Start the server app on some device that's connected to your wifi network. Get this device's IP address (I found the value from my Wifi router's app)
4. Now set the switch value from any device in your wifi network.
   1. Toggle switch - visit `http://192.168.x.y:3000/toggle`. Fill in `x` and `y` to match server device's IP address.
   2. Get current switch value - visit `http://192.168.x.y:3000/toggle`.

How I use it - I use my mobile to toggle the switch. I'm planning to make a small React Native app to do this (since opening the browser and writing the URL is a pain)

### Note

1. Actuation delay (acceptable) - there is a significant delay (average: 3 seconds, max: 5 seconds), due to the board. This is is not a huge problem, since the server is very fast and status is known instantly, just that actuation is a little slow.
2. Stack problem (not there) - since the delay is from the board, and it always reads the state from the server, the wasted/ignored values (if I toggle many times in a second) are ignored, only the latest state is considered, which we always know (since the server is quite fast usually).
