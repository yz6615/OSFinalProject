#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>
#include <math.h>
#include <pthread.h>

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
			args[i].buf = (unsigned int *)malloc(offsetBlock);
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
    char * fileName; // file name
    int blockSize; // block size (divided by 4)
    int blockCount; // block count
	int threadNum; // thread number (threadNum = blockCount // n)

    if (argc != 6) {
        printf("Invalid inputs");
		return 0;
    }

    fileName = argv[1];
	blockSize = atoi(argv[3]);
    blockCount = atoi(argv[4]);
	threadNum = atoi(argv[5]);
	
	switch(argv[2][1]) {// read or write
		case 'r':
			thread_read(blockSize, blockCount, fileName, threadNum);
		break;
		
		case 'w':
			file_write(blockSize, blockCount, fileName);
		break;
	}

	return 0;
}