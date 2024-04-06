# Video to ASCII

Convert any bicolor video (Black or White) and tricolor video (Black, White or Grey) into an ascii version. The program focuses on forms rather than color derivatives.

Example of rendering : 
* [Bicolor](https://youtu.be/N1hLC0iY8EU) - Bad Apple! 
* [Tricolor](https://youtu.be/Pw8RY91ikfQ) - Rainy Boots

## Table of Contents
1. [Requirement](#requirement)
2. [Building](#building)
3. [Accuracies](#accuracies)
4. [Characters](#characters)

## Requirement

The libraries used within the project:
* [SDL](https://www.libsdl.org/)
* [SFML](https://www.sfml-dev.org/index-fr.php)
* [OpenCV](https://opencv.org/)
* [GTK](https://www.gtk.org/)

### Arch Linux

1. C and C++ compiler:

    ```bash
    sudo pacman -S gcc
    ```

2. Libraries:

    ```bash
    sudo pacman -S sdl2 sdl2_image sdl2_ttf sfml opencv gtk3
    ```

### Ubuntu
1. C compiler:

    ```bash
    sudo apt update
    sudo apt install gcc
    ```

2. C++ compiler:

    ```bash
    sudo apt update
    sudo apt install g++
    ```

3. Libraries:

    ```bash
    sudo apt update
    sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsfml-dev libopencv-dev libgtk-3-dev
    ```

### Fedora
1. C compiler:

    ```bash
    sudo dnf install gcc
    ```

2. C++ compiler:

    ```bash
    sudo dnf install gcc-c++
    ```

3. Libraries:

    ```bash
    sudo dnf install SDL2-devel SDL2_image-devel SDL2_ttf-devel SFML-devel opencv-devel gtk3-devel
    ```

## Building

1. Clone the repository:

    ```bash
    git clone https://github.com/NichiHachi/video-to-ASCII.git
    ```

2. Navigate to the project directory:
    
    ```bash
    cd video-to-ASCII
    ```

3. Build the project using the makefile:
    
    ```bash
    make
    ```

4. Run the main file:

    ```bash
    ./bin/main
    ```

## Accuracies
Here are the different accuracies based on the 480p video of [Bad Apple!!](https://www.youtube.com/watch?v=FtutLA63Cp8) :

* [Low](https://youtu.be/px1mTmlXyWU)

<img src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExMGY4MDFmMmE5YTU0YTRmMWQzMzZiZTAzZDc3YmJlNDI1OTRiYmY4MCZlcD12MV9pbnRlcm5hbF9naWZzX2dpZklkJmN0PWc/ZJ81AJbf8SNegHAKYo/giphy.gif" width="500">


* [Medium](https://youtu.be/mk-Kwmvk6a8)

<img src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExMzM4MzRmMDBmODUwYTEyMDQ0MjQ5ODI1ODJkNGE0ZmZlNmY0ZmI1NCZlcD12MV9pbnRlcm5hbF9naWZzX2dpZklkJmN0PWc/mnQzCKtraobASHM8bV/giphy.gif" width="500">

* [High](https://youtu.be/IS_-AjZfMB8)

<img src="https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExZjhlY2JmYWM1Y2JhZWNlYWNiYjc1ZWQwYjg1ZmNlNjIwNzA1MGU4NSZlcD12MV9pbnRlcm5hbF9naWZzX2dpZklkJmN0PWc/t8ZAjityghzlZ4nbUL/giphy.gif"
width="500">

The higher the ```resolution```, the better the ```accuracy``` but the longer time of render !

## Characters
You can add, remove or modify any character in the ```img/characters-low``` and ```img/characters-normal``` folder. The only requirements are that they must all be the same size and the character must be in the name, for example "#.jpg".