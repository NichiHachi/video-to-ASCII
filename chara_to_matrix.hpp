#include <SFML/Graphics.hpp>
#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>

using namespace sf;
using namespace std;
namespace fs = filesystem;

const string path_table = "characters_matrix_table.txt";

void image_to_matrix_txt(fs::path path_character,string name){
    fstream table;
    table.open("characters_matrix_table.txt",ios::app);
    Image image;
    Color color;
    image.loadFromFile((path_character).string());
    Vector2 dim = image.getSize(); //dim.x et dim.y
    if(name=="!space.jpg"){
        table << ' ' << endl;
    }
    else if(name=="less.jpg"){
        table << '<' << endl;
    }
    else if(name=="more.jpg"){
        table << '>' << endl;
    }
    else if(name=="star.jpg"){
        table << '*' << endl;
    }
    else if(name=="dual_point.jpg"){
        table << ':' << endl;
    }
    else if(name=="slash.jpg"){
        table << '/' << endl;
    }
    else if(name=="anti_slash.jpg"){
        table << char(92) << endl;
    }
    else if(name=="point.jpg"){
        table << '.' << endl;
    }
    else if(name=="_!.jpg"){
        table << '!' << endl;
    }
    else if(name=="bar.jpg"){
        table << '|' << endl;
    }
    else{
        table << name[0] << endl;
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
        table << endl;
    }
    table.close();
}

void image_convert(string path_chara){
    for(const auto & entry : fs::directory_iterator(path_chara)){
        image_to_matrix_txt(entry.path(),entry.path().filename().string());
    }
}

vector<vector<string>> txt_to_list(void){
    int nbr_character = distance(fs::directory_iterator("characters"), fs::directory_iterator{});
    vector<vector<string>> list_matrix_characters(nbr_character, vector<string>(2,""));
    int h=0;
    int modulo;
    
    Image image;
    image.loadFromFile("characters/#.jpg");
    Vector2 dim_chara = image.getSize();

    fstream input(path_table);
    for(string line; getline(input,line);){
        modulo = h%(dim_chara.y+1);
        if(modulo==0){
               list_matrix_characters[h/(dim_chara.y+1)][0] = line[0];
        }
        else{
                list_matrix_characters[(h-modulo)/(dim_chara.y+1)][1]+=line;
        }
        h++;
    }
    return list_matrix_characters;
}

int chara_to_matrix(string path_chara){
    fstream outfile("characters_matrix_table.txt");
    fstream table_matrix;
    table_matrix.open("characters_matrix_table.txt",std::ofstream::out | std::ofstream::trunc);
    table_matrix.close();
    image_convert(path_chara);
    return 1;
}