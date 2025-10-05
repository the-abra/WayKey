#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Function to send a key using wtype
void send_key(const char *key) {
    if (key == NULL) {
        return;
    }

    char command[256];
    // Construct the wtype command
    snprintf(command, sizeof(command), "wtype '%s'", key);

    // Log the key event to stdout for debugging
    g_print("Sending key: %s\n", key);

    // Execute the wtype command
    int result = system(command);
    if (result != 0) {
        g_printerr("Failed to send key: %s\n", key);
    }
}

// Callback function for all keyboard buttons
void on_key_clicked(GtkWidget *widget, gpointer data) {
    const char *key = (const char *)data;
    send_key(key);
}

// Main function
int main(int argc, char *argv[]) {
    // Check if wtype is available
    int wtype_check = system("command -v wtype > /dev/null 2>&1");
    if (wtype_check != 0) {
        g_printerr("Error: wtype command not found. Please install wtype.\n");
        return 1;
    }

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "WayKey – Wayland Virtual Keyboard");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 400);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Handle window close event
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a main vertical box container
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // Create header label
    GtkWidget *header_label = gtk_label_new("WayKey – Wayland Virtual Keyboard");
    gtk_box_pack_start(GTK_BOX(main_box), header_label, FALSE, FALSE, 0);

    // Create a grid for the keyboard layout
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_box_pack_start(GTK_BOX(main_box), grid, TRUE, TRUE, 0);

    // Define the keyboard layout
    const char *keys[4][12] = {
        {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "="},
        {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]"},
        {"A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "\\"},
        {"Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "Space", "Backspace"}
    };

    // Create buttons for the keyboard
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 12; col++) {
            if (keys[row][col] == NULL) {
                continue; // Skip empty keys
            }

            GtkWidget *button;
            
            // Special handling for space and backspace
            if (strcmp(keys[row][col], "Space") == 0) {
                button = gtk_button_new_with_label("Space");
                g_signal_connect(button, "clicked", G_CALLBACK(on_key_clicked), (gpointer)" ");
            } else if (strcmp(keys[row][col], "Backspace") == 0) {
                button = gtk_button_new_with_label("⌫");
                g_signal_connect(button, "clicked", G_CALLBACK(on_key_clicked), (gpointer)"\b");
            } else {
                button = gtk_button_new_with_label(keys[row][col]);
                g_signal_connect(button, "clicked", G_CALLBACK(on_key_clicked), (gpointer)keys[row][col]);
            }

            // Set button size
            gtk_widget_set_size_request(button, 60, 50);
            gtk_grid_attach(GTK_GRID(grid), button, col, row, 1, 1);
        }
    }

    // Create an extra row for Enter and other special keys
    GtkWidget *bottom_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(main_box), bottom_box, FALSE, FALSE, 0);

    // Enter key
    GtkWidget *enter_button = gtk_button_new_with_label("Enter");
    gtk_widget_set_size_request(enter_button, 120, 50);
    g_signal_connect(enter_button, "clicked", G_CALLBACK(on_key_clicked), (gpointer)"\n");
    gtk_box_pack_start(GTK_BOX(bottom_box), enter_button, FALSE, FALSE, 0);

    // Tab key
    GtkWidget *tab_button = gtk_button_new_with_label("Tab");
    gtk_widget_set_size_request(tab_button, 80, 50);
    g_signal_connect(tab_button, "clicked", G_CALLBACK(on_key_clicked), (gpointer)"\t");
    gtk_box_pack_start(GTK_BOX(bottom_box), tab_button, FALSE, FALSE, 0);

    // Additional symbols
    GtkWidget *symbol_button = gtk_button_new_with_label("?123");
    gtk_widget_set_size_request(symbol_button, 80, 50);
    g_signal_connect(symbol_button, "clicked", G_CALLBACK(on_key_clicked), (gpointer)"");
    gtk_box_pack_start(GTK_BOX(bottom_box), symbol_button, FALSE, FALSE, 0);

    // Show all widgets
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}