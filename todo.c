#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <direct.h> // for _mkdir()

#define MAX_TASKS 100
#define MAX_LEN 100

int load_tasks(char tasks[][MAX_LEN]) {
    char path[512];
    snprintf(path, sizeof(path), "%s\\OneDrive\\Documents\\ToDoListDocuments\\todo_list.doc", getenv("USERPROFILE"));

    FILE *fp = fopen(path, "r");
    int count = 0;
    if (!fp) return 0;

    char line[MAX_LEN + 10];
    fgets(line, sizeof(line), fp); // Skip title
    fgets(line, sizeof(line), fp); // Skip empty line

    while (fgets(line, sizeof(line), fp) != NULL && count < MAX_TASKS) {
        char *task_start = strstr(line, ". ");
        if (task_start) {
            task_start += 2;
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
    int task_count = load_tasks(tasks);
    int choice;

    while (1) {
        printf("\n--- Personal To-Do List ---\n");
        printf("1. Add Task\n2. View Tasks\n3. Delete Task\n4. Save Tasks\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            if (task_count >= MAX_TASKS) printf("Task list is full!\n");
            else {
                printf("Enter your task: ");
                fgets(tasks[task_count], MAX_LEN, stdin);
                tasks[task_count][strcspn(tasks[task_count], "\n")] = 0;
                task_count++;
                printf("Task added!\n");
            }
        } else if (choice == 2) {
            if (task_count == 0) printf("No tasks yet!\n");
            else {
                printf("\nYour Tasks:\n");
                for (int i = 0; i < task_count; i++)
                    printf("%d. %s\n", i + 1, tasks[i]);
            }
        } else if (choice == 3) {
            if (task_count == 0) printf("No tasks to delete!\n");
            else {
                printf("\nWhich task number to delete?\n");
                for (int i = 0; i < task_count; i++)
                    printf("%d. %s\n", i + 1, tasks[i]);
                int del_num;
                scanf("%d", &del_num);
                getchar();
                if (del_num < 1 || del_num > task_count) printf("Invalid task number!\n");
                else {
                    for (int i = del_num - 1; i < task_count - 1; i++)
                        strcpy(tasks[i], tasks[i + 1]);
                    task_count--;
                    printf("Task deleted!\n");
                }
            }
        } else if (choice == 4) {
            char folder[512];
            snprintf(folder, sizeof(folder), "%s\\OneDrive\\Documents\\ToDoListDocuments", getenv("USERPROFILE"));
            _mkdir(folder); // Create folder if not exist

            char path[512];
            snprintf(path, sizeof(path), "%s\\todo_list.doc", folder);

            FILE *fp = fopen(path, "w");
            if (!fp) printf("Error creating file!\n");
            else {
                fprintf(fp, "Personal To-Do List\n\n");
                for (int i = 0; i < task_count; i++)
                    fprintf(fp, "%d. %s\n", i + 1, tasks[i]);
                fclose(fp);
                printf("Tasks saved to %s\n", path);
            }
        } else if (choice == 5) break;
        else printf("Invalid choice.\n");
    }
    return 0;
}
