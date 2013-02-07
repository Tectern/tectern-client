#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <linux/input.h>

int main(int argc, char** argv) {
    int fd, byteCount, readLength, i;
    struct input_event cardBuffer[64];
	fd = open("/dev/input/by-id/usb-IDTECH_IDTECH_MiniMag_II_USB-HID_Keyboard_Reader-event-kbd", O_RDONLY);
	if(fd==-1)
	{
		fprintf(stderr, "Could not open USB card reader.");
		fflush(stderr);
		exit(1);
	}
    byteCount = 0;
    while(byteCount == 0) {
        byteCount = read(fd, cardBuffer, sizeof(cardBuffer));
    }
    printf("%s\n",strerror(errno));
    printf("Read %d bytes\n", byteCount);
    readLength = byteCount/sizeof(struct input_event);
    for(i = 0; i<readLength; i++) {
        printf("Event: %u %hd %hd %d\n", cardBuffer[i].time, cardBuffer[i].type, cardBuffer[i].code, cardBuffer[i].value);
    }
    close(fd);
}
