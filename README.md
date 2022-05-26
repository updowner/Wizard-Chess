# Wizard-Chess
inspired by Harry potter Chess
I like playing chess game and up to now I played a lot on the Internet or with different Apps. But I was frustrated to pass all this time behind a screen.

I started to look into the electronic chess games, but I rapidly realize that the best choice was to do it by myself. I took time to learn about AI, game engine, chess pieces detection to finally design my own chess game.
So, this  describes an automated chessboard.
The automated chess board is composed of an XY table with an electromagnet on the moving trolley. The chessboard is placed above the XY table and the chess pieces are equipped with magnets. When the electromagnet is below the chess piece a magnetic contact can be generated through the chessboard, so that the chess piece follows the trolley when it moves.
8x8 led matrix interface to show where is the pieces. 
- Programming Language : Arduino C/C++
- Powering : 9V / 2A (2 Lithume battary)
- The mechanical part is not complicated I used very simple way to make it (it's not the best way but the cheapest way).
![ezgif com-gif-maker (1)](https://user-images.githubusercontent.com/29107541/170455054-b75d02bb-857a-4295-84c1-5f220be638cf.gif)

Localization of chess pieces :
The chess pieces localization is essential. The objective is to inform the software of the displacement of the human player chess pieces.

Magnetic sensors are placed below each chess piece square. So, when the magnet of the chess piece is above the sensor, the sensor will be activated. By comparing the activated sensors before and after the move, the chess piece displacement can be determined.

The magnetic sensor or Reed switch is composed of a hermetic glass envelope with two flexible metal contacts inside. The contact is normally open when there is no magnetic field and close if a magnetic field is present.
![Magnetic-reed-switch-Above-Closed-and-open-reed-switch-in-response-to-magnet-placement](https://user-images.githubusercontent.com/29107541/170457158-c234c664-45f5-4af0-bf44-d37fe7fa21c8.png)

The AI proposed by this Instructable is based on an existing one. There are several efficient AI chess programs available on the internet.

For this project, the Micro Max chess program from H.G. Muller and ported for Arduino by [Diego Cueva]{https://create.arduino.cc/projecthub/rom3/arduino-uno-micromax-chess-030d7c} was preferred. It is a very smart, efficient and it is particularly small (<2kB).

The other main advantage is this program being an open source, so we can freely play with it, special THANKS to H.G. Muller & Diego Cueva.
