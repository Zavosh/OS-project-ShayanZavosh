#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <getopt.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>


//Macros
#define DEFAULT_COMMAND_SIZE (size_t) 20
#define DEFAULT_ARGUMENT_COUNT 10

//Global Variables
char *shell_name;
char *shell_version;
char **log_buffer;

static int mem_cleanup(char *shell_name, char *shell_version, size_t *command_size, char **lineptr, char **argv){
	
	free((void *) shell_name);
	free((void *) shell_version);

	free((void *) *lineptr);
	free((void *) command_size);
	
	free((void *) argv);

	free((void *) log_buffer);
}

static int shell_exit(void){	
	fprintf(stdout, "Good Luck!\n");
	return 0;
}

static int init_shell(char name[], char version[], char **command_line, size_t **command_size, char ***lineptr, char ***argv, int argument_count_coeff){
	shell_name    = (char *) malloc(strlen(name) * sizeof(char));
	shell_version = (char *) malloc(strlen(version) * sizeof(char));

	strcpy(shell_name, name);
	strcpy(shell_version, version);
	
	(*command_line) = (char *) malloc(DEFAULT_COMMAND_SIZE);
	(*lineptr) = command_line;
	
	(*command_size) = (size_t *) malloc(sizeof(int));
	(**command_size) = DEFAULT_COMMAND_SIZE;

	(*argv) = (char **) malloc((argument_count_coeff) * DEFAULT_ARGUMENT_COUNT * sizeof(char *));

	return 0;
}

void commandline_log(char* str){
}

int main(int argc, char* argv[]){ 
	int retval;
	int arg_counter;
	int argument_count_coeff = 1;

	size_t *command_size;

	char *delimiter = " ";
	char *command_line;
	char **lineptr;
	const char* const short_options = "l:uht:b:";
	const struct option long_options[] = {
		{"logput", 1, NULL, 'l'},
		{"user", 0, NULL, 'u'},
		{"binpath", 1, NULL, 'b'},
		{"hostname", 0, NULL, 'h'},
		{"time", 1, NULL, 't'},
		{NULL, 0, NULL, 0}
	};
	char* log_filename;
	char* username = "null";
	char* binpath;
	char* machinename = (char*) malloc(sizeof(char) * 20);
	int machine_flag = 0;
	int path_flag = 0;
	int time;
	int next_option;
	do{
		next_option = getopt_long(argc, argv, short_options, long_options, NULL);
		switch(next_option){
			case 'l':
				log_filename = optarg;
				break;
			case 'u':
				username = getlogin();
				break;
			case 'h':
				machine_flag = 1;
				gethostname(machinename, 20);
				break;
			case 'b':
				binpath = optarg;
				path_flag = 1;
				break;
			case 't':
				time = atoi(optarg);
				break;
			case '?':
				fprintf(stderr, "No such option\n");
				break;
			case -1:
				break;
			defualt:
				abort();
		}
	}while(next_option != -1);
	username = getlogin();
	gethostname(machinename, 20);
	char* paths[100];
	int cnt = 0;
	if (path_flag) {
		char* tok = strtok(binpath, ":");
		while (tok != NULL){
			paths[cnt] = tok;
			cnt++;
			fprintf(stderr, "%s\n", tok);
			tok = strtok(NULL, ":");
		}
	}
	init_shell("LeSh", "0.1", &command_line, &command_size , &lineptr, &argv, argument_count_coeff);
	fprintf(stdout, "Welcome to \"%s-v%s\"\n", shell_name, shell_version);
	do{	
		arg_counter = 0;
		fprintf(stdout, "%s$%s ", username, machinename);
		retval = getline(lineptr, command_size, stdin);
		(*lineptr)[retval-1] = '\0';
		commandline_log(*lineptr);
		
		for(argv[arg_counter] = strtok(*lineptr, delimiter) ; argv[arg_counter] != NULL ; arg_counter++, argv[arg_counter] = strtok(NULL, delimiter)){
			if(arg_counter == DEFAULT_ARGUMENT_COUNT - 1)
				argv = realloc(argv, (++argument_count_coeff) * DEFAULT_ARGUMENT_COUNT * sizeof(char *));
		}
		pid_t pid = fork();
		if (pid == 0) {
			if (path_flag == 0) {
				//fprintf(stderr, "%ssalam\n", argv[0]); 
				if (strcmp(argv[0], "echo") == 0) {
					//fprintf(stderr, "YES");
					int fd = open(argv[3], O_CREAT | O_RDWR, S_IRWXU);
					int len = strlen(argv[1]);
				//	fprintf(stderr, "len is :%d\n", len);
				//	fprintf(stderr, "string is: %s\n", argv[1]);
					int k = (int) write(fd, argv[1], len);
					fprintf(stderr, "%d byte written\n", k);
					close(fd);
				}
			   	else {
					execvp(argv[0], argv);
					fprintf(stderr, "No such program!\n");
				}
			} else {
				int cnt2 = 0, cnt3 = 0;
				for (cnt2 = 0; cnt2 < cnt; cnt2++) {
					char* final_name = (char*)malloc(sizeof(char)*(strlen(argv[0]) + 2 + strlen(paths[cnt2])));
					final_name[0] = '/';
					for (cnt3 = 1; cnt3 <= strlen(argv[0]) + 1; cnt3++){
						final_name[cnt3] = argv[0][cnt3 - 1];
					}
				//char* final_name = (char*)malloc(sizeof(char)*(strlen(argv[0]) + 1 + strlen(paths[cnt2])));
			  		final_name = strcat(paths[cnt2], final_name);
					fprintf(stderr, "%s\n", final_name);	
					execv(final_name, argv);
				}
				fprintf(stderr, "No such program!\n");
			}
		} else {
			int status;
			wait(&status);
		}
		/*cmd_lookup(argv[0], argv);*/

	}while(strcmp(*lineptr, "exit") != 0);		
	mem_cleanup(shell_name, shell_version, command_size, lineptr, argv);

	return 0;
}
