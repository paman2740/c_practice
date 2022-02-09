#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>

#define TOK_DELIM " \t\r\n"//these are the delimeters for splitting the line.
#define RED "\033[0;31m"
#define RESET "\e[0m"
#define TK_BUFF_SIZE 64

char *read_line();
char **split_line(char *);
int dash_exit(char **);
int dash_execute(char **);

int dash_execute(char **args) {
  pid_t cpid;
  int status;

  if (strcmp(args[0], "exit") == 0) {
    return dash_exit(args);
  }
// creating a child process 
  cpid = fork();

  if (cpid == 0) // meaning process was created and execvp is called. 
    {
    if (execvp(args[0], args) < 0)
      printf("dash: command not found: %s\n", args[0]);
    exit(EXIT_FAILURE);
   }
   else if (cpid < 0)// meaing child process creation was failed 
    printf(RED "Error forking"
      RESET "\n");
  else {
    waitpid(cpid, & status, WUNTRACED);// waiting for the process to complete 
    //I tried to implement & operator but it did'nt worked. 
  }
  return 1;
}

int dash_exit(char **args) {
  return 0;
}
//this method  splits line so we can parse it eaisly. 
char **split_line(char * line) {
  int buffsize = TK_BUFF_SIZE, position = 0;
  char **tokens = malloc(buffsize * sizeof(char *));
  char *token;

  if (!tokens) {
    fprintf(stderr, "%sdash: Allocation error%s\n", RED, RESET);
    exit(EXIT_FAILURE);
  }
  token = strtok(line, TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= buffsize) {
      buffsize += TK_BUFF_SIZE;
      tokens = realloc(tokens, buffsize * sizeof(char * ));

      if (!tokens) {
        fprintf(stderr, "%sdash: Allocation error%s\n", RED, RESET);
        exit(EXIT_FAILURE);
      }
    }
    token = strtok(NULL, TOK_DELIM);
  }
  tokens[position] = NULL;

  return tokens;
}
// this is to read line.
char *read_line() {
  int buffsize = 1024;
  int position = 0;
  char *buffer = malloc(sizeof(char) * buffsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "%sdash: Allocation error%s\n", RED, RESET);
    exit(EXIT_FAILURE);
  }

  while (1) {
    c = getchar();
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    if (position >= buffsize) {
      buffsize += 1024;
      buffer = realloc(buffer, buffsize);

      if (!buffer) {
        fprintf(stderr, "dash: Allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}//end read method 


//this is the loop that helps going in our shell.
void loop() {
  char *line;//for reading the line 
  char **args;//for seperating the words. or parsing
  int status = 1;

  do {
    printf("Custom ShellCS4250$--> ");
    line = read_line();//reading the line using the read line method
    args = split_line(line);//spliting the line using the splimethod
    status = dash_execute(args);
    free(line);
    free(args);
  } while (status);
}
//and this is the main method. 
int main() {
  printf("************************************\n");
  printf("*******Aman Patel's SHELL***********\n");
  printf("*******Thankyou for using***********\n");
  printf("************************************\n");

  loop();
  return 0;
}





