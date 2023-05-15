# Development of Smart Trolley for Retail Project 

The Smart Trolley is a shopping cart that can be used by a supermarket customer without the intervention of any store employees. The processes of adding the items to the trolley and making the payment is automated. The smart trolley is shown in figure 2.1


 <p align="center"> 
 <img src="https://github.com/DerickFernando51/SmartTrolley/assets/124335793/f59fcbbe-0ce5-4642-ba03-9951548ae597" width="500"> </p>
 
 <p align="center">  Figure 2.1 Smart Trolley  </p> 



When customers add items to the trolley, they must scan its barcode using the barcode scanner mounted near the handles of the trolley. If they want to remove any items they can press the remove button, scan the barcode of the product and remove it from the trolley. Once all products have been selected customers can press the end button and receive the total amount and instructions to make the payment.

Four load cells were connected to the four wheels of the trolley. These load cells recorded the weight of items added. This was done in order to prevent the theft of items. The weight of the load cells was compared against the listed weight of items. If any items are added to the cart without scanning its barcode the two weights would not be equal and the system would notify supermarket employees to inspect the trolley.
 
 Figure 2.2 Flow chart of Smart Trolley System




 

Figure 2.3 Smart Trolley System architecture


Figures 2.2 and 2.3 show the work flow and the architecture of the Smart Trolley system respectively.

I was tasked with testing the existing features, adding new functionality to the system and developing the smart trolley mobile application. 



2.1.1 Adding remove button 

The position and circuit diagram of the remove button is shown in figures 2.4 and 2.5

 
Figure 2.4 Remove button position in Smart Trolley


 
Figure 2.5 Remove button circuit diagram

•	When connecting the remove push button to the Smart Trolley system a pulldown resistor was used as shown. This is done to ensure that the wire is pulled to a logic level at the absence of an input and prevents the floating state of a pin.

•	The pulldown resistor was connected between the ground terminal of the push button and the and the ground pin of the microcontroller. The other terminal of the push button was connected to GPIO pin 27 of the ESP 32.


 
 

Figure 2.6 Remove button code

The code shown in figure 2.6 can be explained as follows:

•	The remove button press is detected using an Arduino hardware interrupt. All the GPIO pins in the ESP32 microcontroller can be used as interrupts. Since the remove button is connected to pin 27 the interrupt is set to this pin.

•	The ISR (interrupt service routine) is the function that is called when the interrupt is triggered. Inside the ISR the state of the remove button is set to 1. The Remove_Item() function is executed if the remove button state is equal to 1.

•	Inside the remove button function the user is prompted to scan the barcode of the item to be removed. Then the scanned barcode is displayed in the Arduino serial monitor. Finally, the remove button state is reset to 0.

I soldered the push button connections and constructed the circuit as shown in figure 2.5. Then, I tested the remove button code and integrated it to the smart trolley system final code after confirming its accuracy.






























2.1.2 Adding end button

The position and circuit diagram of the end button is shown in figures 2.7 and 2.8

 
Figure 2.7 End button position in Smart Trolley


 
Figure 2.8 End button circuit diagram
 
 
Figure 2.9 End button code

The code shown in figure 2.9 can be explained as follows:

•	The end button press is detected using an Arduino hardware interrupt. All the GPIO pins in the ESP32 microcontroller can be used as interrupts. Since the remove button is connected to pin 5 the interrupt is set to this pin.

•	The ISR (interrupt service routine) is the function that is called when the interrupt is triggered. Inside the ISR the state of the remove button is set to 1. The End_purchasing() function is executed if the remove button state is equal to 1.

•	Inside the end button function the total amount is displayed in the Arduino serial monitor.  


I soldered the end push button connections and constructed the circuit. Then, I tested the end button code and integrated it to the smart trolley system final code after confirming that it functions as expected.
2.1.3 Testing load cells

Four 100kg CZL601 load cells shown in figure 2.10 were used for the purpose of preventing theft in this system. I was instructed to comprehensively test these load cells and determine whether the project could be progressed using this weight sensor and if not to propose a different sensor/method for this process.

 
Figure 2.10 CZL601 load cell


Figure 2.11 shows the parts of the setup required to test load cells and figure 2.12 shows the setup created in the lab for this purpose.

 
Figure 2.11 Mounting a load cell apparatus



   
Figure 2.12 Load cell mounting setup 



The calibration curve was plotted to verify the accuracy of the load cells. Known weights in the range of 100g – 38 kg were used to obtain the required data points. Firstly, the actual weight was measured using a scale and then the weight measured by the load cells was obtained. The data points and the calibration curve are shown in table 2.1 and figure 2.13 respectively. The calibration factor is obtained by dividing the measured weight by the applied weight.


Table 2.1 Load cell calibration curve data points

Applied weight (g)	Measured weight (g)
[Trolley and known weight]	Calibration factor
340	1873	5.508
290	1596	5.503
1965	10820	5.506
138	760	5.507
1220	6722	5.509
38930	212600	5.447
12130	66961	5.520
5150	28175	5.471

 
Figure 2.14 Load cell calibration curve


 
Figure 2.15 Typical calibration curve for load vs. output

Figure 2.15 shows the calibration curve given in CZL601 load cell datasheet. These load cells had an error of ±0.023% of full scale. The tests performed confirmed that the error in the weight measurement of products was within this range.

2.1.4 RFID proposal

The smart trolley system prevents theft by calculating the total weight of products and comparing it with the listed weight. I proposed using RFID tags instead of load cells for this purpose.

For this proposal to be practically implemented RFID tags would need to be pasted on all the supermarket products. Then, the barcode scanner is replaced by a RFID scanner. The system architecture of the RFID proposal is detailed in figure 2.16


 
Figure 2.16 Proposed system architecture

Drawbacks of using load cells:
•	Accuracy is not enough for light weight products. CZL601 load cell belong to accuracy class C3 (error of ±0.023% of full scale)
•	Products with similar weight, but different prices can be stolen without being detected by the system

Advantages of using RFID tags:
•	RFID scanner is a faster scanner than a barcode scanner
•	Can scan multiple items at once
•	Does not require direct line of sight. Uses NFC (Near Field Technology)
•	Will reduce the changes need to be made to the product as load cells will not need to be fixed to the four legs.

Disadvantage of using RFID tags:
•	RFID tags need to be pasted on all products


This proposal was rejected due to the above disadvantage listed. A decision was made to use a camera and identify products using computer vision and machine learning principles.
2.1.5 Smart trolley mobile app

I created the Smart Trolley mobile application. This was done in Android studio IDE (Integrated development environment) using Kotlin programming language. Figure 2.17 details the architecture of the Smart Trolley system.

 
Figure 2.17 Smart trolley system architecture

Firebase was used to create a real time database and the Arduino code was modified to store all scanned barcodes in this database. Figure 2.18 shows how the barcode data was saved in the Firebase database.

 
Figure 2.18 Barcode data stored in Firebase database
 
XML (Extensible Markup Language) was used to define the UI layout of this application. The layout of the main activity page is shown in figure 2.19 The android studio application was linked to the Firebase database and showed the stored data in real time using recycler view.


 

Figure 2.19 Main activity page of Smart Trolley mobile app


The QR code shown in figure 2.20 was generated and the mobile application could be opened by scanning this code. The intention was to paste this on the Smart Trolley allowing customers to easily open the app. The necessary changes were made to the manifest file of the Smart Trolley application to allow the app to be opened by scanning the QR code.

 

Figure 2.20 QR code generated to open Smart Trolley mobile app




