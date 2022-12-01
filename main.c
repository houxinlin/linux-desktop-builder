#include <stdio.h>
#include <sys/stat.h>
#include <gtk/gtk.h>
#include<unistd.h>

#define APPLICATION_PATH "/.local/share/applications/"

GtkWidget *window;

GtkTextView *execPathTextView;
GtkTextView *iconPathTextView;
GtkTextView *desktopNameTextView;
GtkTextView *applicationNameTextView;

int file_exists(char *filename) {
    struct stat buffer;
    int result =stat(filename, &buffer);
    return result;
}

void show_message_dialog(char *message) {
    GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    GtkWidget *dialog = gtk_message_dialog_new(window,
                                               flags,
                                               GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_CLOSE,
                                               "%s",
                                               message,
                                               "");
    gtk_dialog_run(GTK_DIALOG (dialog));
    gtk_widget_destroy(dialog);
}

char *get_text_view_value(GtkTextView *textview) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gchar *text;

    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer), &start, &end);
    const GtkTextIter s = start, e = end;
    text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(buffer), &s, &e, FALSE);
    return text;
}

void do_generator(char *desktopName, char *appPath, char *appIcon, char *name) {

    char *userHome = getenv("HOME");
    int strLength = strlen(userHome) + strlen(APPLICATION_PATH) + strlen(desktopName) + 8;
    char *targetPath = (char *) calloc(1, strLength);

    strcat(targetPath, userHome);
    strcat(targetPath, APPLICATION_PATH);
    strcat(targetPath, desktopName);
    strcat(targetPath, ".desktop");

    if (file_exists(targetPath)==0){
        show_message_dialog("文件名已存在!");
        return;
    }
    if (file_exists(appPath)==-1){
        show_message_dialog("可执行路径不存在!");
        return;
    }
    if (file_exists(appIcon)==-1){
        show_message_dialog("图标不存在!");
        return;
    }
    FILE *fp = fopen(targetPath, "w");

    fprintf(fp, "%s\n", "[Desktop Entry]");
    fprintf(fp, "%s\n", "Encoding=UTF-8");
    fprintf(fp, "%s\n", "Type=Application");
    fprintf(fp, "Exec=%s\n", appPath);
    fprintf(fp, "Name=%s\n", name);
    fprintf(fp, "Icon=%s\n", appIcon);
    fflush(fp);
    close(fp);
    free(targetPath);
    show_message_dialog("创建成功");
}

void generator_desktop_file(__attribute__((unused)) GtkWidget *widget,
                            __attribute__((unused)) gpointer data) {

    if (strlen(get_text_view_value(desktopNameTextView)) == 0 ||
        strlen(get_text_view_value(applicationNameTextView)) == 0 ||
        strlen(get_text_view_value(execPathTextView)) == 0 ||
        strlen(get_text_view_value(iconPathTextView)) == 0) {
        show_message_dialog("还有数据没有填写");
        return;
    }
    do_generator(get_text_view_value(desktopNameTextView),
                 get_text_view_value(execPathTextView),
                 get_text_view_value(iconPathTextView),
                 get_text_view_value(applicationNameTextView));

}

void create_select_dialog(GtkTextView *textview) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    dialog = gtk_file_chooser_dialog_new("Open File",
                                         window,
                                         action,
                                         ("_Cancel"),
                                         GTK_RESPONSE_CANCEL,
                                         ("_Open"),
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);
    res = gtk_dialog_run(GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
        gtk_text_buffer_set_text(buffer, filename, -1);
    }

    gtk_widget_destroy(dialog);
}

void select_icon_path(__attribute__((unused)) GtkWidget *widget,
                      __attribute__((unused)) gpointer data) {
    create_select_dialog(iconPathTextView);
}

void select_exec_path(__attribute__((unused)) GtkWidget *widget,
                      __attribute__((unused)) gpointer data) {
    create_select_dialog(execPathTextView);
}


int main(int argc, char **argv) {

    GtkBuilder *builder = NULL;
    gtk_init(&argc, &argv);
    char *buf;
    buf=(char *)malloc(100*sizeof(char));
    getcwd(buf,100);
    printf("\n %s \n",buf);

    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "gtk.glade", NULL) == 0) {
        printf("gtk_builder_add_from_file FAILED\n");
        return 0;
    }
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));

    execPathTextView = GTK_WIDGET(gtk_builder_get_object(builder, "text_view_exec_path"));
    desktopNameTextView = GTK_WIDGET(gtk_builder_get_object(builder, "text_view_desktop_name"));
    iconPathTextView = GTK_WIDGET(gtk_builder_get_object(builder, "text_view_icon_path"));
    applicationNameTextView = GTK_WIDGET(gtk_builder_get_object(builder, "text_view_application_name"));

    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}