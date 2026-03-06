
### 10.01.2026

``
When writing the winning logic, at first I thought of taking the main field and making arrays for the tic-tac-toe separately. And then compare the winning combinations. After consulting with Chat GPT, He proposed a more optimized and simpler solution — to take the values from the main playing field according to the indexes of each winning combination and compare them, if they match, victory. After analyzing it, I decided that the logic is indeed more correct, since it is not worth creating new arrays for such a simple system as tic-tac-toe. Well, then I just implemented the usual logic to identify the winner, draw or end the game.
``


---


### 11.01.2026

``
In fact, the main logic of the game is written, there are small edits and the correct order of function calls. And also the output of all this to the matrix. There is still no understanding of how the matrix works, what indexes, etc.. I am waiting for the delivery of the 16×16 RGB matrix, first I will play with it (I light up individual pins), I will begin to feel its work, then, when I reach an understanding of the logic of interaction, I will integrate it into the project by writing code corresponding to the work of the matrix
``


---


### 15.01.2026

``
The implementation of the main class is already ready, it remains to polish the points and move on to the RGB class, but it is more like the front end of the project, it reacts to a change in the main class — the game class (Field). Due to the heterogeneity of the system, I'm starting to get a little lost in the structure and decomposition of the project, as I keep everything in my head. It is a good idea to write blocks with the project structure in diagrams.net I think the correct structure for constructing the decomposition scheme will be the following:
``

``
-- we are thinking about what tasks the project has, if we take the example of the current tic-tac-toe, then these are: the implementation of the game itself, which would be similar to tic-tac-toe in the web version in Python, but Arduino-oriented; the implementation of user input; the implementation of output.
``

``  
-- what are the points of each task? If we take the rules of the tic-tac-toe game, then these are: the beginning of the game, the moves, alternately, the end of the game (win or draw).
``

``
-- each item has characteristics, for example: the beginning of the game is an empty field, the cursor is on [0][0] in the field array, we
can put it in any position as the initial one, and there is also a starting player (in our case: a tic-tac-toe). The end of the game is a victory or the field is filled (a draw), to fix the victory (hence the dependence in the RGB class, since the fixation occurs exclusively for the player, and not for the correct functionality of the system). And so on.
``

``
Next, it remains to correctly implement the code for each characteristic of each item of each task.
``

``
That's actually how I wrote the code, but it was all in my head. When more than 100 lines were written, I already began to forget what I wanted to realize, ideas appeared and went out like supernova explosions. I started writing notes in the code and came to the conclusion that implementing a decomposition scheme is a very good idea. To the next project (like tamagotchi) this will be mandatory due to the multitasking nature of the code.
``

``
And it's also worth thinking about soldering, since now every project of mine is based on a solderless breadboard and it's inconvenient. Extremely inconvenient.
``

---


### 17.01.2026

``
Yesterday, an RGB matrix arrived at my place, I thought that I would power it directly from arduino. But realizing that there was not enough power and voltage, I came to the conclusion that an additional power source was inevitable. The original idea was to work with the power supply. That is, to strip the wires from the outlet, connect them to the power supply, on the other hand RGB. But it looked very strange and dangerous. Hey, we're doing a tic-tac-toe project! There were thoughts to add videos to the repository, as my friend and I play tic-tac-toe. And the exposed parts, even if they are protected by thermal pads, are more dangerous than the level of a child's toy. Plus, I've never worked with 220V and it would be dangerous for me too. By studying different methods, I decided to work with a 5V and 8A power supply. This is enough for RGB and safe for both the project and me. When using a project, I don't want to think about where I might get hit and where I might not. And the real architecture implies complete security.
``

---


### 09.02.2026

``
I don't have much time for Arduino. Nevertheless, I dedicate a lot of myself to him. The code has changed quite a lot over the last month. I came to the point that in addition to the main class with the logic of the virtual game and in addition to the display class on the matrix, a controller class is also needed, which will structure the methods that will be called from loop() in the right order.
The RGB class has an XY function that translates the XY coordinates into an index in the table. I was very pleased with myself when I wrote the formula for displaying it on the matrix. Although I deduced the formula without any prompting, I didn't fully understand why it worked hahaha, somehow it turned out intuitively. After the output to the real matrix, I discovered that I initially misunderstood the matrix indexes and the snake of LEDs goes in the other direction and the indexes correspondingly... Yes, I magically rewrote the formula and it turned out to be working, although I don't fully understand how exactly. Yes, I would never have thought that writing a children's game was not so easy.
``


---



### 07.03.2026

``
Finally I reached the final stage — the code is completely written.
The main difficulty turned out to be calling the functions in the correct way.
At different moments ideas for functions came to me in the format of:
“oh, actually, I could write a function that would act as a connecting link”,
and I wrote them in Telegram IDE haha.
Later I edited the format and rewrote everything in Arduino IDE. Since I didn’t build a logical scheme of the project from the beginning and kept everything in my head, I got confused about what exactly should be called and in what order.
Eventually I finished writing the necessary functions.
Yes, in terms of style and code cleanliness it’s not perfect, but keeping the logic of ~500 lines of code entirely in your head is honestly not the best idea.
In the next projects, one of the first steps will be to describe the project logic in words.
``

``
And then, at 2:20 AM, I launched the project.
Three bugs appeared:
the cursor flies around (reading delays are needed)
the button is read automatically (I’ll try to fix it by reversing HIGH/LOW)
the indices of the winning combination are mixed up
``

``
Honestly, I’m shocked that there are so few bugs and that the Arduino didn’t explode from my code the moment I first connected it to power hahaha.
Probably that will happen in the future…
``