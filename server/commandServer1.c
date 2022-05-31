/*
  johntech codes

*/



#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h> 
#include <readline/readline.h>
#include <readline/history.h> 
#include <stdbool.h> 
#include <signal.h> 

void ctrlCHandler(int signum)
{
  fprintf(stderr,"Command server terminated using <ctrl>C\n");  
  exit(1); 
  } 


//Examples of stub routines - the purpose here is to test commmand and control
char * f1()
{
  fprintf(stdout,"You selected feature1\n"); 
  return "Command 'one' was received"; 
  
  } 


char *  f2(char *cmd)
{
  fprintf(stdout,"You selected feature2\n"); 
  return "Command 'two' was received"; 
  
  } 

char * f3()
{
  fprintf(stdout,"You selected feature3\n"); 
  return "Command 'three' was received"; 
  
  } 

char * f4(char **remainingTokens)
{
  fprintf(stdout,"You selected feature4\n"); 
  for(int i=0;remainingTokens[i]; i++)
     fprintf(stdout,"%d. %s\n",i,remainingTokens[i]);
  
  } 



char *interpret(char *cmdline)
{
  char **tokens;
  char *cmd;

  tokens=history_tokenize(cmdline);   //Split cmdline into individual words.  
  if(!tokens) return "no response needed"; 
  cmd=tokens[0]; 
  if(strcasecmp(cmd,"one")==0) return f1(); 
  if(strcasecmp(cmd,"two")==0) return f2(cmd); 
  if(strcasecmp(cmd,"three")==0) return f3(cmd,tokens); 
  if(strcasecmp(cmd,"four")==0) return f4(&tokens[1]); 
  else return "command not found"; 

 } 

int main(int argc, char * argv[],char * envp[])
{
  char cmd[100]; 
  char *cmdLine; 
  int  nBytes; //size of msg rec'd

  signal(SIGINT,ctrlCHandler); 
  rl_completer_quote_characters="'\"";
  while(true) {

          cmdLine=readline("Enter a command: ");   
          if(!cmdLine) break; 
          char *response=interpret(cmdLine); 
          fprintf(stdout,"%s\n",response);    

	  //Special case to exit the loop, or use <ctrl>c 
          if(strncasecmp("cmdLine","bye",3)==0) break ;         
        }  

  fprintf(stdout,"Server is now terminated \n"); 
  return 0;
  }
