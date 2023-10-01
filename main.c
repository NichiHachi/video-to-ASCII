#include <gtk/gtk.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>

static char* text_entry;
static int FPS_entry = 0;
static int FPSMax = 0;
static int color_option = 1;
static int color_sub_option = 1;
static int accuracy_option = 1;
static int canLaunch = 0;
static int frameNumber = 0;
static int ASCIINumber = 0;
static int frameASCIINumber = 0;
static GtkWidget *window;
static GtkWidget *grid;
static GtkWidget *entry;
static GtkWidget *entryFPS;
static GtkWidget *option1_radio;
static GtkWidget *option2_radio;
static GtkWidget *sub1_option1_radio;
static GtkWidget *sub1_option2_radio;
static GtkWidget *sub2_option1_radio;
static GtkWidget *sub2_option2_radio;
static GtkWidget *sub2_option3_radio;
static GtkWidget *accuracy_option1_radio;
static GtkWidget *accuracy_option2_radio;
static GtkWidget *accuracy_option3_radio;
static GtkWidget *launchButton; 
static GtkWidget *progressbarASCII;
static GtkWidget *progressbarFrameASCII;
static guint update_timerASCII = 0;
static guint update_timerFrameASCII = 0;
static GtkWidget *quitButton;
static int labelASCIIAdded = 0;
static int labelFrameASCIIAdded = 0;


static void color_sub_option_button(GtkToggleButton *button, gpointer data){
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

static void show_additional_radios(GtkToggleButton *button, gpointer data) {
    if (gtk_toggle_button_get_active(button)){
        if (!sub1_option1_radio) {
            color_option = 1;
            color_sub_option = 1;
            
            sub1_option1_radio = gtk_radio_button_new_with_label(NULL, "White background with Black font");
            sub1_option2_radio = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(sub1_option1_radio), "Black background with White font");

            g_signal_connect(G_OBJECT(sub1_option1_radio), "toggled", G_CALLBACK(color_sub_option_button), NULL);
            g_signal_connect(G_OBJECT(sub1_option2_radio), "toggled", G_CALLBACK(color_sub_option_button), NULL);

            gtk_grid_attach(GTK_GRID(grid), sub1_option1_radio, 0, 11, 1, 1);
            gtk_grid_attach(GTK_GRID(grid), sub1_option2_radio, 0, 12, 1, 1);

            gtk_widget_show_all(window);
        }
    } 
    else {
        if (sub1_option1_radio) {
            gtk_widget_destroy(sub1_option1_radio);
            gtk_widget_destroy(sub1_option2_radio);
        }
    }
}

static void show_additional_radios_bis(GtkToggleButton *button, gpointer data) {
    if (gtk_toggle_button_get_active(button)) {
        if (!sub2_option1_radio) {
            color_option = 2;
            color_sub_option = 1;
            
            sub2_option1_radio = gtk_radio_button_new_with_label(NULL, "White background with Black and Grey font");
            sub2_option2_radio = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(sub2_option1_radio), "Black background with White and Grey font");
            sub2_option3_radio = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(sub2_option1_radio), "Grey background with Black and White font");


            g_signal_connect(G_OBJECT(sub2_option1_radio), "toggled", G_CALLBACK(color_sub_option_button), NULL);
            g_signal_connect(G_OBJECT(sub2_option2_radio), "toggled", G_CALLBACK(color_sub_option_button), NULL);
            g_signal_connect(G_OBJECT(sub2_option3_radio), "toggled", G_CALLBACK(color_sub_option_button), NULL);

            gtk_grid_attach(GTK_GRID(grid), sub2_option1_radio, 0, 11, 1, 1);
            gtk_grid_attach(GTK_GRID(grid), sub2_option2_radio, 0, 12, 1, 1);
            gtk_grid_attach(GTK_GRID(grid), sub2_option3_radio, 0, 13, 1, 1);

            gtk_widget_show_all(window);
        }
    } 
    else {
        if (sub2_option1_radio) {
            gtk_widget_destroy(sub2_option1_radio);
            gtk_widget_destroy(sub2_option2_radio);
            gtk_widget_destroy(sub2_option3_radio);
        }
    }
}

static void get_entry_FPS(GtkEntry *entryFPS, gpointer data){
    const gchar *text = gtk_entry_get_text(entryFPS);
    FPS_entry = atoi(text);
    if(FPS_entry<=0 || FPS_entry>FPSMax){
        FPS_entry = 0;
    }
}

static void get_entry_text(GtkEntry *entry, gpointer data) {
    text_entry = gtk_entry_get_text(entry);
    int isCorrect = 1;

    if (access(text_entry, F_OK) != -1) {
        char command[512];
        sprintf(command, "./videoFPSCounter %s", text_entry);
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

static void accuracy_option_button(GtkToggleButton *button, gpointer data){
    if(gtk_toggle_button_get_active(button)){
        const gchar *label = gtk_button_get_label(GTK_BUTTON(button));
        if(label=="Normal"){
            accuracy_option = 1;
        }
        else if(label=="High"){
            accuracy_option = 2;
        }
        else{
            accuracy_option = 3;
        }
    }
}

static void countFrame(){
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
    frameNumber = count;
}

static void countASCII(){
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
    ASCIINumber = count;
}

static void countFrameASCII(){
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
    frameASCIINumber = count;
}

static gboolean update_progressASCII(GtkProgressBar *progressbar){
    countASCII();
    if(ASCIINumber!=0){
        countASCII();
        if(frameNumber==0){
            countFrame();
            GtkWidget *labelToRemove = gtk_grid_get_child_at(GTK_GRID(grid), 0, 0);
            if (labelToRemove != NULL) {
                gtk_container_remove(GTK_CONTAINER(grid), labelToRemove);
            }
            char labelFrameTotal[128];
            sprintf(labelFrameTotal, "%i frames extracted.", frameNumber);
            GtkWidget *labelFrame = gtk_label_new(labelFrameTotal);
            gtk_grid_attach(GTK_GRID(grid), labelFrame, 0, 0, 1, 1);
        }
        if(!labelASCIIAdded){
            GtkWidget *labelASCII = gtk_label_new("Transforming frames into ASCII...");
            gtk_grid_attach(GTK_GRID(grid), labelASCII, 0, 50, 1, 1);
            GtkWidget *labelASCIIalt = gtk_label_new("Grab a cup of coffee and lay down, this operation takes some time !");
            gtk_grid_attach(GTK_GRID(grid), labelASCIIalt, 0, 51, 1, 1);
            gtk_grid_attach(GTK_GRID(grid), progressbarASCII, 0, 52, 1, 1);
            labelASCIIAdded = 1;
        }
        double ratio = (double)ASCIINumber/frameNumber;
        double fraction = gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progressbar));
        if(ratio!=fraction){
            gtk_progress_bar_set_fraction(progressbar,ratio);
        }
        if(ASCIINumber==frameNumber){
            return FALSE;
        }
        gtk_widget_show_all(window);
    }
    return TRUE;
}

static gboolean update_progressFrameASCII(GtkProgressBar *progressbar){
    if(frameNumber!=0 && ASCIINumber==frameNumber){
        countFrameASCII();
        if(!labelFrameASCIIAdded){
            GtkWidget *labelFrameASCII = gtk_label_new("Transforming ASCII into frames...");
            gtk_grid_attach(GTK_GRID(grid), labelFrameASCII, 0, 54, 1, 1);
            gtk_grid_attach(GTK_GRID(grid), progressbarFrameASCII, 0, 55, 1, 1);
            labelFrameASCIIAdded = 1;
        }
        double ratio = (double)frameASCIINumber/(double)frameNumber;
        double fraction = gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progressbar));
        if(ratio!=fraction){
            gtk_progress_bar_set_fraction(progressbar,ratio);
        }
        if(frameASCIINumber>=frameNumber){
            return FALSE;
        }
        gtk_widget_show_all(window);
    }
    return TRUE;
}


static void quit(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

static void addQuitButton() {
    quitButton = gtk_button_new_with_label("Quit");
    g_signal_connect(quitButton, "clicked", G_CALLBACK(quit), NULL);
    gtk_grid_attach(GTK_GRID(grid), quitButton, 0, 80, 1, 1);
    gtk_widget_show_all(window);
}

static void VideoToASCIIThread() {
    char command[128];
    sprintf(command, "./VideoToASCII %s %i %i %i %i", text_entry, FPS_entry, color_option, color_sub_option, accuracy_option);
    system(command);    

    g_idle_add((GSourceFunc)addQuitButton, NULL);
}

static void launch(GtkButton *button,gpointer widget){
    if(FPS_entry!=0 && color_option!=0){
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "VIDEO TO ASCII BY NICHI HACHI");
        g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
        gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
  
        GThread *background_thread = g_thread_new("BackgroundThread", (GThreadFunc)VideoToASCIIThread, NULL);

        grid = gtk_grid_new();
        gtk_container_add(GTK_CONTAINER(window), grid);

        GtkWidget *labelFrameASCII = gtk_label_new("Extracting frames from the video...");
        gtk_grid_attach(GTK_GRID(grid), labelFrameASCII, 0, 0, 1, 1);

        progressbarASCII = GTK_PROGRESS_BAR(gtk_progress_bar_new());
        progressbarFrameASCII = GTK_PROGRESS_BAR(gtk_progress_bar_new());

        update_timerASCII = g_timeout_add(1000, (GSourceFunc)update_progressASCII, progressbarASCII);
        update_timerFrameASCII = g_timeout_add(1000, (GSourceFunc)update_progressFrameASCII, progressbarFrameASCII);

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
    
    g_signal_connect(entry, "changed", G_CALLBACK(get_entry_text), NULL);

    gtk_grid_attach(GTK_GRID(grid), entry, 0, 1, 2, 1);


    GtkWidget *labelColor = gtk_label_new("Color of the video");
    gtk_grid_attach(GTK_GRID(grid), labelColor, 0, 9, 1, 1);

    option1_radio = gtk_radio_button_new_with_label(NULL, "Bicolor");
    option2_radio = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(option1_radio), "Tricolor");

    g_signal_connect(G_OBJECT(option1_radio), "toggled", G_CALLBACK(show_additional_radios), NULL);
    g_signal_connect(G_OBJECT(option2_radio), "toggled", G_CALLBACK(show_additional_radios_bis), NULL);

    gtk_grid_attach(GTK_GRID(grid), option1_radio, 0, 10, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), option2_radio, 1, 10, 1, 1);


    GtkWidget *labelAccuracy = gtk_label_new("Degree of accuracy");
    gtk_grid_attach(GTK_GRID(grid), labelAccuracy, 0, 19, 1, 1);

    accuracy_option1_radio = gtk_radio_button_new_with_label(NULL, "Normal");
    accuracy_option2_radio = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(accuracy_option1_radio), "High");
    accuracy_option3_radio = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(accuracy_option1_radio), "Very high");

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(accuracy_option1_radio), TRUE);

    g_signal_connect(G_OBJECT(accuracy_option1_radio), "toggled", G_CALLBACK(accuracy_option_button), NULL);
    g_signal_connect(G_OBJECT(accuracy_option2_radio), "toggled", G_CALLBACK(accuracy_option_button), NULL);
    g_signal_connect(G_OBJECT(accuracy_option3_radio), "toggled", G_CALLBACK(accuracy_option_button), NULL);

    gtk_grid_attach(GTK_GRID(grid), accuracy_option1_radio, 0, 20, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), accuracy_option2_radio, 0, 21, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), accuracy_option3_radio, 0, 22, 1, 1);

    launchButton = gtk_button_new_with_label("Launch");
    g_signal_connect(G_OBJECT(launchButton), "clicked", G_CALLBACK(launch), (gpointer) window);
    gtk_grid_attach(GTK_GRID(grid), launchButton, 0, 25, 1, 1);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
