import matplotlib.pyplot as plt
fileName = "data.txt"
blockSize = []
performance = []
file = open(fileName, mode = 'r')
for line in file:
    line = line.split()
    blockSize.append(line[0])
    performance.append(line[1])
plt.plot(x,y)
plt.xlabel('blockSize')
plt.ylabel('performance (MiB/s)')
file.close()
