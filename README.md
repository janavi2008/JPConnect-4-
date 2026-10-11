# JPConnect(4)
a 2-player connect 4 game hardware project from scratch with a minimalist design.

# Design
Uses an arduino nano, 2 MAX7219 driver chips, 2 7x7 led matrices of different colours, one for each player, push buttons for control, some leds to indicate turns and a buzzer for winning sounds. the components are smd for a cleaner look and to save space, except for the nano.

<img width="254" height="386" alt="image" src="https://github.com/user-attachments/assets/94e1e993-d683-491a-b407-bc8e1ba97960" />
<img width="271" height="406" alt="image" src="https://github.com/user-attachments/assets/d4e42ad3-9957-4b6d-8730-0dc2ac7f27b4" />

# GamePlay
its played like a regular connect 4 game, the actual game is a 6x7 grid and there is one row for the "cursor", which blinks over the selected column. there are three push buttons on each side of the grid, one set for each player (left, right, and confirm). the entire game display flips to face the player whose turn it is. 
