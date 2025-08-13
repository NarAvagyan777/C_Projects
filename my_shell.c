#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#define MAX_COMMAND_LINE 1024
int fd;
char buf[1000] = {0};
char prompt[MAX_COMMAND_LINE] = {"My_shell >>"};

void help_command() {
	printf("Mini Shell - Built-in Commands:\n");
    printf("  cd <dir>       Change directory to <dir>\n");
    printf("  exit           Exit the shell\n");
    printf("  help           Show this help message\n");
	printf("  clear          Clear the terminal screan\n");
	printf("  echo <text>    Print <text> to the terminale\n");
	printf("  pwd            Print the current working directory\n");
	printf("  chprompt <new_prompt> Change the shell prompt to <new_prompt>\n");
    printf("Redirection:\n");
    printf("  < file         Redirect input from <file>\n");
    printf("  > file         Redirect output to <file>\n");
	printf("  unsetenv <var> Remove environment variable <var>\n");
}

void exit_command() {
	printf("Exiting shell ...\n");
	close(fd);
	exit(0);
}

void log_command(const char *input) {
    snprintf(buf, sizeof(buf), "%s\n", input);

    write(fd, buf, strlen(buf));
}

void clear_command() {
	printf("\033[H\033[J");
}

void pwd_command() {
	char cwd[MAX_COMMAND_LINE];
	if (getcwd(cwd,sizeof(cwd)) != NULL) {
		printf("%s\n",cwd);
	} else {
		perror("getcwd failed\n");
	}
}

void cd_command(const char* dir) {
	if (chdir(dir) != 0) {
        perror("cd failed");
    }
}

void echo_command(const char* text) {
	if (text == NULL || strlen(text) == 0) {
        printf("\n");
        return;
    }

    if (text[0] == '$') {
        char* var_name = (char*) malloc(strlen(text));
        if (!var_name) {
            perror("malloc failed");
            return;
        }

        strcpy(var_name, text + 1); 
        
        char* value = getenv(var_name);
        if (value != NULL) {
            printf("%s\n", value);
        } else {
            printf("Environment variable '%s' not found.\n", var_name);
        }

        free(var_name);
    } else {
        printf("%s\n", text);
    }
}

void chprompt_command(const char* new_prompt) {
	if (new_prompt != NULL && strlen(new_prompt) > 0) {
		 snprintf(prompt, sizeof(prompt), "%s", new_prompt);
		 printf("Prompt changed to: %s\n", prompt);	
	}else {
		printf("prompt failed\n");
	}
}

void setenv_command(const char* var_name,const char* value) {

	if (var_name == NULL || value == NULL || strlen(var_name) == 0 || strlen(value) == 0) {
        printf("Usage: setenv <variable_name> <value>\n");
        return;
    }

	if (setenv(var_name, value, 1) == 0) {
        printf("Environment variable %s set to %s\n", var_name, value);
    } else {
        perror("setenv failed");
    }
}

void unsetevn_command(const char* var_name) {
		if (var_name == NULL && strlen(var_name) == 0) {
			printf("Usage: unsetenv <variable_name>\n");
			return;
		}
	if (unsetenv(var_name) == 0) {
        printf("Environment variable %s unset successfully\n", var_name);
    } else {
        perror("unsetenv failed");
    }
}

void execute_external_command(char* input) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {
        char* args[MAX_COMMAND_LINE];
        char* token = strtok(input, " ");
        int i = 0;

        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(1);
        }
    } else {
        wait(NULL);  
    }
}


void parse_and_execute(char* input) {

	log_command(input);

	if (strcmp(input, "help") == 0) {
		help_command();
	}
	else if (strcmp(input, "exit") == 0) {
		exit_command();
	}
	else if (strcmp(input, "clear") == 0) {
		clear_command();
	}
	else if (strcmp(input, "pwd") == 0) {
		pwd_command();
	}
	else if (strncmp(input,"cd",2) == 0) {
		
		char* dir = input + 3;

		if (strlen(dir) == 0) {
			dir = getenv("HOME");
		}

		cd_command(dir);
	}
	else if (strncmp(input,"echo",4) == 0) {
		char* text = input + 5;

		if (strlen(text) > 0) {
			echo_command(text);
		} else {
			printf("\n");
		}
	}
	else if (strncmp(input,"chprompt",8) == 0) {
		char* new_prompt = input + 9;

		while (*new_prompt == ' ') new_prompt++; 
        if (strlen(new_prompt) > 0) {
            chprompt_command(new_prompt);
		}
	}
	else if (strncmp(input,"setenv",6) == 0) {
		char* var_name = input + 7;
		char* value = strchr(var_name,' ');

		if (value != NULL) {
			*value = '\0';
			value++;
		}
		while (*value == ' '){
			value++;
		}

		 if (strlen(value) > 0 && strlen(var_name) > 0) {
			setenv_command(var_name,value);
		} else {
			printf("Usage: setenv <variable_name> <value>\n");
        }
		return;
	}else if (strncmp(input,"unsetevn",8) == 0) {
		char* var_name = input + 9;
		while (*var_name == ' ') {
			var_name++;
		}
		if (strlen(var_name) > 0) {
			unsetevn_command(var_name);
		} else {
			printf("Usage: unsetenv <variable_name>\n");
		}
	}
	else {
		execute_external_command(input);
	}
}


int main() {
	char input[MAX_COMMAND_LINE];
	fd = open("history.txt",O_CREAT | O_RDWR | O_APPEND,0777);
	while (1) {
		printf("%s ",prompt);

		if (fgets(input,sizeof(input),stdin) == NULL) {
			printf("\n");
			break;
		}
		input[strlen(input) - 1]  = '\0';
		parse_and_execute(input);
	}
	return 0;
}