#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TASKS 1000
#define MAX_TASK_LENGTH 255
#define FILENAME "tasks.dat"

// Task structure to hold task information
typedef struct {
    int id;
    char description[MAX_TASK_LENGTH];
    int completed;  // 0 = not completed, 1 = completed
    time_t created_at;
} Task;

// Global array to hold tasks
Task tasks[MAX_TASKS];
int task_count = 0;

// Function prototypes
void load_tasks();
void save_tasks();
int generate_id();
void add_task(const char* description);
void delete_task(int id);
void update_task_status(int id, int completed);
void list_tasks();
void print_json_output();
void print_task_json(Task task);

int main(int argc, char *argv[]) {
    // Load existing tasks from file
    load_tasks();
    
    // Parse command line arguments
    if (argc < 2) {
        printf("Usage: %s [add|delete|update|list] [arguments]\n", argv[0]);
        return 1;
    }
    
    char *command = argv[1];
    
    if (strcmp(command, "add") == 0) {
        if (argc < 3) {
            printf("Error: Task description required for add command\n");
            return 1;
        }
        add_task(argv[2]);
        save_tasks();
        print_json_output();
    }
    else if (strcmp(command, "delete") == 0) {
        if (argc < 3) {
            printf("Error: Task ID required for delete command\n");
            return 1;
        }
        int id = atoi(argv[2]);
        delete_task(id);
        save_tasks();
        print_json_output();
    }
    else if (strcmp(command, "update") == 0) {
        if (argc < 4) {
            printf("Error: Task ID and status required for update command\n");
            return 1;
        }
        int id = atoi(argv[2]);
        int completed = atoi(argv[3]);
        update_task_status(id, completed);
        save_tasks();
        print_json_output();
    }
    else if (strcmp(command, "list") == 0) {
        print_json_output();
    }
    else {
        printf("Error: Unknown command '%s'\n", command);
        return 1;
    }
    
    return 0;
}

// Load tasks from binary file
void load_tasks() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        // File doesn't exist, start with empty task list
        task_count = 0;
        return;
    }
    
    // Read the number of tasks
    fread(&task_count, sizeof(int), 1, file);
    
    // Read all tasks
    if (task_count > 0) {
        fread(tasks, sizeof(Task), task_count, file);
    }
    
    fclose(file);
}

// Save tasks to binary file
void save_tasks() {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("Error: Could not save tasks to file\n");
        return;
    }
    
    // Write the number of tasks
    fwrite(&task_count, sizeof(int), 1, file);
    
    // Write all tasks
    if (task_count > 0) {
        fwrite(tasks, sizeof(Task), task_count, file);
    }
    
    fclose(file);
}

// Generate a unique ID for new tasks
int generate_id() {
    int max_id = 0;
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].id > max_id) {
            max_id = tasks[i].id;
        }
    }
    return max_id + 1;
}

// Add a new task
void add_task(const char* description) {
    if (task_count >= MAX_TASKS) {
        printf("Error: Maximum number of tasks reached\n");
        return;
    }
    
    Task new_task;
    new_task.id = generate_id();
    strncpy(new_task.description, description, MAX_TASK_LENGTH - 1);
    new_task.description[MAX_TASK_LENGTH - 1] = '\0';  // Ensure null termination
    new_task.completed = 0;
    new_task.created_at = time(NULL);
    
    tasks[task_count] = new_task;
    task_count++;
}

// Delete a task by ID
void delete_task(int id) {
    int found = 0;
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].id == id) {
            // Shift all tasks after this one to the left
            for (int j = i; j < task_count - 1; j++) {
                tasks[j] = tasks[j + 1];
            }
            task_count--;
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Error: Task with ID %d not found\n", id);
    }
}

// Update task completion status
void update_task_status(int id, int completed) {
    int found = 0;
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].id == id) {
            tasks[i].completed = completed;
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Error: Task with ID %d not found\n", id);
    }
}

// Print a single task as JSON
void print_task_json(Task task) {
    printf("{\"id\":%d,\"description\":\"%s\",\"completed\":%s,\"created_at\":%ld}",
           task.id,
           task.description,
           task.completed ? "true" : "false",
           task.created_at);
}

// Print all tasks as JSON array
void print_json_output() {
    printf("[");
    for (int i = 0; i < task_count; i++) {
        print_task_json(tasks[i]);
        if (i < task_count - 1) {
            printf(",");
        }
    }
    printf("]");
}
