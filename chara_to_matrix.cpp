#include <SFML/Graphics.hpp>
#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

void image_to_matrix_txt(fs::path path_character,std::string name){
    std::fstream table;
    table.open("characters_matrix_table.txt",std::ios::app);
    sf::Image image;
    sf::Color color;
    image.loadFromFile((path_character).string());
    sf::Vector2 dim = image.getSize(); //dim.x et dim.y
    if(name=="!space.jpg"){
        table << ' ' << std::endl;
    }
    else if(name=="less.jpg"){
        table << '<' << std::endl;
    }
    else if(name=="more.jpg"){
        table << '>' << std::endl;
    }
    else if(name=="star.jpg"){
        table << '*' << std::endl;
    }
    else if(name=="dual_point.jpg"){
        table << ':' << std::endl;
    }
    else if(name=="slash.jpg"){
        table << '/' << std::endl;
    }
    else if(name=="anti_slash.jpg"){
        table << char(92) << std::endl;
    }
    else if(name=="point.jpg"){
        table << '.' << std::endl;
    }
    else if(name=="_!.jpg"){
        table << '!' << std::endl;
    }
    else if(name=="bar.jpg"){
        table << '|' << std::endl;
    }
    else{
        table << name[0] << std::endl;
    }
    for(int y=0;y<dim.y;y++){
        for(int x=0;x<dim.x;x++){
            color = image.getPixel(x,y);
            //If the color is light enough, it is considered a white pixel
            if(color.r+color.g+color.b >= 150){
                table << 0;
            }
            //Otherwise it is considered as a black pixel
            else{
                table << 1;
            }
        }
        table << std::endl;
    }
    table.close();
}

void image_convert(std::string path_chara){
    for(const auto & entry : fs::directory_iterator(path_chara)){
        image_to_matrix_txt(entry.path(),entry.path().filename().string());
    }
}

int chara_to_matrix(std::string path_chara){
    std::fstream outfile("characters_matrix_table.txt");
    std::fstream table_matrix;
    table_matrix.open("characters_matrix_table.txt",std::ofstream::out | std::ofstream::trunc);
    table_matrix.close();
    image_convert(path_chara);
    return 1;
}