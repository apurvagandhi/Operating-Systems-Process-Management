int main(int n, char **name) {
    char *name1 = name[1]; 
    char *rounds = name[2];
    int count = atoi(rounds);
    char random [1];
    struct timespec start;
    clock_gettime(CLOCK_REALTIME, &start); // start of program to track time
    int ssecond = start.tv_sec;
    int snsecond = start.tv_nsec;
    int points = 3;
    int x;
    int fd = open("/dev/urandom", O_RDONLY); // opening file to get random number 
        if (fd == -1) { // the open function returns -1 when errors occur
            printf("Oops, we ran into a problem opening the file"); // do not use fd, because it is not a valid result
        }
    while(count > 0) {
        int fd1 = read(fd, random, 1); // reading one byte from the random file
            if (fd1 == -1) { // error checking
                printf("Oops, looks like the read function retuned an error");
            }
        x = atoi(random); // converting the char to int
        x = (x % 20) + 1; // ensuring the dice roll is between 1 and 20
        printf("%d", x); 
        points = points + x; // adding the points in order to get total 
        if (points > 50) { //checking if total point ever goes above 50
            points = points / 10; // divide it by 10
        } 
        printf("%s (process %d, parent %d) with %d rolls left, rolls %d and now has %d points\n", name1, getpid(), getppid(), n, x, points);

        sleep(1); // sleep for one second for each loop
        count = count - 1; // update n
    }
    int fd2 = close(fd);
        if (fd2 == -1) { // error checking
            printf("Oops looks like we had trouble closing the file");
        }
    struct timespec minus;
    clock_gettime(CLOCK_REALTIME, &minus); // getting the time that have passed 
    int msecond = minus.tv_sec;
    int mnsecond = minus.tv_nsec;
    int newsecond = msecond - ssecond;
    int newmsecond = mnsecond - snsecond;
    printf("%s (process %d, parent %d) finished in %d.%09d seconds, quit with exit code %i\n", name1, getpid(), getppid(), newsecond, newmsecond, points);
    return points; 
}