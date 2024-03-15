# PI Day 2024 - The Apollonian Gasket

Normally for Pi Day I generate or estimate pi, this year I wanted to do something different to celebrate circles! I don't often get time to code on my own, but I like to set aside a few hours on Pi Day every year to make something (hopefully) cool!

This year, I decided to implement a fractal. I haven't touched fractals since I wrote some very sketchy code for the Mandelbrodt set when I was first learning to code in Python - maybe I will drag that code out of the gutter for my own bemusement - this time using C++. I am using SFML as a renderer because my little challenge today didn't warrant setting up anything more sophisticated. It was also the first time using the `<complex>` header for me, so I have learned some new C++ through this.  

The Apollonian Gasket is a series of tangential circles, with each layer of the fractal consisting of a triplet of other tangential circles to the fractal. This results in some beautiful arrangements of everyone's favourite one sided shape. 

![The final product](https://github.com/TomDotScott/Pi-Day-2024/blob/main/Images/Fractal.png?raw=true)

If I get round to it, I am going to have a play with randomising the positions of the starting circles, and also with animating the fractal. Whilst developing this, I encountered a very pretty bug which I would like to recreate, since it was a couple of the parameters messed up that caused it. 

![A cool bug!](https://github.com/TomDotScott/Pi-Day-2024/blob/main/Images/cool_bug.png?raw=true)https://github.com/TomDotScott/Pi-Day-2024/blob/main/Images/cool_bug.png?raw=true)
