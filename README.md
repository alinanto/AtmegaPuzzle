# AtmegaPuzzle
Puzzle based on Apollo 13 moon mission implemented on Atmega 32 microcontroller

## External dependencies
The work uses 2 external libraries for LCD and UART interface provided by Peter Fleury.
These following files mentioned below are used as such from his library.  
uart.h  
uart.c  
These following files mentioned below are used with slight improvements from his library.  
lcd.h  
lcd.c  
The original version of his work can be obtained from http://www.peterfleury.epizy.com/

## Pin mappings  
The pin mappings for the hardware are mentioned in the header files for the same.

## Puzzle : Situation Apollo 13
You are the chief engineer for moon mission Apollo 13. Half way through the
journey a flow valve malfunctions and two of four oxygen tanks exploded, damaging
vital components including power reserve. This forced the crew to abandon the mission
and return to earth. The crew has only 13 amps of continuous power available. For reentry
to Earth's surface, vital components must be loaded. Your task as a chief engineer for this
mission is to arrive at a safe step by step procedure which will enable the crew to return
safely. The procedure should be send via a weak communication link, which supports
only one way communication with no feedback. Therefore the procedure must be tested
ok with a our simulator first. Good Luck!

#### DETAILS OF MODULES THAT ARE AVAILABLE
The vehicle has two modules (Lunar module and command module) each with its own sub
modules. The current demand of each is given in the table below.  

| Sl. no. |     Lunar module     | Current Demand |    Command module    | Current Demand |
|:-------:|:--------------------:|:--------------:|:--------------------:|:--------------:|
|    1    | Communication system |     4 Amps     | Communication system |     5 Amps     |
|    2    |       Thrusters      |     2 Amps     |       Thrusters      |     3 Amps     |
|    3    |       Gyroscope      |     3 Amps     |      Parachutes      |     5 Amps     |
|    4    |        Airlock       |     5 Amps     |  Navigation computer |     6 Amps     |

A description of each module is given in the below table.  

|     **MODULE**     |                                            **DESCRIPTION**                                                                 |
|:------------------:|:---------------------------------------------------------------------------------------------------------------------------|
|Communication system|For communication with Earth station ( At least one of the communication sub-modules should be active at a given time )     |
|     Thrusters      |Must be used for re-entry pitch alignment( Once thrusters are fired, continuous power available reduces to 10A permanently )|
|     Gyroscope      | For the calibration of navigation computer ( Must be used before firing thrusters )                                        |
|     Parachute      | For safe landing. ( Must be enabled as the final step. )                                                                   |
|      Airlock       | Enabling airlock separates Lunar module from the Command module ( Must be used before the parachutes )                     |
|Navigation computer | Controls both thrusters and gyroscope.  ( should be active while toggling thrusters and gyroscope )                        |

#### CONSTRAINTS
*1)Initially the Lunar communication module alone is enabled.*  
*2)A maximum of 13 steps in the procedure.*  
*3)Maximum power usage should not exceed 13 amps.*  
*4)At least one of the communication modules should be active at a given time.*  
*5)Enabling of thrusters is mandatory for reentry pitch alignment.*  
*6)Gyroscope must be used for calibrating orientation before enabling the thrusters.*  
*7)Navigation computer controls the thrusters and the gyroscope. It must be active for turning on and off both thrusters and gyroscope.*  
*8)Once the thruster is enabled, the continuous usage limit decreases from 13 amps to 10 amps (permanently).*  
*9)Before the parachutes are enabled the airlock must be activated to separate and abandon Lunar module.*  
*10)Once Lunar module is disabled, no lunar- sub-modules can be loaded.*  
*11)Parachute must be enabled at the end only.*  

#### PROCEDURE SYNTAX
A statement consists of 3 letters.  
First letter is either E or D. (i.e To Enable or To Disable)  
Second letter is either L or C.(L for Lunar module, C for Command module)  
Last letter is a digit which represents the serial number of sub modules.  

###### for example:-
**EL1**- *Enable Lunar Communication module*   
**DL1**- *Disable Lunar Communication module*  
**EC4**- *Enable navigation computer.(in Command module)*
