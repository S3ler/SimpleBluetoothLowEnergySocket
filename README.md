# SimpleBluetoothLowEnergySocket
A simple tcp style socket for Bluetooth Low Energy for Linux using purely Bluez 5.46 DBUS API

## Basic Idea
Bluetooth programming ist very hard with Bluez - especially the missing documentation and tutorials.
The beginning of learning a new technologie is the Hello World but this is very difficult without an example or a tutorial.
This project shall provide a easy playground to send arbitrary data over Bluetooth Low Energy (BLE 4.0) characteristics - similiar to Nordic Semiconductors Uart Service (NUS).

### tcp
When programmin with tcp socket often the following pattern is used for handling tcp connections:
For a easier understanding and programming every method is synchronously.

A class handling the server socket, we call it **connector**:
1. create tcp socket
2. await incoming connection
3. start new **connection**
4. goto 2.

A second class handling the **connection**
1. create and start a new thread (by **connector**)
2. establishes connection
3. perform application handshake
4. handles application logic
5. close connection

### udp
Of course udp sockets are simpler - we can handle incoming packets one by one - or we can use a similiar patter:
(Of couse outgoing packets are send over the same udp socket used in the **connect**)
(Not really easy but with the right interfaces, threading, caching and synchronisation it provides a similiar programming logic.) 

**connector**:
1. create udp socket
2. await incoming packet
3. if no connection for source address + port exists: goto 3.1 else goto 3.2
3.1 start new **connection** handling packet
3.2 give already existing connection the incoming packet
4. goto 2.

**connection**:
1. create and start a new thread (by **connector**)
2. establishes connection (by handling the packet)
3. perform application handshake (by receiving packets from **connector 3.2**)
4. handles application logic (by receiving packets from **connector 3.2**)
5. close connection (not really necessary with udp - only the thread finishes)

### Bluetooth Low Energy
Unfortunately Bluetooth Low Energy provides a mixture of both.
Messages are received asynchronously but peripheral and central devices are connected by connections.
Furthermore peripherals are the server and central devices are the clients.
But in most Sensor/Act[ua|o]r networks data is send from a peripheral device to a central device to be processed.

This leads us to the following paradigma: connections are established by the spatial locality of a peripheral device to a central device. Resulting in interpreting a BLE advertisment as a request to connect - the connection itself is started by the central device.

We use the following pattern for BLE4.0 then:
(fortunately a peripheral cannot advertise and be connected to a central at the same time)
(for simplicity some steps are missing)

**connector**:
1. start discovering Bluetooth peripherals
2. await advertisments
3. on advertisment received start new **connection**
4. goto 2.

**connection**:
1. create and start a new thread (by **connector**)
2. establishes connection by:
2.1 check available services and characterstics
2.2 if needed services and characteristics are available: goto 2.3 else goto 5.
2.3 register notifications (connection is established) goto 3.
3. perform application handshake
4. handles application logic
5. close connection

For all (tcp, udp, BLE) technologies a **connector** and a **connection** class shall be implemented a **SimpleConnectorInterface** and a **SimpleConnectionInterface**.
This shall make the whole project even estensible to other technologies like ZigBee, Lora and so on.





