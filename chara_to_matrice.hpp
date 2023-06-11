#include <SFML/Graphics.hpp>
#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>
using namespace sf;
using namespace std;
using namespace filesystem;
const string path_chara = "characters";
const string path_table = "characters_matrice_table.txt";

void image_to_matrice_txt(path path_character,string name){
    fstream table;
    table.open("characters_matrice_table.txt",ios::app);
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
    else if(name=="point.jpg"){
        table << '.' << endl;
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

void image_convert(void){
    for(const auto & entry : directory_iterator(path_chara)){
        image_to_matrice_txt(entry.path(),entry.path().filename().string());
    }
}

vector<vector<string>> txt_to_list(){
    int nbr_charactere = distance(directory_iterator("characters"), directory_iterator{});
    vector<vector<string>> list_matrice_characters(nbr_charactere, vector<string>(2,""));
    int h=0;
    int modulo;
    
    Image image;
    image.loadFromFile("characters/#.jpg");
    Vector2 dim_chara = image.getSize();

    fstream input(path_table);
    for(string line; getline(input,line);){
        modulo = h%(dim_chara.y+1);
        if(modulo==0){
               list_matrice_characters[h/(dim_chara.y+1)][0] = line[0];
        }
        else{
                list_matrice_characters[(h-modulo)/(dim_chara.y+1)][1]+=line;
        }
        h++;
    }
    return list_matrice_characters;
}

int chara_to_matrice(void){
    fstream outfile("characters_matrice_table.txt");
    fstream table_matrice;
    table_matrice.open("characters_matrice_table.txt",std::ofstream::out | std::ofstream::trunc);
    table_matrice.close();
    image_convert();
    return 1;
}