/***********************************************************
* Name: Apurva Gandhi
* Instructor: Professor Walsh
* Date: February 18, 2022
* Course: CSCI346
* Assignment: Project1
* Description: The launcher.c program works as a laucnher for cointoss
* program. It creates processes of cointoss using fork() and execlp()
************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int ac, char **av) 
{
    setbuf(stdout, NULL);

    char *ta = av[1];
    char *tb = av[2];
    char *tc = av[3];
    char *td = av[4];
    if(ta == NULL || tb == NULL || tc == NULL || td == NULL || av[5] == NULL)
    {
        printf("Something went wrong while reading command line arguments. Run it like: 5 6 7 8 10");
        return 1;
    }
    //atof does not detect error
    float te = atof(av[5]);
    if(te < 0)
    {
        printf("The last parameter should be greater than or equal to 0!\n");
        return 1;
    }
    printf("I am Process Eve. I am the parent. My Id is %i \n", getpid());
    
    //Process E creating new process -- process A
    int childIdA = fork();
    if(childIdA == 0) // Child A will execute the code in this conditional
    {
        //process A creating new child process -- Process C
        int childIdC = fork();
        if(childIdC == 0) // Child C will execute the code in this conditional
        {
            if(execlp("./cointoss", "./cointoss", "Cal", tc, NULL) == -1)
            {
                printf("Sorry, there was a problem while running execlp command for process %i\n", getpid());
            } 
        }
        else
        {
            //Process A creating new child process -- Process D
            int childIdD = fork();  
            if(childIdD == 0)// Child D will execute the code in this conditional
            {
                if(execlp("./cointoss", "./cointoss", "Dot", td, NULL) == -1)
                {  
                    printf("Sorry, there was a problem while running execlp command for process %i\n", getpid());
                } 
            }
            else
            {
                //process A calling execlp 
                if(execlp("./cointoss", "./cointoss", "Ada", ta, NULL) == -1)
                {
                    printf("Sorry, there was a problem while running execlp command for process Ada %i\n", getpid());
                    printf("printf failed! %i\n", errno);
                }
            }
        }
    }
    //Process E creating new process - Process B
    else 
    {
        int childIdB = fork();
        if(childIdB == 0) // Child B will execute the code in this conditional
        {
            if(execlp("./cointoss", "./cointoss", "Bev", tb, NULL) == -1)
            {
                printf("Sorry, there was a problem while running execlp command for process %i\n", getpid());
            }          
        }
        else
        {
            printf("Eve (Process %i, parent %i ) will sleep for %f seconds\n", getpid(), getppid(), te);
            if(usleep(te*1000000) != 0)
            {            
                printf("Sorry, something went wrong while while executing sleep in process %i.\n", getpid());
            }
            printf("Eve (Process %i, parent %i) will now call wait(). \n", getpid(), getppid());
            int childStatus;
            int childId_done = wait(&childStatus);
            
            //TODO: ADD statuses for process C and Process D
            if(childId_done >= 0)
            {
                if(WIFEXITED(childStatus))
                {
                    printf("Eve (Process %i, parent %i) got pid %i and status %i from wait.\n", getpid(), getppid(), childId_done, (WEXITSTATUS(childStatus)));
                }
                else if(WIFSIGNALED(childStatus))
                {
                    printf("Eve's (Process %i, parent %i) child process was terminated by a signal number %i\n", getpid(), getppid(), WTERMSIG(childStatus));
                }
                else if(WCOREDUMP(childStatus))
                {
                    printf("Eve's (Process %i, parent %i) child process produced a core dumpi\n", getpid(), getppid());
                }
            }
            else
            {
                printf("Error encountered while running wait() in process %i\n", getpid());
            }      
        }
        return 0;
    }
}    