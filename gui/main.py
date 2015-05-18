import sys, copy, time, pickle, string
import numpy as np
import Tkinter, tkFileDialog, Tkconstants 
from Tkinter import *
import Tkinter as ttk
import ist
import matplotlib.pyplot as plt
import scipy
import scipy.ndimage as ndimage
import scipy.ndimage.filters as filters

def calculate(*args):

    threshold = float(thresh.get())
    niter = int(itrCount.get())
    isfx = int(X.get())*2   # sparcity
    isfy = int(Y.get())*2   # sparcity
    ist.calc_ist(isfx,isfy,niter,fileNm.get(),fileOut.get(),threshold)


    arr2 = [[] for _ in range(isfx/2)]
    with open(fileOut.get()) as f:
           lines = f.readlines()
           for line in lines:
        	line_data= line.split(',')
	    	arr2[int(line_data[0])].append(float(line_data[2].replace('\n','')))


    data2= np.array(arr2)
    # show original spectra
    cl = data2.std() * 3* 1.2 ** np.arange(10)
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.contour(data2, cl, colors='blue')




    plt.show()
   

def openDirectory():
    dirname = tkFileDialog.askopenfile(parent=root, initialdir='/home/', title='Select your file')
    fileNm.set(dirname.name)

def openDirectory2():
    dirname =  tkFileDialog.askopenfile(parent=root, initialdir='/home/', title='Select your file')
    fileOut.set(dirname.name)

niter =0
threshold = 0.0
root = Tk()
root.title("IST data")

mainframe = ttk.Frame(root)
mainframe.grid(column=0, row=0, sticky=(N, W, E, S))
mainframe.columnconfigure(0, weight=1)
mainframe.rowconfigure(0, weight=1)

thresh = StringVar()
fileNm = StringVar()
fileOut = StringVar()
itrCount = StringVar()
X = StringVar()
Y = StringVar()
fileNm_entry = ttk.Entry(mainframe, width=7, textvariable=fileNm)
fileNm_entry.grid(column=2, row=1, sticky=(W, E))
ttk.Button(mainframe, text="...", command=openDirectory).grid(column=3, row=1, sticky=W)


fileOut_entry = ttk.Entry(mainframe, width=7, textvariable=fileOut)
fileOut_entry.grid(column=2, row=2, sticky=(W, E))
ttk.Button(mainframe, text="...", command=openDirectory2).grid(column=3, row=2, sticky=W)



itrCount_entry = ttk.Entry(mainframe, width=7, textvariable=itrCount)
itrCount_entry.grid(column=2, row=3, sticky=(W, E))

X_entry = ttk.Entry(mainframe, width=7, textvariable=X)
X_entry.grid(column=2, row=4, sticky=(W, E))

Y_entry = ttk.Entry(mainframe, width=7, textvariable=Y)
Y_entry.grid(column=3, row=4, sticky=(W, E))

thresh_entry = ttk.Entry(mainframe, width=7, textvariable=thresh)
thresh_entry.grid(column=2, row=5, sticky=(W, E))

ttk.Button(mainframe, text="Calculate", command=calculate).grid(column=3, row=6, sticky=W)
ttk.Label(mainframe, text="In File").grid(column=1, row=1, sticky=(W, E))
ttk.Label(mainframe, text="Out File").grid(column=1, row=2, sticky=(W, E))
ttk.Label(mainframe, text="Iterations").grid(column=1, row=3, sticky=(W, E))
ttk.Label(mainframe, text="Dimension X.Y(original)").grid(column=1, row=4, sticky=(W, E))
ttk.Label(mainframe, text="Threshold").grid(column=1, row=5, sticky=(W, E))
ttk.Label(mainframe, text="Note: Input file X.Y Must be double of original frequency and in IFT sparse mode.").grid(column=1, row=6, sticky=(W, E))
ttk.Label(mainframe, text="File must be .txt format with  each line stating x,y,I").grid(column=1, row=7, sticky=(W, E))
for child in mainframe.winfo_children(): child.grid_configure(padx=7, pady=7)


root.bind('<Return>', calculate)

root.mainloop()
