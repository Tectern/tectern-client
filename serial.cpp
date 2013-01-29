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
//	CURL *curl;
//	CURLcode res;

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
//	curl_global_init(CURL_GLOBAL_ALL);
	fcntl(fd, F_SETFL, 0);
//	char* cuid = (char*)malloc(sizeof(char)*9);
//	while(true)
//	{
		/*
		scanf("%s",cuid);
		if(cuid[0]=='C')
		{
			cuid+=1;
		}
		fprintf(stdout,"%s",cuid);
		curl = curl_easy_init();
		if(curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, "http://130.127.49.216/tectern-server/authenticate.json");
			char* post_data = malloc(sizeof(char)*13);
			post_data = strcat(post_data,"id=");
			post_data = strcat(post_data,cuid);
			curl_easy_setopt(curl,CURLOPT_POSTFIELDS, post_data);
			char* response;
			curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, write_to_string);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);	
		}
		*/
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
//	}

	close(fd);
}
