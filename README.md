# SimpleBluetoothLowEnergySocket
A simple tcp style socket for Bluetooth Low Energy for Linux using purely Bluez 5.46 DBUS API

## Basic Idea
Bluetooth programming ist very hard with Bluez - especially the missing documentation and tutorials.
The beginning of learning a new technologie is the Hello World but this is very difficult without an example or a tutorial.
This project shall provide a easy playground to send arbitrary data over Bluetooth Low Energy (BLE 4.0) characteristics - similiar to Nordic Semiconductors Uart Service (NUS).

When programmin with tcp socket often the following pattern is used for handling tcp connections:
For a easier understanding and programming every method is synchronously.

A class handling the server socket, we call it **connector**:
1. create server socket
2. await incoming connection
3. start new thread handling the **connection**
4. goto 2.

A second class handling the **connection**
1. create and start a new thread
2. establishes connection
3. perform application handshake
4. handles application logic
5. close connection

