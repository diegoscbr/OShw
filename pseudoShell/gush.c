#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


void gush_loop();
char* readLine();
char** divideLine(char* line); //parse the line into tokens and return an array of the tokens
void executeCommand(char** argumentsArray);


int main(int argc, char* argv[]){
//interactive mode
    gush_loop();
    return 0;
}





/*****************************/
/*****************************/
void gush_loop(){
    
    char* line; //line entered by user

    char** argsArr; //array of tokens

    int EXIT_FLG = 0;
    do{
        printf("gush>");
        //initially reado of line
        line = readLine();
        if(strcmp(line,"exit") == 0){EXIT_FLG = 1;} //check if user wants to exit
        argsArr = divideLine(line); //parse the line into tokens array
        //printf("args[0]: %s\n", argsArr[0]);
       // printf("args[1]: %s\n", argsArr[1]);
       if(strcmp(argsArr[0], "ls") == 0){
              argsArr[0] = "/bin/ls";
              executeCommand(argsArr); 
       }
        
        free(line);

        
    } while (EXIT_FLG == 0);
}
/*****************************/
/*****************************/
/*SIMPLE READ INITIALLY*/
char* readLine() {
    char* line = NULL;
    size_t bufsize = 0;
    getline(&line, &bufsize, stdin); //getline will allocate memory for line
    line[strcspn(line, "\n")] = 0; //removes the newline character from the end of the string
    return line;
}
/*****************************/
/*****************************/
/*PARSE THE LINE:
 ** into commands and arguments to be read*/
char** divideLine(char* line){
    int capacity = 10;
    int size = 0;

    char** tokArr = malloc(capacity * sizeof(char*));
    if (tokArr == NULL){
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }
    char* tokArrElement = strtok(line, " ");//get the first token

    while(tokArrElement != NULL){
        if(size >= capacity){
            capacity *= 2;
            tokArr = realloc(tokArr, capacity * sizeof(char*));
        if (tokArr == NULL){
            fprintf(stderr, "allocation error\n");
            exit(EXIT_FAILURE);
        }
    }
    tokArr[size] = tokArrElement;
    size++;

    tokArrElement = strtok(NULL, " ");
    }
    if(size >= capacity){
        capacity *= 2;
        tokArr = realloc(tokArr, (capacity+1) * sizeof(char*));
        if (tokArr == NULL){
            fprintf(stderr, "allocation error\n");
            exit(EXIT_FAILURE);
        }
    }
    tokArr[size] = NULL;
    return tokArr;  
}
/*****************************/
/*****************************/
void executeCommand(char** args) {
    // Fork a new process
    pid_t pid = fork();

    if (pid == -1) {
        // Fork failed
        fprintf(stderr, "fork failed\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // This is the child process. Execute the command.
        char* envp[] = { NULL }; // Empty environment for execve
        if (execve(args[0], args, envp) == -1) {
            perror("execve failed");
            exit(EXIT_FAILURE);
        }
    } else {
        // This is the parent process. Wait for the child to finish.
        int status;
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

