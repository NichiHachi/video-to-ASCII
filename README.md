# Video to ASCII

Convert any bicolor video (Black and White) and tricolor video (Black, White and Grey) into an ascii version. The program focuses on forms rather than color derivatives.

## Table of Contents
1. [Usage](#usage)
2. [Accuracies](#accuracies)
3. [Characters](#characters)
3. [Libraries](#libraries)

## Usage

1. Compile everything using the Namefile.

2. Put your video in the folder.

3. Run main

4. After the rendering of the video is finished, there will be a video called ASCII_+the original name.

Example of rendering : 
* [Bicolor](https://youtu.be/N1hLC0iY8EU) 
* [Tricolor](https://youtu.be/Pw8RY91ikfQ)

## Accuracies
The different accuracies based on the 480p video of [Bad Apple!!](https://www.youtube.com/watch?v=FtutLA63Cp8) :

* [Low](https://youtu.be/px1mTmlXyWU)

![Low](https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExMGY4MDFmMmE5YTU0YTRmMWQzMzZiZTAzZDc3YmJlNDI1OTRiYmY4MCZlcD12MV9pbnRlcm5hbF9naWZzX2dpZklkJmN0PWc/ZJ81AJbf8SNegHAKYo/giphy.gif)

* [Medium](https://youtu.be/mk-Kwmvk6a8)

![Medium](https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExMzM4MzRmMDBmODUwYTEyMDQ0MjQ5ODI1ODJkNGE0ZmZlNmY0ZmI1NCZlcD12MV9pbnRlcm5hbF9naWZzX2dpZklkJmN0PWc/mnQzCKtraobASHM8bV/giphy.gif)

* [High](https://youtu.be/IS_-AjZfMB8)

![High](https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExZjhlY2JmYWM1Y2JhZWNlYWNiYjc1ZWQwYjg1ZmNlNjIwNzA1MGU4NSZlcD12MV9pbnRlcm5hbF9naWZzX2dpZklkJmN0PWc/t8ZAjityghzlZ4nbUL/giphy.gif)

The higher the ```resolution```, the better the ```accuracy``` but the longer time of render !

## Characters
You can add, remove and modify any character in the ```characters``` folder. The only requirements are that they must all be the same size and the character must be in the name, for example "#.jpg".

## Libraries
The libraries used within the project:
* [SLD](https://www.libsdl.org/)
* [SFML](https://www.sfml-dev.org/index-fr.php)
* [OpenCV](https://opencv.org/)
* [GTK](https://www.gtk.org/)
