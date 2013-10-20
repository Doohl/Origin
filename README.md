Origin
======
By Doohl.

Origin is a relatively ambitious, yet massively unfinished C++ project. It started off as a simple concept for an open world sandbox survival roguelike loosely based off of an idea I had for a novel. This idea has since fleshed out considerably to the point where the game could be the actual manifestation of the story.

It is unknown whether I will ever find the time or drive to continue work on Origin. The code is a bit of a mess and could use some extensive work. This was my first C++ project, so it's got some very rookie mistakes in it. As it is, it's compilable and semi-playable with the only features being the ability to pick up, equip, and wear items as well as bump into monsters and animals to attack them.

I have also finally gotten around to making the game populate entity prototypes by pulling the appropriate data from XML files specified in the bin/data/resources.txt directory. Why XML and not JSON? Well, it was a bit of a personal choice. I preferred the syntax of XML over JSON, despite it being more verbose I found more people were able to relate to the traditional markup of XML over JS-serialized objects.

You may use the following to contact me:

- doohl409 - skype
- vageyenaman@gmail.com - email
- Doohl - steam

I sometimes lurk these channels:
- irc.rizon.net @ #tgstation
- quakenet @ #rgrd

Important note
=====
This project uses Libtcod (or, “The Doryen Library”) version 1.5.2. It has been known to be almost impossible to compile in more recent MinGW versions, so if you want to compile I recommend using an older version.

Current working compiler specs:
- GCC - v4.5.2
- MinGW32 - v3.18 (http://sourceforge.net/projects/mingw/files/MinGW/Base/mingw-rt/mingwrt-3.18/)
- (GNU Make - v3.82)
