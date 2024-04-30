# Questions

1. **How can we replace the RFID module with a bluetooth module in the embedded part? Justify your answer.**

The RFID module was used to get the user's ID and send it to the board for authentication.  
To replace the RFID module with a bluetooth module, first of all we need to get a bluetooth module as the Arduino Uno board does not have a built-in bluetooth module. A commonly used bluetooth module is the HC-05 module.

The HC-05 module can be connected to the Arduino Uno board using the serial communication pins (RX and TX). We can then use the built-in serial communication functions in the Arduino IDE to communicate with the bluetooth module. There are also libraries available that can be used to simplify the communication process.

The Arduino code needs to be adjusted to listen for bluetooth's incoming data instead of the RFID module's data. The bluetooth module can be configured to send the user's ID when a connection is established. The Arduino code can then read this data and proceed with the authentication process.

Now to send the user ID using bluetooth connectivity, we have to first pair the devices.  
The user's device can be paired with the bluetooth module. After the pairing is completed, the ID can be sent to the bluetooth module from the user's device. The bluetooth module can then send this data to the Arduino board for authentication.  
The bluetooth pairing process differs in different standard versions of bluetooth used. For example, in Bluetooth Classic, the pairing process involves exchanging a PIN code between the devices. In Bluetooth Low Energy (BLE), the pairing process can be done using a passkey or without any user interaction. There is also Bluetooth Secure Simple Pairing (SSP) which provides a more secure pairing process using public key cryptography.

We can also only use the bluetooth pairing authentication to authenticate the user instead of sending the ID. The user's device can be paired with the bluetooth module and the Arduino board can authenticate the user based on the pairing information. Such information could be the MAC address of the device that is trying to connect. Or it can be the pairing keys.

We can also authenticate without pairing the devices. This is explained in the next question.

2. **Can we transfer the required data (users' ID) without pairing the bluetooth modules? Justify your answer.**

Yes, we can transfer the required data without the pairing process. There are some methods that we can use for this purpose:

We can use the bluetooth's advertising feature to broadcast our data. The bluetooth module listens for the advertisements and extracts the data needed. The security of this method is much lower than pairing and anyone in range can access the data. This is more used with BLE devices as it is designed for low power consumption and small data transfers and the range is much more limited.

We can also use Pre-configured Pairing Key (PPK) which establishes a connection without the need for user interaction using a pre-configured key. Devices can be pre-configured with unique pairing keys during the development process. These keys enable automatic authentication and connection establishment without user intervention. This method is more secure than the advertisement method but still not as secure as actual pairing.

There are also out-of-bound pairing methods which use a separate channel to exchange authentication information which does not rely on bluetooth pairing. Such methods can also be used to remove the need for direct pairing between devices. For example, NFC can be used to exchange pairing information between devices.

3. **Assuming that we are using a bluetooth module, how can we replace the timer for closing the entrance door? Justify your answer.**

We can use the bluetooth beacon technology to replace the timer for closing the entrance door.  
This means that the door can be closed once the user's device is out of range for the bluetooth module to detect its existence.  
Since bluetooth has low range, the entrance door can be closed quite quickly after the user leaves the range of the bluetooth module.

We can further improve the system by using the Received Signal Strength Indicator (RSSI) value of the bluetooth signal to determine the distance between the user's device and the bluetooth module. This can be used to trigger the door closing mechanism when the user is at a certain distance from the door. This provides better security for the entrance door as the closing distance is lower.

By combining the bluetooth signal with a proximity sensor, we can detect the presence of the user near the door and correct the bluetooth signal's error margin. This can be used to ensure that the door is closed only when the user is actually leaving it and further improve the security of the entrance door.
