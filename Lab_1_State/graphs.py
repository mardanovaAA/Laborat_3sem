import matplotlib.pyplot as plt
import numpy as np

i, pt = np.loadtxt('set_state_1.txt', unpack = True)


plt.figure(figsize=(10,7))
plt.grid(True)
plt.plot(i, pt)
plt.title(r'$Зависимость \: вероятности \: от \: числа \: испытаний \: при \: s=(1, 3, 5, 7, 23, 48, 57, 60, 90, 99)$', size = 20)
#plt.text(0, 0, r"$test \: count \: = 10000$")
plt.xlabel(r'test_count', fontsize = 16)
plt.ylabel(r'probability', fontsize = 16)
plt.savefig('set_state_1.png')
plt.show()