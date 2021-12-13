import matplotlib.pyplot as plt
fileName1 = "thread_data_cache.txt"
fileName2 = "thread_data_no_cache.txt"
blockSize= []
performance = []
threads = []
threadNums = ["1 thread","2 threads","4 threads","8 threads","16 threads","32 threads"]
file = open(fileName1, mode = 'r')
count = 0;
for line in file:
    line = line.split()
    blockSize.append(line[0])
    performance.append(round(float(line[1]),1))
    count += 1
    if count == 7:
        count = 0
        threads.append((blockSize,performance))
        blockSize= []
        performance = []

#print(threads)

for thread in threads:
    #print(thread)
    x1 = thread[0]
    y1 = thread[1]
    plt.plot(x1,y1)
    
plt.xlabel('blockSize')
plt.ylabel('performance (MiB/s)')
plt.title('Performance Vs. BlockSize With Cache')
plt.legend(threadNums)
plt.savefig("part6_with_cache.png")

file.close()
