# GPS-Neo-6M

<p align="justify">A code for interfacing the ATMega32A microcontroller with the Neo-6M GPS module.</p>

<p align="justify">The u-blox Neo-6 series GPS modules, as well as their successive versions, use an inbuilt GPS positioning chip to determine the exact latitude and longitude of 
the current position of the module, along with some other data. This data is transmitted continuously as a series of strings over USART, SPI or USB communication protocols and is in the NMEA data format. The string required to determine the position begins with the tag $GPGGA and includes the latitude, longitude, GMT time and altitude of the current position of the module seperated by semi-colons. Depending on whether the signal is to be acquired from a cold start (the first time a connection is made with a satelite, either during the first time the device is used or when the previous connection has been lost) or a hot start (a connection has been made with a satelite recently) the device could take between 1 to 30 seconds to make a connection and begin transmitting data.</p>

<p align="justify">In this code, within a USART RX interrupt routine it is determined if the received string begins with $GPGGA and if it does, the remaining portion is extracted and saved in the array "buffer" from which the latitude and longitude portions are extracted and transmitted over USART (along with the entire GPGGA string) to be used or displayed.</p>
