
import numpy as np
from matplotlib import pyplot as plt


data = np.genfromtxt('analise.csv',delimiter=',',unpack=True,)

dw= []
dh= []
for i in range(0, len(data[0])-1):
	dw.append(data[0][i+1]-data[0][i])
	dh.append(data[1][i+1]-data[1][i])

#data[0] width
#data[1] Height
plt.show()

f, (p, pd) = plt.subplots(1,2)

p.plot(data[0], label="width", c="red")
p.plot(data[1], label="Height",c="blue")

pd.plot(dw, label="dw", c="red")
pd.plot(dh, label="dh", c="blue")


plt.legend()
plt.show()

	
