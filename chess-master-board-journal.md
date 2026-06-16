# Chess Master Board — Journal Export

- Exported at: 2026-06-16T14:20:01Z
- Project ID: 2256
- Entries: 53

## Entry 1
- ID: 2718
- Author: ajakovski07
- Created At: 2026-04-18T14:27:07Z

### Content

The start of this project consists of creating the chess board and most importantly deciding every parameter of it. For example what should be the size of the board, finding a color with a high transparency that will help in the future with installing RGB lights bellow the board. Researching popular materials for a chess board is also crucial, something that can be light for transport, durable and high quality is needed for a better consumer experience. The size of the board is 16x16 inches considered to be a medium sized chess board for casual players. Also i have already left some space for the electronics to be built into the chess board which will bring the idea to life. Chess pieces are my next priority for implementing into this project making the 3D process done and then i will focus on the hardware and software of this project. The idea so far is to have the chess board as a base with everything built on it and a chess padding which can be sticked on top of the chess board and cover all of the LED hardware below which is also a great feature enabling to option for the consumer to choose from multiple designs for a chess cover. Everything is made in Onshape web app which was recommended by the hackclub community and overall it is a pleasant working experience for a somewhat experienced 3D creator. Most of theese decisions will probably be modified in the future but you have to start from somewhere in order to make it better in the future :)
Below you can see what is accomplished with the chess board made so far.
![Screenshot_2.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6NTcwMiwicHVyIjoiYmxvYl9pZCJ9fQ==--15fdf4c81f9de49b2d02de937a4ce0949ee5d9c0/Screenshot_2.png)


### Recording Links

- https://lookout.hackclub.com/api/media/cacbee0a-57fd-4316-97a7-535406c0f8bf/video.mp4

## Entry 2
- ID: 2871
- Author: ajakovski07
- Created At: 2026-04-19T13:51:51Z

### Content

Second day of making this project a reality. A couple things have changed starting from the visual charateristic of the chess board. Realised that there is a timer on the side of chess boards which needs to be pressed every single time a move has beed played. That implementation will be made as soon as possible in the 3D onshape file. I've also started making the list of components needed. Had some issues with finding good RGB LEDs that would be strong enough to be placed below the transparent chess padding/mat. Wasnt able to find a prebuild 8x8 matrix RGB LED that is big enough for my chess board so i decided that the best way would be to just build a PCB with 8x8 LEDs that would either have to be connected with the main PCB or create a cable that would be connected internally in the chess board. Well leave the PCB questions for the future. As a former CircuitMaker user i had some issues with KiCad but noticing that there is an immediate Bill of Materials makes it so much better for use. The main implementation that i forgot to mention before in this journal is the OLED screen that would be a perfect integration to the project. As previously mentioned the main idea is to integrate Stockfish into the  chess board as a helping hand while playing/learning chess. So while the LEDs guide you for the best move the OLED will explain why that move is the best. Also it will visualy display the current mode that will be available trough a switch on the side of the board. The main purpose of 2 mode stockfish is for either helping the player while playing or setting the mode in analysis mode whereyou would be able to analyse the game afterwards.
The main goal for start is to create the visual aspect of the project so that i can know what im having as hardware in order to build the software.
Still a long way for making it a reality but only with small steps will it become a success.  
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6NjAwOSwicHVyIjoiYmxvYl9pZCJ9fQ==--407d6d7d458abc331d946515875d853f4134eb75/image.png)


![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6NjAxMCwicHVyIjoiYmxvYl9pZCJ9fQ==--7d499d24c283f58c0ba4abfc5017abf72f562504/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/6856d238-49d3-4d18-a4d2-7d6ba8548191/video.mp4

## Entry 3
- ID: 3086
- Author: ajakovski07
- Created At: 2026-04-20T18:38:04Z

### Content

I've made little upgrades on the 3D version of the chess board and also added the OLED part in Kicad. Kind of a trouble finding the part because i had to include it myself into a KiCad library parts but it got quickly solved thanks to googling it and finding an already made SSD1306 from GitHub (the link will be provided at the end). Also i am preparing the chess board to be compatible with the large PCB that will be mounted from the backproviding the crucial Stockfish animation. Also included 2 push buttons which will act like a timer for both players just like old-school chess 🙂
Here is the github link: https://github.com/pforrmi/KiCad-SSD1306-128x64
--------------------------------------------------------------
I am writing this journal from 2 separate works at once just because they create more than 60 minutes togheter.
In the second part i researched a crucial part for my realisation of this project with the hardware part.
The thing that i researched in the second timelapse was finding a way to register a piece on the board.
At first i tought a magnetic recognition and finding it's location for each square would be the best but that requeres more space and it will negatively affect the transparency of the LEDs bellow the chess board. So i came up witha new idea with implementing  laser ToF sensors with small gaps into the chess board in the middle. They will also be easier to control with a basic matrix mapping and also less power consuption which would be needed for a movable object far from frequent charging. Also those sensors seem far more easier to implement directly onto the PCB even tho that remains as a further issue that needs to be analysed.
I've also found a pre-build chess pieces that i just need to check their sized so that they are proportional to a 16inch chess board (the pieces are from this link: https://www.printables.com/model/32741-chess-set/files). Soon i will also need to start writtingthe code which would probably be in C++ as native language for the arduino microcontrollers. Thankfully there is a prebuild Stockfish library for me to utilise into my project.


![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6NjEwNywicHVyIjoiYmxvYl9pZCJ9fQ==--e791c2178e9301f22291c88007d7aa92af145fe4/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6NjQxMCwicHVyIjoiYmxvYl9pZCJ9fQ==--e495e124db210e15a8fe31d5e22cf6890c1a2b8a/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6NjQxMSwicHVyIjoiYmxvYl9pZCJ9fQ==--a6b45ce1693fb305b62c0806f432e3379c963ea5/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/da957559-6b58-4e53-af5d-cab5420c4c31/video.mp4
- https://lookout.hackclub.com/api/media/82f61945-2ddb-4f52-a02d-36dda2da79f4/video.mp4

## Entry 4
- ID: 3263
- Author: ajakovski07
- Created At: 2026-04-21T19:42:21Z

### Content

A little morning work session. Realised that i did not check the sizes of chess pieces. Taking that in consideration i immediately researched the avarage chess piece sizes and hoped that they are alright. Thankfully they are just on the higher end, the avarage was 75-85% piece diameter from a square and theese are 83.5% based on my chess board. Being on the higher end requires a bit more material but it ensures a better functionality for the lasers bellow the pieces that would be responsible for the Stockfish magic to happen. Had a little trouble finding the difference between a 3mf and stl file but i quickly solved that thanks to google ofcourse.
I also checked how would it look a 30mm diameter sized piece on the board and it look pretty good. Also found out that my chess board is categorized as a large travel chess board or a small/medium regular chess board which i think is alright considering i need to install electronics under it. If a PCB size becomes a problem atleast i would have a solution for that.
--------------------------------------------------------------
Second timelapse of the day. Now this one is before going to bed considering how busy of a day was. Managed to get done a couple of things now. But i also had big issues with finding a KiCad library for implementing my part into the schematic. The cost of it was a concern to me but at the end when i found a whole website for downloading libraries (SnapMagic) i also found the price stating 0.94$ a piece which is amazing. Implemented it into the schematic but now a pin availability becomes a concer. I will probably need to find multiplexers and demultiplexers for this project considering im managing a 8x8 matrix of data at once for the chess board to work. Soon i will have to start connecting everything togheter and probably adding passive components for everything to be stable. Then i will translate it to a PCB version and try to print it :).
Definetely a great way to end the day.

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6NjU2OSwicHVyIjoiYmxvYl9pZCJ9fQ==--2179e43302dbc68c6c61193936618efa072227ca/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6NjU3MCwicHVyIjoiYmxvYl9pZCJ9fQ==--e16ccd5e3a96d993dce7421eeffb7bbf2e387573/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6Njc3NywicHVyIjoiYmxvYl9pZCJ9fQ==--3a1fc8ffaa9ad516e5efe05413e1b905afb916c3/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6Njc3OCwicHVyIjoiYmxvYl9pZCJ9fQ==--2ac63d53627653e66fcfd97a0f8aee8c3ff80d33/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/9889b6f4-f325-4049-9375-26e043b4aff2/video.mp4
- https://lookout.hackclub.com/api/media/f05b166c-4b5d-4f7e-9761-a8c957e6da87/video.mp4

## Entry 5
- ID: 3445
- Author: ajakovski07
- Created At: 2026-04-22T20:11:02Z

### Content

Today i did something that i feared the most from the start but thankfully it happened sooner than later. I had to reconfigure my schematic and its components because i found out that the "blue pill" arduino stm32 is incompatible but i found out trough research that the ESP32-S3 is compatible and that is my favourite MCU! I've built a whole FreeRTOS Cargo Drone on that microcontroller that was custom built meaning that i will progress quicker with the software and hardware connection. I also finally found a module that works on magnetic fields and also that is cheap and small. The reed switch or Ky-25 or just the bare wire that is affected from magnetism will be so much better both with PCB construction and current draw. It is also possible to daisy chain everything considering it is i2c bus meaning i will control 64 pieces via 8 74hc165 register shifter which will also be daisy chained creating a 72 piece architecture controlled by 3 MCU pins!!! 
If that becomes a reality the pin scarsity mindset will fade away. Also making the schematic paper into A3 format scares me knowing the fact that i will have to connect hundreds of wires and then do the same on a PCB.
Also a new idea has popped in my head and that is making 2 separate PCBs and then connecting then with flex cable. I will have to further research that with reading a fer documentation or videos without timelapsing because that is not considered work towards the project.
I also researched the Stockfish versions compatible on MCUs, also 1 of the main reasons for removing my STM32 chip was because it was running ARM Cortex M3 making it insufficient for my needs. I found out that i can only use an older version Stockfish 11 from 2020 (3500 ELO) because it is run on a HCE logic and the newer are on NNUE which is far more demanding for a basic MCU.  The ESP32-S3 WROOM2 N32R16V covers for the Stockfish 11 with his ARM Cortex M7 and much more PSRAM. Also i love the ESP-IDF environment meaning that the project will be built in C code. Definetely not needed for such powerfull language but i have experience with it from before.
Next thing i have to do is modify the chess pieces or find new with mountable magnets.

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6NzA5MCwicHVyIjoiYmxvYl9pZCJ9fQ==--64e7d3d0f7e42920163095f347e2960d93cfd060/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/de058f3c-d8eb-4da2-9d69-cb752df5a59e/video.mp4

## Entry 6
- ID: 3618
- Author: ajakovski07
- Created At: 2026-04-23T20:49:48Z

### Content

Today i continued my research and improvement with BOM creation. The reason why i spend so much time researching behind the timelapse and adding the best components why timelapsing is because i want to be sure in my hardware once i start building it in real life and also having a clearer concept of what should the code look like. My main goal is to minimise the pin usage in order to use just 1 MCU because as you can see managing 8x8 matrix with a sensor and output modules is kind of a hussle. Today i changed the RGB LEDs exactly because of the pin scarcity issue. The older LEDs were individual and yes i had drivers but it was still too much pin consumption. So i used the same tactic as i did with my reed sensor and that is daisy chaining. Yes it will be a struggle daisy chainging 2 moduels 64 times but that is the only solution i can think of at the moment. I also chose to pick the bigger 5050 LED version just because i want to be sure that they are strong enough to display trough a trasnparent enough printed material. In total the LEDs at max usage consume around 1.6Ah but i will never ever use more than 4 at once so power is not an issue. Just to be safe i included a 1P2S Li-Ion battery for supplying this project just so that i'm sure that everything can run for a longer period. In order for everything to work i will need a couple step up/down regulators but that will be also sorted ASAP. For the LEDs i had to add step up regulators because they work on a 5V logic. Now that im writting this i realised that wasting my time for a 3A regulator was a waste of time because all 64LEDs consume 1.8Ah at max and i have split them into 4 LEDs per regulator :).
Ill put that on the list to be changed just because it is a luxury not needed in my project. 
I also still need to find chess pieces with mountable magnets but i suppose that will stay for the soon future that is to come.
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6NzQ3OSwicHVyIjoiYmxvYl9pZCJ9fQ==--b3c7604da9b14776748116d9adf74ec3b0ff59f0/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/bdbbb7c0-d788-4c0f-b9fd-202a9aecd99c/video.mp4

## Entry 7
- ID: 4229
- Author: ajakovski07
- Created At: 2026-04-27T15:44:02Z

### Content

Finally continuing this project after a weekend of delaying because of a national robotics tournament. Anyways thanks to the delayed project submission on fallout i am confident of making this project a reality by the end of june.
Im getting closer to finalizing the BOM list with the active and bigger modules. After that is fully finished i will have to read parts of their documentations just so that i can see the recommended passive components for the project.
I've made a couple changes to the schematic that still isn't wired:
-I've added everything that i think will be needed for managing the power source. Considering that i have only a 1S2P Li-Ion battery i will have 4V of power which might not be always sufficient. So in order to have every voltage and also a stable source i added a couple modules for this proble. I added a booster converter made, a battery charger with Power-Path management, another boost converter with low input voltage and a low dropout regulator with great stability for my 3.3V MCU.
I also read a little bit about daisy chaining which got me to the conclussions that i need only 1 rather than 16 SN74AHCT125D level shifters for the LED. Having 3 outputs i will split the chess board onto 3 sections accordingly. 
I've also found compatible chess pieces with a hole under for magnets. They are small pieces for my chess board but making them 160% of their original size will make them 78% of the square size of my chess board (speaking for the king piece). I had to research for magnet types and i found that n52 neodymium magnets are the most compatible for my project. The Nx naming stands for their quality. Considering that it will stay under a 3D printed board i will need a higher accuracy so taking a higher quality i think is needed and it still wont cost a lot considering they will be small like 5x5mm for example.
My next plan is to keep a 2 week streak in order to get back on track until end of june and before my other nationals tournament. I hope i design it fully in the next 2 weeks so that i can start building it after my national robotics.
Credits to the following link for the 3D printing scheme: https://www.printables.com/model/142745-travel-chess-set-magnetic-board-magnetic-pieces-bo/files


![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6ODc4MSwicHVyIjoiYmxvYl9pZCJ9fQ==--be2488efa7090f93d17f17dba8997de414c37a9e/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6ODc4MiwicHVyIjoiYmxvYl9pZCJ9fQ==--cbac07dd5ce9e72ae095a07ce48ad07122eb1f03/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/f39c5f01-1b3e-4864-bbc4-350f1ad169b7/video.mp4

## Entry 8
- ID: 4403
- Author: ajakovski07
- Created At: 2026-04-28T21:03:01Z

### Content

A little late night tired from today but working on the project. So previously i had basic reed swtiches that were mechanicaly operated. But how do you implement that on a PCB? You don't, i suppose. So i went on a journey once again with the goal to find a PCB mountable reed switch.
I did find some very good ones but thankfully i saw their price. To honest i am not willing to spend 30 hours on this project just to have high quality chess piece allocation. So i googled once again but this time i also used our fallout group for help. Supposedly a hall effect sensor is just fine too. I googled a bit about this module and i hope that it is precise enough. It is way more cheaper saving me around 100$ from the previous module. One downside is that i now need a power rail for all of theese modules but nothing else changes. I also had to increase my schematic paper to A1. The wiring will be painfull considering i have so many modules there. I will also need to start adding passive components real soon and then regenaret my BOM. I hope that it doesnt exceed 150$ so that i can save some Koi after being approved for travelling :).  I also started my github repository on which everyone would be having the luxury of accessing every documentation of a module, my documentation for building and everything else about this project.

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6OTE0NiwicHVyIjoiYmxvYl9pZCJ9fQ==--90d0c83ce4848c3d8192399ef5b4e8cd062e94a2/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/ab295e86-dcb7-4ee8-a941-512a141a6db2/video.mp4

## Entry 9
- ID: 4532
- Author: ajakovski07
- Created At: 2026-04-29T19:27:51Z

### Content

Finally back home on my PC which gives me a bigger efficency with the wiring. Tonight i fully connected the 8x8 matrix for hall effect sensors. So from a 64 sensor scheme i managed to connected them with 3 GPIO pins which is soo good for my project because i will need a lot of GPIO pins for other modules also. I had to read a bit about daisy chaining specifically this shift register which wasn't timelapsed considering the rules given from soup :).
I also added some decoupling capacitors for stability over the whole matrix. I will need a special 3.3V copper rail i suppose for the power distribution because the sensors use a lot of it. Also a new idea has risen in my head and that is a two plated PCB connected with FFC wiring. That way i keep the brain and powering on the bottom and only the sensor and LED on the top really close to the board ensuring greater efficiency.

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6OTQwNSwicHVyIjoiYmxvYl9pZCJ9fQ==--4f22d63098f8e1db11eddb32fd3d970e35d8b63c/image.png)

I really spent some time on making it all symmetrical so that it doesnt get messy in the future. Also adding comments troughout the whole designing helps a lot.
Just finished writing this and realised i need 2 more minutes for making 60 minutes.... Don't be mad at me for stalling the timelapse :)

### Recording Links

- https://lookout.hackclub.com/api/media/70a98280-01fd-422c-afdf-c600b57d524a/video.mp4
- https://lookout.hackclub.com/api/media/de258521-e1c8-4e68-8a3b-89fb710b5415/video.mp4

## Entry 10
- ID: 4651
- Author: ajakovski07
- Created At: 2026-04-30T18:03:58Z

### Content

Well this was a long timelapse...... Somehow i managed to loose like 50-65 minutes of work because i wasnt opening the file via a project resulting in no restore file. Also why is KiCad crashing so much when i try to open a library???
The app crashed 3 times resulting in a lost progess which suprisingly made me realise that i can do the same thing much quicker. Anyways today i connected the LED matrix with the MCU. The first time i made it flawlessly in every aspect but when i started adding the capacitors it crashed... Anyways i recreated it afterwards a bit more messier but i really can't do it all over again beacuse its sooo much wires. Atleast i managed to connect 64 RGB LEDs with 6 I/O wires from the PCB and other modules. I daisy chained everything and just so that i am sure with the stability over the long wires i added a bunch of 0402 100nF ceramic capacitors for stability and 2 resistors for the SPI communication just in case if in the PCB scheme i have to create long connections. I also found more accurate scheme for my MCU but with a smaller SRAM which would be a problem for my Stockfish program but i hope i can fix that when i order everything.
Im also still trying to figure out how to use a FFC connector on KiCad but i hope that wont be a problem in the future. Maybe if i just split everything in 2 files it will have a logical use....
Anyways so much time for a mediocre amount of work dissapoints me but i couldnt do anything with my app crashing a bunch of times. Atleast now i have created a project and not just a file that ensures a savefile :)
Ok how did my timelapse lose 20 minutes like if i connected wires around the computer didnt realise it was a different row like HELLOO I WASTED SO MUCH TIME RECREATING IT TO GET DEDUCTED....

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6OTc0OSwicHVyIjoiYmxvYl9pZCJ9fQ==--4e1b8341f9ac93abee9a17287f94f11db829b0a1/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6OTc1MCwicHVyIjoiYmxvYl9pZCJ9fQ==--728c5e1b9c0dbb9833182cbb9123fae527fd71b2/image.png)

### Recording Links

- https://lookout.hackclub.com/api/media/c862c869-931d-4fe0-a4c2-dc90597691b3/video.mp4

## Entry 11
- ID: 4739
- Author: ajakovski07
- Created At: 2026-05-01T10:37:41Z

### Content

Today i started wiring the power management system. I've also created a separate files for the main and secondary PCB. Im still working on the main PCB where everything is included aand the after i will just copy paste everything accordingly. About todays work i had to add a couple more modules. FIrst of all i realised that i dont have a USB-C port so i found a module for that and connected it. Also while reading about it outside of the timelapse i saw that it is suggested to use an ESD protection module. So i integrated that also into my project and then i started wiring. The wiring was quite an exquisite task to accomplish. I still havent finished everything but atleast my battery management system is completed. I had to read a couple documentations and copy the suggested scheme for theese modules. I added a couple of passive components for a good signal troughout the whole system. I had some confusions tho considering i have never built the modules by myself because in the past i would just buy theese components and solder then but now i need to do it myself and only get the chip of the module...
Anyways i hope it looks and works as good as i focused on it. There is a small confusion with the triple VBus connection but i hope slack fallout members help me with that issue :)
Next thing i need to do is to connect the regulators for the other modules of the project.

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTAwMTYsInB1ciI6ImJsb2JfaWQifX0=--9956393729af645be642d5f23ec8a808ea326b9e/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/4e91c5d4-e421-4136-bb39-d43f9608ce1b/video.mp4

## Entry 12
- ID: 5072
- Author: ajakovski07
- Created At: 2026-05-02T07:02:50Z

### Content

In this timelapse i managed to finish the whole power management circuit. I mostly associated myself with the recommended connection scheme provided by the manufacturers  from every module. But i did have some personal tweaks towards the passive components and their values. Overall i kept it original and electrically accurate. I'm a bit concerned about the electrical rules checker which creates a lot of warnings but i hope it gets fixed on it's own in the separate schemes of the both PCBs that i'm going to print. I didnt manage to find a JST PH-2 connector for my batteries so i just left the whole 18650 holders do that job for me with solder mounts. I think that there is enough space in my chess board for putting them in there between the 2 PCB plates....
For my next wiring i need to focus on the push buttons and the OLED screen that will stay on one side of the chess board. Also i need to find the identical scheme of my MCU in order to start connecting everything on the MCU aswell. I wasn't able to test out my progress which leads to a final test once everything is done!
Now i have stable 5V/3.3V outputs capable of powering everything that's on my board.

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTAxMDAsInB1ciI6ImJsb2JfaWQifX0=--dfa39c6ba0d9cf40d58ace71c13cafe632a3ddad/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/ba574cd1-a831-4996-814c-ca62c2ba535f/video.mp4

## Entry 13
- ID: 5241
- Author: ajakovski07
- Created At: 2026-05-03T07:45:57Z

### Content

Today i wired the OLED into my scheme! Had a little trouble finding the correct size for my KiCad and i did fail to achieve that but atleast i have the same model. I read a little trough the documentation just so thati can find the screen size in order to modify my 3D printable chess board. My idea is to have the OLED with an FFC connector going straight to the upper PCB where it will be met with a couple passive components and then trough another FFC the data will flow to the main PCB and then the MCU. The core idea of the OLED is to recreate the realistic clock timer for all chess players. That way both players will have time which will be stopped with a push button. I fixed the dimensions in Onshape for both OLED and push buttons and also found a pretty cool Pushbutton that i think will be a good match for my project. Sadly i cannot modify its size... I tried some quick solutions but i will need to download a compatible software for this issue without timelapse running in the background because i have to follow Soups guidelines :). But i like the model and i just need to increase the hat of the push button. Here is the reference link: https://www.printables.com/model/236991-tactile-switch-push-button-cap/files

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTAxNzMsInB1ciI6ImJsb2JfaWQifX0=--c66099547b017fdbdf40d9e3ab3703a6875f2afa/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTAxNzYsInB1ciI6ImJsb2JfaWQifX0=--4fd9cc30e569a21bfa9e516e7df01a6f211a270c/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/263274f4-1929-4c49-84be-9445c43320ae/video.mp4

## Entry 14
- ID: 5440
- Author: ajakovski07
- Created At: 2026-05-04T14:41:34Z

### Content

Today i added another smaller PCB into my project. I wired up the clock system for the chess board. My first idea was to keep it on the upper PCB but the buttons need to bea really close to the surface which would have holes at that position. On the other hand the "Upper PCB" has hall effect sensors and LEDs which will cover 90% of the surface but i dont want to have 10% of my PCB empty, i just think its a waste of money. So that's why i will make the upper PCB wide 90% and then have a smaller PCB on the side which would be easier to manouver for installing it with a peeking component. I also found a compatible switch for my PCB.  A tacticle switch that will stay on the PCB and i will install a 3D printed cap for it. Thank god i didn't have to rely on the basic push-buttons because those would be hard to mount and alos low quality. I also tried once again to find a schematic for the OLED that i wanted (2.42" 128x64) but the wirring logic is atleast the same with the model that im using. I added a bunch of passive components considering that this would be a long route from the Main PCB all the way to the Clock PCB. Probably via FFC but a  JST wire is also an option. I also made a visual separation of every PCB on my main schematic so that i can split them easier when i need to make the PCB design. The project is really starting to take shape and i hope there wont be trouble in the PCB wiring aswell.

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTAzNzQsInB1ciI6ImJsb2JfaWQifX0=--7700dd8eec511b612de6af45fac8b5a1cbb98318/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTAzNzYsInB1ciI6ImJsb2JfaWQifX0=--1dfd62d11ae404ba7d96c8b553c697a3a48be6dd/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTAzNzgsInB1ciI6ImJsb2JfaWQifX0=--03b5c41abd459fee2b5b875dad40e982ec63e624/image.png)

### Recording Links

- https://lookout.hackclub.com/api/media/54e9edb5-7fe7-41c5-9e4c-cd85600637c1/video.mp4

## Entry 15
- ID: 5584
- Author: ajakovski07
- Created At: 2026-05-05T11:40:18Z

### Content

Ohhhhh this was a long one.... So in this journal i FINALLY FINISHED the schematic wiring. I also found the right symbol for my MCU after a long search. I also chose to use a DevKitC-1 because i want to have the ability of a USB control in-case my USB receotacle fails. I rewired everything via FFC and into the MCU. I checked most of the wiring, fixed some issues (i hope those were all of the issues :) ) and overall completed the schematic. It was a hustle considering how big of a project this is and it did take me time. So much effort was put that i forgot to pause the timelapsing and it filmed for some time my spotify playlist xDD. Anyways i excpect that period to get deducted but i am also impressed by what i have achieved to create. After so much documentation, googling and looking around i connected everything. There are still some questions in my head that are unsolved but i think i am ready to continue towards the PCB schematic which will be far more complex. I will probably re-check everything without timelapsing just to be sure and then continue with the PCB design.

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTA1NTAsInB1ciI6ImJsb2JfaWQifX0=--d82385799a7ea82b2e8706e267959a2e7124eeea/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTA1NTIsInB1ciI6ImJsb2JfaWQifX0=--dc8cb64be6ad2ef05b03f411d01ce07a6e8183d6/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTA1NTMsInB1ciI6ImJsb2JfaWQifX0=--d053ace58b32d67615b06fd238cc086cbc10a5e2/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTA1NTUsInB1ciI6ImJsb2JfaWQifX0=--faf8193f23d8744d539c4e4e780c7e9767913504/image.png)

### Recording Links

- https://lookout.hackclub.com/api/media/8c324aea-c10c-4ea3-9b5e-2b8c16ea719c/video.mp4

## Entry 16
- ID: 5747
- Author: ajakovski07
- Created At: 2026-05-06T15:19:03Z

### Content

Today was pretty much a preparation day. By means of checking the electrical rules and fixing 99% of the errors. I also recieved a suggestion from fallout members on slack for using Label Nets and Hierarchial sheets because i will be printing 3 separate PCBs. The FFC connectors are doubled but that is just a visual preference from my side that will hopefully help me with the PCB designing. I also added the missing files for some of my components. Why were they missing you might ask... Well i move between my dorm and my home so i work on the project sometimes from my laptop and KiCad isn't synced. Anyways it took some time to finish theese rather easy tasks but i needed some time to even find theese options on KiCad considering that in the past i have only used CircuitMaker. Overall this schematic definetely needs a sanity check but i will continue now with the PCB and be prepared for errors left on the path of this great project :)


![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTA5MTAsInB1ciI6ImJsb2JfaWQifX0=--4d61f0dfbff4eacf28bc4500b14b4a921340c7f5/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTA5MTEsInB1ciI6ImJsb2JfaWQifX0=--72b20d416b6d56fd19b4f2d03ee9ff071a00c717/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTA5MTIsInB1ciI6ImJsb2JfaWQifX0=--4fdaadc7ee33ef340f893dae8d0bc7d2cb5e9cba/image.png)

### Recording Links

- https://lookout.hackclub.com/api/media/40f65f07-4b33-431d-9836-4fc587070b2a/video.mp4

## Entry 17
- ID: 5942
- Author: ajakovski07
- Created At: 2026-05-07T19:36:45Z

### Content

Todayyy i almost finished the closing lid for my chess board. The design is definetely in the top priority for future improvements after researching this topic but i wanted to start with something and go with the flow. And the flow gave me an idea of using magnets!!! So just the lid will slide in but it wont stay closed while traveling. So putting small magnets on the end of the lid and inside the board will be a cool solution. Now that im writing this i am realising that it will cause EMF disruptions with the elctronics but the main PCB doesn't need to be as big as the whole space inside so i will just place it further from the magnets. I created also a gap for the lid to slide in but now i dont have the time to create the magnet gaps inside the chess board. I also did a small research about the material once again but i will need a deeper look in order to chose the material that is money-efficient, transparent and available for printing. After i finish this the PCB needs to get started :)).

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTE2ODYsInB1ciI6ImJsb2JfaWQifX0=--bd89ececf522fb671749717a0f37c3af866fc210/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTE2ODcsInB1ciI6ImJsb2JfaWQifX0=--fb261425127f5f5edad5da58028ee45d5e569f88/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/5407860b-7bbb-45b7-8bce-a21c54831611/video.mp4

## Entry 18
- ID: 6095
- Author: ajakovski07
- Created At: 2026-05-08T14:59:29Z

### Content

In this timelapse i completed the closing lid, "assembled" it in OnShape and started the PCB editing. About the 3D printing everything went so smooth adding some light tolerance values for the closing lid in case it is a bit larger than the opening. Even tho i don't believe that a 3D printer is capable of distinguishing 0.1-0.9mm but atleast i gave the dimensions :). Adding magnets in there will be really good for locking the closing lid inside and now that im writing this i am thinking... If the magnets hold real good will i be able to open it because i have no pull method. I could add small handles on the closing lid for pulling back. That would be a cool idea. Also im happy that i started the closing lid without any experience, scared of not fitting or falling out and now i am thinking of the problem if it sticks too hard xDDD. Anyways about the PCB i wanted to accomplish more today but taking in notice that i have never created a PCB before it is going slow. Because i change from my laptop to PC i dont have every library in my laptop and without them i cant integrate the modules from the schematic into the PCB. I tried adding a few but then realised from my freshly generated BOM that i have missing libraries. I can find them trough the description of every module but that would be a hustle and waste of time in my opinion. I will fix that tomorrow from my PC and download all of the files accordingly on both computers. Also in the meantime i will watch tutorials for PCB designing because i will need for the routing :)


![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTIxMTEsInB1ciI6ImJsb2JfaWQifX0=--5da326f425cf506ccee272399515dd2b5b745622/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTIxMTIsInB1ciI6ImJsb2JfaWQifX0=--51fe457f6154b5138a4176f9ca304d7a854d44ad/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTIxMTMsInB1ciI6ImJsb2JfaWQifX0=--7dd8f1a8f58f467197417a0b7b9e8e26c1f61ae4/image.png)


![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTIxMTQsInB1ciI6ImJsb2JfaWQifX0=--32e15b4dff38266743f960f437f35801243db7cd/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTIxMTUsInB1ciI6ImJsb2JfaWQifX0=--2575cbd77c1dfa4add95f7510d55eda716a6e2db/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/0c6fa15c-6598-40e7-ae80-df20056720a4/video.mp4
- https://lookout.hackclub.com/api/media/aba531a3-46ee-4a0c-9c95-e3747ea18b37/video.mp4

## Entry 19
- ID: 6281
- Author: ajakovski07
- Created At: 2026-05-09T15:55:03Z

### Content

Today i started the PCB designing. Such a pain!!! I am literally lost on how to do this, the schematic doesnt transfer as it should even tho i have all of the folders added and even if i try to replace the electronics it stays as unrecognized by any library. I also lost the progress in the middle of working on the project which delayed the progress for such long timelapse. Having no prior experience in PCBs for such a demanding project is really painfull but i will try tomorrow again with a different approach. I wil do it manually on 3 different plates. Yes the import of wiring and modules helps but it is useless if i cant add the previous passive components. Also i have problems with adding the needed MCU. I must have 16MB of PSRAM because otherwise i wont be able to run the StockFish as all.  I suppose tomorrow will be an all over again work because i am really lost on how to start this.
Also adding some standards and constants into the PCB schematic is so confusing like and immense. I wish this part couldve been done automatically and i have the power of only choosing where to place the electronics...

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTMwMzQsInB1ciI6ImJsb2JfaWQifX0=--8e04019207a93592b5e86b45e67e21b46ad49ece/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTMwMzYsInB1ciI6ImJsb2JfaWQifX0=--f38159948197ff50190968c5fd17bb4340088105/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTMwMzksInB1ciI6ImJsb2JfaWQifX0=--6a037f0e842e788e84bd653bc1ef68d6db965f09/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/b4b3fdc9-3aee-430f-87c7-cdd3bfe21ce5/video.mp4

## Entry 20
- ID: 6397
- Author: ajakovski07
- Created At: 2026-05-10T07:52:08Z

### Content


In this timelapse or actually second day of PCB designing i finally made some progres!!! Ofcourse you first of all need to learn something in order to create it and thats what i did. I spent some time away from timelapsing on watching PCB designing videos. I also need to deside where to pu the PCB holders on my 3D design. The main pcb will defienetely be small but big enough to have my esp32-s3 a bit further from both power management and the connecting magnets on the chess board. I will also need to create a hole for the USC-C to connect with my board. I worked today strictly on just the power management section on the PCB and i really hope the wiring is alright and that i do not need to connect every pin. Also i might need to change their locations in order to make it smaller and more dense in sections that will be further apart from themselves for EMF protection. The way i design the PCB is to manually copy the design of the schematic because i cant transfer it immediately due to library issues that i had the first day of designing the PCB. Also the layers and the wirings are so confusing and their widths i really hope i get a better understanding in the future. A lot of time spent working but not much to journal because this process seems to be very complex and hard considering there is no one to tell me if i am wrong with any of my work.

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTMzMzgsInB1ciI6ImJsb2JfaWQifX0=--41d2ca7943c7b3d1b3d3b01905ff6901d9976c72/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTMzMzksInB1ciI6ImJsb2JfaWQifX0=--6a6b549b309cb80f55646c423ac632e88965f41e/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/9fc8559c-13fc-4591-8312-2f7e011b4549/video.mp4

## Entry 21
- ID: 6576
- Author: ajakovski07
- Created At: 2026-05-11T11:50:23Z

### Content

In this journal marking the third day of the PCB designing im pretty sure i finished the power management circuit. I excpect a lot of errors but i have to start from somewhere :)). It looks messy and now on the last moment of ending this timelapse a fellow member of the fallout community on slack told me that size does matter!!! I wasnt really thinking that it does but apparently i was wrong. I cant place the smallest smd package for every type of passive component.... So now i have to watch out because i barely made everything fit into my tight power management circuit. I dont know if its smart to place everything close togheter but i hope it is because i dont want to rework everything again. Also doing it manually costs me not having the designated nets from the schematic which is really bad and i hope that doesn't affect my work in the future. I also learned something new about vias! Ofcourse that came trough work and brainstorming questions on every topic about PCB designing. I hope that when in the future i send this design for a sanity check no one gets a heart attack but im trying my best to make it work even tho when i ran the DRC check it didn't look so well... Anyways tomorrow my work is to check every passive components package and change it if that is needed accordingly and then move onto the other logic revolving around the mcu on the main PCB. I suppose doing a step by step procedure and having the components split into blobs is the best way of making this thing to work.

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTM4OTgsInB1ciI6ImJsb2JfaWQifX0=--4fbbaddf8a5dd2c498fea08d8b7ff408a1a58b83/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTM4OTksInB1ciI6ImJsb2JfaWQifX0=--d27e53d1c45df2c75f5f916677827770fad46c18/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/151ba295-7268-4c7f-bd9b-60074499467c/video.mp4

## Entry 22
- ID: 6838
- Author: ajakovski07
- Created At: 2026-05-12T20:23:10Z

### Content

Sooo just as i said in the previous journal.... I had mistakes in my PCB circuit wiring and so far i am aware only of the mistakes with the packaging and i fixed those in this journal. As expected i googled it on which sizes should i use and i modified my schematic accordingly. I HAD A HUGE CONFUSION with the fricking net and auto resolving net class. The GND net somehow sticked onto my output of my battery charger and i just couldnt remove it. And then i lost internet connection :))))). I lost somewhere about 20 minutes of journaling..... I got mad at that point considering it has happened in the past too  but half of those 20 minutesi was just wandering around trying to change the net of that wire. And i did it thank GOD... I then proceeded on finishing with the smd packaging, renamed everything accordingly, double checked and messaged my slack friends for a sanity check (that i probably wont have a feedback on because it is early in the day for the chat to be active). Anyways now i can with ease continue with different parts of the wiring my PCB. Also track sizes are a big confusion, next research that i should do is what is the maximum current passage trough a track width....

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTQyMDIsInB1ciI6ImJsb2JfaWQifX0=--abc70d4898bae2cae710c2722f30a400a4e5f053/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTQyMDMsInB1ciI6ImJsb2JfaWQifX0=--92227348c5fb7cf29889aedda59ca0369f4ef101/image.png)

### Recording Links

- https://lookout.hackclub.com/api/media/cdba32a9-5553-4d30-80c2-255d79b704e9/video.mp4

## Entry 23
- ID: 6897
- Author: ajakovski07
- Created At: 2026-05-13T07:10:25Z

### Content

In this timelapse i did what i said i would do in the previous journal. Woouww a man of his word. Anyways i checked the trace width did some unproffesional calculator manouvers and i think i have this section covered. 3A max is not a lot and i hope nothing burns up or browns out in real life. What i also did is ask my fellow slack fallout friends for a sanity check on the power management and they did provide me with useful tips!!! What I learned from them is that i dont have to occupy entire layers with filled zones or so called "pours" which was a myth for me before now. I am trying to keep the pours smaller now giving me the power of having universal layers! I also changed them to solid, after some research i found out that those were the best and considering i have no pcb soldering experience nor the right equipment i will most significantly lean towards a machine soldering product and trough googling i read that solid layer fils are the best for that approach. I also started focusing now on other sections of the schematic, first i will create the starting wires from the ESP32-S3 WROOM 2 and then i will add those FFC connectors and wire everything up accordingly. That will be a topic that i should research also but we will get onto that sooner or later into this week. I am also leaning towards macking the PCB as smaller as possible so that it is easier to attack onto the chess board. The idea of attaching will defienetly go with screws onto mounts that i will create on the 3D model and the PCB. The antenna will be placed as far from the sliding magnets as possible, only 1 will be bellow the antenna for a split seconds but i wont power on the chess board untill its closed ofcourse. Multitasking this while having 2 tournaments this week will be an issue but the grind and streak for soup must continue!!!

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTQzNzYsInB1ciI6ImJsb2JfaWQifX0=--128f49f4e7f77e9a3410f1d5ebdc02dc41fc8d71/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTQzNzcsInB1ciI6ImJsb2JfaWQifX0=--4b2d5cdec406e201fc7775a77337cba4c13cf238/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTQzNzgsInB1ciI6ImJsb2JfaWQifX0=--fe6cae28f6ec04406586c07c49f5e8eac7b6496f/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/0d508c5a-2a22-48a9-bf65-7b54cdaad857/video.mp4

## Entry 24
- ID: 7155
- Author: ajakovski07
- Created At: 2026-05-14T19:26:06Z

### Content

Is it allowed to fricking crash out...... I literally timelapsed this whole thing for NOTHING!!! Because i forgot to transfer the kicad files from my PC to my laptop so that i can work from my dorm i decided to finish the Clock PCB. AND I DID BUT KICAD DECIDED TO CRASH ON A DRC CHECK. It crashed and i was left with nothing. Even the backup is empty.... Atleast i can journal what i was doing i guess. In this timelapse i did the Clock PCB yayyy so exciting. And then i lost everything.... I mean the second attempt will be far more efficient and faster but whyyy does my kicad crash on a basic function given by the app.
This PCB will be 2 layer because i only have 1 power and i will use the second layer for ground. I tried applying nets but it didnt really work out. Atleast i got the needed libraries and created some missing footprints for the project. OMG now i dont even have screnshoot proof....

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTUxMDIsInB1ciI6ImJsb2JfaWQifX0=--91b84883c559d24df0c78a6bd26bc446f4865043/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/086a3783-4f38-45e4-b16e-e035a6d83afa/video.mp4

## Entry 25
- ID: 7263
- Author: ajakovski07
- Created At: 2026-05-15T15:03:30Z

### Content

A late night grind :) It is just past midnight and i wanted to keep up the daily grind even tho i was on the Faculty of electronics and IT in Skopje, N.Macedonia for 11 hours a tournament on which i got ranked 4th out of 12 contenders i still decided to log in atleast 60 minutes of work for the hackaton :))) Multitasking everything is really hard but we keep up with the good work. In this timelapse i reworked what was lost due to crashing KiCad and now i frequently saved the file even tho i had a mini crash at the start but quickly recovered. In the future i will definetely try and avoid pcb work from my laptop but that would be possible only on weekends when im at home. About the PCB i finished the clock PCB, everything was DRC approved and i kinda liked the effectivenes when you have nets for your wires. I wish i had that ability for the main PCB but i just couldnt find the libraries with footprings.... I also started/prepared the work environment for the upper PCB. The reason why i stopped working on the Main PCB is because that file is on my PCB and in the meantime i will work from my laptop on the peripherals and then connect everything.  The clock PCB is 2 layered one because i didnt find the need of pours  considering its a basic schematic. On the other side the Upper PCB will definetely be a 4 layer considering it will draw a lot of current and also have multiple different power supplies. That work will be left for the next timelapse tho. Im going to bed so hard rn.... Tomorrow im again on the faculty to get my diploma :)))
Another thing i should not is that i raelly enjoy using vias but i hope they dont increase the cost of the pcb by a lot tho.

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTU2NzcsInB1ciI6ImJsb2JfaWQifX0=--daa1e24250a8ee881390f1d01cece90a04643e3a/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTU2NzgsInB1ciI6ImJsb2JfaWQifX0=--3b58efe9ac63b97f2f3847551c7da1e4a2f96680/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/b0c1655e-defa-4605-93ae-5e465bdb03d8/video.mp4

## Entry 26
- ID: 7583
- Author: ajakovski07
- Created At: 2026-05-17T10:33:46Z

### Content

Soo today i worked on the overall PCB. I read that nets are very important so i tried to implement atleaste some components from the schematic so that i have the net wiring in function and it really helps a lot. I replaced each component and rechecked the wiring if anything was missing. Thanks to the schematic i found a couple wirring errors which is good :). I also worked a bit around the FFC connectors and now im starting to think about the dimensions of the main PCB. It should be small but not small enough that there is a bunch of parasitic signalling going on. What i also did is that i fixed all of the vias (so far with my knowledge). More precisely i placed each GND via to connect only to Layer 1 and Layer 4(GND) because i suppose otherwise i will short each power in the circuit.... I am really starting to see the project becoming alive after so much work and so much confussion which is so motivating. Wow yesterday just when i wanted to upload it i lost internet connection which resulted in a use of the streak freeze:(

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTYyMTQsInB1ciI6ImJsb2JfaWQifX0=--eff2419dd13d277f6341dc6bff6b2cd46ad4d2f6/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTYyMTUsInB1ciI6ImJsb2JfaWQifX0=--3713da54593a479dac45328b6919bc05045de596/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTYyMTYsInB1ciI6ImJsb2JfaWQifX0=--a779f675e264849635df5f34d0e691a7ec37f442/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/8002165b-7534-441e-970b-a112a2ff25ef/video.mp4

## Entry 27
- ID: 7789
- Author: ajakovski07
- Created At: 2026-05-18T14:12:46Z

### Content

In this timelapse i am pretty sure that i have  have finished the main PCB. And honestly when i started this PCB designing I HAD NO IDEA OF WHAT I WAS DOING. Fast forward a couple of hours timelapsing i am done with a definetely amateur design but hopefully a valid one :). I will recheck everything in the next timelapse and then continue with the upper PCB. I also need to determine the size now and then reconfigure the 3D model for the mounting. I learned a couple of things while creating this PCB thanks to my slack fallout friends :). More specifically on what i did today was adding the level shift converter for the upper pcb and connecting everything around the PCB. I also made everything tighter for a smaller pcb and yeah i think it looks pretty good. I have some questions that i will google for my PCB but that will be done in the next timelapse.

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTY1ODEsInB1ciI6ImJsb2JfaWQifX0=--612bc0766d6d5921fd3041e10ec85ab43c80ff80/image.png)
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTY1ODIsInB1ciI6ImJsb2JfaWQifX0=--c2c91b1b04de9488e588be31b72cf8b17478c9bc/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTY1ODMsInB1ciI6ImJsb2JfaWQifX0=--d9f2e530b4510f8e99cda28c43fb28591a346cb3/image.png)

### Recording Links

- https://lookout.hackclub.com/api/media/81c6ce47-776f-4f69-8854-253f47c8a279/video.mp4

## Entry 28
- ID: 8038
- Author: ajakovski07
- Created At: 2026-05-19T21:10:36Z

### Content

A DRC CHECK!!! Honestly i didnt expect to have so much errors but they are overall pretty much just amateur mistakes. Which can get fixed pretty quick. I am still wondering how to fix the mask aperture for every of my modules. That value was set automatically i suppose but it is too big and it bridges every single pin. Other than that i learned about annual rings and how they should be placed. The simplest formula is to have the via diameter be double the size of the hole inside. I also tend to fix the track widths according to the vias and i changed the netcalss presets. I significantly reduced the clearance width which might me an issue for parasitic signals but i guess i will have to watch out on that manually because some places do not require signal precision. I am also placing nets for every single wire and pin because the passive components didnt transfer from te schematic, just the net names. so far i fixed 140/280 errors that were found and the others will be sorted out pretty quickly tomorrow from my laptop in my dorm. I also changed some layering settings and adjusted it to PCBWay requirements. After this i need to do the same for the other PCBs.

Before:
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTY3MDcsInB1ciI6ImJsb2JfaWQifX0=--7182d7b012b87a9252219a268e49cb4fed483c1c/image.png)

After:
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTY3MTEsInB1ciI6ImJsb2JfaWQifX0=--8d45962158349a13995842c15424d0d453f6d09d/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTY3MTQsInB1ciI6ImJsb2JfaWQifX0=--f693afcdebb68f6caf9c6ce68f68ff5362bcbf4a/image.png)

### Recording Links

- https://lookout.hackclub.com/api/media/8dd85c0a-2493-4d00-967f-566bcd24fed8/video.mp4
- https://lookout.hackclub.com/api/media/29cf7d3f-f7be-4155-9717-cf1b4a12811f/video.mp4

## Entry 29
- ID: 8369
- Author: ajakovski07
- Created At: 2026-05-21T20:45:37Z

### Content

In this journal i almost finished the whole DRC!!! (I've noticed that i have mistaken ERC for DRC but its similar anyways) I'm still lost with the solder mask issue tho. But i still haven't googled that issue so i hope it gets sorted out soon. On the other side i fixed 130 issues today and most of the warnings. They were mosto of them track net problems because i wasn't able to transfer the whole schematic. Anyways that is fixed so far as i can see. It was mostly labour that it had to be done. Nothing much i need to explain because it was the same as the previous timelapse. I will google a couple more stuff in the future just to make sure that i have a good PCB and also i started following a bunch of PCB makers to learn something :)).

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTcwNDYsInB1ciI6ImJsb2JfaWQifX0=--12429ed7aeae9ed218931e8f172f9f5b5a27de2c/image.png)


![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTcwNDcsInB1ciI6ImJsb2JfaWQifX0=--99b77b8ba2769f524d95a3603b97bf729ac2bb70/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/456af5b3-acf0-494d-8f17-c15bc8618c7f/video.mp4

## Entry 30
- ID: 8507
- Author: ajakovski07
- Created At: 2026-05-22T17:47:40Z

### Content

A rushy timelapse i have to admit. I've gotten myself into a complex task while in a hurry. It's an exam week and im in such a hurry with everything but i still want to keep up with the hackaton so the 60 minute bare minimum must be achieved!!! Tonight i took my head off a bit from DRC checking and i focused on the clock PCB. I don't know why i havent tought of this earlier but the way i place the modules is how they will stay.... And thats why i am redisigning my clock PCB. Once again having trouble with the 2.42" inch OLED display and i desided afte so much suffering to accept another suffering task and that is to make it on my own. In my logic i just need to place the pins right which will be trough hole for me to solder whatever i want. And that will be an SSD1309 2.42" OLED display! How smart am i right :). I also needed to take the distance values out of the 3D design so that everything alligns as it should. I hope the buttons work well with the 3D printed buttons tho.... This is still work in progress but i have to go to sleep and do 3 EXAMS TOMORROW LIKE WHY ARE MY PROFESSORS SO EVIL......

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTc1NzQsInB1ciI6ImJsb2JfaWQifX0=--311cd468737e213c53045f801952393c2b070fea/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTc1NzUsInB1ciI6ImJsb2JfaWQifX0=--519111834008ef88c8dfe962920a0fe06b4898c6/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/cbb604ba-6fbd-4ee6-95c3-9d072d7aed42/video.mp4

## Entry 31
- ID: 8608
- Author: ajakovski07
- Created At: 2026-05-23T07:02:21Z

### Content

Been so much in a hurry i barely get time for even sleeping... Exams week and also building a whole shipping available project for an Erasmus project is really draining me and making me lack everything a bit. Now i focused on fallout and even tho i did not finish it, i made some progress. It's 12pm and i remember in a normal occasion i wouldve gone to sleep at 10pm and be productive way more but now its such a mess. In this timelapse i worked once again on the Clock PCB and oh boy have i just found a 3D parameter error. Considering it is quite a large project i guess i will adapt to the mistake in the PCB sizes and it wont be noticable much. I also added mounting holes for the screws and created holes on the top of the 3D model. It might not be aesthetic but it will hopefully work rather than making complex 3D bridges for the screws to hold on. That is all i am very sleepy and my brain is shutting down.
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTgzNDUsInB1ciI6ImJsb2JfaWQifX0=--d8e8f87baa1df0e484b4659ea785f5daccd1235f/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTgzNDYsInB1ciI6ImJsb2JfaWQifX0=--9b7aac1c8a5d8f35fb85d70a2f15f48e7b764faa/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTgzNDcsInB1ciI6ImJsb2JfaWQifX0=--56c6a1c7c4798cc214532408a8ca6f314356356c/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/57cb4687-c835-4030-aba5-f80a2fa135cc/video.mp4

## Entry 32
- ID: 8819
- Author: ajakovski07
- Created At: 2026-05-24T08:43:41Z

### Content

In this timelapse i worked a bit scattered around or more accurately, fixing the scattered errors on the PCB. The PCB will dfienetely be the largest part of the project and now im even thinking to changing the ESP32-S3 WROOM2 into a DevKitC1 with female extention pins but i will analyse that behind active lookout. Anyways i am moving forward, a bit slow because i have had some personal disturbances but the daily avarage must continue in order to go to Shenzhen :)  I wired everything on the Clock PCB with no DRC errors and then i worked a bit on fixing Main PCB errors and then headed to making the Upper PCB. Im still confused why i couldnt change the solder mask on the whole chip instead i had to do it on each pin. Also kinda weird why i cant connect the Upper PCB files the same way as Clock PCB so i had to try and making both Main/Upper PCB in 1 file considering they will have the same prerequesits for printing. I just need to fix some library errors because working on 2 different PCs has caused library losses and now i am sufferin because of that. Anyways i will sort it out and if i manage to transfeer a fully connected schematic into the PCB it will flows way faster. I also made both FFC connectos universal on the upper PCB because in reality there will be only 1.
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTg2NjIsInB1ciI6ImJsb2JfaWQifX0=--688f21e25349175bf02b9467ec866e777351b690/image.png)
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTg2NjMsInB1ciI6ImJsb2JfaWQifX0=--6f1e9a93446e4d7142b6e97c0b748d9cc7ae1253/image.png)
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTg2NjQsInB1ciI6ImJsb2JfaWQifX0=--c746b29177fda120a388c01e8d7e694f0fbb0298/image.png)

### Recording Links

- https://lookout.hackclub.com/api/media/cc237826-cecc-47dc-b3e9-ae8e15f4b3e3/video.mp4

## Entry 33
- ID: 9056
- Author: ajakovski07
- Created At: 2026-05-25T07:11:34Z

### Content

Workingg on the upper PCB!!! I started laying out everything taking in notice the parameters given from the onshape 3D design. Huge boost in efficiency was managing to transfer everything from the schematic which will take effect when i connect the wires and everything on that massive 288mmX288mm plate. Also a lot of modules that need to be precisely placed in order to have symmetry. It does take time boot soon i will finish it. Making a big milestone considering im getting close to 50 timelapsed hours!! :) Im really trying to take in notice everything on how will it look IRL and inside the chess board so that it looks nice or atleast attacheable. Next timelapse i just need to finish off the placement and then we will start wiring :). Also my idea is to do it in the main PCB file and then transfer it to a separate file in order to order it from a manufracturer. Sadly this PCB will most definetely have to be a 4 layer considering it has diferent power supplies and a lot of signals going around.

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTg4ODgsInB1ciI6ImJsb2JfaWQifX0=--c82322b68b8819dcfad990db5ff21b7b90b59c28/image.png)

### Recording Links

- https://lookout.hackclub.com/api/media/49f2f962-3ac0-4e96-b1cf-8490bc26b1fe/video.mp4

## Entry 34
- ID: 9331
- Author: ajakovski07
- Created At: 2026-05-26T10:51:58Z

### Content

Damn im spamming theese 1 hour timelapses... The reason is (just to state that i am not farming streaks intentionally) because i am going on a school trip for 6-7 ;)  days, no honestly i wont be able to work on the project and i want to continue the streak by just logging them. Anyways in this rushed timelapse i just continued the work on Upper PCB. Fixed some smaller erros, placed everything and now i am planning the wiring for 1 row and then i will do the same for every single row correspondigly. I want everything to be symetrical and similar so that it looks nice and works nice. Quite a big PCB board ehh. Im starting to get pricing concerns so im also googling on how to minimise costs also because i heard that vias are expensive and i am using a lot of them.  I had some small problems with the net assignments on the different clock pulses tho. Anyways i fixed that and now everything is updated. In next timelapse i will most definetely construct a wiring plan for atleast 1 row that i can copy further on. I also realised that if you create a pour on any layer, it doesnt mean that you have totally occupied that layer. In reality i can still use universal signal wires that will automatically isolate from the pour. That could've helped a lot if i knew it before but oh well you live and learn :)
 
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTg5NjAsInB1ciI6ImJsb2JfaWQifX0=--ad24731c1ce4b6dde246ac292706e6974313ed3b/image.png)
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTg5NjEsInB1ciI6ImJsb2JfaWQifX0=--d3e159cce04015937cf339aded5542d2fba57cca/image.png)
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTg5NjIsInB1ciI6ImJsb2JfaWQifX0=--409bd4b52dc32d5f185fc12ff32c33cc2221a38d/image.png)

### Recording Links

- https://lookout.hackclub.com/api/media/420e9d51-0984-43a3-9f58-a17f431dd0dd/video.mp4

## Entry 35
- ID: 9621
- Author: ajakovski07
- Created At: 2026-05-27T10:13:33Z

### Content

Ok another journal, taking in account the streak freezes i will have enough troughout the school trip but i will make 1-2 more just to make sure incase i get busy with the last week of school which is filled with exams. Anyways in this timelapse it kinda got messy, i started wiring everything and OHH BOY ARE THERE A LOT OF TRACKS. Trying to create a standard based on which type of signals can use only 1/2 layers of the PCB. Honestly i hope this wont be an expensive project because there are really a lot of tracks. Also considering the fact that the clock pulses will have to be similar length is taking its tool. I finished half of the hall effect sensors connections and then i will have todo all over with the RGB lights which are serial. There really is a lot of wiring and i atleast hope that it will run at the end with no major parasitic signals disturbing anything crucial. Oh and now that i think of DRC is making me insane. This project will for sure take +70 hours in order to be done.

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTg5OTMsInB1ciI6ImJsb2JfaWQifX0=--68c1c862fd3c8b15e1e05864ae3fbcd4de73e220/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTg5OTQsInB1ciI6ImJsb2JfaWQifX0=--296f5ff11495f34a5227ec6a88e3c2440d6a6b3f/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTg5OTUsInB1ciI6ImJsb2JfaWQifX0=--b92e65ae36ceb98b66c098908653fc82c063cb7c/image.png)

### Recording Links

- https://lookout.hackclub.com/api/media/81e5397b-8602-4bf5-abe8-a16e777f3176/video.mp4

## Entry 36
- ID: 9835
- Author: ajakovski07
- Created At: 2026-05-28T05:05:38Z

### Content

The last timelapse before i go to a whole week school trip. This thing will probably get journaled in Thursday by my father who i have assigned for logging theese journals. After this i will do regular journals without splitting them but i hope you understand my situation and desire for this project and its streak which is 28 right now. Based on the work that i have done it was also pretty good. I found a better solution for a couple of things (also fixed some errors with via diameter). First of all, taking in notice that the usb port on the main PCB is facing a wall (which will have a hole for plugging in) it will cause a problem on the Upper FFC cable considering it is facing the chess board wall. That is unacceptable so i thankfully just rotated everything and it stayed in contact without any DRC errors. Then i moved onto the Upper PCB. A clever idea that came on my mind is moving the chips that were on the bottom with the FFC connector onto the middle section, thus ensuring smaller track lengths for clock pulse and an easier environment for wiring every section of the board. That saved a bit of track length but oh man will it be a big PCB and probably an expensive one. Cheers to the signed contract with jlcpb for 15$ voucher that i will use hehehe. But on the other hand jlcpcb makes only 2 layered ones so only Clock PCB will be ordered from that part. After i come from the holiday i will have a prebuild structure for me to just copy it on every other row accordingly. Maybe then i will have even better idea for both lowering cost and improving quality :)))
Cheers!!!

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTk1MDUsInB1ciI6ImJsb2JfaWQifX0=--558fe72dc99fae72df2fe6b9f3203fb0475c426a/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTk1MDYsInB1ciI6ImJsb2JfaWQifX0=--6e9b85c013285a3a491f958a82b951cac64236b7/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MTk1MDcsInB1ciI6ImJsb2JfaWQifX0=--67da262f0359c8f0ad7ce8277bd4ecbcf5ba827b/image.png)

### Recording Links

- https://lookout.hackclub.com/api/media/9944c51f-1b1b-49d9-82df-0a62a74a6953/video.mp4

## Entry 37
- ID: 10361
- Author: ajakovski07
- Created At: 2026-05-30T10:40:43Z

### Content

Just came back from the school trip!! :))))
I am very tired considering i have been travelling for 6 days constantly but i also need to complete this project ASAP. It's almost june nad im close to 52 hours on this project but far from completing the project. So in the following 2 weeks i will intensify everything. Based on this particular timelapse i fixed more than 130 errors. More precisely i just connected the hanging pads that had a net to connect to. I am having slight trouble with the layering concern and would it be able to do the upper plate on 2 layered PCB.
Considering that it is 1PM and my i fell asleep wokring on the project 2 times i will go to bed now because my avarage was 5 hours of sleep for the week :)
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MjM0ODgsInB1ciI6ImJsb2JfaWQifX0=--f59fb23f53b2ccb6cb2527d5159d41b64aa3b95c/image.png)
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MjM0ODksInB1ciI6ImJsb2JfaWQifX0=--ea56314568bf17148e132e21c3e1a4bdd147bbef/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/373b3b4b-fd5c-456f-bbdd-c1ba42e5a33a/video.mp4

## Entry 38
- ID: 10645
- Author: ajakovski07
- Created At: 2026-05-31T12:32:35Z

### Content

Quite a long timelapse ayy... I finished the Upper PCB and with that i think that i have fully finished the schematic and routing and all of that madness (small iterations and error fixes are still possible). More precisely in this timelapse i literaly routed everything. As you know from previous timelapses i have created a layout for every row/column on how it should be connected and now i just did that for every module on the board. I ensured good FFC connection with the filled zones and also created bridges for connecting the power supply because i had to use those layers for signals too creating big lines of clearance blocking the power line efficiently. Everything is 'pretty much' symmetrical but which is something that i really enjoy and i hope the PCB will look tasty when i get it IRL. I tried using tips and tricks from google and slack friends for making it as cheaper as possible. One of which is removing annual rings from unconnected layers but doing that on 600vias is a torture so i just stopped eventually doing that later on. The amount of connections made is absolutely crazy and looking at it right now amazes me so much. I didnt place the capacitors as it was in the schematic because it is not really effective i suppose considering the power comes from a filled zone so instead i just placed 100nF and 1uF on the FFC pad for 3.3V and 5V hoping that it is enough. The clock pulse has achieved pretty much the same length for every connection (around 110mm length).

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MjQyNjYsInB1ciI6ImJsb2JfaWQifX0=--5777662fd3fdee1b4200fdfa27f3f8c383216814/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MjQyNjMsInB1ciI6ImJsb2JfaWQifX0=--6d342db6104548f7637f272bc6dbaa3b0be393ca/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MjQyNjcsInB1ciI6ImJsb2JfaWQifX0=--47a053fd219197a231f641b6297ab1448fe0be4a/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/04628c26-fad4-4755-8ef0-34dd92460962/video.mp4

## Entry 39
- ID: 11658
- Author: ajakovski07
- Created At: 2026-06-04T17:30:16Z

### Content

CODING HAS APPEARED IN THIS PROJECT!!!
I had some "days off" from timelapsing resulting into all of my streak freezes being used but they werent for nothing. In that period of 3 days i had 5 exams but other than that i researched for this majestic chess board :). What i researched is how to code this thing. I wanted to use primarly stockfish for this great idea but reading trought the documentation (which is not allowed to timelapse resulting into 6-10 hours of research in those 3 days not being accounted into the project) i saw that the licence is kinda complicated for a student to use that github repository or even at all i need a licence for stockfish. Also the coding part was soooo hard, considering that it wasn't optimised for an MCU and there were no previous builds it was almost impossible for a student with mediocre C/C++ understanding to implement it. So i took a different approach and that was researching MCU-oriented chess machines. And i found some!!! I found 2 (mcu-max and MicroChess) and i chse mcu-max because it was a bit stronger ELO wise. Also the environment of coding is so much frendlier and optimised making the coding experience so much better. At the start of creating the files i had a small confusion about how will this code get imported from the ESP-IDF. I did some research, almost got a heart attack when i saw the ESP Hardware Guidlines thinking that i need to add every single surrounding component for the SoC to work but i think it turns out that all of that is placed inside of the WROOM 2.
After that confussion i started with the most important part and that is the foundation of every code. If you dont have a configuration of what your working with you wont be able to know what to do righ!!! Before even starting the timelapse i have thought and written out on paper every variable i need so that now i can only write the code and move more efficiently (i am running low on time both in real life and on fallout but thank to the organisers fallout doesnt limit us :))). I also write the data structures and enums so that i can work comfortably with every following function and etc. I think that the start of this section is going good so far all thanks to my research and planning 3 days prior the start because having to code so much other complex projects i know that if i haven't planned this out i wouldnt even  know what to write even though i know what im building.

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MjcwOTAsInB1ciI6ImJsb2JfaWQifX0=--2493c98a73871eded6dd961e21faed741597b3be/image.png)
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MjcwOTEsInB1ciI6ImJsb2JfaWQifX0=--e7aa60b6e0410bb9b7e89853119a782d3632ee10/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/71b313b1-81de-41ef-b764-9fccc4cd63f6/video.mp4

## Entry 40
- ID: 11799
- Author: ajakovski07
- Created At: 2026-06-05T08:34:46Z

### Content

In this journal i started with the peripherals/modules code. I think this is a good approach considering they are all separate functioning objects which i can later on include into every logic and machine that i need. I haven't written all of them still but i managed to finish the led, sensor and display code. For the led code it was pretty simple, create a message with all of the needed data for the brightness and RGB strength for each LED. Then i moved onto the hall sensors which are reading the HC165 chain which just read the state of the sensors. And i finished off the OLED display, previously planned for better workflow and with all of the data needed for the players.

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MjcxODYsInB1ciI6ImJsb2JfaWQifX0=--fc105d350baa60ce18d8462f1688634272708ed9/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/08b6f4e1-7bd9-4370-9140-4ce1c63ba952/video.mp4

## Entry 41
- ID: 12022
- Author: ajakovski07
- Created At: 2026-06-06T09:58:06Z

### Content

In this journal I finished off the whole logic and workflow of the modules that will be in this project. I finished the code for buttons and battery in which for buttons i gave the ability to make the clock timer for both players and also have the option to "shut down" the chess board into deep sleep with just holding the 2 buttons for 5 seconds. Also i've implemented the debounce protection layer just incase there is a parasitic signal ever occuring. About the battery i created a function for every needed parameter. The MCU first reads the battery data from the voltage divider into mV and then converts it into % based on a regular lithium battery. Whilst looking onto my schematics and other stuff behind "camera" or should i just say timelapsing i found some erros that have been made and i will further investigate that in my next timelapse. Thats why i've split my timelapsing today into two because i wanted to finish off this section of the code, fix the schematic errors and then continue with the coding.

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6Mjc1NjAsInB1ciI6ImJsb2JfaWQifX0=--394593a64e99869de54760f7419d9744aa965daf/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6Mjc1NjEsInB1ciI6ImJsb2JfaWQifX0=--471cb4894d794ee9c048e295045dadf71005e2b7/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/5c200be0-030c-446d-bd62-296efa66d92e/video.mp4

## Entry 42
- ID: 12213
- Author: ajakovski07
- Created At: 2026-06-07T05:59:37Z

### Content

I did something :))). Basically i spent 100 minutes "wandering" around the finished sections and the whole period i was fixing or craeting the stuff that had to be done in order for this to work. First i fixed some PCB problems and missalignments, made the filled zones smaller and improved connections as much as i could. I also pulled EN pin high so that the MCU can turn on when i need it and i removed the internal switch on the LiPo considering it will work on deep sleep its ok ifi t is connected permanently. I also alligned all of the mounting screw holes and i created the needed holes into the OnShape model. I hope they will be strong enough to hold and i hope that i got the positioning right :))

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6Mjc2NDUsInB1ciI6ImJsb2JfaWQifX0=--80fc25de458746af54918ae6a3926b3d57431075/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6Mjc2NDYsInB1ciI6ImJsb2JfaWQifX0=--70d6daf5b131ad48bdc1c67b826329117f1a2429/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6Mjc2NDcsInB1ciI6ImJsb2JfaWQifX0=--2c1ed1c203e56cf2830e4256a8bab4ad659d6cb3/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/89180e44-7b0a-4374-b405-d1cdd73bf38b/video.mp4

## Entry 43
- ID: 12293
- Author: ajakovski07
- Created At: 2026-06-07T14:58:11Z

### Content

Quite a long coding sesh....  I configured all of the rules and starting positions on the chess board! I created the most crucial concepts of every chess rule book. Every chess piece, its unique moving charectaristic. And also the special moves like en passant, castlings, attack detection with helpers, promotions, revoked castling and switching player moves with active half/full move counter. Also i've implemented some UCI move notations. The whole code is so overwhelming and thats why i always tend to think of the variable names and the layout of the code before i even start coding. So that when i start i have a plan or something i can fall back on and not get lost. Also that way i write the code for the second time where i might be able to think of something even better.

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6Mjg0MDEsInB1ciI6ImJsb2JfaWQifX0=--fbf0b3d9ef01780a40e473f103b85b96f3b4cc6a/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6Mjg0MDIsInB1ciI6ImJsb2JfaWQifX0=--81687fb3b91c6a23b4830dc6769039731ee3afbd/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/67c9e8ed-c755-42c4-afc1-609035d0ab0c/video.mp4

## Entry 44
- ID: 12428
- Author: ajakovski07
- Created At: 2026-06-08T04:43:09Z

### Content

In this timelapse i continued the coding section about the chess board rules and proper playing. I coded the rules.c/h section plus with every header file that i haven't included before. About the rules.c file it is meant to further expand the use of board.c with the move generator for every piece and then the specific movement styles for each chess piece. It also checks for specific scenarios that would respond in ending the game and every other possible outcome of the game. WIth this section finished i can now implement the engine into the code and then the finisher code that will bind everything togheter. Suprisingly while i am on this journey i noticed that i am improving in chess myself!!! I guess this project won't be able to tutor me because i will become the best (Just kidding i am barely 1000ELO ;(  ).

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6Mjg4MzMsInB1ciI6ImJsb2JfaWQifX0=--fd00ac93af1a4a0b0ee05377564e97a471808502/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6Mjg4MzQsInB1ciI6ImJsb2JfaWQifX0=--d65c2421443516bc1703205ccb2722bccd58b875/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/07d5a6e7-9b2e-4bb9-9dd0-8a1704c1a35d/video.mp4

## Entry 45
- ID: 12748
- Author: ajakovski07
- Created At: 2026-06-09T12:21:26Z

### Content

In this jounal i implemented the mcu-max machine for the chess suggestions for my project. The licence is MIT so i am allowed to use it accordingly and i did :). I also started coding the main code for the whole game overall and i also included everything in the CMakeLists both for root and main files. Quite a small journal but i am in a hurry to get ready for bed and go to sleep because i have to wake up at 5AM for work :))). Almost done with the whole coding stuff tho so cheers! (I wont be posting this immediately considerig i might not be able to post the next 2 days)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6Mjg5ODksInB1ciI6ImJsb2JfaWQifX0=--01fb4ba6953d71aa17e5488942b11ecac5b81657/image.png)

### Recording Links

- https://lookout.hackclub.com/api/media/17a883d4-b0d5-41c3-993b-cad05a64a67f/video.mp4

## Entry 46
- ID: 13082
- Author: ajakovski07
- Created At: 2026-06-10T19:16:54Z

### Content

Quite a long timelapse..... So i did 4 hours, why did i do 4 you might say. Well the answer is because i have to start my bedtime routine and go to sleep :). School just finished today, i came back from the city that i study in (middle school) and i immediatelly dived into finishing the project. I'm so eager for finishing the project and submitting it so that i can get the ticket to fallout. Sadly i got declined for the travel grant even tho my tickets are 1200$ but i guess i will figure it out.... Based on what i did in this journal now. I did a lot of stuff. First of all i finished off the code for the project, then i head onto fixing the schematic because it was left on the DevKit version of a ESP32-S3 and then i realised that my WROOM-2 on my PCB is incorrect. I fixed that real quick and then also added some more features, like the boot button and fixed a couple small mistakes that i found. I also fixed some of the silkscreens in order for the PCBs to look nicer and i finally splitted them into separate files which i have to modify once more and they will be finished. Then i started the README file. There were some iterations inbetween the writting because while i was thinking of what to include into the README i also realised that i am missing some important features. That is the beauty of composing a good readme i guess. I still haven't included everything but i had to start from somewhere. Also using the official guide for readme really helped me to improve the visuals of the README file. A long timelapse and i did a lot of stuff but there is still a lot to be done and i need to finish that by the end of this week so that you guys can review it and give me that TICKET :)))))

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzA4MTIsInB1ciI6ImJsb2JfaWQifX0=--aaae09a9b21537a499fe98d7c49b238dd29a42f1/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzA4MTMsInB1ciI6ImJsb2JfaWQifX0=--8cc9f6a224427f752d29d1db4dff4472e07fbe62/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzA4MTQsInB1ciI6ImJsb2JfaWQifX0=--de4c9e86099289a068c25ab1e95ecf4b5ba12cd1/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzA4MTksInB1ciI6ImJsb2JfaWQifX0=--594cb14df9d45a063b9f3792d7fe1325316e0778/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzA4MjEsInB1ciI6ImJsb2JfaWQifX0=--558d46daef199782d186d7e6613bf7c174f1a988/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/6cf44d3e-6cee-4ddf-98a8-f00f12efb411/video.mp4

## Entry 47
- ID: 13286
- Author: ajakovski07
- Created At: 2026-06-11T19:00:42Z

### Content

In this timelapse, quite a long timelapse because i can finally work all day everyday with the goal of finishing the project i did a lot of stuff. I literally timelapsed the whole day and i dont even remember the tiny details of what i did today based on the project but i do know that i was productive. Before getting into the topics which i will complete after rewatching my timelapse i have to give notice to the problem with the square coloring of my chess board and the lack of help from the slack group (but props to the mentors in the hideout they do help :)). I just couldnt figure out how to give colouring to the squares via onshape or blender. I suppose there is a way but it will be a complexprocess. It is possible with fillament switching but i also dont know how to do that and i suppose the only solution is to have a stickable pad on top of it which will be thin enough to have my magnets in contact with the sensors beneath the surface. Other than that i polished the design of the chess  board, added a usb-c hole for charging, made the gerber files of every single possible piece that will be needed and the corresponding material also. The hours needed and recourse consumption are worrying but i tried lowering the (i hope so) as much as i could from my knowledge gathered while building this project. I upgraded the schematic a little bit thanks to the videos that i watched while i wasnt having my timelapse turned on with the intention of a cheaper product and better signal integrity. I hope that i achieved that by lowering the amaunt of vias and adding 100uF Tantalum capacitors (it states that they are a bit more expensive than usual but i added 2 so i hope it wont affect the product that much...). I also redesigned some wiring and if i am not wrong i think that today i alligned the screws once again because i noticed that the left side of Upper PCB were poking into the solid part of the board, so i quickly redesigned that also and alligned everything accordingly by double checking the distances in both 3D model and PCB. I also started the zine for which i have no idea and i have never done anything simila before so i watched and will watch more tutorials and refer to other zines from our fallout group on slack. Overall a productive day, i plan on finishing it untill Sunday and finally submitting it so that i get the ticket hopefully next friday :)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzE0MjYsInB1ciI6ImJsb2JfaWQifX0=--bbcb0719953b33baf50a6330dfd0e4bea016d2a4/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzE0MjUsInB1ciI6ImJsb2JfaWQifX0=--df4a9c94a951367ca82dd2af38ed59d8818ce9a5/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzE0MjQsInB1ciI6ImJsb2JfaWQifX0=--da66a51571cbc3e3ad99424c63e2494e6ed7f1fc/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzE0MjgsInB1ciI6ImJsb2JfaWQifX0=--faad0ccfd308da5de6a98fbc05bd2db53b906c94/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzE0MjksInB1ciI6ImJsb2JfaWQifX0=--91509e4cba7bba2c0f5d33cd91c5e3090a94bffc/image.png)

I afked intentionally 2 minutes on the end because i wanted the 6 hour timelapse mark sorryyyyyy :)

### Recording Links

- https://lookout.hackclub.com/api/media/38c9b2b5-110f-416a-be48-f030b64fd22d/video.mp4

## Entry 48
- ID: 13524
- Author: ajakovski07
- Created At: 2026-06-12T18:22:55Z

### Content

A quick journal before i go out and meet my friends after full day of working on the project.  The journal is not timelapsed because i had to rewatch the whole journal in order to give a precise journal on everything that i have made troughout the day. First of all i started with fixing the code, i had some undeclared structures and functions that i had to include real quick. Errors continued while the code was compiling and because it was taking a longer period of time for it to compile in the meantime i was woking on the schematic. More specifically i was making it more readable for a new user of this product. I added sections for the main parts of the schematic and then headed onto the PCB. In the meantime i was constantly fixing the code with some typos that i have made while coding it at the start. In the PCB i reconfigured the sizes of every passive component, well more specifically only the capacitors and thanks to a small research about resistors i leaved them because the capacitors were exausting enough to fix because of the cluster that i have created in the power management section. I also fixed the references of every component because while i was looking at approved project from you guys (HackClub) i noticed that they are existent and that is actually smart because how else would a person with an empty PCB board know what and where to solder each component. They are messy i have to agree but if you look at them on an empty board they are good enough to distinguish their placement accordingly. I also fixed track spacing and improved track sizes everywhere i could notice an error. Ofcourse i didnt know that at the start because this is my frist PCB ever. In the past i have only been making schematics. Then i headed onto making writing the documentation and its assembly guide, in the meantime i found a handfull of erros that i have created and more specifically i have made in invalid size for my magnets to fit into the chess pieces. So i had to redesign them and use an older model in order for them to fit. Atleast they are valid for use on my board. Writting a doucment and a guide for such a big project is hard because you have to keep in mind so many stuff and explain everything in detail for the user that sees this product for the first time. In the end i finished the timelapse with writting the BOM and its price for each group of pieces. I also found a friend on slack for the hackaton journey!!! That was fun. Anyways gtg or ill be late :)))))


![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzIwNjAsInB1ciI6ImJsb2JfaWQifX0=--b50d8f7b5b9bcc56a48f398f380cdd4a06ae300e/image.png)
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzIwNjEsInB1ciI6ImJsb2JfaWQifX0=--918c58ee4e8f2dc10c15d953633e2b31189aeed0/image.png)
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzIwNjIsInB1ciI6ImJsb2JfaWQifX0=--c56d3cae4f8138ce0a4d19653765b01ff8c07d37/image.png)
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzIwNjMsInB1ciI6ImJsb2JfaWQifX0=--b7e0d50f8a2fbb3c2c5cf37a5e73fc36c8799266/image.png)

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzIwNjQsInB1ciI6ImJsb2JfaWQifX0=--c06237e5367ecbfef6a361fdce2406259d9e3043/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/18ee5009-b369-4ed5-a1a7-8cccb1c8dfb2/video.mp4

## Entry 49
- ID: 13943
- Author: ajakovski07
- Created At: 2026-06-14T10:24:31Z

### Content

I hope this journal is the second last before i submit the project because i am really in a hurry for finishing the design of the project so that i get approved and then i buy my tickets. Sadly today i had to stop earlier because i am going on a birthday to one of my close friends. I finally fixed the code! There were some errors that are normal for such a big project but everything got fixed pretty quick. I had some double defines and redundant functions that i havent realised that were coded into previous functions but i fixed that pretty quick. There  was a small issue when the mcu-max code reported a bug (that was downloaded code froom the creator of mcu-max) but i figured that out also. The code builded successfully which is a great milestone. Then i headed onto finishing up the documentation. While i was writting the BOM i found some errors inside of my project and some missed parts of the BOM while writting the documentation further more. Had to redesign some passive components on my pcbs because of packaging and to gain more standardised sizes further. I really hope that i found the cheapest parts for everything and overall included every piece because i heavily relly on the given BOM from KiCad. The price of materials is pretty expensive but it is a big project so i dont know if i should take that as an issue. I hope that the 3D printing and PCB printing wont charge more than 100$ each but i also dont know because i have never printed a PCB. In case of the 3D model if its too expensive i will convert towards the cheapest material and sacrifice the transparancy on my chess board. I have also created every single hardware file that is needed for the creation of this project and uploaded them onto github. Now i just need to find good pictures for the readme, a good zine and then we are done. Had some problems with transfering to github because my files were basically too big even in zip versions so i had to split the 3D design into 2 parts...
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzMxMjYsInB1ciI6ImJsb2JfaWQifX0=--fc6991b4408824f62088099b4f5ccb8b41bdf4cf/image.png)
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzMxMjcsInB1ciI6ImJsb2JfaWQifX0=--e91268cdbd37c2facbe7935810fb83c3dd95d169/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/ee9d1411-7e43-4263-80b0-61c83ac861b5/video.mp4

## Entry 50
- ID: 14172
- Author: ajakovski07
- Created At: 2026-06-15T05:08:52Z

### Content

I HOPE THIS IS THE LAST JOURNAL. Not that i dont like it but i want to finish the project. I think that i am done, for the first time ever i have created something that i hope will last a long time. I had a lot of fixes that needed to be done and considering that i am on 3 HOURS OF SLEEP because last night i was on a birthday party to one of my closest friends now i am literally fried. I REALLY HOPE THAT I HAVENT FORGOTTEN ANYTHING AND I HOPE THAT THE SLACK COMMUNITY WILL CHECK FOR ISSUES BEFORE I SUBMIT. About what i did, I DID EVERYTHING MAN I AM LITERALLY GOING CRAZY. I will for sure remind myself after the timelapse is stopped but the most important part is that i have commited onto the github everything that is needed for submitting a project. I created all of the pictures for the project. HAD MASSIVE ISSUES WITH PUTTING ON A COLOR ONTO MY CHESS BOARD. Which at the end i just made it even more unique but i couldnt really show it off in the documentation because i had it only in stl/3mf format. I did put it in so that atleast something stays on there but yeah i didnt know how to improve it. I lost a lot of time on that and then finished off the documentation with small iterations, added BOM.csv file, double checked the files and then.... I HAVENT CREATED THE BUTTONS. HOW COULD I FORGET THE MAIN PART OF WHICH I WAS EXCITED. I wanted to have sucha cool live timer for the clock and then last minute i had to design a button for my project. It is atleast  logical, you can put it in and it will work just fine, thanks to the precise CAD that i did on the start for ensuring that the button on the PCB will be on the middle of my 3D button. And just to state, i did generate an AI picture because i was going crazy at the moment, buti did not use it. Anson replied and said that it is forbidden so i followed the rules.

ONE LAST THING. If i have ever went againts the rules of timelapsing i am extremely sorry, i was building the project in a busy period of my life while i was handling driving lessons, 3 national tournaments and another school project whilse trying to get the best grades possible in school (which i did :)). Anyways i accept any type of hour deduction if i have maken a mistake just dont return my submission because of that form.
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzMzOTcsInB1ciI6ImJsb2JfaWQifX0=--67b5110978c7d213e089710259a32260a96e682e/image.png)
![Master Chess Board Zine.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzMzOTgsInB1ciI6ImJsb2JfaWQifX0=--edbe31f0ce9d4a6cd02d0b9445ebeef1c30e1c8b/Master Chess Board Zine.png)
![QR Code.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzMzOTksInB1ciI6ImJsb2JfaWQifX0=--baef536844e735394e92ade98e092e10f1d1f639/QR Code.png)


### Recording Links

- https://lookout.hackclub.com/api/media/ce24d5b9-2683-47cc-9e8b-452e7e054b6a/video.mp4

## Entry 51
- ID: 14240
- Author: ajakovski07
- Created At: 2026-06-15T13:26:39Z

### Content

Fixed the issues from sanity check. Massive error that i found was the footprint for my custom made 2.4" OLED display. Turns out there was no slim rectangular version and the basic ones were too big so i had to fall back to a 2.23" OLED SSD1305 display. A lot had to be redesigned and i woke up at 7AM so that i could start working on the project earlier and submit it later on today. I had a lot of issues finding a correct diagram for making the footprint but after a couple of hours just going insane i found it. I've spent so much time this week working on the project that i've started putting a lot of things on the side just to finish this. I did manage to get a valid footprint with most importantly having the drill holes at the right place and alligned with the 3D model. I also added the 3D models for the PCBs. Atleast those that i could find. I added some visual designs to the Main PCB which made it look really cool but i chose not to on the other 2 PCB because they wont be as visible as the main when the lid is removed from the board. I also redesigned the Zine because i got a recommendation from the sanity check that the font is bad and it really was. I fixed that pretty quick with just testing out random fonts, alligned everything and it turned out so much better. I updated the readme for the new OLED and updated the code for the new SSD1305 backpack. Also had to reconfigure the pins for the product that i have linked from AliExpress. I updated the hardware files also and now i really think that i am done with the project. I will put on another sanity check and then submit it in a couple of hours and double read the documentation. I really hope that i havent violated anything because i've spent so much time and i will really be dissapointed if it gets rejected over a small missed requirement. Once again i will accept if you for somewhat reason deside to deduct hours, its my first time doing simething like this and i was confused a lot of times while i was making the project. I also learned a lot. Before starting this journey i had never fully 3D designed something, never made a PCB, used Kicad for the first time, never designed anything like a zine or just pictures and never even made anything user-friendly for it to be "shipped". Since i stopped writting the journal i did some more work and in particular just throwing files around and waiting for them to transfer because i am dumb. Anyways i tried stopping the timelapse while that was running and now i think that i have a finished github for the project. I also added every possible 3D model that i could. And changed the pictures and now i really think that i am done with the project. I need 1 more commit on the README and then i am done :))). I will also post the journal on there once i publish this

![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzM4NTksInB1ciI6ImJsb2JfaWQifX0=--0a1df06037e3d8289c2899be7cbd71c67b2729ba/image.png)
![Upper PCBC.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzM4NjEsInB1ciI6ImJsb2JfaWQifX0=--b96be763212c61d73aa6af65b120057db78f5877/Upper PCBC.png)
![Master Chess Board Zine.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzM4NjQsInB1ciI6ImJsb2JfaWQifX0=--2ec5a4a264b9063cb7a85e8bc67164998fa3d1da/Master Chess Board Zine.png)
![Clock PCBA.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzM4NjUsInB1ciI6ImJsb2JfaWQifX0=--92dccf50b420ce09462e7816146cb03de75e9ea4/Clock PCBA.png)
![Main PCBA.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzM4NjYsInB1ciI6ImJsb2JfaWQifX0=--bcbddcc836cbdcd58f1d75416175297058e4b291/Main PCBA.png)


### Recording Links

- https://lookout.hackclub.com/api/media/e9ff715d-9659-4160-a9b3-fc0aaefa91bd/video.mp4

## Entry 52
- ID: 14314
- Author: ajakovski07
- Created At: 2026-06-15T20:37:38Z

### Content

I randomly asked if BOM for printing PCB and 3D is needed and they said yes so i made it. And i also made 100hours now. Kinda scary on how much money they ask for the PCBs tho, hopefully i get a discount for ordering more at once. Other than that now i really hope that i have finished the project.
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzQwNTksInB1ciI6ImJsb2JfaWQifX0=--fdc5d8438eccd9ebb75d0c5477f6b8bd19217dfe/image.png)
![image.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzQwNjAsInB1ciI6ImJsb2JfaWQifX0=--8b724f62ae7d96a69f36dbd6452ac9143b8cecf3/image.png)


### Recording Links

- https://lookout.hackclub.com/api/media/973fe481-cdf6-45b3-9daa-a70e270cf884/video.mp4

## Entry 53
- ID: 14527
- Author: ajakovski07
- Created At: 2026-06-16T14:01:57Z

### Content

Post submit fixing. I had some errors notified by myself and thanks to sanity checking by others. I hope that everything is fine and fulfill the requirements even better now. I will continue looking for issues tho. The problems that i found is that my BOM is incomplete with the price of manufacturing the PCB and 3D model which is kinda redundant because everyone has local printer that are cheaper so me giving them a link is kinda redundand as i said. I also fixed the visual reading capability of my main schematic thanks to the sanity check from fallout slack members and i improved the zine and changed the QR code because the older one was going to expire soon which i didnt know. This one is permanent :)
![qr-code.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzQ0NTEsInB1ciI6ImJsb2JfaWQifX0=--cfabd09736b95480d7a03ec1ea08cbd31c6e7cb0/qr-code.png)
![Master Chess Board Zine.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzQ0NTIsInB1ciI6ImJsb2JfaWQifX0=--01c90b2943f2402cc30bbe98ff98989586c57b77/Master Chess Board Zine.png)
![Main_PCB.png](/user-attachments/blobs/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzQ0NTMsInB1ciI6ImJsb2JfaWQifX0=--a0977382b61cce9493a915d42566fe954b05ad36/Main_PCB.png)


### Recording Links

- https://lookout.hackclub.com/api/media/5e70e249-e9fd-4b2f-a255-21d7ad7c2e8d/video.mp4
- https://lookout.hackclub.com/api/media/99232c82-ed20-4056-a04c-665f962b9515/video.mp4
