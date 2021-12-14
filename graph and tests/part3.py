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
    performanceWithoutCache.append(round(float(line1[1]),1))
    blockSizeWithCache.append(line2[0])
    performanceWithCache.append(round(float(line2[1]),1))
    

x1 = blockSizeWithoutCache
y1 = performanceWithoutCache
x2 = blockSizeWithCache
y2 = performanceWithCache


plt.plot(x1,y1)
plt.xlabel('blockSize')
plt.ylabel('performance (MiB/s)')
plt.title('Performance Vs. BlockSize Without Cache')
plt.plot(x2,y2)
plt.legend(['Without Cache', 'With Cache'])
plt.savefig("plot.png")

file.close()
