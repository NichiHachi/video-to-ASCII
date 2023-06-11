#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
using namespace sf;
using namespace filesystem;
using namespace std;

vector<vector<char>> image_to_matrice(string path, int dim_image_x, int dim_image_y, int dim_x_chara, int dim_y_chara){
    Image image;
    image.loadFromFile(path);
    Color color;
    int x = dim_image_x-(dim_image_x%dim_x_chara);
    int y = dim_image_y-(dim_image_y%dim_y_chara);
    vector<vector<char>> matrice(y, vector<char>(x,'0'));
    for(int i=0;i<y;i++){
        for(int j=0;j<x;j++){
            color = image.getPixel(j,i);
            if(color.r+color.g+color.b < 300){
                matrice[i][j] = '1';
            }
        }
    }
    return matrice;
}

void comparaison(vector<vector<string>>list_matrice_characters,vector<vector<char>> matrice_image, int dim_image_x, int dim_image_y,int dim_x_chara, int dim_y_chara,string number_frame,int precision){
    int number_x_box = dim_image_x/dim_x_chara*precision;
    int number_y_box = dim_image_y/dim_y_chara*precision;
    int nbr_charactere = distance(directory_iterator("characters"), directory_iterator{});

    fstream outfile("frame_ascii/"+number_frame+".txt");
    fstream table;
    table.open("frame_ascii/"+number_frame+".txt",std::ofstream::out | std::ofstream::trunc);
    table.close();
    table.open("frame_ascii/"+number_frame+".txt",ios::app);

    for(int y_box=0 ; y_box<number_y_box ; y_box++){
        for(int x_box=0 ; x_box<number_x_box ; x_box++){

            char chara_max = ' ';
            int max_score = -1000;
            int score;
            bool can_have_better_score;

            for(int index_chara=0 ; index_chara<nbr_charactere ; index_chara++){
                score = 0;
                can_have_better_score = true;

                for(int y_pixel_in_box=0 ; y_pixel_in_box<dim_y_chara ; y_pixel_in_box++){
                    for(int x_pixel_in_box=0 ; x_pixel_in_box<dim_x_chara ; x_pixel_in_box++){

                        if(matrice_image[(y_pixel_in_box+y_box*dim_y_chara)/precision][(x_pixel_in_box+x_box*dim_x_chara)/precision]==list_matrice_characters[index_chara][1][y_pixel_in_box*dim_x_chara+x_pixel_in_box]){
                            //If both (x,y) pixel is Black substract 2 to the score and if the pixel is white it's -1.
                            score-=int(list_matrice_characters[index_chara][1][y_pixel_in_box*dim_x_chara+x_pixel_in_box])-47; 
                        }
                        else{
                            //If the (x,y) pixel is black on one and the other it's white, add 2 to the score.
                            score+=2;
                        }
                        if(score+(dim_x_chara*(dim_y_chara-y_pixel_in_box-1)+dim_x_chara-1-x_pixel_in_box)*2<max_score){
                            can_have_better_score = false;
                            break;
                        }
                    }
                    if(!can_have_better_score){
                        break;
                    }
                }

                if(score>max_score){
                    chara_max = list_matrice_characters[index_chara][0][0];
                    max_score = score;
                }

            }
            table << chara_max;
        } 
        table<<endl;
    }
    table.close();
    matrice_image.clear();
}

int ascii_transform(string path, vector<vector<string>> list_matrice_characters, int precision){
    Image image;
    image.loadFromFile("frame_video/0.jpg");
    Vector2 dim_original_image = image.getSize();

    Image chara;
    chara.loadFromFile("characters/#.jpg");
    Vector2 dim_chara = chara.getSize();

    vector<vector<char>> matrice_image = image_to_matrice("frame_video/"+path,dim_original_image.x,dim_original_image.y,dim_chara.x,dim_chara.y);

    //Name of the file without the .jpg
    path.string::resize(path.length()-4);
    comparaison(list_matrice_characters,matrice_image,dim_original_image.x,dim_original_image.y,dim_chara.x,dim_chara.y,path,precision);
    return 1;
}