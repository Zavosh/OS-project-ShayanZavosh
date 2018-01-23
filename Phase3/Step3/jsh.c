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
	int arg_counter;//argument counter
	size_t* command_size = (size_t*) malloc(sizeof(size_t));
	*command_size = DEFAULT_COMMAND_SIZE;
	char* lineptr = (char*)malloc(DEFAULT_COMMAND_SIZE * sizeof(char)); //command
	char* delimiter = " "; //delimiter in commands
	char* username = (char*) malloc(sizeof(char) * DEFAULT_NAME_SIZE);//user name string initialization
	char* machinename = (char*) malloc(sizeof(char) * DEFAULT_NAME_SIZE); //machine name string initialization
	char** arguments = (char**) malloc(DEFAULT_ARGUMENT_COUNT * DEFAULT_ARGUMENT_SIZE * sizeof(char*));//arguments intialization
	char* shell_name = "LeSh";//shell name
	char* shell_version = "0.1";//shell version
	username = getlogin();//retrieving username
	gethostname(machinename, 20);//get machine name
	fprintf(stdout, "Welcome to \"%s-v%s\"\n", shell_name, shell_version);//welcome message
	do{
		arg_counter = 0;
		fprintf(stdout, "%s$%s ", username, machinename);//printing username and machinename
		retval = getline(&lineptr, command_size, stdin);//getting one line from command line
		lineptr[retval-1] = '\0';//adding null at the end		
		int flag_redirect = 0;//initializing redirect flag
		//parsing arguments using strtok function
		for(arguments[arg_counter] = strtok(lineptr, delimiter) ; arguments[arg_counter] != NULL ; arg_counter++, arguments[arg_counter] = strtok(NULL, delimiter)){
			//fprintf(stderr, "%s\n", arguments[arg_counter]);
			if (strcmp(arguments[arg_counter], ">") == 0){
				flag_redirect = 1;
			}
			if(arg_counter >= DEFAULT_ARGUMENT_COUNT - 1)
				arguments = realloc(arguments, (++arg_counter) * DEFAULT_ARGUMENT_SIZE * sizeof(char *));
		}
		pid_t pid = fork();//creating a new process
		if (pid == 0) { //in child process
			if ((strcmp(arguments[0], "echo") == 0) && (flag_redirect == 1)) {//if echo and has redirect
				//fprintf(stderr, "%c\n", arguments[1][1]);
				//fprintf(stderr, "%d and %d\n", strlen(arguments[0]), strlen(arguments[1]));
				char** new_arg = (char**) malloc(sizeof(char*) * 2);
				//new_arg[0] = (char*) malloc((strlen(arguments[0]) + 1) * sizeof(char));
				//new_arg[1] = (char*) malloc((strlen(arguments[1]) + 1) * sizeof(char));
				new_arg[0] = arguments[0];
				new_arg[1] = arguments[1];//arguments preceding redirect operators
				//fprintf(stderr, "%s and %s\n", new_arg[0], new_arg[1]);
				int fd = open(arguments[3], O_CREAT | O_RDWR, S_IRWXU);// open a new file for redirecting
				dup2(fd, 1);// redirecting stdout of child to file with file description stored in fd
				//dup2(fd, 2);
				close(fd);
				execvp(new_arg[0], new_arg);// executing command from bin file
				//fprintf(stderr, "%d\n", k);
				fprintf(stderr, "No such program");
			} else {//otherwise
				execvp(arguments[0], arguments);
				if (strcmp(arguments[0], "exit") != 0)
					fprintf(stderr, "No such program!\n");
			}
		}
		else {
			int status;
			wait(&status);//parent should wait for its child to finish
		}
	}while(strcmp(lineptr, "exit") != 0);	//condition of exiting from shell
	return 0;
}
