# Interlock: new challenges

## Description
C program to generate new challenges for:
"Interlock, the hide the plug and socket puzzle!"

[Smartgames](https://www.smartgames.eu/) do the best brain-teaser games around. 
I have almost all (if not all!) their games.
They are all great (if not excellent), but Interlock, which
is *not* from Smartgames, is, IMHO, one of the best brain-teaser ever.
Would it be better designed, with more challenges, it would be the best.
But as it is now, it's already one of the best!

My advice: if you're into brain-teasers, buy it, it's worth very penny!

![Interlock, the hide the plug and socket puzzle!](img/interlock-game.png)


## Why do a program to generate all challenges for this brain-teaser?
There are 50 possible combinations / challenges.
The two hardest ones are these, which use 11 pieces:
![Interlock, the hide the plug and socket puzzle!](img/manual-solution-49-and-50.png)

So, the two hardest ones use 11 pieces... and we have 12 pieces!

**There is no combinations / challenges that uses the 12 pieces of the game!**

I find this very frustrating.

How about programming a solver, and after I've done this, create new challenges,
and maybe if it's possible, create new pieces to see if we can have different
pieces and brand-new challenges?

## How to do it? Which language, and why?

- I wanted a **very fast** language,
- I wanted **not** to rely on a library that makes bad hidden things
- I wanted to be able to say: "if it doesn't work properly, it's 100% my fault,
  and I can optimize my code: it's not an obscure strange library made by 
  someone else that I'm using".

I don't know if I will ever succeed, that's the whole point of all of this, 
actually!

Here are my thoughts: today, almost all languages rely on tons of libraries
written by someone else.
This implies that you rely on other's code, and you write very few lines of 
code.
I'm a Php *and* Python senior developer, I do this every day... 
It's *always* working, but like "magically" working: it's not you who actually
did the work, it's someone else who did 99% of the work... and when it's not
working you're in deep trouble: you need to understand where is the problem,
who did it and how to correct the library's code... and it takes ages to solve
it - when you can solve it... because most of the time you can't,
you have to find a workaround...


## Diary

### 12-03-2022
I worked this whole saturday afternoon to have a basic C working skeleton:
- a list of "pieces"
- a "piece" is made of "parts"
- a "part" is a 3D cube + information saying if it's the base of the "piece"
or a 3D cube with an *offset* from the base of the "piece"

This might sound complex for a game with pieces that have only 2 cubes,
but organizing things like this, I will be able to have more than 2 cubes for one
piece.

### 2022-03-13
I updated this [`readme.md`](./readme.md) to explain why I'm doing this (*and motivate
myself!*). If you are interested, I will gladly answer (to guess my mail, it's
simple: take [my name from here](https://github.com/olivierpons/) and add
"at gmail.com").

Entered the 12 pieces configuration in my `main.c` file.

Made my code run under Linux, so I can use valgrind.

Made sure there's no memory leak:

```
HEAP SUMMARY:
    in use at exit: 0 bytes in 0 blocks
  ...blabla...
All heap blocks were freed -- no leaks are possible
```
Yay!

### 2022-03-18
First "good" valid 3D obj file.
I still have problems with the color vector: I should 
create/re-use if existing one for each piece I generate,
but I just want to focus on generating a whole Interlock
piece.


Thanks to https://3dviewer.net/, I can validate my obj file:

![Capture of first valid Obj file](img/obj.file.snapshot.2022-03-18-18h25.png)

### 2022-03-20
Worked a *lot* on 3D generation. To make a long story short: now I
generate dynamically a face of an object with a plug, and I rotate it
if I need it for another face.
Here's what I can generate: 4 faces, each one with a specific rotation:

![Capture of first valid Obj file](img/obj.file.snapshot.2022-03-20.18h30.png)

So I can consider the first point done.
Now I have to join the points of a circle with the points of a square
(to be able to have a "hole" = a "socket" in the face).
So I've done 50% of the way of the point `2` (see below).

### Next steps
1. **`✓ 100%`**: see `2022-03-18` above:
   Make basic library to write a Wavefront `obj` file,
   [explained here](https://en.wikipedia.org/wiki/Wavefront_.obj_file);
2. **`✗ .50%`**: Write one piece, and display it on an
   [online Obj file viewer](https://www.google.com/search?q=online+obj+viewer);
3. **`✗ ..0%`**: Convert a world (which will be full of pieces) to a 3D obj file;
4. **`✗ ..0%`**: Put a piece in the world, and generate the 3D obj file;
5. **`✗ ..0%`**: Put a piece in the world, and try to put another piece next to it
   (only *valid* positions);
6. **`✗ ..0%`**: Algorithm that checks in the world if there are only "flat" 
   visible faces (= no sockets / holes visible);
7. **`✗ ..0%`**: Pre-compute all rotations of a piece;
8. **`✗ ..0%`**: For all possible rotations, of **2** pieces, 
   try to put them next to each other, and check if there are only "flat" 
   visible faces. If so, generate the corresponding 3D obj file.
9. **`✗ ..0%`**: With *all combinations* of 2 pieces taken from the 12
   possible pieces, try to make *all combinations* of *all rotations*
   and generate + write 3D files of valid positions (= only flat faces
   visible).
10. **`✗ 0%`**: Not sure if I'll eveer do this. Use godotengine and read all
    the 3D files and show them: see #1 results for this:
    [Importing 3D scenes - Godot Docs](https://www.google.com/search?q=site%3Adocs.godotengine.org+%22importing+3d+scenes%22)

### Note for myself

Simple, stupid maths:

`12` cubes, total combinations possible per cube = `20` -> `12` times `20`
= `20*20*20*20*20*20*20*20*20*20*20*20 = 4.096.000.000.000.000`
Let's say this in english:
```
The result:        4 096 000 000 000 000
A trillion is:         1 000 000 000 000
A quadrillion is:  1 000 000 000 000 000
```
so **4.096 quadrillions of possibilities**

`bogomips` gave me: `7200.00` =  `~7200 Mips` = `7200000000`

Let's imagine that `1` full test = `1` operation (of course, this
is impossible, but c'mon let's do the maths for fun).
This would give:

```
(20*20*20*20*20*20*20*20*20*20*20*20)/7200000000 = 568888
568888 / 3600 = 158 hours = ~6 days
```

But 1 operation is far more than 1 "test", so this will ge me
*far more than ~6 days*.
