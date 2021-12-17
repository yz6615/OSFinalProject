import matplotlib.pyplot as plt
fileName = "part3Data.txt"
blockSize = []
performance = []
file = open(fileName, mode = 'r')
for line in file:
    line1 = line.split()
    blockSize.append(line1[0])
    performance.append(round(float(line1[1]),1))
    

x1 = blockSize
y1 = performance

plt.figure(figsize=(12,8))
plt.plot(x1,y1)
plt.xlabel('blockSize')
plt.ylabel('performance (MiB/s)')
for i, j in zip(x1,y1):
    plt.text(i,j,'('+str(i)+', '+str(j)+')')

plt.title('Part3 Figure. Performance Vs. BlockSize')
plt.savefig("part3Plot.png")

file.close()
