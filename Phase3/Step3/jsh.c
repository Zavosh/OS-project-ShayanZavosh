#include <stdio.h>
#include <string.h>
#include <wait.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

//Macros
#define DEFAULT_COMMAND_SIZE (size_t) 20
#define DEFAULT_ARGUMENT_COUNT 10
#define DEFAULT_ARGUMENT_SIZE 10
#define DEFAULT_NAME_SIZE 20

int main(int argc, char* argv[]){ 
	int retval;
	int arg_counter;
	size_t* command_size = (size_t*) malloc(sizeof(size_t));
	*command_size = DEFAULT_COMMAND_SIZE;
	char* lineptr = (char*)malloc(DEFAULT_COMMAND_SIZE * sizeof(char));
	char* delimiter = " ";
	char* username = (char*) malloc(sizeof(char) * DEFAULT_NAME_SIZE);
	char* machinename = (char*) malloc(sizeof(char) * DEFAULT_NAME_SIZE); 
	char** arguments = (char**) malloc(DEFAULT_ARGUMENT_COUNT * DEFAULT_ARGUMENT_SIZE * sizeof(char*));
	char* shell_name = "LeSh";
	char* shell_version = "0.1";
	username = getlogin();
	gethostname(machinename, 20);
	fprintf(stdout, "Welcome to \"%s-v%s\"\n", shell_name, shell_version);
	do{
		arg_counter = 0;
		fprintf(stdout, "%s$%s ", username, machinename);
		retval = getline(&lineptr, command_size, stdin);
		lineptr[retval-1] = '\0';		
		int flag_redirect = 0;
		for(arguments[arg_counter] = strtok(lineptr, delimiter) ; arguments[arg_counter] != NULL ; arg_counter++, arguments[arg_counter] = strtok(NULL, delimiter)){
			//fprintf(stderr, "%s\n", arguments[arg_counter]);
			if (strcmp(arguments[arg_counter], ">") == 0){
				flag_redirect = 1;
			}
			if(arg_counter >= DEFAULT_ARGUMENT_COUNT - 1)
				arguments = realloc(arguments, (++arg_counter) * DEFAULT_ARGUMENT_SIZE * sizeof(char *));
		}
		pid_t pid = fork();
		if (pid == 0) { 
			if ((strcmp(arguments[0], "echo") == 0) && (flag_redirect == 1)) {
				fprintf(stderr, "%c\n", arguments[1][1]);
				fprintf(stderr, "%d and %d\n", strlen(arguments[0]), strlen(arguments[1]));
				char* new_arg[1];
				new_arg[0] = (char*) malloc((strlen(arguments[0]) + 1) * sizeof(char));
				new_arg[1] = (char*) malloc((strlen(arguments[1]) + 1) * sizeof(char));
				strcpy(new_arg[0], arguments[0]);
				strcpy(new_arg[1], arguments[1]);
				int fd = open(arguments[3], O_CREAT | O_RDWR, S_IRWXU);
				dup2(fd, 1);
				dup2(fd, 2);
				close(fd);
				execvp(arguments[0], new_arg);
				fprintf(stderr, "No such program");
			} else {
				execvp(arguments[0], arguments);
				if (strcmp(arguments[0], "exit") != 0)
					fprintf(stderr, "No such program!\n");
			}
		}
		else {
			int status;
			wait(&status);
		}
	}while(strcmp(lineptr, "exit") != 0);	
	return 0;
}
