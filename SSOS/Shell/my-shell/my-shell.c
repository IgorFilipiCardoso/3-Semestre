#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "queue.h"

#define BUFFER_SIZE 300

void type_prompt();
void read_command(char*, char*);
void intern_commands(char*, char*);
void history_command(Queue, char*);
void broke_string(char*, char*);
char* get_username();
char* get_hostname();
struct tm* get_time();
char** get_path(char* command, char* parameters);


int main() {
    Queue history;
    bool exit = false;

    init(&history);

    do {

        char* command = malloc(sizeof(char) * BUFFER_SIZE);
        char* parameters = malloc(sizeof(char) * BUFFER_SIZE);
        int status = 0;

        type_prompt();
        read_command(command, parameters);

        enqueue(history, command);

        if (!strcmp(command, "exit\n")) { exit = true; }
        if (!exit) {
            if (!strcmp(command, "history\n")) { history_command(history, command); }
            if (!strcmp(command, "c")) { intern_commands(command, parameters); }
            if (fork() != 0) {
                waitpid(-1, &status, 0);
            } else {
                char* args[] = {"/bin/ls", NULL};
                char* nullArg[] = {NULL};
                char cmd[] = "ls";
                execve(cmd, args, nullArg);
            }
        }

    } while (!exit);

    return 0;
}

void history_command(Queue history, char* command) {
    int queue_size = size(history);

    for (int i = 0; i < queue_size; i++) {
        char* temp_string;
        temp_string = dequeue(history);
        enqueue(history, temp_string);
        printf("[%d]: %s\n", i + 1, temp_string);
    }

    char* string = malloc(sizeof(char) * BUFFER_SIZE);
    char* nothing;
    read_command(string, nothing);
    string = strtok(string, "!");

    int index = atoi(string);

    for (int i = 0; i < queue_size; i++) {
        char* temp_string;
        temp_string = dequeue(history);
        enqueue(history, temp_string);

        if (i == index - 1)
            command = temp_string;
    }

    enqueue(history, command);
}

char** get_path(char* command, char* parameters) {
    char* path = malloc(sizeof(char) * 100);
    strcpy(path, "/bin/");
    strcat(path, command);
    char** args = malloc(sizeof(char*) * 3);
    args[0] = path;
    args[1] = parameters;
    args[2] = NULL;
    return args;
} 

void type_prompt() {
    struct tm* time = get_time();
    printf("%s@%s[%02d:%02d:%02d]$ ", get_username(), get_hostname(), time->tm_hour, time->tm_min, time->tm_sec);
}

struct tm* get_time() {
    time_t segundos;
    time(&segundos);

    return localtime(&segundos);
}

char* get_username() {
    char* username = getenv("USER");
    return username;
}

char* get_hostname() {
    char* hostname = malloc(sizeof(char) * 100);
    gethostname(hostname, 100);
    return hostname;
}

void intern_commands(char* command, char* parameters) {
    printf("oi");
}


void read_command(char* command, char* parameters) {
    fgets(command, BUFFER_SIZE, stdin);
    broke_string(command, parameters);
}

void broke_string(char* command, char* parameters) {
    char* temp = strtok(command, " ");
    strcpy(command, temp);
    temp = strtok(NULL, " ");
    strcpy(parameters, temp);
}