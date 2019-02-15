# vendingLock
Source code and information regarding the lock on the SAS PX vending machine.


Modify this code to change the cards that can access the system. 

How the mechanism functions:

The HZ1050 board is a 125khz Radio Frequency Identification reader, and connects to the arduino over serial.
The board is only capable of transmitting the unique identifier on each 125k card/tag. It is not capable of writing.  The reason we chose this system over the more common MFRC522 is the ease of card duplication. MFRC522 devices can be read and writted with UIDS easily with the correct parts off Taobao, costing about RMB25. The 125k cards require a ProxMark tool to write, and thus the barrier for entry is much higher. 

The arduino interprets the serial code, refrences it to the array of approved values, then activates the appropriate relay to open the door with the solenoid lock.  The code provided is that for the arduino nano used.

The code was initially set up with front and back doors, and that is why many parts of the code are written twice. This has been left in for future expansion, along with the relay and lock that are already wired up. It just hasn't been installed fully. 

A full schematic of the wiring can be viewed in the documentation book for the machine, however most of the connections are pretty self explanitory.

This code is provided as open source. Feel free to modify it to fit your own project. This is one example of implimentation for the HZ1050 board, I thought I would make it avalable because there are so few examples of working code for this system. 

Just don't upload code to the machine's board without authorization :) Thanks! 
