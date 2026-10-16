# JPConnect(4)
a 2-player connect 4 game hardware project from scratch with a minimalist design. Connect 4 is a fun boardgame that ive played throughout my childhood, and i thought it'd be an interesting challenge to replicate it on a pcb. Since its portable, its a great way to pass the time on trips, or anywhere else.

it operates on a 6x7 (vertical) grid, with one player getting red coins and the other yellow. the players take turns placing a coin, with the objective of getting 4 of their colour in a row (horizontal, vertical and diagonal count)

besides the connect for game, this project has tons of scope for upgrading in the future, without hardware changes - just changing the code can allow a variety of games to be added - X and 0s, snake, pong, etc.

# Design
Uses an arduino nano, 2 MAX7219 driver chips, 2 7x7 led matrices of different colours, one for each player, push buttons for control, some leds to indicate turns and a buzzer for winning sounds. the components are smd for a cleaner look and to save space, except for the nano.

The nano(microcontroller) acts as the brain, taking data from the buttons(input devices), processing it(deciding what to do next), and then sending insstructions to the MAX7219 chips(display drivers), and the buzzer(output device). the MAX7219s are required as we have 98 leds in the matrices, and the nano has only 22 input/output pins. using the MAX7219s allows us to operate all 98 leds using only 3 pins - clock, load, and data in.

im currently planning on using usb as the primary power source, but since the goal is to make it portable, ill be adding a way to power it using batteries once its ready.

<img width="254" height="386" alt="image" src="https://github.com/user-attachments/assets/94e1e993-d683-491a-b407-bc8e1ba97960" />
<img width="271" height="406" alt="image" src="https://github.com/user-attachments/assets/d4e42ad3-9957-4b6d-8730-0dc2ac7f27b4" />

# GamePlay
its played like a regular connect 4 game, the actual game is a 6x7 grid and there is one row for the "cursor", which blinks over the selected column. there are three push buttons on each side of the grid, one set for each player (left, right, and confirm). the entire game display flips to face the player whose turn it is.

# Bill of Materials
-  PCB  x1
-  Arduino Nano  x1
-  MAX7219 IC chip (or MAX7221)  x2
-  female header pins  x30
-  1206 smd LED (red)  x50
-  1206 smd LED (yellow)  x50
-  0805 smd 1k resistor  x2
-  0805 smd 20k resistor  x2
-  0805 smd 10uF capacitor  x2
-  0805 smd 100nF capacitor  x2
-  12mm through hole push button  x6
-  8x10 smd 470uF electrolytic capacitor  x1
-  12x9.5mm active buzzer  x1

