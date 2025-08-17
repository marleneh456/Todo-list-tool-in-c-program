#include <stdio.h>
#include <string.h>

#define MAX_TASKS 100
#define MAX_LEN 100

// Function to load tasks from file
int load_tasks(char tasks[][MAX_LEN]) {
    FILE *fp = fopen("todo_list.doc", "r");
    int count = 0;
    if (fp == NULL) {
        // File does not exist; nothing to load
        return 0;
    }

    char line[MAX_LEN + 10];
    fgets(line, sizeof(line), fp); // Skip title line
    fgets(line, sizeof(line), fp); // Skip empty line

    // Read each task line
    while (fgets(line, sizeof(line), fp) != NULL && count < MAX_TASKS) {
        // Find the position of ". " (e.g., "3. Task text")
        char *task_start = strstr(line, ". ");
        if (task_start) {
            // Move pointer after ". "
            task_start += 2;
            // Remove newline
            task_start[strcspn(task_start, "\n")] = 0;
            strcpy(tasks[count], task_start);
            count++;
        }
    }
    fclose(fp);
    return count;
}

int main() {
    char tasks[MAX_TASKS][MAX_LEN];
    int task_count = 0;
    int choice;

    // Load tasks from file at startup
    task_count = load_tasks(tasks);

    while (1) {
        printf("\n--- Personal To-Do List ---\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Delete Task\n");
        printf("4. Save Tasks to .doc File\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Remove newline

        if (choice == 1) {
            // Add Task
            if (task_count >= MAX_TASKS) {
                printf("Task list is full!\n");
            } else {
                printf("Enter your task: ");
                fgets(tasks[task_count], MAX_LEN, stdin);
                tasks[task_count][strcspn(tasks[task_count], "\n")] = 0;
                task_count++;
                printf("Task added!\n");
            }
        } else if (choice == 2) {
            // View Tasks
            if (task_count == 0) {
                printf("No tasks yet!\n");
            } else {
                printf("\nYour Tasks:\n");
                for (int i = 0; i < task_count; i++) {
                    printf("%d. %s\n", i + 1, tasks[i]);
                }
            }
        } else if (choice == 3) {
            // Delete Task
            if (task_count == 0) {
                printf("No tasks to delete!\n");
            } else {
                printf("\nWhich task number do you want to delete?\n");
                for (int i = 0; i < task_count; i++) {
                    printf("%d. %s\n", i + 1, tasks[i]);
                }
                printf("Enter the task number to delete: ");
                int del_num;
                scanf("%d", &del_num);
                getchar();

                if (del_num < 1 || del_num > task_count) {
                    printf("Invalid task number!\n");
                } else {
                    for (int i = del_num - 1; i < task_count - 1; i++) {
                        strcpy(tasks[i], tasks[i + 1]);
                    }
                    task_count--;
                    printf("Task deleted!\n");
                }
            }
        } else if (choice == 4) {
            // Save Tasks to .doc File
            FILE *fp = fopen("todo_list.doc", "w");
            if (fp == NULL) {
                printf("Error creating file!\n");
            } else {
                fprintf(fp, "Personal To-Do List\n\n");
                for (int i = 0; i < task_count; i++) {
                    fprintf(fp, "%d. %s\n", i + 1, tasks[i]);
                }
                fclose(fp);
                printf("Tasks saved to todo_list.doc!\n");
            }
        } else if (choice == 5) {
            printf("Goodbye!\n");
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}