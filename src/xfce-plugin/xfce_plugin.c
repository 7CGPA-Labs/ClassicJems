#include <gtk/gtk.h>
#include <libxfce4panel/libxfce4panel.h>

static void xfce_classicjems_construct(XfcePanelPlugin *plugin);
XFCE_PANEL_PLUGIN_REGISTER(xfce_classicjems_construct);

static void classicjems_button_clicked(GtkWidget *button, XfcePanelPlugin *plugin) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    GtkWidget *toplevel = gtk_widget_get_toplevel(GTK_WIDGET(plugin));
    
    dialog = gtk_file_chooser_dialog_new("Select J2ME or Symbian ROM",
                                         (toplevel && GTK_IS_WINDOW(toplevel)) ? GTK_WINDOW(toplevel) : NULL,
                                         action,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Open", GTK_RESPONSE_ACCEPT,
                                         NULL);

    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Java & Symbian ROMs (*.jar, *.sis, *.sisx)");
    gtk_file_filter_add_pattern(filter, "*.jar");
    gtk_file_filter_add_pattern(filter, "*.sis");
    gtk_file_filter_add_pattern(filter, "*.sisx");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        
        const char *platform = "J2ME";
        if (g_str_has_suffix(filename, ".sis") || g_str_has_suffix(filename, ".sisx")) {
            platform = "Symbian";
        }

        // Spawn classicjems-player process asynchronously
        gchar *cmd = g_strdup_printf("classicjems-player \"%s\" %s", filename, platform);
        g_spawn_command_line_async(cmd, NULL);
        g_free(cmd);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

static void xfce_classicjems_construct(XfcePanelPlugin *plugin) {
    xfce_panel_plugin_set_expand(plugin, FALSE);
    
    GtkWidget *button = xfce_panel_create_button();
    GtkWidget *icon = gtk_image_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_BUTTON);
    gtk_container_add(GTK_CONTAINER(button), icon);
    gtk_widget_show_all(button);
    
    gtk_widget_set_tooltip_text(button, "ClassicJems ROM Player");
    gtk_container_add(GTK_CONTAINER(plugin), button);
    
    g_signal_connect(button, "clicked", G_CALLBACK(classicjems_button_clicked), plugin);
}
