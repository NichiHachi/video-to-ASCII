#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

namespace fs = std::filesystem;

const std::string path_folder_frame_video = "frame_video";
const std::string path_folder_ascii = "frame_ascii";
const std::string path_folder_ascii_color_1 = "frame_ascii_color_1";
const std::string path_folder_ascii_color_2 = "frame_ascii_color_2";
const std::string path_table = "characters_matrix_table.txt";

std::vector<std::vector<char>> bicolor_image_maping_matrix(std::string path_file, int dim_array_x, int dim_array_y){
    sf::Image image;
    image.loadFromFile(path_file);
    sf::Color color;
    int color_add;
    std::vector<std::vector<char>> matrix_image(dim_array_y, std::vector<char>(dim_array_x, '0'));
    for(int y=0;y<dim_array_y;y++){
        for(int x=0;x<dim_array_x;x++){
            color = image.getPixel(x,y);
            color_add = color.r+color.g+color.b;
            if(color_add < 192){
                matrix_image[y][x] = '1';
            }
        }
    }
    return matrix_image;
}

std::vector<std::vector<char>> tricolor_image_maping_matrix(std::string path_file, int dim_array_x, int dim_array_y){
    sf::Image image;
    image.loadFromFile(path_file);
    sf::Color color;
    int color_add;
    std::vector<std::vector<char>> matrix_image(dim_array_y, std::vector<char>(dim_array_x, '1'));
    //A changer
    for(int y=0;y<dim_array_y;y++){
        for(int x=0;x<dim_array_x;x++){
            color = image.getPixel(x,y);
            color_add = color.r+color.g+color.b;
            if(color_add < 192){
                matrix_image[y][x] = '2';
            }
            else if(color_add > 573){
                matrix_image[y][x] = '0';
            }
        }
    }
    return matrix_image;
}

void bicolor_frameToASCII(std::vector<std::vector<std::string>>list_matrix_characters,std::vector<std::vector<char>>matrix_image, std::string name_file, int precision, int color_factor, int dim_video_x, int dim_video_y, int dim_x_chara, int dim_y_chara){
    int number_x_box = dim_video_x/dim_x_chara*precision;
    int number_y_box = dim_video_y/dim_y_chara*precision;
    int nbr_character;
    if(precision==3){
        nbr_character = std::distance(fs::directory_iterator("characters_low"), fs::directory_iterator{});
    }
    else{
        nbr_character = std::distance(fs::directory_iterator("characters"), fs::directory_iterator{});
    }
    int score;
    int max_score;
    char max_chara;
    bool can_have_better_score;
    int color_image_at_cord;
    int color_char_at_cord;

    std::fstream outfile(path_folder_ascii+'/'+name_file+".txt");
    std::fstream table;
    table.open(path_folder_ascii+'/'+name_file+".txt",std::ios::app);

    for(int y_box=0 ; y_box<number_y_box ; y_box++){
        for(int x_box=0 ; x_box<number_x_box ; x_box++){
            max_score = -1000;

            for(int index_chara=0 ; index_chara<nbr_character ; index_chara++){
                score = 0;
                can_have_better_score = true;

                for(int y_pixel_in_box=0 ; y_pixel_in_box<dim_y_chara ; y_pixel_in_box++){
                    for(int x_pixel_in_box=0 ; x_pixel_in_box<dim_x_chara ; x_pixel_in_box++){

                        if(y_pixel_in_box + y_box * dim_y_chara / precision < matrix_image.size() && x_pixel_in_box + x_box * dim_x_chara / precision < matrix_image[0].size() 
                        && index_chara < list_matrix_characters.size() && y_pixel_in_box * dim_x_chara + x_pixel_in_box < list_matrix_characters[index_chara][1].size()){

                            color_image_at_cord = matrix_image[(y_pixel_in_box+y_box*dim_y_chara)/precision][(x_pixel_in_box+x_box*dim_x_chara)/precision];
                            color_char_at_cord = list_matrix_characters[index_chara][1][y_pixel_in_box*dim_x_chara+x_pixel_in_box];
                            if(color_image_at_cord==color_char_at_cord){
                                if(color_image_at_cord=='0'){
                                    score+=15*color_factor; 
                                }
                                else{
                                    score+=1*color_factor;
                                }
                            }
                            else{
                                //If the (x,y) pixel is black on one and the other it's white.
                                score-=15*color_factor;
                            }

                            if(score+(dim_x_chara*(dim_y_chara-y_pixel_in_box-1)+dim_x_chara-1-x_pixel_in_box)*10<max_score){
                                can_have_better_score = false;
                                break;
                            }
                        }
                    }
                    //If the perfect score can't be better than max_score, it's useless to continue
                    if(!can_have_better_score){
                        break;
                    }

                }
                if(score>max_score){
                    max_chara = list_matrix_characters[index_chara][0][0];
                    max_score = score;
                }
            }
            table << max_chara ;
        } 
        table<<std::endl;
    }
    table.close();
}

void tricolor_frameToASCII(std::vector<std::vector<std::string>>list_matrix_characters,std::vector<std::vector<char>>matrix_image, char color_1, char color_2, std::string name_file, int precision, int dim_video_x, int dim_video_y, int dim_x_chara, int dim_y_chara){
    int number_x_box = dim_video_x/dim_x_chara*precision;
    int number_y_box = dim_video_y/dim_y_chara*precision;
    int nbr_character = std::distance(fs::directory_iterator("characters"), fs::directory_iterator{});
    int score_color_1;
    int score_color_2;
    int max_score_1;
    int max_score_2;
    char max_chara_1;
    char max_chara_2;
    bool can_have_better_score;
    char color_image_at_cord;
    char color_char_at_cord;

    std::fstream outfile1(path_folder_ascii_color_1+'/'+name_file+".txt");
    std::fstream ascii_color_1;
    ascii_color_1.open(path_folder_ascii_color_1+'/'+name_file+".txt",std::ios::app);
    
    std::fstream outfile2(path_folder_ascii_color_2+'/'+name_file+".txt");
    std::fstream ascii_color_2;
    ascii_color_2.open(path_folder_ascii_color_2+'/'+name_file+".txt",std::ios::app);

    for(int y_box=0 ; y_box<number_y_box ; y_box++){
        for(int x_box=0 ; x_box<number_x_box ; x_box++){
            max_score_1 = -1000;
            max_score_2 = -1000;

            for(int index_chara=0 ; index_chara<nbr_character ; index_chara++){
                score_color_1 = 0;
                score_color_2 = 0;
                can_have_better_score = true;

                for(int y_pixel_in_box=0 ; y_pixel_in_box<dim_y_chara ; y_pixel_in_box++){
                    for(int x_pixel_in_box=0 ; x_pixel_in_box<dim_x_chara ; x_pixel_in_box++){
                        color_image_at_cord = matrix_image[(y_pixel_in_box+y_box*dim_y_chara)/precision][(x_pixel_in_box+x_box*dim_x_chara)/precision];
                        color_char_at_cord = list_matrix_characters[index_chara][1][y_pixel_in_box*dim_x_chara+x_pixel_in_box];
                        //SCORE COLOR 1
                        if(color_image_at_cord==color_1 && color_char_at_cord=='1'){
                            score_color_1+=15; 
                        }
                        else if(color_image_at_cord!=color_1 && color_char_at_cord=='0'){
                            score_color_1+=1;
                        }
                        else{
                            score_color_1-=1;
                        }

                        //SCORE COLOR 2
                        if(color_image_at_cord==color_2 && color_char_at_cord=='1'){
                            score_color_2+=15; 
                        }
                        else if(color_image_at_cord!=color_2 && color_char_at_cord=='0'){
                            score_color_2+=1;
                        }
                        else{
                            score_color_2-=1;
                        }

                        if(score_color_1+(dim_x_chara*(dim_y_chara-y_pixel_in_box-1)+dim_x_chara-1-x_pixel_in_box)*15<max_score_1 && score_color_2+(dim_x_chara*(dim_y_chara-y_pixel_in_box-1)+dim_x_chara-1-x_pixel_in_box)*15<max_score_2){
                            can_have_better_score = false;
                            break;
                        }
                    }
                    //If the perfect score can't be better than max_score, it's useless to continue
                    if(!can_have_better_score){
                        break;
                    }

                }
                if(score_color_1>max_score_1){
                    max_chara_1 = list_matrix_characters[index_chara][0][0];
                    max_score_1 = score_color_1;
                }
                if(score_color_2>max_score_2){
                    max_chara_2 = list_matrix_characters[index_chara][0][0];
                    max_score_2 = score_color_2;
                }
            }
            if(max_score_1>max_score_2){
                ascii_color_1<<max_chara_1;
                ascii_color_2<<' ';
            }
            else{
                ascii_color_2<<max_chara_2;
                ascii_color_1<<' ';
            }
        } 
        ascii_color_1<<std::endl;
        ascii_color_2<<std::endl;
    }
    ascii_color_1.close();
    ascii_color_2.close();
}

std::vector<std::vector<std::string>> txt_to_list(int precision){
    int nbr_character = std::distance(fs::directory_iterator("characters"), fs::directory_iterator{});
    std::vector<std::vector<std::string>> list_matrix_characters(nbr_character, std::vector<std::string>(2,""));
    int h=0;
    int modulo;
    
    sf::Image chara;
    if(precision==3){
        chara.loadFromFile("characters_low/#.jpg");
    }
    else{
        chara.loadFromFile("characters/#.jpg");
    }
    sf::Vector2 dim_chara = chara.getSize();

    std::fstream input(path_table);
    for(std::string line; getline(input,line);){
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

int frameToASCII(int precision, int color_selection, bool isTricolor){
    sf::Image image;
    image.loadFromFile("frame_video/0.jpg");
    sf::Vector2 dim_video = image.getSize();

    sf::Image chara;
    if(precision==3){
        chara.loadFromFile("characters_low/#.jpg");
    }
    else{
        chara.loadFromFile("characters/#.jpg");
    }
    sf::Vector2 dim_chara = chara.getSize();

    std::vector<std::vector<std::string>> character_matrix = txt_to_list(precision);

    int dim_video_x = dim_video.x-(dim_video.x%dim_chara.x);
    int dim_video_y = dim_video.y-(dim_video.y%dim_chara.y);

    int frameTotal = std::distance(fs::directory_iterator(path_folder_frame_video), fs::directory_iterator{});

    double precision_factor;
    if(precision==2){
        precision_factor=2;
    }
    else{
        precision_factor=1;
    }

    if(!isTricolor){
        int color_factor; 
        if(color_selection==1){
            color_factor = 1;
        }
        else{
            color_factor = -1;
        }
        
        std::vector<std::vector<char>> arr(dim_video_x, std::vector<char>(dim_video_y));
        for(int frameNumber=0; frameNumber<frameTotal;frameNumber++){
            std::vector<std::vector<char>> arr = bicolor_image_maping_matrix(path_folder_frame_video+'/'+std::to_string(frameNumber)+".jpg", dim_video_x, dim_video_y);
            bicolor_frameToASCII(character_matrix,arr,std::to_string(frameNumber), precision, color_factor, dim_video_x, dim_video_y, dim_chara.x, dim_chara.y);
        }
    }
    else{
        char color_1,color_2;
        if(color_selection==1){
            color_1 = '2';
            color_2 = '1';
        }
        else if(color_selection==2){
            color_1 = '0';
            color_2 = '1';
        }
        else{
            color_1 = '2';
            color_2 = '0';
        }
        
        std::vector<std::vector<char>> arr(dim_video_x, std::vector<char>(dim_video_y));
        for(int frameNumber=0; frameNumber<frameTotal;frameNumber++){
            std::vector<std::vector<char>> arr = tricolor_image_maping_matrix(path_folder_frame_video+'/'+std::to_string(frameNumber)+".jpg", dim_video_x, dim_video_y);
            tricolor_frameToASCII(character_matrix,arr,color_1,color_2,std::to_string(frameNumber), precision, dim_video_x, dim_video_y, dim_chara.x, dim_chara.y);
        }
    }
    return 1;
}