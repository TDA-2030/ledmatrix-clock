EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
Title "power"
Date "2020-09-01"
Rev "V1.0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:GND #PWR?
U 1 1 5F232324
P 1250 3550
AR Path="/5F232324" Ref="#PWR?"  Part="1" 
AR Path="/5F2186C0/5F232324" Ref="#PWR?"  Part="1" 
AR Path="/5F4ECFED/5F232324" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1250 3300 50  0001 C CNN
F 1 "GND" H 1255 3377 50  0000 C CNN
F 2 "" H 1250 3550 50  0001 C CNN
F 3 "" H 1250 3550 50  0001 C CNN
	1    1250 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 2050 2250 2050
Connection ~ 1250 3550
$Comp
L Connector:USB_C_Receptacle_USB2.0 J?
U 1 1 5F4D6B71
P 1550 2650
F 0 "J?" H 1657 3517 50  0000 C CNN
F 1 "USB_C_Receptacle_USB2.0" H 1657 3426 50  0000 C CNN
F 2 "Connector_USB:USB_C_Receptacle_HRO_TYPE-C-31-M-12" H 1700 2650 50  0001 C CNN
F 3 "https://www.usb.org/sites/default/files/documents/usb_type-c.zip" H 1700 2650 50  0001 C CNN
	1    1550 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1250 3550 1550 3550
NoConn ~ 2150 2250
NoConn ~ 2150 2350
NoConn ~ 2150 3150
NoConn ~ 2150 3250
$Comp
L Regulator_Linear:AMS1117-3.3 U?
U 1 1 5F24A068
P 3950 5550
AR Path="/5F24A068" Ref="U?"  Part="1" 
AR Path="/5F2186C0/5F24A068" Ref="U?"  Part="1" 
AR Path="/5F4ECFED/5F24A068" Ref="U?"  Part="1" 
F 0 "U?" H 3950 5792 50  0000 C CNN
F 1 "AMS1117-3.3" H 3950 5701 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 3950 5750 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 4050 5300 50  0001 C CNN
	1    3950 5550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5F24A06E
P 4650 5550
AR Path="/5F24A06E" Ref="#PWR?"  Part="1" 
AR Path="/5F2186C0/5F24A06E" Ref="#PWR?"  Part="1" 
AR Path="/5F4ECFED/5F24A06E" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4650 5400 50  0001 C CNN
F 1 "+3.3V" V 4650 5800 50  0000 C CNN
F 2 "" H 4650 5550 50  0001 C CNN
F 3 "" H 4650 5550 50  0001 C CNN
	1    4650 5550
	0    1    1    0   
$EndComp
Wire Wire Line
	4250 5550 4450 5550
$Comp
L power:GND #PWR?
U 1 1 5F24A075
P 3950 5850
AR Path="/5F24A075" Ref="#PWR?"  Part="1" 
AR Path="/5F2186C0/5F24A075" Ref="#PWR?"  Part="1" 
AR Path="/5F4ECFED/5F24A075" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3950 5600 50  0001 C CNN
F 1 "GND" H 3955 5677 50  0000 C CNN
F 2 "" H 3950 5850 50  0001 C CNN
F 3 "" H 3950 5850 50  0001 C CNN
	1    3950 5850
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F24A081
P 3500 5700
AR Path="/5F24A081" Ref="C?"  Part="1" 
AR Path="/5F2186C0/5F24A081" Ref="C?"  Part="1" 
AR Path="/5F4ECFED/5F24A081" Ref="C?"  Part="1" 
F 0 "C?" H 3615 5746 50  0000 L CNN
F 1 "10uf" H 3615 5655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3538 5550 50  0001 C CNN
F 3 "~" H 3500 5700 50  0001 C CNN
	1    3500 5700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F24A087
P 4450 5700
AR Path="/5F24A087" Ref="C?"  Part="1" 
AR Path="/5F2186C0/5F24A087" Ref="C?"  Part="1" 
AR Path="/5F4ECFED/5F24A087" Ref="C?"  Part="1" 
F 0 "C?" H 4565 5746 50  0000 L CNN
F 1 "10uf" H 4565 5655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4488 5550 50  0001 C CNN
F 3 "~" H 4450 5700 50  0001 C CNN
	1    4450 5700
	1    0    0    -1  
$EndComp
Connection ~ 4450 5550
Connection ~ 3950 5850
Wire Wire Line
	3650 5550 3500 5550
Wire Wire Line
	3950 5850 4450 5850
Wire Wire Line
	4450 5550 4650 5550
Wire Wire Line
	3500 5850 3950 5850
$Comp
L Interface_USB:CP2102N-A01-GQFN28 U?
U 1 1 5F567655
P 5150 3250
F 0 "U?" H 4800 1950 50  0000 C CNN
F 1 "CP2102N-A01-GQFN28" H 5700 1950 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-28-1EP_5x5mm_P0.5mm_EP3.35x3.35mm" H 5600 2050 50  0001 L CNN
F 3 "https://www.silabs.com/documents/public/data-sheets/cp2102n-datasheet.pdf" H 5200 2500 50  0001 C CNN
	1    5150 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5F56C1C8
P 4450 2100
F 0 "R?" H 4350 2050 50  0000 C CNN
F 1 "10K" H 4350 1950 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 4380 2100 50  0001 C CNN
F 3 "~" H 4450 2100 50  0001 C CNN
	1    4450 2100
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5F5762E2
P 6600 2650
F 0 "R?" V 6393 2650 50  0000 C CNN
F 1 "10K" V 6484 2650 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 6530 2650 50  0001 C CNN
F 3 "~" H 6600 2650 50  0001 C CNN
	1    6600 2650
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5F57697D
P 6600 3250
F 0 "R?" V 6393 3250 50  0000 C CNN
F 1 "10K" V 6484 3250 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 6530 3250 50  0001 C CNN
F 3 "~" H 6600 3250 50  0001 C CNN
	1    6600 3250
	0    1    1    0   
$EndComp
Wire Wire Line
	6450 2650 6450 2950
Wire Wire Line
	6450 2950 7050 2950
Wire Wire Line
	7050 2950 7050 3050
Wire Wire Line
	6400 2850 6400 3250
Wire Wire Line
	6400 3250 6450 3250
Wire Wire Line
	6400 2850 7050 2850
Text HLabel 7350 3450 2    50   Output ~ 0
CHIP_IO0
Text HLabel 7350 2450 2    50   Output ~ 0
CHIP_EN
Wire Wire Line
	7050 2450 7350 2450
Connection ~ 6400 3250
Connection ~ 6450 2650
Text HLabel 5900 2450 2    50   Output ~ 0
TXD
Text HLabel 5900 2350 2    50   Input ~ 0
RXD
Wire Wire Line
	5650 2350 5900 2350
Wire Wire Line
	5650 2450 5900 2450
Wire Wire Line
	5650 2650 6450 2650
Wire Wire Line
	5650 2250 6200 2250
Wire Wire Line
	6200 2250 6200 3250
Wire Wire Line
	6200 3250 6400 3250
Wire Wire Line
	5150 1950 5150 1900
Wire Wire Line
	5150 1700 4450 1700
Wire Wire Line
	4450 1700 4450 1950
Wire Wire Line
	4450 2250 4450 2350
Wire Wire Line
	4450 2350 4650 2350
$Comp
L power:GND #PWR?
U 1 1 5F5A892F
P 5150 4550
AR Path="/5F5A892F" Ref="#PWR?"  Part="1" 
AR Path="/5F2186C0/5F5A892F" Ref="#PWR?"  Part="1" 
AR Path="/5F4ECFED/5F5A892F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5150 4300 50  0001 C CNN
F 1 "GND" H 5155 4377 50  0000 C CNN
F 2 "" H 5150 4550 50  0001 C CNN
F 3 "" H 5150 4550 50  0001 C CNN
	1    5150 4550
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F5A9CF0
P 4250 2400
AR Path="/5F5A9CF0" Ref="C?"  Part="1" 
AR Path="/5F2186C0/5F5A9CF0" Ref="C?"  Part="1" 
AR Path="/5F4ECFED/5F5A9CF0" Ref="C?"  Part="1" 
F 0 "C?" H 4400 2250 50  0000 C CNN
F 1 "1uf" H 4400 2350 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4288 2250 50  0001 C CNN
F 3 "~" H 4250 2400 50  0001 C CNN
	1    4250 2400
	-1   0    0    1   
$EndComp
$Comp
L Device:C C?
U 1 1 5F5AB474
P 5300 1900
AR Path="/5F5AB474" Ref="C?"  Part="1" 
AR Path="/5F2186C0/5F5AB474" Ref="C?"  Part="1" 
AR Path="/5F4ECFED/5F5AB474" Ref="C?"  Part="1" 
F 0 "C?" V 5250 2000 50  0000 L CNN
F 1 "0.1uf" V 5150 1850 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 5338 1750 50  0001 C CNN
F 3 "~" H 5300 1900 50  0001 C CNN
	1    5300 1900
	0    1    1    0   
$EndComp
Connection ~ 5150 1900
Wire Wire Line
	5150 1900 5150 1700
$Comp
L power:GND #PWR?
U 1 1 5F5ADB3F
P 5450 1900
AR Path="/5F5ADB3F" Ref="#PWR?"  Part="1" 
AR Path="/5F2186C0/5F5ADB3F" Ref="#PWR?"  Part="1" 
AR Path="/5F4ECFED/5F5ADB3F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5450 1650 50  0001 C CNN
F 1 "GND" H 5455 1727 50  0000 C CNN
F 2 "" H 5450 1900 50  0001 C CNN
F 3 "" H 5450 1900 50  0001 C CNN
	1    5450 1900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4050 2550 4250 2550
Connection ~ 4250 2550
Wire Wire Line
	4250 2550 4650 2550
$Comp
L power:GND #PWR?
U 1 1 5F5B107E
P 4250 2250
AR Path="/5F5B107E" Ref="#PWR?"  Part="1" 
AR Path="/5F2186C0/5F5B107E" Ref="#PWR?"  Part="1" 
AR Path="/5F4ECFED/5F5B107E" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4250 2000 50  0001 C CNN
F 1 "GND" H 4255 2077 50  0000 C CNN
F 2 "" H 4250 2250 50  0001 C CNN
F 3 "" H 4250 2250 50  0001 C CNN
	1    4250 2250
	-1   0    0    1   
$EndComp
Wire Wire Line
	2150 2750 2150 2850
Wire Wire Line
	2150 2650 2150 2550
$Comp
L Device:R R?
U 1 1 5F524A26
P 3000 2450
F 0 "R?" V 3100 2550 50  0000 C CNN
F 1 "22" V 3100 2400 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 2930 2450 50  0001 C CNN
F 3 "~" H 3000 2450 50  0001 C CNN
	1    3000 2450
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 5F525525
P 3000 2650
F 0 "R?" V 3100 2750 50  0000 C CNN
F 1 "22" V 3100 2600 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 2930 2650 50  0001 C CNN
F 3 "~" H 3000 2650 50  0001 C CNN
	1    3000 2650
	0    -1   -1   0   
$EndComp
Connection ~ 2150 2550
Wire Wire Line
	2150 2750 2750 2750
Connection ~ 2150 2750
NoConn ~ 5650 2150
NoConn ~ 5650 2550
NoConn ~ 5650 2750
NoConn ~ 5650 2850
NoConn ~ 5650 3050
NoConn ~ 5650 3150
NoConn ~ 5650 3550
NoConn ~ 5650 3450
NoConn ~ 5650 3350
NoConn ~ 5650 3750
NoConn ~ 5650 3850
NoConn ~ 5650 3950
NoConn ~ 5650 4050
NoConn ~ 5650 4150
NoConn ~ 5650 4250
NoConn ~ 5650 4350
Wire Wire Line
	2150 2550 2650 2550
Wire Wire Line
	3450 2750 4650 2750
Wire Wire Line
	3150 2650 4650 2650
Wire Wire Line
	3450 2450 3150 2450
Wire Wire Line
	3450 2450 3450 2750
Wire Wire Line
	2850 2450 2650 2450
Wire Wire Line
	2650 2450 2650 2550
Wire Wire Line
	2850 2650 2750 2650
Wire Wire Line
	2750 2650 2750 2750
$Comp
L Transistor_BJT:MMBT5551L Q?
U 1 1 5FA32740
P 6950 2650
F 0 "Q?" H 7141 2696 50  0000 L CNN
F 1 "MMBT5551L" H 7141 2605 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 7150 2575 50  0001 L CIN
F 3 "www.onsemi.com/pub/Collateral/MMBT5550LT1-D.PDF" H 6950 2650 50  0001 L CNN
	1    6950 2650
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:MMBT5551L Q?
U 1 1 5FA3811B
P 6950 3250
F 0 "Q?" H 7141 3204 50  0000 L CNN
F 1 "MMBT5551L" H 7141 3295 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 7150 3175 50  0001 L CIN
F 3 "www.onsemi.com/pub/Collateral/MMBT5550LT1-D.PDF" H 6950 3250 50  0001 L CNN
	1    6950 3250
	1    0    0    1   
$EndComp
Wire Wire Line
	7050 3450 7350 3450
$Comp
L power:VBUS #PWR?
U 1 1 5FA3C304
P 2250 2050
F 0 "#PWR?" H 2250 1900 50  0001 C CNN
F 1 "VBUS" V 2265 2178 50  0000 L CNN
F 2 "" H 2250 2050 50  0001 C CNN
F 3 "" H 2250 2050 50  0001 C CNN
	1    2250 2050
	0    1    1    0   
$EndComp
$Comp
L power:VBUS #PWR?
U 1 1 5FA3C75A
P 4050 2550
F 0 "#PWR?" H 4050 2400 50  0001 C CNN
F 1 "VBUS" V 4065 2677 50  0000 L CNN
F 2 "" H 4050 2550 50  0001 C CNN
F 3 "" H 4050 2550 50  0001 C CNN
	1    4050 2550
	0    -1   -1   0   
$EndComp
$Comp
L power:VBUS #PWR?
U 1 1 5FA3C991
P 5050 1950
F 0 "#PWR?" H 5050 1800 50  0001 C CNN
F 1 "VBUS" H 5065 2123 50  0000 C CNN
F 2 "" H 5050 1950 50  0001 C CNN
F 3 "" H 5050 1950 50  0001 C CNN
	1    5050 1950
	1    0    0    -1  
$EndComp
$Comp
L Diode:1N4148WS D?
U 1 1 5FA51EAF
P 3100 5150
F 0 "D?" H 3100 4933 50  0000 C CNN
F 1 "1N4148WS" H 3100 5024 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-323" H 3100 4975 50  0001 C CNN
F 3 "https://www.vishay.com/docs/85751/1n4148ws.pdf" H 3100 5150 50  0001 C CNN
	1    3100 5150
	-1   0    0    1   
$EndComp
$Comp
L Diode:1N4148WS D?
U 1 1 5FA531EA
P 3100 5550
F 0 "D?" H 3200 5600 50  0000 C CNN
F 1 "1N4148WS" H 3100 5700 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-323" H 3100 5375 50  0001 C CNN
F 3 "https://www.vishay.com/docs/85751/1n4148ws.pdf" H 3100 5550 50  0001 C CNN
	1    3100 5550
	-1   0    0    1   
$EndComp
Wire Wire Line
	3250 5150 3500 5150
Wire Wire Line
	3500 5150 3500 5550
Connection ~ 3500 5550
Wire Wire Line
	3250 5550 3500 5550
$Comp
L power:VBUS #PWR?
U 1 1 5FA553E7
P 2600 5150
F 0 "#PWR?" H 2600 5000 50  0001 C CNN
F 1 "VBUS" V 2615 5277 50  0000 L CNN
F 2 "" H 2600 5150 50  0001 C CNN
F 3 "" H 2600 5150 50  0001 C CNN
	1    2600 5150
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5FA5674E
P 2600 5550
F 0 "#PWR?" H 2600 5400 50  0001 C CNN
F 1 "+5V" V 2615 5678 50  0000 L CNN
F 2 "" H 2600 5550 50  0001 C CNN
F 3 "" H 2600 5550 50  0001 C CNN
	1    2600 5550
	0    -1   -1   0   
$EndComp
$Comp
L Diode:1N4148WS D?
U 1 1 5FA573BE
P 2800 5300
F 0 "D?" V 2891 5220 50  0000 R CNN
F 1 "1N4148WS" V 2800 5220 50  0000 R CNN
F 2 "Diode_SMD:D_SOD-323" H 2800 5125 50  0001 C CNN
F 3 "https://www.vishay.com/docs/85751/1n4148ws.pdf" H 2800 5300 50  0001 C CNN
F 4 "NC" V 2700 5200 50  0000 R CNN "Field4"
	1    2800 5300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2600 5150 2800 5150
Connection ~ 2800 5150
Wire Wire Line
	2800 5150 2950 5150
Wire Wire Line
	2600 5550 2800 5550
Wire Wire Line
	2800 5450 2800 5550
Connection ~ 2800 5550
Wire Wire Line
	2800 5550 2950 5550
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 5FA635CD
P 1250 4750
F 0 "J?" H 1168 4425 50  0000 C CNN
F 1 "Conn_01x02" H 1168 4516 50  0000 C CNN
F 2 "" H 1250 4750 50  0001 C CNN
F 3 "~" H 1250 4750 50  0001 C CNN
	1    1250 4750
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5FA64DD9
P 1450 4650
F 0 "#PWR?" H 1450 4500 50  0001 C CNN
F 1 "+5V" V 1465 4778 50  0000 L CNN
F 2 "" H 1450 4650 50  0001 C CNN
F 3 "" H 1450 4650 50  0001 C CNN
	1    1450 4650
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FA65831
P 1450 4750
AR Path="/5FA65831" Ref="#PWR?"  Part="1" 
AR Path="/5F2186C0/5FA65831" Ref="#PWR?"  Part="1" 
AR Path="/5F4ECFED/5FA65831" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1450 4500 50  0001 C CNN
F 1 "GND" H 1455 4577 50  0000 C CNN
F 2 "" H 1450 4750 50  0001 C CNN
F 3 "" H 1450 4750 50  0001 C CNN
	1    1450 4750
	0    -1   -1   0   
$EndComp
$EndSCHEMATC
