/*CS 5348 - Operating Systems Project-1: myshell.c */
/*Team Member: Rohith Reddy Krishnareddi Gari(rxk152130), Kartheek()*/
/*Compile Instructions: gcc -o myshell myshell.c*/
/*Run Instructions: ./myshell*/
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>
#define BUFF_SIZE 512 //Maximum length for the commandline entry
#define DELIM " \t\n" //Possible Delimiters in the commandline entry

char *internalcomm[] = {"cd","help","exit"}; //Internal Commands, new commands can be added easily
char *internalcomm1[] = {"Change Directory", "Displays Internal Commands", "Stops the shell"}; //Internal Command Descriptions

/*Code for 'cd' command*/
int cdcomm(char **args)
{
	if(args[1] == NULL)
	{
		printf("Wrong Command, Expected 'cd'\n");	
	}
	else
	{
		if(chdir(args[1]) != 0) //Using chdir() to execute 'cd' command
			perror("Shell"); //Generates error if directory is not found	
	}
	return 1;
}

/*Code for 'help' command*/ 
int helpcomm(char **args)
{
	int i;
	int n = sizeof(internalcomm) / sizeof(char *); //Finding the no. of internal commands
	printf("The internal commands for this shell:\n");
	for(i=0;i<n;i++) //Printing the internal commands and their descriptions
		printf(" %s -- %s\n", internalcomm[i],internalcomm1[i]);
	printf("\nTo know more about other commands, use <commandname> --help or man <commandname>\n");
	return 1;
}

/*Code for 'exit' command*/
int exitcomm(char **args)
{
	return 0; //Returns '0' for status variable in main function, breaks out of the while loop
}
int (*internalfunc[]) (char **) = { &cdcomm,&helpcomm,&exitcomm};//Function to process internal commands, uses array to function pointers

/*Function to create child process and execute command(not internal commands)*/
int createprocess(char **args)
{
	int status;
	pid_t pid; //Declaring a variable 'pid' to store pid of child process. Not mandatory
	
	if((pid=fork())==0)/*Child Process*/
	{
		//printf("Child Process Created, PID = %d,PPID = %d\n",getpid(),getppid());
		if(execvp(args[0],args) == -1) //execvp() execute all the commands other than internal commands
		{
			perror("Shell"); //Gives error for unrecognised commands		
		}	
		else if(pid<0)
		{
			perror("Unable to create a child process"); //Error if forking goes wrong
		}
	}	
	else /*Parent Process*/
	{
		wait(&status); //Wait till child process terminates
		//printf("Parent Process, PID = %d\n",getpid());	
	}
}

/*Main execution which executes all the commands*/
int execcomm(char **args)
{
	int i;
	int n = sizeof(internalcomm) / sizeof(char *); //Total no. of internal commands
	if(args[0] == NULL)
		return 1; //If no command entered, just prompt again

	for(i=0;i<n;i++)
	{
		if(strcmp(args[0], internalcomm[i]) == 0) //Find which internal command
			return (*internalfunc[i])(args); //Execute Internal Command
	}
	
	return createprocess(args); //Execute the command if not internal command
}

/*Splits the command and its arguments*/
char **splitarg(char str[])
{
	int buffsize = BUFF_SIZE;
	int pos=0;
	char **args = malloc(buffsize * sizeof(char*)); //Allocating memory for arguments
	char *arg; //Temporary argument
	
	if(!args)
	{
		printf("Unable to allocate memory\n");
		exit(-1);	
	}
	arg = strtok(str, DELIM); //Uses strtok() and DELIM to split the arguments. space is the the delimiter in our case
	while(arg != NULL)
	{
		args[pos] = arg; //Write to the actual argument variable from the temporary one
		pos++;
		arg = strtok(NULL,DELIM);		
	}
	args[pos] = NULL; //Last argument is NULL
	return args;
}

/*Main Function*/
int main()
{
	char str[BUFF_SIZE]=""; //Stores the input command
	char **args; //Where the arguments are stored after splitting
	int status=1; //Flag variable which changes on exit command
	int n=0; //Variable to count no. of commands entered
	printf("Shell is starting...\n");
	printf("Made by Rohith & Kartheek\n");
	while(status)
	{
		printf("<myshell>$");
		scanf("%[^\n]",str); //Read the command from command line
		args = splitarg(str); //Split the arguments
		status = execcomm(args); //Executes command and returns '0' for exit command
		n++;
		free(args); //Free up memory
		/*Flushing Input Buffer*/
		int c;
        	while((c = getchar()) != '\n' && c != EOF);
	}
	if(strcmp(str,"exit")==0)
		printf("Total number of commands executed = %d\n",n);
return 0;
}


