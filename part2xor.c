#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>
#include <math.h>
#include <sys/time.h>

double now() {
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

unsigned int xorbuf(unsigned int *buffer, int size) {
    unsigned int result = 0;
    for (int i = 0; i < size; i++) {
        result ^= buffer[i];
    }
    return result;
}

void file_read(int blockSize, int blockCount, char *fileName) {
	int size = blockSize/4;
	double start, end;
	unsigned int buf[size];
	unsigned int xor = 0;
	int fd = open(fileName, O_RDONLY); //open the image file
	if(fd == -1) {
		printf("File error: %s -- could not be opened\n", fileName);
		return;
	}
	else {
		int r;
		int count = 0;
		start = now();
		while ((count < blockCount) && (r=read(fd, buf, blockSize)) > 0) {
			//printf("r:%d\n", r);
			xor ^= xorbuf(buf, size);
			count++;
			//continue;
		}
		end = now();
		printf("Read Time: %f seconds\n", end - start);
	}
	printf("xor: %08x\n", xor);
	close(fd);
}

void file_write(int blockSize, int blockCount, char *fileName) {
	char buf[blockSize];
	for (int i=0; i < blockSize; i++) {
		buf[i] = '2';
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
			file_read(blockSize, blockCount, fileName);
		break;
		
		case 'w':
			file_write(blockSize, blockCount, fileName);
		break;
	}

	return 0;
}