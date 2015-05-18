import sys, copy, time, pickle, string
import numpy as np
import matplotlib.pyplot as plt


import numpy as np
import scipy
import scipy.ndimage as ndimage
import scipy.ndimage.filters as filters

arr = [[] for _ in range(128)]
with open('sample_NMR.txt') as f:
    lines = f.readlines()
    for line in lines:
    	line_data= line.split(',')
    	arr[int(line_data[0])].append(float(line_data[3].replace('\n','')))


data= np.array(arr)




print type(data)
print len(data)
print len(data[0])



arr2 = [[] for _ in range(256)]
with open('istout.txt') as f:
    lines = f.readlines()
    for line in lines:
    	line_data= line.split(',')
    	arr2[int(line_data[0])].append(float(line_data[2].replace('\n','')))


data2= np.array(arr2)

# show original spectra
cl = data.std() * 3* 1.2 ** np.arange(10)
cl2 = data2.std() * 3* 1.2 ** np.arange(10)
fig = plt.figure()
ax = fig.add_subplot(111)
ax.contour(data, cl, colors='blue')
ax.contour(data2, cl2, colors='red')




plt.show()
