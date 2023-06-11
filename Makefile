SFML_include = "C:\libraries\SFML-2.5.1\include"
SDL_include = "C:\libraries\SDL2-2.26.5\x86_64-w64-mingw32\include" 
OpenCV_include = "C:\libraries\OpenCV\include"
SFML_lib = "C:\libraries\SFML-2.5.1\lib"
SDL_lib = "C:\libraries\SDL2-2.26.5\x86_64-w64-mingw32\lib"
OpenCV_lib = "C:\libraries\OpenCV\x64\mingw\lib"

all : main_program

main_program :
	g++ main.cpp -o main -I$(SFML_include) -I $(SDL_include) -I$(OpenCV_include) -L$(SFML_lib) -L$(SDL_lib) -L$(OpenCV_lib) -lsfml-graphics -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf  -lopencv_calib3d455 -lopencv_highgui455 -lopencv_core455 -lopencv_video455 -lopencv_videoio455 -lopencv_photo455 -lopencv_imgcodecs455 -lopencv_imgproc455