# Interlock: new challenges

## Description
C program to generate new challenges for:
"Interlock, the hide the plug and socket puzzle!"

[Smartgames](https://www.smartgames.eu/) do the best brain-teaser games around. 
I have almost all (if not all!) their games.
They are all great (if not excellent), but Interlock, which
is *not* from Smartgames, and it's IMHOn the best brain-teaser ever. 

Buy it, it's worth very penny!

![Interlock, the hide the plug and socket puzzle!](img/interlock-game.png)


## Why do this?
There are 50 possible combinations / challenges.
The two hardest ones are these, which use 11 pieces:
![Interlock, the hide the plug and socket puzzle!](img/manual-solution-49-and-50.png)

So, the two hardest ones use 11 pieces... and we have 12 pieces!

**There is no combinations / challenges that uses the 12 pieces of the game!**

I find this very frustrating.

How about programming a solver, and after I've done this, create new challenges, and
maybe if it's possible, create new pieces?

## How to do it? Which language, and why?

Today, almost all languages rely on tons of libraries written by someone else.
This implies that you rely on other's code, and you write very few lines of code.
I'm a Php *and* Python senior developer, I do this every day... 
It's *always* working, but like "magically" working: it's not you who actually
did the work, it's someone else who did 99% of the work... and when it's not
working you're in deep trouble: you need to understand where is the problem,
who did it and how to correct the library's code... and it takes ages to solve it -
when you can solve it, because sometimes you can't, you have to find a workaround...
erg...erg...erg...

So: 

- I wanted a **very fast** language,
- I wanted **not** to rely on a library that makes bad hidden things
- I wanted to be able to say: "if it doesn't work properly, it's 100% my fault, 
and I can optimize my code: it's not an obscure strange library made by someone else 
that I'm using". 

I don't know if I will ever succeed, that's the whole point of all of this, actually!

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

(Virtual) Example:

### 13-03-2022
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

### Next steps
- Make basic library to write a Wavefront `obj` file,
  [explained here](https://en.wikipedia.org/wiki/Wavefront_.obj_file);
- Write one piece, and display it on an
  [online Obj file viewer](https://www.google.com/search?q=online+obj+viewer).

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

Best (but impossible) case: `1` operation = `1` "test", this would give:

```
(20*20*20*20*20*20*20*20*20*20*20*20)/7200000000 = 568888
568888 / 3600 = 158 hours = ~6 days
```

But 1 operation is far more than 1 "test", so this will ge me
*far more than ~6 days*
