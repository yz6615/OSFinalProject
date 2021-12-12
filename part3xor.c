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

void performance(int blockSize, int blockCount, double duration, FILE* fWrite) {
    unsigned int file = (blockCount / 1000000) * blockSize;
	printf("File size: %d MB\n", file);
    printf("Performance: %f MiB/s ", file/duration);
    fprintf(fWrite, "%d %f\n",blockSize, file/duration);
    
}

void file_read(int blockSize, int blockCount, char *fileName, FILE* fWrite) {
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
		printf("blockSize: %d\n", blockSize);
		start = now();
		while ((count < blockCount) && (r=read(fd, buf, blockSize)) > 0) {
			//printf("r:%d\n", r);
			xor ^= xorbuf(buf, size);
			count++;
			//continue;
		}
		end = now();
        performance(blockSize, blockCount, end - start, fWrite);
		printf("Read Time: %f seconds\n", end - start);
	}
	printf("xor: %08x\n", xor);
	close(fd);
}

int main(int argc, char *argv[]) {
    FILE *fWrite = fopen("data.txt","w");
    char * fileName; //file name
    int blockSize = 4;
    int blockCount = 5000000;
    if (argc != 3) {
        printf("Invalid inputs");
		return 0;
    }

    fileName = argv[1];
    
    if (compareArrays(argv[2],"-r")) {
        
        while(blockSize <= 4096) {
            file_read(blockSize, blockCount, fileName, fWrite);
            blockSize *= 2;
        }
    }
    fclose(fWrite);
	return 0;
}
