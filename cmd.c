#include <stdlib.h>
#include <stdio.h>
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

char *read_line(FILE* fp){
	size_t size = 10;
    int ch;
    size_t len = 0;
    char *str;

    str = malloc(sizeof(char) * size);//size is the start size of the string
    if(!str)return str;

    while(EOF != (ch = fgetc(fp)) && ch != '\n'){
        str[len++] = ch;
        if(len == size){
            str = realloc(str, sizeof(char) * (size += 16));
            if(!str)return str;
        }
    }

    str[len++] = '\0';
    return realloc(str, sizeof(char) * len);
}


char **split_line(char *line) // this function took from the internet!!!! that split the line
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}



int exec(char **args)
{
 	pid_t pid;
	pid_t wpid;
	int status;

 	pid = fork();
	if (pid == 0)// Child process
	{
		execvp(args[0], args);
    	exit(EXIT_FAILURE); 
 	}else if(pid > 0)// Parent process
 	{
   		do{
      		wpid = waitpid(pid, &status, WUNTRACED);
    	} while (!WIFEXITED(status));	
    }else
    {
    	perror("error forking");
    } 
	return 1;
}


void loop()
{
  char *line;
  char **args;
  int status;
  while(1)
  {
  	printf(">>");
    line = read_line(stdin);
    args = split_line(line);
    status = exec(args);
    free(line);
    free(args);
  }
    
}


int main(int argc, char **argv)
{
	loop();
 	return 1;
}
