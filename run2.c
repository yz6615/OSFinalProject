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

int file_read(int blockSize, char *fileName) {
	int blockCount = 0;
	int size = blockSize/4;
	unsigned int buf[size];
	unsigned int xor = 0;
	double start, curr;
	int fd = open(fileName, O_RDONLY); //open the image file
	if(fd == -1) {
		printf("File error: %s -- could not be opened\n", fileName);
		return 0;
	}
	else {
		int r;
		
		start = now();
		curr = now();
		while ((curr - start < 10) && (r=pread(fd, buf, blockSize,0)) > 0) {
			//printf("r:%d\n", r);
			xor ^= xorbuf(buf, size);
			blockCount++;
			//continue;
			curr = now();
			
		}
	}
	printf("time: %f\n", curr - start);
	//printf("xor: %08x\n", xor);
	close(fd);
	
	return blockCount;
}

int main(int argc, char *argv[]) {
    char * fileName; //file name
    int blockSize; //block size
    int blockCount; // block count

    if (argc != 3) {
        printf("Invalid inputs");
		return 0;
    }

    fileName = argv[1];
	blockSize = atoi(argv[2]);

	blockCount = file_read(blockSize, fileName);
	printf("%d\n", blockCount);

	return 0;
}