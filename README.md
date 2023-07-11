# Automated Sprint Timing Gate 
Automated sprint time measuring device with ultrasonic sensors to take the time of two sprinters between the start signal and the sprinters crossing the finish line.


Two devices are necessary, one at the start, one at the finish line, each of them is based on a Raspberry Pi (Model B).
| | | 
|:-------------------------:|:-------------------------:|
|<img width="400" alt="Startgerät 1" src="https://github.com/tobiasjungmann/WSeminar2017/assets/32565407/1da77940-a7b5-4b2d-b995-a20faaefe01d">|<img width="400" alt="Startgerät 1" src="https://github.com/tobiasjungmann/WSeminar2017/assets/32565407/04ce93fc-d4f2-4e1e-aa8f-6dad1dbfa078">|
|<img width="400" alt="Startgerät 1" src="https://github.com/tobiasjungmann/WSeminar2017/assets/32565407/079cf8fa-892a-4edf-8950-c635095e30db">|<img width="400" alt="Startgerät 1" src="https://github.com/tobiasjungmann/WSeminar2017/assets/32565407/5474534b-e127-4a6f-a58f-77ebf1572866">|



Runners are not detected by a laser as in similar devices, but by two ultrasonic sensors (HC-SR04). Therefore, no additional reflectors are required for the gate, which can be knocked over. The most complex part of the project was to read out the ultrasonic sensors quickly enough to reliably detect runners. Therefore, a complex state machine is used since both sensors must work independently. In hindsight, separate threads would have been the better solution. 
|||
|-|-|
|<img width="1000" alt="Startgerät 1" src="https://github.com/tobiasjungmann/WSeminar2017/assets/32565407/cdadb09f-734c-4b6a-95f6-25e306739842">|<img width="800" alt="Startgerät 1" src="https://github.com/tobiasjungmann/WSeminar2017/assets/32565407/d0224d8b-7c9f-4282-b568-bf41964fa59c">|
|Both communicate via 433Mhz antennas over a maximum distance of 100 meters. To reach this distance bipole antennas are used and the module is connected to a 9V batterie instead of teh 5V provided by the Raspberry Pi. |<img width="800" alt="Startgerät 1" src="https://github.com/tobiasjungmann/WSeminar2017/assets/32565407/1eec5405-8811-464c-a745-f0da48b06870">



This project was created as part of a school project (W-Seminar) 2017 in my eleventh school year.
Parts of the code are borderline awful, because I have implemented it without little to no prior knowledge. Since the final result is fully functional and it is a nice project in my opinion, I put it here.

Both, the 433Mhz communication and the handling of the ultrasonic sensors, is done without external libraries.

[Wiring Pi](http://wiringpi.com/) is used to read and write GPIOs and display characters on the LCD display.

[mpg123](https://www.mpg123.de/) used to play the start signal. 
