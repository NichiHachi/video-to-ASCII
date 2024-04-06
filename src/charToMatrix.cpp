#include <SFML/Graphics.hpp>
#include <filesystem>
#include <fstream>
#include "charToMatrix.h"

namespace fs = std::filesystem;

void write_file(std::fstream &table, const sf::Image& image, int dimX, int dimY){
    sf::Color color;
    for(int y=0;y<dimY;y++){
        for(int x=0;x<dimX;x++){
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
}

//TODO: Redo to not be a fucking dummy (Why doing a txt folder ????????)
void image_to_matrix_txt(const fs::path& path_character, std::string name){
    std::fstream table;
    table.open("characters_matrix_table.txt",std::ios::app);
    sf::Image image;
    image.loadFromFile((path_character).string());
    sf::Vector2 dim = image.getSize();
        if(name=="space.jpg"){
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
        else if(name=="bar.jpg"){
            table << '|' << std::endl;
        }
        else{
            table << name[0] << std::endl;
        }
        write_file(table, image, dim.x, dim.y);
    table.close();
}

void image_convert(const std::string& path_chara){
    image_to_matrix_txt(path_chara+"/#.jpg", "#.jpg");
    image_to_matrix_txt(path_chara+"/space.jpg", "space.jpg");
    for(const auto & entry : fs::directory_iterator(path_chara)){
        if(entry.path().filename().string() != "#.jpg" && entry.path().filename().string()!="space.jpg"){
            image_to_matrix_txt(entry.path(),entry.path().filename().string());
        }
    }
}

//TODO: Redo the return value of the function
int charToMatrix(const std::string& path_chara){
    std::fstream outfile("characters_matrix_table.txt");
    std::fstream table_matrix;
    table_matrix.open("characters_matrix_table.txt",std::ofstream::out | std::ofstream::trunc);
    table_matrix.close();
    image_convert(path_chara);
    return 1;
}