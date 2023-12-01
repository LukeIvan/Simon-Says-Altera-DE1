 
 # Game of Simon Says being written for use on the Altera Nios II De1 board.

-> Played by using 4 push buttons on the physical board, the game generates
a randomized pattern using timer interrupts. The user must repeat the pattern back 
to the board using the appropriate push buttons, with each button corresponding to an LED.
The game runs in cycles, where the pattern is flashed to the user, and the user repeats the pattern. 
After each cycle, the pattern increases in length, which can proceed up to a maximum of 50 "flashes", and 4^50 total possible patterns.
