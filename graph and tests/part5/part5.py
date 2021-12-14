import matplotlib.pyplot as plt
fileName = "part5_data.txt"
fileSize = []
performance = []
file = open(fileName, mode = 'r')
for line in file:
    line = line.split()
    fileSize.append(int(float(line[0])/1000000))
    performance.append(round(float(line[1]),3))

print(fileSize,performance)

x1 = fileSize
y1 = performance



plt.plot(x1,y1)
plt.xlabel('File Size')
plt.ylabel('performance (MiB/s)')
plt.title('Performance Vs. File Size')

plt.savefig("part5.png")

file.close()
