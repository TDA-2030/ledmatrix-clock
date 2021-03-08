EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 3
Title "LED Matrix Clock"
Date "2020-11-02"
Rev "V1..0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:C C1
U 1 1 5F4D2C09
P 1450 2000
F 0 "C1" H 1565 2046 50  0000 L CNN
F 1 "0.1uf" H 1565 1955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 1488 1850 50  0001 C CNN
F 3 "~" H 1450 2000 50  0001 C CNN
	1    1450 2000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5F4D3040
P 1450 1700
F 0 "R1" H 1520 1746 50  0000 L CNN
F 1 "10K" H 1520 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 1380 1700 50  0001 C CNN
F 3 "~" H 1450 1700 50  0001 C CNN
	1    1450 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 1850 1450 1850
Connection ~ 1450 1850
$Comp
L power:+3.3V #PWR0101
U 1 1 5F4D574F
P 1450 1550
F 0 "#PWR0101" H 1450 1400 50  0001 C CNN
F 1 "+3.3V" H 1465 1723 50  0000 C CNN
F 2 "" H 1450 1550 50  0001 C CNN
F 3 "" H 1450 1550 50  0001 C CNN
	1    1450 1550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0102
U 1 1 5F4D5EAB
P 2850 1050
F 0 "#PWR0102" H 2850 900 50  0001 C CNN
F 1 "+3.3V" H 2865 1223 50  0000 C CNN
F 2 "" H 2850 1050 50  0001 C CNN
F 3 "" H 2850 1050 50  0001 C CNN
	1    2850 1050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 5F4D6B4E
P 1450 2150
F 0 "#PWR0103" H 1450 1900 50  0001 C CNN
F 1 "GND" H 1455 1977 50  0000 C CNN
F 2 "" H 1450 2150 50  0001 C CNN
F 3 "" H 1450 2150 50  0001 C CNN
	1    1450 2150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 5F4D7574
P 2850 4450
F 0 "#PWR0104" H 2850 4200 50  0001 C CNN
F 1 "GND" H 2855 4277 50  0000 C CNN
F 2 "" H 2850 4450 50  0001 C CNN
F 3 "" H 2850 4450 50  0001 C CNN
	1    2850 4450
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x06 J1
U 1 1 5F4D7FCD
P 1100 3450
F 0 "J1" H 1200 2950 50  0000 C CNN
F 1 "download" H 1250 3050 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 1100 3450 50  0001 C CNN
F 3 "~" H 1100 3450 50  0001 C CNN
	1    1100 3450
	-1   0    0    1   
$EndComp
Text Label 3600 1950 0    50   ~ 0
TXD0
Text Label 3600 2150 0    50   ~ 0
RXD0
Text Label 2000 1850 0    50   ~ 0
EN
Wire Wire Line
	3450 1850 3600 1850
Wire Wire Line
	3450 1950 3600 1950
Wire Wire Line
	3450 2150 3600 2150
Text Label 1300 3450 0    50   ~ 0
TXD0
Text Label 1300 3550 0    50   ~ 0
RXD0
Text Label 3600 1850 0    50   ~ 0
BOOT
Text Label 1300 3250 0    50   ~ 0
BOOT
Text Label 1300 3350 0    50   ~ 0
EN
$Comp
L power:GND #PWR0105
U 1 1 5F4DA4BE
P 1300 3650
F 0 "#PWR0105" H 1300 3400 50  0001 C CNN
F 1 "GND" H 1305 3477 50  0000 C CNN
F 2 "" H 1300 3650 50  0001 C CNN
F 3 "" H 1300 3650 50  0001 C CNN
	1    1300 3650
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0106
U 1 1 5F4DAAEF
P 1300 3150
F 0 "#PWR0106" H 1300 3000 50  0001 C CNN
F 1 "+3.3V" H 1315 3323 50  0000 C CNN
F 2 "" H 1300 3150 50  0001 C CNN
F 3 "" H 1300 3150 50  0001 C CNN
	1    1300 3150
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 5F4DB89B
P 2700 1450
F 0 "C3" V 2750 1300 50  0000 L CNN
F 1 "10uf" V 2850 1350 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 2738 1300 50  0001 C CNN
F 3 "~" H 2700 1450 50  0001 C CNN
	1    2700 1450
	0    1    1    0   
$EndComp
$Comp
L Device:C C2
U 1 1 5F4DDB9E
P 2700 1150
F 0 "C2" V 2750 1000 50  0000 L CNN
F 1 "1uf" V 2850 1050 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2738 1000 50  0001 C CNN
F 3 "~" H 2700 1150 50  0001 C CNN
	1    2700 1150
	0    1    1    0   
$EndComp
Wire Wire Line
	2850 1050 2850 1150
Connection ~ 2850 1150
Wire Wire Line
	2850 1150 2850 1450
Connection ~ 2850 1450
Wire Wire Line
	2850 1450 2850 1650
Wire Wire Line
	2550 1150 2550 1450
$Comp
L power:GND #PWR0107
U 1 1 5F4E0168
P 2550 1450
F 0 "#PWR0107" H 2550 1200 50  0001 C CNN
F 1 "GND" H 2555 1277 50  0000 C CNN
F 2 "" H 2550 1450 50  0001 C CNN
F 3 "" H 2550 1450 50  0001 C CNN
	1    2550 1450
	0    1    1    0   
$EndComp
Connection ~ 2550 1450
$Comp
L Device:LED D1
U 1 1 5F5659E0
P 4650 2050
F 0 "D1" H 4643 1795 50  0000 C CNN
F 1 "LED_BLUE" H 4643 1886 50  0000 C CNN
F 2 "LED_SMD:LED_0603_1608Metric" H 4650 2050 50  0001 C CNN
F 3 "~" H 4650 2050 50  0001 C CNN
	1    4650 2050
	-1   0    0    1   
$EndComp
Wire Wire Line
	4000 2050 3450 2050
$Comp
L power:GND #PWR0108
U 1 1 5F56B6A0
P 4800 2050
F 0 "#PWR0108" H 4800 1800 50  0001 C CNN
F 1 "GND" H 4805 1877 50  0000 C CNN
F 2 "" H 4800 2050 50  0001 C CNN
F 3 "" H 4800 2050 50  0001 C CNN
	1    4800 2050
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R2
U 1 1 5F56C2A8
P 4150 2050
F 0 "R2" V 4250 2000 50  0000 L CNN
F 1 "4.7K" V 4050 1950 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 4080 2050 50  0001 C CNN
F 3 "~" H 4150 2050 50  0001 C CNN
	1    4150 2050
	0    1    1    0   
$EndComp
Wire Wire Line
	4300 2050 4500 2050
NoConn ~ 2250 2050
NoConn ~ 2250 3050
NoConn ~ 2250 3150
NoConn ~ 2250 3250
NoConn ~ 2250 3350
NoConn ~ 2250 3450
NoConn ~ 2250 3550
$Comp
L RF_Module:ESP32-WROOM-32D U1
U 1 1 5F4D0EEA
P 2850 3050
F 0 "U1" H 2400 4400 50  0000 C CNN
F 1 "ESP32-WROOM-32D" H 3300 4450 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 2850 1550 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf" H 2550 3100 50  0001 C CNN
	1    2850 3050
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x08_Odd_Even J2
U 1 1 5FA617FF
P 9950 3050
F 0 "J2" H 10000 3567 50  0000 C CNN
F 1 "Conn_02x08_Counter_Clockwise" H 10000 3476 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x08_P2.54mm_Vertical" H 9950 3050 50  0001 C CNN
F 3 "~" H 9950 3050 50  0001 C CNN
	1    9950 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0109
U 1 1 5FA66D2D
P 10350 3450
F 0 "#PWR0109" H 10350 3200 50  0001 C CNN
F 1 "GND" H 10355 3277 50  0000 C CNN
F 2 "" H 10350 3450 50  0001 C CNN
F 3 "" H 10350 3450 50  0001 C CNN
	1    10350 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	10250 3350 10350 3350
Wire Wire Line
	10350 3350 10350 3450
Wire Wire Line
	10250 3450 10350 3450
Connection ~ 10350 3450
$Comp
L power:GND #PWR0110
U 1 1 5FA683C6
P 10450 2750
F 0 "#PWR0110" H 10450 2500 50  0001 C CNN
F 1 "GND" H 10455 2577 50  0000 C CNN
F 2 "" H 10450 2750 50  0001 C CNN
F 3 "" H 10450 2750 50  0001 C CNN
	1    10450 2750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10250 2950 10250 2850
Connection ~ 10250 2850
Wire Wire Line
	10250 2850 10250 2750
Wire Wire Line
	10450 2750 10250 2750
Connection ~ 10250 2750
Text Label 3550 3150 0    50   ~ 0
LED_SCK
Text Label 3550 3050 0    50   ~ 0
LED_STCP
Text Label 3550 3850 0    50   ~ 0
LED_G2
Text Label 3550 3750 0    50   ~ 0
LED_G1
Text Label 3550 3350 0    50   ~ 0
LED_R1
Text Label 3550 3450 0    50   ~ 0
LED_OE
Text Label 3550 2450 0    50   ~ 0
LED_A
Text Label 3550 2550 0    50   ~ 0
LED_B
Text Label 3550 2650 0    50   ~ 0
LED_C
Text Label 3550 2750 0    50   ~ 0
LED_D
NoConn ~ 3450 2850
NoConn ~ 3450 2950
$Sheet
S 4350 3450 1400 750 
U 5FA7C878
F0 "Sheet5FA7C877" 50
F1 "speaker.sch" 50
F2 "SHUTDOWN" I L 4350 3550 50 
F3 "INPUT" I L 4350 3650 50 
F4 "I2C_SDA" I R 5750 3550 50 
F5 "I2C_SCL" I R 5750 3650 50 
$EndSheet
Wire Wire Line
	3450 3550 4350 3550
Wire Wire Line
	3450 3650 4350 3650
Wire Wire Line
	3450 3750 3550 3750
Wire Wire Line
	3550 3850 3450 3850
Wire Wire Line
	3450 3450 3550 3450
Wire Wire Line
	3550 3350 3450 3350
Wire Wire Line
	3450 3250 3550 3250
Wire Wire Line
	3550 3150 3450 3150
Wire Wire Line
	3450 3050 3550 3050
Wire Wire Line
	3450 2450 3550 2450
Wire Wire Line
	3550 2550 3450 2550
Wire Wire Line
	3450 2650 3550 2650
Wire Wire Line
	3550 2750 3450 2750
NoConn ~ 2250 2150
$Comp
L Sensor_Humidity:SHTC3 U2
U 1 1 5FA8C053
P 6200 2350
F 0 "U2" H 6444 2396 50  0000 L CNN
F 1 "SHTC3" H 6444 2305 50  0000 L CNN
F 2 "Sensor_Humidity:Sensirion_DFN-4-1EP_2x2mm_P1mm_EP0.7x1.6mm" H 6400 2000 50  0001 C CNN
F 3 "https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/0_Datasheets/Humidity/Sensirion_Humidity_Sensors_SHTC3_Datasheet.pdf" H 5900 2800 50  0001 C CNN
	1    6200 2350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0111
U 1 1 5FA8EA19
P 6300 2650
F 0 "#PWR0111" H 6300 2400 50  0001 C CNN
F 1 "GND" H 6305 2477 50  0000 C CNN
F 2 "" H 6300 2650 50  0001 C CNN
F 3 "" H 6300 2650 50  0001 C CNN
	1    6300 2650
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0112
U 1 1 5FA8F16D
P 6300 1900
F 0 "#PWR0112" H 6300 1750 50  0001 C CNN
F 1 "+3.3V" H 6315 2073 50  0000 C CNN
F 2 "" H 6300 1900 50  0001 C CNN
F 3 "" H 6300 1900 50  0001 C CNN
	1    6300 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 5FA8F931
P 6550 2000
F 0 "C4" V 6600 1850 50  0000 L CNN
F 1 "1uf" V 6700 1900 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6588 1850 50  0001 C CNN
F 3 "~" H 6550 2000 50  0001 C CNN
	1    6550 2000
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0113
U 1 1 5FA906A7
P 6700 2000
F 0 "#PWR0113" H 6700 1750 50  0001 C CNN
F 1 "GND" H 6705 1827 50  0000 C CNN
F 2 "" H 6700 2000 50  0001 C CNN
F 3 "" H 6700 2000 50  0001 C CNN
	1    6700 2000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6300 1900 6300 2000
Wire Wire Line
	6400 2000 6300 2000
Connection ~ 6300 2000
Wire Wire Line
	6300 2000 6300 2050
$Comp
L Device:R R4
U 1 1 5FA4BB55
P 5750 1950
F 0 "R4" H 5820 1996 50  0000 L CNN
F 1 "10K" H 5820 1905 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 5680 1950 50  0001 C CNN
F 3 "~" H 5750 1950 50  0001 C CNN
	1    5750 1950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 5FA4C577
P 5450 1950
F 0 "R3" H 5520 1996 50  0000 L CNN
F 1 "10K" H 5520 1905 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 5380 1950 50  0001 C CNN
F 3 "~" H 5450 1950 50  0001 C CNN
	1    5450 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 2100 5450 2250
Wire Wire Line
	5450 2250 5900 2250
Wire Wire Line
	5750 2100 5750 2350
Wire Wire Line
	5750 2450 5900 2450
Wire Wire Line
	5450 1800 5750 1800
$Comp
L power:+3.3V #PWR0114
U 1 1 5FA60A7D
P 5750 1800
F 0 "#PWR0114" H 5750 1650 50  0001 C CNN
F 1 "+3.3V" H 5765 1973 50  0000 C CNN
F 2 "" H 5750 1800 50  0001 C CNN
F 3 "" H 5750 1800 50  0001 C CNN
	1    5750 1800
	1    0    0    -1  
$EndComp
Connection ~ 5750 1800
NoConn ~ 3450 4050
NoConn ~ 3450 4150
$Comp
L power:+5V #PWR0116
U 1 1 5FA96C5D
P 8200 2450
F 0 "#PWR0116" H 8200 2300 50  0001 C CNN
F 1 "+5V" H 8215 2623 50  0000 C CNN
F 2 "" H 8200 2450 50  0001 C CNN
F 3 "" H 8200 2450 50  0001 C CNN
	1    8200 2450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0120
U 1 1 5FA9D94C
P 8200 6200
F 0 "#PWR0120" H 8200 5950 50  0001 C CNN
F 1 "GND" H 8205 6027 50  0000 C CNN
F 2 "" H 8200 6200 50  0001 C CNN
F 3 "" H 8200 6200 50  0001 C CNN
	1    8200 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 3450 7150 3450
Wire Wire Line
	7150 3350 7700 3350
Wire Wire Line
	7700 3250 7150 3250
Wire Wire Line
	7150 3150 7700 3150
Wire Wire Line
	7700 3050 7150 3050
Wire Wire Line
	7150 2950 7700 2950
Wire Wire Line
	7700 2850 7150 2850
Wire Wire Line
	7150 2750 7700 2750
Text Label 7150 2750 0    50   ~ 0
LED_SCK
Text Label 7150 2850 0    50   ~ 0
LED_STCP
Text Label 7150 2950 0    50   ~ 0
LED_G2
Text Label 7150 3050 0    50   ~ 0
LED_G1
Text Label 7150 3150 0    50   ~ 0
LED_D
Text Label 7150 3250 0    50   ~ 0
LED_C
Text Label 7150 3350 0    50   ~ 0
LED_B
Text Label 7150 3450 0    50   ~ 0
LED_A
Wire Wire Line
	10250 3250 10600 3250
Wire Wire Line
	10250 3150 10750 3150
Wire Wire Line
	10250 3050 10900 3050
Text Label 7150 5100 0    50   ~ 0
LED_OE
Text Label 7150 5000 0    50   ~ 0
LED_R1
Text Label 7150 4900 0    50   ~ 0
LED_R2
Wire Wire Line
	7150 4900 7700 4900
Wire Wire Line
	7150 5000 7700 5000
Wire Wire Line
	7150 5100 7700 5100
NoConn ~ 8700 5200
NoConn ~ 8700 5300
NoConn ~ 8700 5400
NoConn ~ 8700 5500
NoConn ~ 8700 5600
NoConn ~ 7700 5600
NoConn ~ 7700 5500
NoConn ~ 7700 5400
NoConn ~ 7700 5300
NoConn ~ 7700 5200
$Comp
L Device:C C5
U 1 1 5FAAEF0D
P 7950 4600
F 0 "C5" H 8050 4600 50  0000 L CNN
F 1 "0.1uf" H 7950 4500 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 7988 4450 50  0001 C CNN
F 3 "~" H 7950 4600 50  0001 C CNN
	1    7950 4600
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C6
U 1 1 5FAB23AD
P 7950 2450
F 0 "C6" H 8050 2450 50  0000 L CNN
F 1 "0.1uf" H 7950 2350 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 7988 2300 50  0001 C CNN
F 3 "~" H 7950 2450 50  0001 C CNN
	1    7950 2450
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5FAB26C3
P 8200 4050
F 0 "#PWR02" H 8200 3800 50  0001 C CNN
F 1 "GND" H 8205 3877 50  0000 C CNN
F 2 "" H 8200 4050 50  0001 C CNN
F 3 "" H 8200 4050 50  0001 C CNN
	1    8200 4050
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR03
U 1 1 5FAB542C
P 7700 3650
F 0 "#PWR03" H 7700 3500 50  0001 C CNN
F 1 "+5V" H 7715 3823 50  0000 C CNN
F 2 "" H 7700 3650 50  0001 C CNN
F 3 "" H 7700 3650 50  0001 C CNN
	1    7700 3650
	0    -1   -1   0   
$EndComp
Text Label 9350 3450 0    50   ~ 0
_LED_SCK
Text Label 9350 3350 0    50   ~ 0
_LED_STCP
Text Label 9350 3250 0    50   ~ 0
_LED_G2
Text Label 9350 3150 0    50   ~ 0
_LED_G1
Text Label 9350 3050 0    50   ~ 0
_LED_D
Text Label 9350 2950 0    50   ~ 0
_LED_C
Text Label 9350 2850 0    50   ~ 0
_LED_B
Text Label 9350 2750 0    50   ~ 0
_LED_A
Wire Wire Line
	9350 2750 9750 2750
Wire Wire Line
	9750 2850 9350 2850
Wire Wire Line
	9350 2950 9750 2950
Wire Wire Line
	9750 3050 9350 3050
Wire Wire Line
	9350 3150 9750 3150
Wire Wire Line
	9750 3250 9350 3250
Wire Wire Line
	9350 3350 9750 3350
Wire Wire Line
	9750 3450 9350 3450
Wire Wire Line
	8700 4900 10600 4900
Wire Wire Line
	8700 5000 10750 5000
Wire Wire Line
	8700 5100 10900 5100
Text Label 10300 3150 0    50   ~ 0
_LED_R1
Text Label 10300 3250 0    50   ~ 0
_LED_R2
Text Label 10300 3050 0    50   ~ 0
_LED_OE
Text Label 3550 3250 0    50   ~ 0
LED_R2
$Comp
L 74xx:74HC245 U4
U 1 1 5FB575C4
P 8200 5400
F 0 "U4" H 8300 6200 50  0000 C CNN
F 1 "74HC245" H 8400 6100 50  0000 C CNN
F 2 "Package_SO:TSSOP-20_4.4x6.5mm_P0.65mm" H 8200 5400 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74HC245" H 8200 5400 50  0001 C CNN
	1    8200 5400
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0115
U 1 1 5FB5F4A3
P 8200 4600
F 0 "#PWR0115" H 8200 4450 50  0001 C CNN
F 1 "+5V" H 8215 4773 50  0000 C CNN
F 2 "" H 8200 4600 50  0001 C CNN
F 3 "" H 8200 4600 50  0001 C CNN
	1    8200 4600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0117
U 1 1 5FB612A0
P 7700 5900
F 0 "#PWR0117" H 7700 5650 50  0001 C CNN
F 1 "GND" H 7705 5727 50  0000 C CNN
F 2 "" H 7700 5900 50  0001 C CNN
F 3 "" H 7700 5900 50  0001 C CNN
	1    7700 5900
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0118
U 1 1 5FB61764
P 7700 5800
F 0 "#PWR0118" H 7700 5650 50  0001 C CNN
F 1 "+5V" H 7715 5973 50  0000 C CNN
F 2 "" H 7700 5800 50  0001 C CNN
F 3 "" H 7700 5800 50  0001 C CNN
	1    7700 5800
	0    -1   -1   0   
$EndComp
Text Label 8700 3450 0    50   ~ 0
_LED_A
Text Label 8700 3350 0    50   ~ 0
_LED_B
Text Label 8700 3250 0    50   ~ 0
_LED_C
Text Label 8700 3150 0    50   ~ 0
_LED_D
Text Label 8700 3050 0    50   ~ 0
_LED_G1
Text Label 8700 2950 0    50   ~ 0
_LED_G2
Text Label 8700 2850 0    50   ~ 0
_LED_STCP
Text Label 8700 2750 0    50   ~ 0
_LED_SCK
$Comp
L 74xx:74HC245 U3
U 1 1 5FB594F2
P 8200 3250
F 0 "U3" H 8350 4000 50  0000 C CNN
F 1 "74HC245" H 8400 3900 50  0000 C CNN
F 2 "Package_SO:TSSOP-20_4.4x6.5mm_P0.65mm" H 8200 3250 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74HC245" H 8200 3250 50  0001 C CNN
	1    8200 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 4600 8200 4600
Connection ~ 8200 4600
Wire Wire Line
	8100 2450 8200 2450
Connection ~ 8200 2450
$Comp
L power:GND #PWR0119
U 1 1 5FB8025B
P 7700 3750
F 0 "#PWR0119" H 7700 3500 50  0001 C CNN
F 1 "GND" H 7705 3577 50  0000 C CNN
F 2 "" H 7700 3750 50  0001 C CNN
F 3 "" H 7700 3750 50  0001 C CNN
	1    7700 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0121
U 1 1 5FB80DA4
P 7800 2450
F 0 "#PWR0121" H 7800 2200 50  0001 C CNN
F 1 "GND" H 7805 2277 50  0000 C CNN
F 2 "" H 7800 2450 50  0001 C CNN
F 3 "" H 7800 2450 50  0001 C CNN
	1    7800 2450
	0    1    1    0   
$EndComp
Wire Wire Line
	10600 3250 10600 4900
Wire Wire Line
	10750 3150 10750 5000
Wire Wire Line
	10900 3050 10900 5100
$Comp
L power:GND #PWR0122
U 1 1 5FB8B361
P 7800 4600
F 0 "#PWR0122" H 7800 4350 50  0001 C CNN
F 1 "GND" H 7805 4427 50  0000 C CNN
F 2 "" H 7800 4600 50  0001 C CNN
F 3 "" H 7800 4600 50  0001 C CNN
	1    7800 4600
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR04
U 1 1 60472B23
P 1600 5200
F 0 "#PWR04" H 1600 4950 50  0001 C CNN
F 1 "GND" H 1605 5027 50  0000 C CNN
F 2 "" H 1600 5200 50  0001 C CNN
F 3 "" H 1600 5200 50  0001 C CNN
	1    1600 5200
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR01
U 1 1 60474054
P 1200 5500
F 0 "#PWR01" H 1200 5350 50  0001 C CNN
F 1 "+3.3V" H 1215 5673 50  0000 C CNN
F 2 "" H 1200 5500 50  0001 C CNN
F 3 "" H 1200 5500 50  0001 C CNN
	1    1200 5500
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C15
U 1 1 60476314
P 1450 5350
F 0 "C15" H 1550 5350 50  0000 L CNN
F 1 "0.1uf" H 1450 5250 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 1488 5200 50  0001 C CNN
F 3 "~" H 1450 5350 50  0001 C CNN
	1    1450 5350
	1    0    0    -1  
$EndComp
Text Label 1300 5100 0    50   ~ 0
IR_SIGNAL
Text Label 3600 3950 0    50   ~ 0
IR_SIGNAL
$Comp
L Device:R R14
U 1 1 60480409
P 1950 5350
F 0 "R14" H 2020 5396 50  0000 L CNN
F 1 "10K" H 2020 5305 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 1880 5350 50  0001 C CNN
F 3 "~" H 1950 5350 50  0001 C CNN
	1    1950 5350
	1    0    0    -1  
$EndComp
$Sheet
S 1050 5900 1250 650 
U 5F4ECFED
F0 "Sheet5F4ECFED" 50
F1 "power.sch" 50
F2 "CHIP_IO0" O R 2300 6350 50 
F3 "CHIP_EN" O R 2300 6250 50 
F4 "TXD" O R 2300 6150 50 
F5 "RXD" I R 2300 6050 50 
$EndSheet
Wire Wire Line
	2300 6350 2450 6350
Wire Wire Line
	2300 6250 2450 6250
Wire Wire Line
	2300 6150 2450 6150
Wire Wire Line
	2300 6050 2450 6050
Text Label 2450 6050 0    50   ~ 0
TXD0
Text Label 2450 6250 0    50   ~ 0
EN
Text Label 2450 6350 0    50   ~ 0
BOOT
Text Label 2450 6150 0    50   ~ 0
RXD0
$Comp
L mylib:VS1838 U8
U 1 1 60468EC3
P 1100 5200
F 0 "U8" H 1033 5525 50  0000 C CNN
F 1 "VS1838" H 1033 5434 50  0000 C CNN
F 2 "esp32:Infrared_Receiver_module" H 1100 5450 50  0001 C CNN
F 3 "https://www.dzsc.com/uploadfile/company/244100/201198153410230.pdf" H 1100 5450 50  0001 C CNN
	1    1100 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 5300 1200 5500
Wire Wire Line
	1200 5500 1450 5500
Wire Wire Line
	1200 5200 1450 5200
Connection ~ 1450 5200
Wire Wire Line
	1450 5200 1600 5200
Wire Wire Line
	1200 5100 1950 5100
Wire Wire Line
	1950 5100 1950 5200
Wire Wire Line
	1450 5500 1950 5500
Connection ~ 1450 5500
Connection ~ 1200 5500
Text Label 3600 2250 0    50   ~ 0
SCL
Text Label 3600 2350 0    50   ~ 0
SDA
Text Label 5850 3650 0    50   ~ 0
SCL
Text Label 5850 3550 0    50   ~ 0
SDA
Wire Wire Line
	5750 3550 5850 3550
Wire Wire Line
	5850 3650 5750 3650
Wire Wire Line
	5450 2250 3450 2250
Connection ~ 5450 2250
Wire Wire Line
	3450 2350 5750 2350
Connection ~ 5750 2350
Wire Wire Line
	5750 2350 5750 2450
Wire Wire Line
	3450 3950 3600 3950
$EndSCHEMATC
