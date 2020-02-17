# UBLOX NEO M8N GPS with COMPASS

![](https://user-images.githubusercontent.com/9809095/74649219-0d0aef80-5177-11ea-8070-70fc26bf44aa.jpg)

This is simples guide for using UBLOX NEO M8N GPS module with Raspberry Pi.

# What do you need:
 - Raspberry Pi (in my case v2)
 - Ublox Neo M8N GPS
 - Jumper cables

# How to connect:
![alt text](https://user-images.githubusercontent.com/9809095/74649225-0ed4b300-5177-11ea-8746-c2d1c5fcffaa.jpg)

 - RED      >   3.3V
 - BLACK    >   GND
 - WHITE    >   UART0 TX
 - YELLOW   >   UART0 RX
 - BLUE     >   I2C1 SDA
 - GREEN    >   I2C1 SCL
 
 # Test connection
 To check if you are receiving data from GPS simply execute:
 >sudo cat /dev/ttyAMA0
 
 You will get something like this:
```
$GPGSV,3,3,11,23,40,051,28,30,16,173,18,33,43,190,*45
$GLGSV,3,1,10,69,38,050,15,70,56,318,22,71,17,273,22,72,,,26*5A
$GLGSV,3,2,10,78,14,029,,79,28,065,27,80,19,131,,84,25,194,21*61
$GLGSV,3,3,10,85,53,258,33,86,25,325,31*6F
$GNGLL,4038.05311,N,00839.60587,W,181918.00,A,A*6A
$GNRMC,181918.10,A,4038.05313,N,00839.60588,W,0.268,,130917,,,A*7E
$GNVTG,,T,,M,0.268,N,0.495,K,A*39
$GNGGA,181918.10,4038.05313,N,00839.60588,W,1,12,0.90,31.7,M,49.7,M,,*66
$GNGSA,A,3,30,17,19,23,06,09,02,03,,,,,1.66,0.90,1.40*13
$GNGSA,A,3,85,84,86,69,70,,,,,,,,1.66,0.90,1.40*16
$GPGSV,3,1,11,02,36,309,31,03,16,092,18,05,04,285,,06,76,255,42*7F
$GPGSV,3,2,11,07,38,144,29,09,71,044,39,17,09,203,24,19,22,220,22*7B
$GPGSV,3,3,11,23,40,051,28,30,16,173,18,33,43,190,*45
$GLGSV,3,1,10,69,38,050,14,70,56,318,22,71,17,273,22,72,,,26*5B
$GLGSV,3,2,10,78,14,029,,79,28,065,27,80,19,131,,84,25,194,21*61
$GLGSV,3,3,10,85,53,258,33,86,25,325,31*6F
$GNGLL,4038.05313,N,00839.60588,W,181918.10,A,A*66
$GNRMC,181918.20,A,4038.05315,N,00839.60589,W,0.240,,130917,,,A*70
$GNVTG,,T,,M,0.240,N,0.444,K,A*3F
...
```

# Explanation
> $GNGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47

```
GGA          Global Positioning System Fix Data
123519       Fix taken at 12:35:19 UTC
4807.038,N   Latitude 48 deg 07.038' N
01131.000,E  Longitude 11 deg 31.000' E
1            Fix quality: 0 = invalid
                         1 = GPS fix (SPS)
                         2 = DGPS fix
                         3 = PPS fix
       4 = Real Time Kinematic
       5 = Float RTK
                         6 = estimated (dead reckoning) (2.3 feature)
       7 = Manual input mode
       8 = Simulation mode
08           Number of satellites being tracked
0.9          Horizontal dilution of position
545.4,M      Altitude, Meters, above mean sea level
46.9,M       Height of geoid (mean sea level) above WGS84
                ellipsoid
(empty field) time in seconds since last DGPS update
(empty field) DGPS station ID number
*47          the checksum data, always begins with *
 ```

**You can check meaning of all of these value in the manual:**
> https://github.com/bartashevich/UBLOX-NEO-M8N-GPS/files/4213373/gps_manual.pdf

# File compilation
```
g++ gps.cpp -o gps -lwiringPi -lserial -lm -std=c++11
```
