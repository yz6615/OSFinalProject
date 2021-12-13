import matplotlib.pyplot as plt
fileName = "data.txt"
blockSize = []
performance = []
file = open(fileName, mode = 'r')
for line in file:
    line = line.split()
    blockSize.append(line[0])
    performance.append(float(line[1]))
print(blockSize,performance)
x = blockSize
y = performance
plt.plot(x,y)
plt.xlabel('blockSize')
plt.ylabel('performance (MiB/s)')
plt.title('Performance Vs. BlockSize')
for i,j in zip(x,y):
    plt.text(i,j, str(j))
plt.savefig("plotWithoutCache.png")
file.close()
