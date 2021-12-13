import matplotlib.pyplot as plt
fileName = "data.txt"
blockSizeWithoutCache = []
performanceWithoutCache = []
blockSizeWithCache = []
performanceWithCache = []
file = open(fileName, mode = 'r')
for line in file:
    line1 = line.split()
    line2 = file.readline()
    line2 = line2.split()
    blockSizeWithoutCache.append(line1[0])
    performanceWithoutCache.append(float(line1[1]))
    blockSizeWithCache.append(line2[0])
    performanceWithCache.append(line2[1])
    

x1 = blockSizeWithoutCache
y1 = performanceWithoutCache
x2 = blockSizeWithCache
y2 = performanceWithCache

print(x1, y1)
plt.subplot(2,1,1)
plt.plot(x1,y1)
plt.xlabel('blockSize')
plt.ylabel('performance (MiB/s)')
plt.title('Performance Vs. BlockSize')
for i,j in zip(x1,y1):
    plt.text(i,j, str(j))

plt.subplot(2,1,2)
plt.plot(x2,y2)
plt.xlabel('blockSize')
plt.ylabel('performance (MiB/s)')
plt.title('Performance Vs. BlockSize')
for i,j in zip(x2,y2):
    plt.text(i,j, str(j))
plt.savefig("plot.png")

file.close()
