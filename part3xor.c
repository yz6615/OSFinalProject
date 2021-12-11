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
int compareArrays(char *arr1, char *arr2) {
	int s1 = strlen(arr1);
    int s2 = strlen(arr2);
    if (s1 != s2) return 0;
	int i;
	for (i = 0; i < s1; i++) {
		if (arr1[i] != arr2[i]) {
			return 0;
		}
	}
	return 1;
}
unsigned int xorbuf(unsigned int *buffer, int size) {
    unsigned int result = 0;
    for (int i = 0; i < size; i++) {
        result ^= buffer[i];
    }
    return result;
}

float performance(int blockSize, int blockCount, float duration) {
    int file = blockSize * blockCount / 1000000;
    printf("Performance: %f MiB/s\n", file/duration);
    return file/duration;
}

float file_read(int blockSize, int blockCount, char *fileName) {
	int size = blockSize/4;
	double start, end;
	unsigned int buf[size];
	unsigned int xor = 0;
    float perform;
	int fd = open(fileName, O_RDONLY); //open the image file
	if(fd == -1) {
		printf("File error: %s -- could not be opened\n", fileName);
		return 0;
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
        perform = performance(blockSize, blockCount, end - start);
		printf("Read Time: %f seconds\n", end - start);
        return perform;
	}
	printf("xor: %08x\n", xor);
	close(fd);
}

int main(int argc, char *argv[]) {
    char * fileName; //file name
    int blockSize = 4;
    int blockCount = 9999999;
    float performance[5];
    int blockSizes[5];
    int count = 0;
    if (argc != 3) {
        printf("Invalid inputs");
		return 0;
    }

    fileName = argv[1];
    
    if (compareArrays(argv[2],"-r")) {  
        while(blockSize <= 4096) {
            performance[count] = file_read(blockSize, blockCount, fileName);
            blockSizes[count] = blockSize;
            blockSize *= 4;
            count++;
        }
    }
    for (int i = 0; i < 5; i ++) {
        printf("blocksize: %d\n", blockSizes[i]);
        printf("performance: %f\n", performance[i]);
    }
	return 0;
}
