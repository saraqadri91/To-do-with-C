/*
 * GTK3 To-Do List Application with Animations
 * Compile: gcc gtk3-todo-ui.c -o gtk3-todo-ui `pkg-config --cflags --libs gtk+-3.0`
 */

#include <gtk/gtk.h>
#include <glib.h>
#include <string.h>
#include <time.h>

#define MAX_TASKS 100
#define MAX_TASK_LENGTH 255

typedef struct {
    int id;
    char description[MAX_TASK_LENGTH];
    gboolean completed;
    time_t created_at;
    GtkWidget *list_item;
    GtkWidget *checkbox;
    GtkWidget *label;
    GtkWidget *delete_button;
} Task;

typedef struct {
    GtkWidget *window;
    GtkWidget *main_box;
    GtkWidget *entry;
    GtkWidget *add_button;
    GtkWidget *task_list;
    GtkWidget *mascot_image;
    GtkWidget *status_label;
    Task tasks[MAX_TASKS];
    int task_count;
    int next_id;
} TodoApp;

// Animation functions
static void animate_mascot_happy(GtkWidget *mascot) {
    // Animate mascot with happy animation
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_size("mascot_happy.png", 64, 64, NULL);
    if (pixbuf) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(mascot), g_object_ref(pixbuf));
        
        // Bounce animation
        gtk_widget_set_name(mascot, "mascot-happy");
        GtkCssProvider *css_provider = gtk_css_provider_new();
        const char *css = "#mascot-happy { animation: bounce 0.6s ease-in-out; }";
        gtk_css_provider_load_from_data(css_provider, css, -1, NULL);
        GtkStyleContext *context = gtk_widget_get_style_context(mascot);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        
        g_object_unref(pixbuf);
    }
}

static void animate_mascot_working(GtkWidget *mascot) {
    // Animate mascot with working animation
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_size("mascot_working.png", 64, 64, NULL);
    if (pixbuf) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(mascot), g_object_ref(pixbuf));
        
        // Spin animation
        gtk_widget_set_name(mascot, "mascot-working");
        GtkCssProvider *css_provider = gtk_css_provider_new();
        const char *css = "#mascot-working { animation: spin 1s linear infinite; }";
        gtk_css_provider_load_from_data(css_provider, css, -1, NULL);
        GtkStyleContext *context = gtk_widget_get_style_context(mascot);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        
        g_object_unref(pixbuf);
    }
}

static void animate_task_add(GtkWidget *task_item) {
    // Slide-in animation for new tasks
    gtk_widget_set_name(task_item, "task-new");
    GtkCssProvider *css_provider = gtk_css_provider_new();
    const char *css = "#task-new { animation: slideIn 0.3s ease-in-out; }";
    gtk_css_provider_load_from_data(css_provider, css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(task_item);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

static void animate_task_complete(GtkWidget *task_item) {
    // Celebration animation for completed tasks
    gtk_widget_set_name(task_item, "task-complete");
    GtkCssProvider *css_provider = gtk_css_provider_new();
    const char *css = "#task-complete { animation: celebrate 0.8s ease-in-out; }";
    gtk_css_provider_load_from_data(css_provider, css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(task_item);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

static void animate_task_delete(GtkWidget *task_item) {
    // Fade-out animation for deleted tasks
    gtk_widget_set_name(task_item, "task-delete");
    GtkCssProvider *css_provider = gtk_css_provider_new();
    const char *css = "#task-delete { animation: fadeOut 0.3s ease-in-out; }";
    gtk_css_provider_load_from_data(css_provider, css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(task_item);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

// Task management functions
static void update_status_label(TodoApp *app) {
    int completed = 0;
    for (int i = 0; i < app->task_count; i++) {
        if (app->tasks[i].completed) {
            completed++;
        }
    }
    
    char status_text[100];
    snprintf(status_text, sizeof(status_text), "Tasks: %d/%d completed", completed, app->task_count);
    gtk_label_set_text(GTK_LABEL(app->status_label), status_text);
}

static void add_task(TodoApp *app) {
    const char *description = gtk_entry_get_text(GTK_ENTRY(app->entry));
    
    if (strlen(description) == 0) {
        return;
    }
    
    if (app->task_count >= MAX_TASKS) {
        return;
    }
    
    // Animate mascot working
    animate_mascot_working(app->mascot_image);
    
    // Create new task
    Task *task = &app->tasks[app->task_count];
    task->id = app->next_id++;
    strncpy(task->description, description, MAX_TASK_LENGTH - 1);
    task->description[MAX_TASK_LENGTH - 1] = '\0';
    task->completed = FALSE;
    task->created_at = time(NULL);
    
    // Create task widget
    GtkWidget *task_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_margin_top(task_box, 5);
    gtk_widget_set_margin_bottom(task_box, 5);
    gtk_widget_set_margin_start(task_box, 10);
    gtk_widget_set_margin_end(task_box, 10);
    
    // Checkbox
    task->checkbox = gtk_check_button_new();
    g_signal_connect(task->checkbox, "toggled", G_CALLBACK(on_task_toggled), task);
    
    // Label
    task->label = gtk_label_new(task->description);
    gtk_widget_set_hexpand(task->label, TRUE);
    gtk_label_set_xalign(GTK_LABEL(task->label), 0.0);
    
    // Delete button
    task->delete_button = gtk_button_new_with_label("Delete");
    g_signal_connect(task->delete_button, "clicked", G_CALLBACK(on_delete_clicked), task);
    
    // Pack widgets
    gtk_box_pack_start(GTK_BOX(task_box), task->checkbox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(task_box), task->label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(task_box), task->delete_button, FALSE, FALSE, 0);
    
    // Add to list
    gtk_list_box_insert(GTK_LIST_BOX(app->task_list), task_box, -1);
    task->list_item = task_box;
    
    // Animate task addition
    animate_task_add(task_box);
    
    // Clear entry
    gtk_entry_set_text(GTK_ENTRY(app->entry), "");
    
    app->task_count++;
    update_status_label(app);
    
    // Animate mascot happy after delay
    g_timeout_add(1000, (GSourceFunc)animate_mascot_happy, app->mascot_image);
}

static void on_task_toggled(GtkWidget *checkbox, gpointer user_data) {
    Task *task = (Task *)user_data;
    TodoApp *app = g_object_get_data(G_OBJECT(checkbox), "app");
    
    task->completed = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbox));
    
    // Update label style
    if (task->completed) {
        // Apply strikethrough style
        PangoAttrList *attr_list = pango_attr_list_new();
        PangoAttribute *attr = pango_attr_strikethrough_new(TRUE);
        pango_attr_list_insert(attr_list, attr);
        gtk_label_set_attributes(GTK_LABEL(task->label), attr_list);
        
        // Animate completion
        animate_task_complete(task->list_item);
        animate_mascot_happy(app->mascot_image);
    } else {
        // Remove strikethrough
        gtk_label_set_attributes(GTK_LABEL(task->label), NULL);
    }
    
    update_status_label(app);
}

static void on_delete_clicked(GtkWidget *button, gpointer user_data) {
    Task *task = (Task *)user_data;
    TodoApp *app = g_object_get_data(G_OBJECT(button), "app");
    
    // Animate deletion
    animate_task_delete(task->list_item);
    
    // Remove after animation
    g_timeout_add(300, (GSourceFunc)gtk_widget_destroy, task->list_item);
    
    // Remove from array (simple approach - just mark as deleted)
    task->list_item = NULL;
    
    update_status_label(app);
}

static void on_add_clicked(GtkWidget *button, gpointer user_data) {
    TodoApp *app = (TodoApp *)user_data;
    add_task(app);
}

static void on_entry_activated(GtkWidget *entry, gpointer user_data) {
    TodoApp *app = (TodoApp *)user_data;
    add_task(app);
}

// CSS styling
static void apply_css(GtkWidget *widget) {
    GtkCssProvider *css_provider = gtk_css_provider_new();
    const char *css = 
        "window { background: linear-gradient(135deg, #667eea, #764ba2); }"
        ".task-item { background: rgba(255, 255, 255, 0.95); border-radius: 15px; margin: 5px; padding: 10px; }"
        ".task-item:hover { transform: scale(1.02); box-shadow: 0 10px 20px rgba(0,0,0,0.1); }"
        "button { background: linear-gradient(135deg, #ff6b6b, #e73c7e); color: white; border-radius: 20px; padding: 10px 20px; border: none; }"
        "button:hover { transform: translateY(-2px); box-shadow: 0 5px 15px rgba(0,0,0,0.2); }"
        "entry { border-radius: 20px; padding: 10px 15px; border: 2px solid #e73c7e; background: rgba(255,255,255,0.9); }"
        "@keyframes bounce { 0%, 20%, 50%, 80%, 100% { transform: translateY(0); } 40% { transform: translateY(-10px); } 60% { transform: translateY(-5px); } }"
        "@keyframes spin { 0% { transform: rotate(0deg); } 100% { transform: rotate(360deg); } }"
        "@keyframes slideIn { from { opacity: 0; transform: translateX(-20px); } to { opacity: 1; transform: translateX(0); } }"
        "@keyframes celebrate { 0% { transform: scale(1) rotate(0deg); } 25% { transform: scale(1.2) rotate(5deg); } 50% { transform: scale(1.1) rotate(-5deg); } 75% { transform: scale(1.15) rotate(3deg); } 100% { transform: scale(1) rotate(0deg); } }"
        "@keyframes fadeOut { from { opacity: 1; } to { opacity: 0; transform: translateX(20px); } }";
    
    gtk_css_provider_load_from_data(css_provider, css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

static void activate(GtkApplication *app, gpointer user_data) {
    TodoApp *todo_app = g_new0(TodoApp, 1);
    
    // Create main window
    todo_app->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(todo_app->window), "To-Do List with Animations");
    gtk_window_set_default_size(GTK_WINDOW(todo_app->window), 600, 700);
    gtk_window_set_position(GTK_WINDOW(todo_app->window), GTK_WIN_POS_CENTER);
    
    // Apply CSS
    apply_css(todo_app->window);
    
    // Main container
    todo_app->main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_widget_set_margin_top(todo_app->main_box, 20);
    gtk_widget_set_margin_bottom(todo_app->main_box, 20);
    gtk_widget_set_margin_start(todo_app->main_box, 20);
    gtk_widget_set_margin_end(todo_app->main_box, 20);
    gtk_container_add(GTK_CONTAINER(todo_app->window), todo_app->main_box);
    
    // Header with mascot
    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(todo_app->main_box), header_box, FALSE, FALSE, 0);
    
    // Mascot image
    todo_app->mascot_image = gtk_image_new();
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_size("mascot_happy.png", 64, 64, NULL);
    if (pixbuf) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(todo_app->mascot_image), pixbuf);
        g_object_unref(pixbuf);
    }
    gtk_box_pack_start(GTK_BOX(header_box), todo_app->mascot_image, FALSE, FALSE, 0);
    
    // Title
    GtkWidget *title_label = gtk_label_new("To-Do List");
    GtkStyleContext *title_context = gtk_widget_get_style_context(title_label);
    gtk_style_context_add_class(title_context, "title");
    gtk_box_pack_start(GTK_BOX(header_box), title_label, TRUE, TRUE, 0);
    
    // Input section
    GtkWidget *input_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(todo_app->main_box), input_box, FALSE, FALSE, 0);
    
    todo_app->entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(todo_app->entry), "What needs to be done?");
    gtk_widget_set_hexpand(todo_app->entry, TRUE);
    g_signal_connect(todo_app->entry, "activate", G_CALLBACK(on_entry_activated), todo_app);
    gtk_box_pack_start(GTK_BOX(input_box), todo_app->entry, TRUE, TRUE, 0);
    
    todo_app->add_button = gtk_button_new_with_label("Add Task");
    g_signal_connect(todo_app->add_button, "clicked", G_CALLBACK(on_add_clicked), todo_app);
    gtk_box_pack_start(GTK_BOX(input_box), todo_app->add_button, FALSE, FALSE, 0);
    
    // Task list
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_box_pack_start(GTK_BOX(todo_app->main_box), scrolled_window, TRUE, TRUE, 0);
    
    todo_app->task_list = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(scrolled_window), todo_app->task_list);
    
    // Status label
    todo_app->status_label = gtk_label_new("Tasks: 0/0 completed");
    gtk_box_pack_start(GTK_BOX(todo_app->main_box), todo_app->status_label, FALSE, FALSE, 0);
    
    // Initialize
    todo_app->task_count = 0;
    todo_app->next_id = 1;
    update_status_label(todo_app);
    
    // Show all widgets
    gtk_widget_show_all(todo_app->window);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.example.todo", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    
    return status;
}
