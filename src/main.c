#include <gtk/gtk.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

char* text_entry;
int FPS_entry = 0;
int FPSMax = 0;
int color_option = 1;
int color_sub_option = 1;
int accuracy_option = 1;
int canLaunch = 0;
int frameNumber  = 0;
int ASCII_number  = 0;
int frame_final_product_number = 0;
GtkWidget *window;
GtkWidget *grid;
GtkWidget *entry;
GtkWidget *entryFPS;
GtkWidget *color_button_1;
GtkWidget *color_button_2;
GtkWidget *bicolor_button_1;
GtkWidget *bicolor_button_2;
GtkWidget *tricolor_button_1;
GtkWidget *tricolor_button_2;
GtkWidget *tricolor_button_3;
GtkWidget *accuracy_button_1;
GtkWidget *accuracy_button_2;
GtkWidget *accuracy_button_3;
GtkWidget *launchButton; 
GtkWidget *progressbar_ASCII;
GtkWidget *progressbar_frame_final_product;
guint update_timerASCII = 0;
guint update_timer_frame_final_product = 0;
GtkWidget *quitButton;
int label_ASCII_added = 0;
int label_frame_final_product_added = 0;


void color_sub_option_button(GtkToggleButton *button, gpointer data){
    if (gtk_toggle_button_get_active(button)){
        char *label = gtk_button_get_label(GTK_BUTTON(button));
        if(strcmp(label,"White background with Black font")==0 || strcmp(label,"White background with Black and Grey font")==0){
            color_sub_option = 1;
        }
        else if(strcmp(label,"Black background with White font")==0 || strcmp(label,"Black background with White and Grey font")==0){
            color_sub_option = 2;
        }
        else{
            color_sub_option = 3;
        }
    }
}

void show_bicolor_button(GtkToggleButton *button, gpointer data) {
    if (gtk_toggle_button_get_active(button)){
        if (!bicolor_button_1) {
            color_option = 1;
            color_sub_option = 1;
            
            bicolor_button_1 = gtk_radio_button_new_with_label(NULL, "White background with Black font");
            bicolor_button_2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(bicolor_button_1), "Black background with White font");

            g_signal_connect(G_OBJECT(bicolor_button_1), "toggled", G_CALLBACK(color_sub_option_button), NULL);
            g_signal_connect(G_OBJECT(bicolor_button_2), "toggled", G_CALLBACK(color_sub_option_button), NULL);

            gtk_grid_attach(GTK_GRID(grid), bicolor_button_1, 0, 11, 1, 1);
            gtk_grid_attach(GTK_GRID(grid), bicolor_button_2, 0, 12, 1, 1);

            gtk_widget_show_all(window);
        }
    } 
    else {
        if (bicolor_button_1) {
            gtk_widget_destroy(bicolor_button_1);
            gtk_widget_destroy(bicolor_button_2);
        }
    }
}

void show_tricolor_button(GtkToggleButton *button, gpointer data) {
    if(gtk_toggle_button_get_active(button)) {
        if(!tricolor_button_1) {
            color_option = 2;
            color_sub_option = 1;
            
            tricolor_button_1 = gtk_radio_button_new_with_label(NULL, "White background with Black and Grey font");
            tricolor_button_2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(tricolor_button_1), "Black background with White and Grey font");
            tricolor_button_3 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(tricolor_button_1), "Grey background with Black and White font");


            g_signal_connect(G_OBJECT(tricolor_button_1), "toggled", G_CALLBACK(color_sub_option_button), NULL);
            g_signal_connect(G_OBJECT(tricolor_button_2), "toggled", G_CALLBACK(color_sub_option_button), NULL);
            g_signal_connect(G_OBJECT(tricolor_button_3), "toggled", G_CALLBACK(color_sub_option_button), NULL);

            gtk_grid_attach(GTK_GRID(grid), tricolor_button_1, 0, 11, 1, 1);
            gtk_grid_attach(GTK_GRID(grid), tricolor_button_2, 0, 12, 1, 1);
            gtk_grid_attach(GTK_GRID(grid), tricolor_button_3, 0, 13, 1, 1);

            gtk_widget_show_all(window);
        }
    } 
    else {
        if(tricolor_button_1) {
            gtk_widget_destroy(tricolor_button_1);
            gtk_widget_destroy(tricolor_button_2);
            gtk_widget_destroy(tricolor_button_3);
        }
    }
}

void get_entry_FPS(GtkEntry *entryFPS, gpointer data){
    const gchar *text = gtk_entry_get_text(entryFPS);
    FPS_entry = atoi(text);
    if(FPS_entry<=0 || FPS_entry>FPSMax){
        FPS_entry = 0;
    }
}

void get_entry_video_name(GtkEntry *entry, gpointer data) {
    text_entry = gtk_entry_get_text(entry);
    int isCorrect = 1;

    if (access(text_entry, F_OK) != -1) {
        char command[512];
        sprintf(command, "./video_FPS_counter %s", text_entry);
        system(command);

        FILE *file = fopen("FPS.txt", "r");
        fscanf(file, "%d", &FPSMax);

        if(FPSMax != -1){
            char fpsTitle[64];
            sprintf(fpsTitle, "FPS (MAX %d)",FPSMax);
            GtkWidget *labelFPS = gtk_label_new(fpsTitle);
            gtk_grid_attach(GTK_GRID(grid), labelFPS, 0, 5, 2, 1);

            entryFPS = gtk_entry_new();
            gtk_entry_set_placeholder_text(GTK_ENTRY(entryFPS), "Enter the number of FPS wanted");
            gtk_widget_set_size_request(entryFPS, 300, -1);
            g_signal_connect(entryFPS, "changed", G_CALLBACK(get_entry_FPS), NULL);
            gtk_grid_attach(GTK_GRID(grid), entryFPS, 0, 6, 2, 1);
        }
        else{
            isCorrect = 0;
        } 
    }
    else{
        isCorrect = 0;
    }

    if(!isCorrect){
        if(entryFPS){
            gtk_widget_destroy(entryFPS);
            entryFPS = NULL;
        }

        GtkWidget *labelToRemove = gtk_grid_get_child_at(GTK_GRID(grid), 0, 5);
        if (labelToRemove != NULL) {
            gtk_container_remove(GTK_CONTAINER(grid), labelToRemove);
        }

        FPS_entry = 0;
    }
    gtk_widget_show_all(window);
}

void accuracy_option_button(GtkToggleButton *button, gpointer data){
    if(gtk_toggle_button_get_active(button)){
        const gchar *label = gtk_button_get_label(GTK_BUTTON(button));
        if(strcmp(label,"Normal")==0){
            accuracy_option = 1;
        }
        else if(strcmp(label,"High")==0){
            accuracy_option = 2;
        }
        else{
            accuracy_option = 3;
        }
    }
}

void count_frame(){
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    dir = opendir("frame_video");

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            count++;
        }
    }

    closedir(dir);
    frameNumber  = count;
}

void count_ascii(){
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    if(color_option==1){
        dir = opendir("frame_ascii");
    }
    else{
        dir = opendir("frame_ascii_color_1");
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            count++;
        }
    }

    closedir(dir);
    ASCII_number  = count;
}

void count_frame_final_product(){
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    dir = opendir("frame_final_product");

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            count++;
        }
    }

    closedir(dir);
    frame_final_product_number = count;
}

gboolean update_progressASCII(GtkProgressBar *progressbar){
    count_ascii();
    if(ASCII_number!=0){
        count_ascii();
        if(frameNumber ==0){
            count_frame();
            GtkWidget *labelToRemove = gtk_grid_get_child_at(GTK_GRID(grid), 0, 0);
            if (labelToRemove != NULL) {
                gtk_container_remove(GTK_CONTAINER(grid), labelToRemove);
            }
            char labelFrameTotal[128];
            sprintf(labelFrameTotal, "%i frames extracted.", frameNumber );
            GtkWidget *labelFrame = gtk_label_new(labelFrameTotal);
            gtk_grid_attach(GTK_GRID(grid), labelFrame, 0, 0, 1, 1);
        }
        if(!label_ASCII_added){
            GtkWidget *labelASCII = gtk_label_new("Transforming frames into ASCII...");
            gtk_grid_attach(GTK_GRID(grid), labelASCII, 0, 50, 1, 1);
            GtkWidget *labelASCIIalt = gtk_label_new("Grab a cup of coffee and lay down, this operation takes some time !");
            gtk_grid_attach(GTK_GRID(grid), labelASCIIalt, 0, 51, 1, 1);
            gtk_grid_attach(GTK_GRID(grid), progressbar_ASCII, 0, 52, 1, 1);
            label_ASCII_added = 1;
        }
        double ratio = (double)ASCII_number/frameNumber ;
        double fraction = gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progressbar));
        if(ratio!=fraction){
            gtk_progress_bar_set_fraction(progressbar,ratio);
        }
        if(ASCII_number==frameNumber ){
            return FALSE;
        }
        gtk_widget_show_all(window);
    }
    return TRUE;
}

gboolean update_progressframe_final_product(GtkProgressBar *progressbar){
    if(frameNumber !=0 && ASCII_number==frameNumber ){
        count_frame_final_product();
        if(!label_frame_final_product_added){
            GtkWidget *labelframe_final_product = gtk_label_new("Transforming ASCII into frames...");
            gtk_grid_attach(GTK_GRID(grid), labelframe_final_product, 0, 54, 1, 1);
            gtk_grid_attach(GTK_GRID(grid), progressbar_frame_final_product, 0, 55, 1, 1);
            label_frame_final_product_added = 1;
        }
        double ratio = (double)frame_final_product_number/(double)frameNumber ;
        double fraction = gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progressbar));
        if(ratio!=fraction){
            gtk_progress_bar_set_fraction(progressbar,ratio);
        }
        if(frame_final_product_number>=frameNumber ){
            return FALSE;
        }
        gtk_widget_show_all(window);
    }
    return TRUE;
}

void quit(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

void add_quit_button() {
    quitButton = gtk_button_new_with_label("Quit");
    g_signal_connect(quitButton, "clicked", G_CALLBACK(quit), NULL);
    gtk_grid_attach(GTK_GRID(grid), quitButton, 0, 80, 1, 1);
    gtk_widget_show_all(window);
}

void VideoToASCII_thread() {
    char command[128];
    sprintf(command, "./VideoToASCII %s %i %i %i %i", text_entry, FPS_entry, color_option, color_sub_option, accuracy_option);
    system(command);    

    g_idle_add((GSourceFunc)add_quit_button, NULL);
}

void launch(GtkButton *button,gpointer widget){
    if(FPS_entry!=0 && color_option!=0){
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "VIDEO TO ASCII BY NICHI HACHI");
        g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
        gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
  
        GThread *background_thread = g_thread_new("BackgroundThread", (GThreadFunc)VideoToASCII_thread, NULL);

        grid = gtk_grid_new();
        gtk_container_add(GTK_CONTAINER(window), grid);

        GtkWidget *labelframe_final_product = gtk_label_new("Extracting frames from the video...");
        gtk_grid_attach(GTK_GRID(grid), labelframe_final_product, 0, 0, 1, 1);

        progressbar_ASCII = GTK_PROGRESS_BAR(gtk_progress_bar_new());
        progressbar_frame_final_product = GTK_PROGRESS_BAR(gtk_progress_bar_new());

        update_timerASCII = g_timeout_add(1000, (GSourceFunc)update_progressASCII, progressbar_ASCII);
        update_timer_frame_final_product = g_timeout_add(1000, (GSourceFunc)update_progressframe_final_product, progressbar_frame_final_product);

        gtk_widget_hide(widget);
        gtk_widget_show_all(window);
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "VIDEO TO ASCII BY NICHI HACHI");
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);


    GtkWidget *labelName = gtk_label_new("Name of the video with the format (.mp4 .avi ...)");
    gtk_grid_attach(GTK_GRID(grid), labelName, 0, 0, 2, 1);

    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter the video name with format");
    gtk_widget_set_size_request(entry, 300, -1);
    
    g_signal_connect(entry, "changed", G_CALLBACK(get_entry_video_name), NULL);

    gtk_grid_attach(GTK_GRID(grid), entry, 0, 1, 2, 1);


    GtkWidget *labelColor = gtk_label_new("Color of the video");
    gtk_grid_attach(GTK_GRID(grid), labelColor, 0, 9, 1, 1);

    color_button_1 = gtk_radio_button_new_with_label(NULL, "Bicolor");
    color_button_2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(color_button_1), "Tricolor");

    g_signal_connect(G_OBJECT(color_button_1), "toggled", G_CALLBACK(show_bicolor_button), NULL);
    g_signal_connect(G_OBJECT(color_button_2), "toggled", G_CALLBACK(show_tricolor_button), NULL);

    gtk_grid_attach(GTK_GRID(grid), color_button_1, 0, 10, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), color_button_2, 1, 10, 1, 1);


    GtkWidget *labelAccuracy = gtk_label_new("Degree of accuracy");
    gtk_grid_attach(GTK_GRID(grid), labelAccuracy, 0, 19, 1, 1);

    accuracy_button_1 = gtk_radio_button_new_with_label(NULL, "Normal");
    accuracy_button_2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(accuracy_button_1), "High");
    accuracy_button_3 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(accuracy_button_1), "Very high");

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(accuracy_button_1), TRUE);

    g_signal_connect(G_OBJECT(accuracy_button_1), "toggled", G_CALLBACK(accuracy_option_button), NULL);
    g_signal_connect(G_OBJECT(accuracy_button_2), "toggled", G_CALLBACK(accuracy_option_button), NULL);
    g_signal_connect(G_OBJECT(accuracy_button_3), "toggled", G_CALLBACK(accuracy_option_button), NULL);

    gtk_grid_attach(GTK_GRID(grid), accuracy_button_1, 0, 20, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), accuracy_button_2, 0, 21, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), accuracy_button_3, 0, 22, 1, 1);

    launchButton = gtk_button_new_with_label("Launch");
    g_signal_connect(G_OBJECT(launchButton), "clicked", G_CALLBACK(launch), (gpointer) window);
    gtk_grid_attach(GTK_GRID(grid), launchButton, 0, 25, 1, 1);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}