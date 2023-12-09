import numpy as np
from scipy.integrate import odeint
import matplotlib.pyplot as plt

# Funktion zum float Iterieren
def my_range(start, end, step):
    while start <= end:
        yield start
        start += step

# Paramter
N = 500
a = 0.198
b = 0.001

# Messwerte
ref_t = [5,10,15,35,60]
ref_i = [5,20,63,32, 2]
# Funktion, die dx/dt berechnet (2d)
def model3(x, t):
    S = x[0]
    I = x[1]
    R = x[2]

    dSdt = -b * S * I
    dIdt = (b * S * I) - (a * I)
    dRdt = a * I
    return [dSdt, dIdt, dRdt]


# Anfangswert
x0 = [N-1, 1, 0]

# Zeitschritte
t = np.linspace(0, 100, 100)

# Graph der Funktion
y = odeint(model3, x0, t)

s = y[:, 0]
i = y[:, 1]
r = y[:, 2]

sum = 0
for index in range(0,5,1):
    print(i[ref_t[index]])
    print(ref_i[index])
    print((i[ref_t[index]] - ref_i[index])**2)
    print("--------------")
    sum += (i[ref_t[index]] - ref_i[index])**2

print(sum)

# --- zeitabhängige Darstellung ---
plt.plot(t, s, 'r--', linewidth=1, label='S')
plt.plot(t, i, 'g:', linewidth=1, label='I')
plt.plot(t, r, 'b--', linewidth=1, label='R')
#plt.semilogy(t,s,'r--',linewidth=1,label='S')
#plt.semilogy(t,i,'g:',linewidth=1, label='I')
#plt.semilogy(t,r,'b--',linewidth=1,label='R')

plt.legend()
plt.xlabel('Zeit')
plt.ylabel('Anzahl Menschen')
plt.show()

plt.plot(t, i, 'g', linewidth=1)
plt.plot(ref_t, ref_i, "ob")

plt.xlabel('Zeit')
plt.ylabel('Anzahl Menschen')
plt.show()

