# Introduction to Robotics (2022 - 2023)

#### Necula Narcis
Introduction to Robotics laboratory homeworks, 3rd year at the Faculty of Matematics and Computer Science, section Computer Science at the University of Bucharest. This will be composed by weekly homeworks who would include code, pictures and information about the progress made (in code and phisical progress to the Arduino project).


### 📗Homework 0️⃣
<br>

 > <i>Install Arduino and create this repository.☝️</i> <br>

<details>
<summary><i><b>Setup photos</b></i></summary>
<br> 
 
![alt text](https://i.imgur.com/OvHTXBm.png)
 
<br>
</details>

### 📗Homework 1️⃣
<br>

 > RGB LED
 
<i><b>Task:</b></i> I used a separate potentiometer in controlling each of the color of the RGB led (Red, Green and Blue). The control was done with digital electronics (aka I had to read the value of the potentiometer with Arduino, and write a mapped value to each of the pins connected to the led).


 [<i><b>Code</b></i>](https://github.com/Narcis22/IntroductionToRobotics/blob/main/Homework_1_RGB_LED/Homework_1_RGB_LED.ino)

<details>
 <summary><i><b>Setup photos</b></i></summary>
 </br></br>
 
![Robotics_Homework_1_setup_2](https://user-images.githubusercontent.com/62501946/197394485-7ab89801-adc1-4e15-951d-9787bf98c7f6.jpg)

![Robotics_Homework_1_setup_1](https://user-images.githubusercontent.com/62501946/197394483-a79f9cf6-b6f3-4325-b838-ec25472c7c68.jpg)

</details>

<i><b>Video with working model:</b></i>
[Go to youtube to see video.](https://www.youtube.com/watch?v=gRUL80wEv6I&ab_channel=NarcisNecula)


### 📗Homework 2️⃣
<br>

 > Crosswalk - 🚦🚸
 
 <i><b>Task:</b></i> I created a crosswalk system which acts like a regular one after pressing the pedestrian button. So, after the button is pressed, a timer starts and after eight seconds the lights start turning yellow and then red for the vehicles and green for pedestrians accompanied by a buzzer. After ten seconds of green light for the pedestrians, the light starts blinking and the buzzing intensifies for five seconds after which it turns back red and the traffic light turns green and the whole process can be started again.


 [<i><b>Code</b></i>](https://github.com/Narcis22/IntroductionToRobotics/blob/main/Homework_2_Crosswalk/Homework_2_Crosswalk.ino)

<details>
 
 <summary> <i><b>Setup photos</b></i> </summary>
</br></br>

![Crosswalk_unlit](https://user-images.githubusercontent.com/62501946/198887521-6087d977-56da-496f-adb0-36144fec0ba5.jpg)

![Crosswalk_lit](https://user-images.githubusercontent.com/62501946/198887522-a0d7b4f9-b529-4397-8370-edde4f75c010.jpg)

</details>

<i><b>Video with working model: </b></i>
[Go to youtube to see video.](https://youtu.be/yUhXWoTgpPg)
 
### 📗Homework 3️⃣
<br>

 > Joystick mania 🕹️
 
 I use the joystick to control the position ofthe segment and ”draw” on the display. The movement between segmentsshould be natural (meaning they should jump from the current positiononly to neighbors, but without passing through ”walls”).


 [<i><b>Code</b></i>](https://github.com/Narcis22/IntroductionToRobotics/blob/main/Homework_3_JoystickMania/Homework_3_JoystickMania.ino)


<details>
<summary> <i><b>Setup photos</b></i> </summary>
</br></br>

![Pic_1](https://user-images.githubusercontent.com/62501946/200411177-2550916e-884f-402b-b0b5-5fce9ec33f24.png)
![Pic_2](https://user-images.githubusercontent.com/62501946/200412244-4736085b-715f-4678-b716-88460aae6d05.png)
![Pic_3](https://user-images.githubusercontent.com/62501946/200411191-bdce90f7-25cc-4eee-802b-0bf4156585d7.png)

</details>

<i><b>Video with working model: </b></i>
[Go to youtube to see video.](https://youtu.be/iD5Ntl5qufo)

### 📗Homework 4️⃣
<br>

 > Display madness  ![favicon-16x16](https://user-images.githubusercontent.com/62501946/201535390-70965b26-ee0b-4216-ba4a-523726bd1025.png)![favicon-16x16](https://user-images.githubusercontent.com/62501946/201535393-4b607354-914e-438b-8b5a-fae12af6e266.png)![favicon-16x16](https://user-images.githubusercontent.com/62501946/201535395-d29cf16b-9ce5-4e9e-80cd-d7c00eadc6e8.png)![favicon-16x16](https://user-images.githubusercontent.com/62501946/201535397-81569f3e-619f-4b91-8796-b4355746a06f.png)🕹️

I use the joystick to move through the 4 digit 7 segment displays digits, press the button to lock in on the current digit and use the other axis to increment or decrement the number. Keep the button pressed to reset all the digit values and the current position to the first digit in the first state.
The selected display is indicated by the digit point of the respective display, blinking if the display is not loked in or solid for the loked in mode.

 [<i><b>Code</b></i>](https://github.com/Narcis22/IntroductionToRobotics/blob/main/Homework_4_DisplayMadness/Homework_4_DisplayMadness.ino)


<details>
<summary> <i><b>Setup photos</b></i> </summary>
</br></br>

![setup_pic_1](https://user-images.githubusercontent.com/62501946/201536662-23cc25a2-1231-4a57-97a4-90dda6ee113f.png)

![setup_pic_2](https://user-images.githubusercontent.com/62501946/201536594-4b4f2388-fdff-44bf-bf9c-8d1b94955c0d.png)


</details>

<i><b>Video with working model: </b></i>
[Go to youtube to see video.](https://youtu.be/-RtheO9I-Wk)

### 📗Homework 5️⃣
<br>

 > Snek 🐍🍎

This is a simplified version of the game Snake (the snake doesn't grow when eating apples and the demo stopps at 10 points). This is the first step of making my own game and is here to show the menu control and functionalities.

 [<i><b>Code</b></i>]()


<details>
<summary> <i><b>Setup photos</b></i> </summary>
</br></br>
![20221209_051511](https://user-images.githubusercontent.com/62501946/206619201-626ed73b-bef1-459e-985c-6d0a9dc7f165.jpg)

![20221209_051524](https://user-images.githubusercontent.com/62501946/206619212-25e09526-c25a-4acb-9fa7-b91bd8ba1703.jpg)

![20221209_051551](https://user-images.githubusercontent.com/62501946/206619217-9a94b08c-65b1-4cf6-8e1e-563cbf709c6e.jpg)

![20221209_051616](https://user-images.githubusercontent.com/62501946/206619224-321d1545-9101-47ad-a63f-bdb52db8c63a.jpg)

</details>

<i><b>Video with working model: </b></i>
[Go to youtube to see video.](https://youtu.be/yfGVG3hl76I)
