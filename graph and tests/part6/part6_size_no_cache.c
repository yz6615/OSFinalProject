#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>

double now() {
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

typedef struct {
	int fd;
    int blockSize;
	int maxCount;
	unsigned int *buf;
	unsigned int offsetBlock;
    unsigned int xor; //store the xor of each thread and return to the parent function
} threadPara;

unsigned int xorbuf(unsigned int *buffer, int size) {
    unsigned int result = 0;
    for (int i = 0; i < size; i++) {
        result ^= buffer[i];
    }
    return result;
}

void performance(int blockSize, unsigned long blockCount, int threadNum, double duration, FILE* fWrite) {
    unsigned int file = (blockCount / 1000000) * blockSize;
    printf("File size: %d MB\n", file);
    printf("Performance: %f MiB/s ", file/duration);
    fprintf(fWrite, "%d %f %d \n",blockSize, file/duration, threadNum);
    
}

void *child_thread (void *args)
{
	threadPara *temp = (threadPara *)args;
	int r;
	unsigned int xor = 0;
	unsigned int offset = temp->offsetBlock * temp->blockSize; //getting the start offset of the thread
	unsigned int max = (temp->offsetBlock + temp->maxCount) * temp->blockSize; //setting the end offset of the thread
	while ((offset < max) && (r=pread(temp->fd, temp->buf, temp->blockSize, offset)) > 0) { //setting limits and loop
		xor ^= xorbuf(temp->buf, temp->blockSize / 4);
		offset += temp->blockSize; //iter the offset and read from next block
	}
	temp->xor = xor;
    pthread_exit (NULL);
}

void thread_read(int blockSize, int blockCount, char *fileName, int threadNum) {
	int size = blockSize/4;
	unsigned int buf[size];
	unsigned int xor = 0;
	unsigned int offsetBlock = 0;
	unsigned int offset = 0;
	int fd = open(fileName, O_RDONLY); //open the image file
	if(fd == -1) {
		printf("File error: %s -- could not be opened\n", fileName);
		return;
	}
	else {
		int r;
		int count = 0;
		int maxCount = blockCount/threadNum;
		pthread_t child[threadNum];
		threadPara args[threadNum];
		for (int i = 0; i < threadNum; i++){
			args[i].offsetBlock = offsetBlock;
			args[i].blockSize = blockSize;
			args[i].maxCount = maxCount;
			args[i].fd = fd;
			args[i].buf = (unsigned int *)malloc(blockSize);
			//printf("offsetBlock in read: %d\n", args[i].offsetBlock);
			pthread_create(&child[i], NULL, child_thread, (void *)&args[i]);
			//printf("offsetBlock in read done: %d\n", args[i].offsetBlock);
			offsetBlock += maxCount;
		}
		
		for (int i=0; i < threadNum; i++) {
			pthread_join(child[i], NULL);
			//printf("xor in %d: %08x\n", i, args[i].xor);
			xor ^= args[i].xor;
		}
		printf("xor: %08x\n", xor);
	}
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
	FILE *fWrite = fopen("thread_data_no_cache.txt","w");
    char * fileName; // file name
    int blockSize; // block size (divided by 4)
    unsigned long blockCount = 7834624; // block count
	int threadNum = 1; // thread number (threadNum = blockCount // n)
	
	double start, end;
	int ratio;

    if (argc != 2) {
        printf("Invalid inputs\n");
		return 0;
    }

    fileName = argv[1];
	
	for (threadNum; threadNum < 64; threadNum *= 2){
		printf("threadNum: %d\n",threadNum);
		for (blockSize = 64; blockSize <= 4096; blockSize*= 2){
			ratio = 4096 / blockSize;
			printf("blockSize: %d, blockCount: %ld\n",blockSize, blockCount * ratio);
			system("sudo sh -c \"/usr/bin/echo 3 > /proc/sys/vm/drop_caches\"");
			start = now();
			thread_read(blockSize, blockCount * ratio, fileName, threadNum);
			end = now();
			performance(blockSize, blockCount * ratio, threadNum, end - start, fWrite);
			printf("Read Time: %f seconds\n", end - start);
			
		}
		
	}
	
	
	fclose(fWrite);
	return 0;
}