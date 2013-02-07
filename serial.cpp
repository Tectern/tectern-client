#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
// #include <curl/curl.h>

int fd, cfd;

void sigIntHandler(int sig)
{
    fprintf(stdout, "CNT-C recieved.\n");
    close(fd);
    exit(0);
}

ssize_t write_to_string(void* ptr, size_t size, size_t count, void* stream)
{
    printf("%s",((char*)ptr));
    return size*count;
}

int main(int argc, char** argv)
{
    if(argc!=2)
    {
        fprintf(stdout,"USAGE: %s [command string]\n", argv[0]);
    }
    signal(SIGINT, sigIntHandler);
    fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd==-1)
    {
        fprintf(stderr, "Could not open Arduino serial port.");
        fflush(stderr);
        exit(1);
    }
    fcntl(fd, F_SETFL, 0);
    int writeBytes = write(fd, argv[1], strlen(argv[1]));
    fprintf(stdout, "Sent %s %d bytes.\n", argv[1], writeBytes);
    fprintf(stdout, "===================\n");
    char b[1];
    int i=0;
    do {
        int n = read(cfd, b, 1);  // read a char at a time
        if( n==-1) printf("Error reading response from card reader.\n");    // couldn't read
        if( n==0 ) {
            usleep( 10 * 1000 ); // wait 10 msec try again
            continue;
        }
        fputc(b[0],stdout);
        fflush(stdout);
        i++;
    } while( b[0] != '^' );
    close(fd);
}
