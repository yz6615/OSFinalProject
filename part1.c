#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>

void file_read(int blockSize, char *fileName) {
	char buf[blockSize];
	int fd = open(fileName, O_RDONLY); //open the image file
	if(fd == -1) {
		printf("File error: %s -- could not be opened\n", fileName);
		return;
	}
	else {
		int r;
		while ((r=read(fd, buf, blockSize)) > 0) {
			printf("%s\n", buf);
			//continue;
		}
	}
	close(fd);
}

void file_write(int blockSize, int blockCount, char *fileName) {
	char buf[blockSize];
	for (int i=0; i < blockSize; i++) {
		buf[i] = 'x';
	}
	int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	for (int i=0; i < blockCount; i++) {
		write(fd, buf, blockSize);
	}
	close(fd);
}

int main(int argc, char *argv[]) {
    char * fileName; //file name
    int blockSize; //block size
    int blockCount; // block count

    if (argc != 5) {
        printf("Invalid inputs");
		return 0;
    }

    fileName = argv[1];
	blockSize = atoi(argv[3]);
    blockCount = atoi(argv[4]);
	
	switch(argv[2][1]) {// read or write
		case 'r':
			file_read(blockSize, fileName);
		break;
		
		case 'w':
			file_write(blockSize, blockCount, fileName);
		break;
	}

	return 0;
}