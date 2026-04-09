/*
 * GTK3 Animation Examples for To-Do List Application
 * Beginner-friendly animation snippets
 * Compile: gcc animation-examples.c -o animation-examples `pkg-config --cflags --libs gtk+-3.0`
 */

#include <gtk/gtk.h>
#include <glib.h>
#include <math.h>

// Animation timing constants
#define ANIMATION_DURATION 500  // milliseconds
#define FRAME_RATE 60           // frames per second

// Animation structure
typedef struct {
    GtkWidget *widget;
    gint start_x, start_y;
    gint end_x, end_y;
    gint current_frame;
    gint total_frames;
    gboolean is_active;
    void (*completion_callback)(GtkWidget *widget);
} Animation;

// Global animation list
#define MAX_ANIMATIONS 50
Animation animations[MAX_ANIMATIONS];
gint active_animations = 0;

// Simple bounce animation
void animate_bounce(GtkWidget *widget) {
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    
    gint original_y = allocation.y;
    gint bounce_height = 20;
    gint frames = 30;
    
    for (gint i = 0; i < frames; i++) {
        g_timeout_add(i * (ANIMATION_DURATION / frames), (GSourceFunc)animate_bounce_frame, 
                      g_new0(gint, 3) {0, original_y, bounce_height});
    }
}

gboolean animate_bounce_frame(gpointer data) {
    gint *params = (gint *)data;
    gint frame = params[0];
    gint original_y = params[1];
    gint bounce_height = params[2];
    
    // Calculate bounce position using sine wave
    gdouble progress = (gdouble)frame / 30.0;
    gdouble bounce = sin(progress * G_PI) * bounce_height;
    
    // Apply transformation (simplified - in real GTK you'd use CSS transforms)
    // This is a conceptual example
    
    g_free(params);
    return FALSE; // Don't repeat
}

// Slide-in animation from left
void animate_slide_in(GtkWidget *widget) {
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    
    // Create CSS animation
    GtkCssProvider *css_provider = gtk_css_provider_new();
    const char *css = 
        "@keyframes slideIn { "
        "from { opacity: 0; transform: translateX(-50px); } "
        "to { opacity: 1; transform: translateX(0); } "
        "} "
        ".slide-in { animation: slideIn 0.5s ease-out; }";
    
    gtk_css_provider_load_from_data(css_provider, css, -1, NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), 
                                  GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_style_context_add_class(context, "slide-in");
    
    // Remove animation class after completion
    g_timeout_add(500, (GSourceFunc)remove_animation_class, widget);
}

gboolean remove_animation_class(GtkWidget *widget) {
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_remove_class(context, "slide-in");
    return FALSE;
}

// Pulse animation (scale up and down)
void animate_pulse(GtkWidget *widget) {
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    
    GtkCssProvider *css_provider = gtk_css_provider_new();
    const char *css = 
        "@keyframes pulse { "
        "0% { transform: scale(1); } "
        "50% { transform: scale(1.1); } "
        "100% { transform: scale(1); } "
        "} "
        ".pulse { animation: pulse 0.8s ease-in-out; }";
    
    gtk_css_provider_load_from_data(css_provider, css, -1, NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), 
                                  GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_style_context_add_class(context, "pulse");
    
    g_timeout_add(800, (GSourceFunc)remove_animation_class, widget);
}

// Color transition animation
void animate_color_transition(GtkWidget *widget, const char *from_color, const char *to_color) {
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    
    // Create CSS transition
    GtkCssProvider *css_provider = gtk_css_provider_new();
    char css[256];
    snprintf(css, sizeof(css), 
        ".color-transition { "
        "background-color: %s; "
        "transition: background-color 0.3s ease; "
        "} "
        ".color-transition:hover { "
        "background-color: %s; "
        "}", from_color, to_color);
    
    gtk_css_provider_load_from_data(css_provider, css, -1, NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), 
                                  GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_style_context_add_class(context, "color-transition");
}

// Shake animation (for errors)
void animate_shake(GtkWidget *widget) {
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    
    GtkCssProvider *css_provider = gtk_css_provider_new();
    const char *css = 
        "@keyframes shake { "
        "0%, 100% { transform: translateX(0); } "
        "10%, 30%, 50%, 70%, 90% { transform: translateX(-5px); } "
        "20%, 40%, 60%, 80% { transform: translateX(5px); } "
        "} "
        ".shake { animation: shake 0.5s ease-in-out; }";
    
    gtk_css_provider_load_from_data(css_provider, css, -1, NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), 
                                  GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_style_context_add_class(context, "shake");
    
    g_timeout_add(500, (GSourceFunc)remove_animation_class, widget);
}

// Fade in animation
void animate_fade_in(GtkWidget *widget) {
    gtk_widget_set_opacity(widget, 0.0);
    gtk_widget_show(widget);
    
    // Animate opacity
    gint frames = 30;
    for (gint i = 0; i <= frames; i++) {
        gdouble opacity = (gdouble)i / frames;
        g_timeout_add(i * (ANIMATION_DURATION / frames), 
                      (GSourceFunc)set_widget_opacity, 
                      g_new0(gdouble, 2) {widget, opacity});
    }
}

gboolean set_widget_opacity(gpointer data) {
    gdouble *params = (gdouble *)data;
    GtkWidget *widget = GTK_WIDGET(params[0]);
    gdouble opacity = params[1];
    
    gtk_widget_set_opacity(widget, opacity);
    g_free(params);
    return FALSE;
}

// Rotate animation
void animate_rotate(GtkWidget *widget) {
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    
    GtkCssProvider *css_provider = gtk_css_provider_new();
    const char *css = 
        "@keyframes rotate { "
        "from { transform: rotate(0deg); } "
        "to { transform: rotate(360deg); } "
        "} "
        ".rotate { animation: rotate 1s linear; }";
    
    gtk_css_provider_load_from_data(css_provider, css, -1, NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), 
                                  GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_style_context_add_class(context, "rotate");
    
    g_timeout_add(1000, (GSourceFunc)remove_animation_class, widget);
}

// Example usage in a To-Do List context
void on_task_added(GtkWidget *task_item) {
    animate_slide_in(task_item);
    animate_pulse(task_item);  // Double animation for emphasis
}

void on_task_completed(GtkWidget *task_item) {
    animate_bounce(task_item);
    animate_color_transition(task_item, "#ffffff", "#e8f5e8");
}

void on_task_deleted(GtkWidget *task_item) {
    animate_shake(task_item);
    // Then fade out and remove
    g_timeout_add(500, (GSourceFunc)animate_fade_out, task_item);
}

gboolean animate_fade_out(GtkWidget *widget) {
    gint frames = 20;
    for (gint i = 0; i <= frames; i++) {
        gdouble opacity = 1.0 - ((gdouble)i / frames);
        g_timeout_add(i * (ANIMATION_DURATION / frames), 
                      (GSourceFunc)set_widget_opacity, 
                      g_new0(gdouble, 2) {widget, opacity});
    }
    
    // Remove widget after fade out
    g_timeout_add(ANIMATION_DURATION, (GSourceFunc)gtk_widget_destroy, widget);
    return FALSE;
}

void on_button_click(GtkWidget *button) {
    animate_pulse(button);
}

void on_error_occurred(GtkWidget *error_widget) {
    animate_shake(error_widget);
    animate_color_transition(error_widget, "#ffffff", "#ffebee");
}

// Mascot animations
void animate_mascot_happy(GtkWidget *mascot) {
    animate_bounce(mascot);
    // Change to happy image
    gtk_image_set_from_file(GTK_IMAGE(mascot), "mascot_happy.png");
}

void animate_mascot_working(GtkWidget *mascot) {
    animate_rotate(mascot);
    // Change to working image
    gtk_image_set_from_file(GTK_IMAGE(mascot), "mascot_working.png");
}

void animate_mascot_excited(GtkWidget *mascot) {
    // Multiple animations for excitement
    animate_bounce(mascot);
    g_timeout_add(200, (GSourceFunc)animate_bounce, mascot);
    g_timeout_add(400, (GSourceFunc)animate_bounce, mascot);
    // Change to excited image
    gtk_image_set_from_file(GTK_IMAGE(mascot), "mascot_excited.png");
}

// Integration example with main application
void setup_animations_for_todo_app(GtkWidget *window) {
    // Apply global CSS animations
    GtkCssProvider *css_provider = gtk_css_provider_new();
    const char *global_css = 
        "window { background: linear-gradient(135deg, #667eea, #764ba2); }"
        ".task-item { "
        "background: rgba(255, 255, 255, 0.95); "
        "border-radius: 15px; "
        "margin: 5px; "
        "padding: 15px; "
        "transition: all 0.3s ease; "
        "} "
        ".task-item:hover { "
        "transform: translateX(5px) scale(1.02); "
        "box-shadow: 0 10px 20px rgba(0,0,0,0.1); "
        "} "
        "button { "
        "background: linear-gradient(135deg, #ff6b6b, #e73c7e); "
        "color: white; "
        "border-radius: 25px; "
        "padding: 12px 24px; "
        "border: none; "
        "font-weight: bold; "
        "transition: all 0.3s ease; "
        "} "
        "button:hover { "
        "transform: translateY(-3px); "
        "box-shadow: 0 10px 20px rgba(0,0,0,0.2); "
        "background: linear-gradient(135deg, #ff5252, #e91e63); "
        "} "
        "entry { "
        "border-radius: 25px; "
        "padding: 12px 20px; "
        "border: 3px solid #e73c7e; "
        "background: rgba(255,255,255,0.9); "
        "transition: all 0.3s ease; "
        "} "
        "entry:focus { "
        "border-color: #ff6b6b; "
        "box-shadow: 0 0 0 3px rgba(255,107,107,0.2); "
        "background: rgba(255,255,255,1); "
        "}";
    
    gtk_css_provider_load_from_data(css_provider, global_css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), 
                                  GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

// Main function to demonstrate animations
int main(int argc, char **argv) {
    gtk_init(&argc, &argv);
    
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Animation Examples");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    
    setup_animations_for_todo_app(window);
    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    gtk_widget_show_all(window);
    gtk_main();
    
    return 0;
}
