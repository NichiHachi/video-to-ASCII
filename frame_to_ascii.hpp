#include <string>
#include <vector>

std::vector<std::vector<char>> bicolor_image_maping_matrix(std::string path_file, int dim_array_x, int dim_array_y);

std::vector<std::vector<char>> tricolor_image_maping_matrix(std::string path_file, int dim_array_x, int dim_array_y);

void bicolor_frame_to_ascii(std::vector<std::vector<std::string>>list_matrix_characters,std::vector<std::vector<char>>matrix_image, 
                            std::string name_file,int precision, int color_factor, int dim_video_x, int dim_video_y, int dim_x_chara, int dim_y_chara);

void tricolor_frame_to_ascii(std::vector<std::vector<std::string>>list_matrix_characters,std::vector<std::vector<char>>matrix_image, char color_1, char color_2, 
                            std::string name_file, int precision, int dim_video_x, int dim_video_y, int dim_x_chara, int dim_y_chara);

std::vector<std::vector<std::string>> txt_to_list(void);
    
int frame_to_ascii(int precision, int color_selection, bool isTricolor);
