import matplotlib.pyplot as plt
import numpy as np

#FN_t,keys_t = np.loadtxt("VFN_priojection_t.txt",float,delimiter=" ", unpack=True)
#FN_f,keys_f = np.loadtxt("VFN_priojection_f.txt",float,delimiter=" ", unpack=True)

n,T,F=np.loadtxt("research.txt",float,delimiter=" ", unpack=True)
modif_t=T[0:11]
vfn_t=T[11:]

modif_f=F[0:11]
vfn_f=F[11:]


axis_noise=np.arange(-10,11,step=2)
"""Вывод графиков"""
fig,axs = plt.subplots(2)
#axs[0].plot(keys_t,FN_t,'-',color='black',linewidth=0.8)
#axs[0].set_xlabel("t,c")
#axs[0].set_ylabel("Сечение ВФН")
#axs[1].plot(keys_f,FN_f,'-',color='black',linewidth=0.5)

axs[0].plot(axis_noise,vfn_t,marker='.',color='black',linewidth=1)
axs[0].plot(axis_noise,modif_t,marker='.',color='green',linewidth=1)
axs[1].plot(axis_noise,vfn_f,marker='.',color='black',linewidth=1)
axs[1].plot(axis_noise,modif_f,marker='.',color='green',linewidth=1)
plt.show()


print(axis_noise)
print(vfn_t)
print(vfn_f)
print(modif_t)
print(modif_f)