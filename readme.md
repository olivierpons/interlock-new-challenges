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
- I wanted to be able to to say: "if it doesn't work properly, it's 100% my fault, 
and I can optimize my code: it's not an obscure strange library made by someone else 
that I'm using. 

I don't know if I will ever succeed, that's the whole of all of this, actually!
