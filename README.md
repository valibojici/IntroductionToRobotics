# Introduction To Robotics
This repository will contain homework for the 'Introduction To Robotics' course.

<details><summary>

## Homework #0
</summary>
Create this repository and readme file.
</details>

<details><summary>

## Homework #1 (control a RGB LED)
</summary>

#### Technical Task
Use a separate potentiometer in controlling each of the color of the RGB led (Red,Green and Blue).  The control must be done with digital electronics (you must read the value of the potentiometer with Arduino, and write a mapped value to each of the pins connected to the led.
#### Components
- RBG  LED  (1  minimum)
- potentiometers  (3  minimum)
- resistors and wires (per logic)
#### Solution
The code is in [Homework#1](/Homework%231.ino) file.
![20221023_125728](https://user-images.githubusercontent.com/68808448/197386955-a1887642-728a-4b61-8830-561f313b14e1.jpg)
#### Demo
A short working demo video can be viewed [here](https://youtu.be/DBqDh8HWV-k).
</details>

<details><summary>

## Homework #2 (traffic lights system)
</summary>

#### Technical Task
The task is building the traffic lights for a crosswalk. You will use 2 LEDs to represent the traffic lights for people (red and green) and 3 LEDs to represent the traffic lights for cars (red, yellow and green). See the states it needs to go through.
##### The system has the following states:
- **State 1** (default, reinstated after state 4 ends): green light for cars,
red light for people, no sounds. Duration: indefinite, changed by
pressing the button.
- **State 2** (initiated by counting down 8 seconds after a button press):
the light should be yellow for cars, red for people and no sounds.
Duration: 3 seconds.
- **State 3** (initiated after state 2 ends): red for cars, green for people
and a beeping sound from the buzzer at a constant interval. Duration:
8 seconds.
- **State 4** (initiated after state 3 ends): red for cars, blinking green
for people and a beeping sound from the buzzer, at a constant interval,
faster than the beeping in state 3. This state should last 4
seconds.

**Pressing the button in any state other than state 1 should NOT yield any actions.**

#### Components
- 5 LEDs (2 red, 1 yellow, 2 green)
- 1 button
- 1 buzzer
- resistors and wires (per logic)

#### Solution
The code is in [Homework#2](/Homework%232.ino) file.
![20221103_151857](https://user-images.githubusercontent.com/68808448/199736406-39c27415-c604-4474-aefb-6cd1624395b6.jpg)

#### Drawing
![image](https://user-images.githubusercontent.com/68808448/199742042-4492618f-a33f-4384-9c56-389bda4d8e3a.png)


#### Demo
A short working demo video can be viewed [here](https://www.youtube.com/watch?v=LWrGxgL1uvg).
</details>


<details><summary>

## Homework #3 (control 7-segment display)
</summary>

#### Technical Task
The task is to use the joystick to control the position of
the segment and ”draw” on the display. The movement between segments
should be natural (meaning they should jump from the current position
only to neighbors, but without passing through ”walls”.
##### The system has the following states:
- **State 1** (default, but also initiated after a button press in State
2): Current position blinking. Can use the joystick to move from
one position to neighbors. Short pressing the button toggles state 2. Long pressing the button in state 1 resets the entire display by
turning all the segments OFF and moving the current position to the
decimal point.
- **State 2** (initiated after a button press in State 1): The current
segment stops blinking, adopting the state of the segment before
selection (ON or OFF). Toggling the X (or Y, you chose) axis should
change the segment state from ON to OFF or from OFF to ON.
Clicking the joystick should save the segment state and exit back to
state 1.
  
|Current segment| UP | DOWN | LEFT | Right|
|:---:|:---:|:---:|:---:|:---:|
**a** |N/A| g |f |b
**b** | a | g| f| N/A
**c**|g|d |e| dp|
**d**|g| N/A| e| c
**e**|g| d| N/A| c
**f**|a| g| N/A| b
**g**|a| d| N/A| N/A
**dp**|N/A| N/A| c| N/A
  
**Long pressing the button to reset should only be available in State 1.**

 **Joystick movements should be done with toggle.**
  
#### Components
- 1 7-segment display
- 1 joystick
- resistors and wires (per logic)

#### Solution
The code is in [Homework#3](/Homework%233.ino) file.
![20221110_142243](https://user-images.githubusercontent.com/68808448/201099821-1e100155-d85d-4f08-abcf-35ffee772819.jpg)
  
#### Demo
A short working demo video can be viewed [here](https://www.youtube.com/watch?v=WQOvGhVXPyQ).
</details>
