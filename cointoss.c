/***********************************************************
* Name: Apurva Gandhi
* Instructor: Professor Walsh
* Date: February 18, 2022
* Course: CSCI346
* Assignment: Project1
* Description: The cointoss.c program rolls a virtual 20-sided dice n times (once per second, printing a message each time) 
* to earn points, then quits with a final goodbye message 
************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> //used to get the time
#include <sys/types.h> //open
#include <sys/stat.h> //open
#include <fcntl.h> //open

/*  returns the current time in an integer array
    it will return -1 if there will be an error
*/
int* getCurrentTime()
{
    struct timespec now;
    int *time = malloc(2 *sizeof (int));
    time[0] = -1;
    time[1] = -1;
    if(clock_gettime(CLOCK_REALTIME, &now) == 0)
    {
        time[0] = now.tv_sec;
        time[1] = now.tv_nsec; 
    }
    else
    {
        printf("Error encountered while getting current time.\n");
    }
    return time;
}

int main(int ac, char **av) 
{
    setbuf(stdout, NULL);

    int *startTime = getCurrentTime(); 
    if(startTime[0] == -1 || startTime[1] == -1)
    {
        printf("Something went wrong while running getCurrentTime() function.\n");
    }
    int *endTime;
    char *name = av[1];
    int n = atoi(av[2]);
    //Error checking for command line arguments; this also includes error checking for atoi
    if(name == NULL || n <= 0)
    {
        printf("Something went wrong while reading command line arguments. Run it like: cointoss alice 5");
        return 1;
    }
    //checks for user inputted error as well as an error for atoi as it also returns 0 on error
    int totalPoints = 3;
    //to keep track of 20 random numbers
    unsigned char randomData[20];
    //opens the file with random numbers
    int descriptor = open("/dev/urandom", O_RDONLY);
    if(descriptor == -1)
    {
        printf("Something went wrong while opening the file\n");
    }
    else
    {
        //reads 20 bytes of data and adds it to the randomData array from file with mentioned descriptor
        if(read(descriptor, randomData , 20) == -1)
        {
            printf("Something went wrong while reading from the file with descriptor %i.\n", descriptor);
        }
        //closed the file with given descriptor
        if(close(descriptor) == -1)
        {
            printf("Something went  wrong while closing the file with descriptor %i.\n", descriptor);
        }
    }
   
    //loops through n number of time and based on the random number of roll, it calculates the total points
    for(int i = 0; i < n; i++)
    {
        //gets the random number in the range of 1,20 (inclusive)
        int roll = (randomData[i] % 20) + 1;
        //updates the points
        totalPoints += roll;
        //if points goes above 50, it divides it by 10
        if(totalPoints > 50)
        {
            totalPoints = totalPoints / 10;
        }
        printf("%s (process %i, parent %i) with %i rolls left, rolls %i and now has %i points.\n", name, getpid(), getppid(), n-i, roll, totalPoints);
        //sleeps for 1 second
        if(usleep(1000000) == -1)
        {
            printf("Sorry, something went wrong while running usleep()\n");
        }
    }
    //gets the end time and calculates the time elapsed in the program. 
    endTime = getCurrentTime();
    if(endTime[0] == -1 || endTime[1] == -1)
    {
        printf("Something went wrong while running getCurrentTime() function.\n");
    }
    else
    {
        printf("%s (process %i, parent %i finished in %i.%09d seconds, quits with exit code %i) \n", name, getpid(), getppid(), endTime[0] - startTime[0], endTime[1] - startTime[1], totalPoints );
    }
    //free malloc from getCurrentTime function
    free(endTime);
    free(startTime);
    //returning total points as exit code
    return totalPoints;
}//end of main