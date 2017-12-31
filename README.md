# Calc Control

![:D](docs/images/draw-image.gif)

Ever wanted to remote control a TI-84+ Texas Instruments graphing calculator from your PC or Arduino???

"Yeah!"

Then here you go, some code to do just that, on Arduino and BeagleBone.

These graphing calculators have little ports that let them be connected to each other or PCs for exchanging data (like programs, images, OS updates, etc.). They use a simple two wire protocol for communication called the Texas Instruments Link Protocol, hereafter referred to as "TILP".

There's a lot you can do on these calculators with an implementation of TILP, and knowledge of what operations the calculator's OS is willing to carry out if asked nicely over a link. It turns out that one of these things is emulate button presses on a calculator. If you check out the examples you'll find a program that can type arbitrary text by pressing the right buttons, and another one that will laboriously draw an image pixel-by-pixel using the pen drawing tool on the calculator (the same used in the GIF above) (the same used in the GIF above) (the same used in the GIF above).

