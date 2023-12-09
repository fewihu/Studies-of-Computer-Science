import numpy as np
from scipy.integrate import odeint
import matplotlib.pyplot as plt

# --- Räuber-Beute-Modell mit beschränkter Beutepopulation ---

#Paramter
a = 2
b = 0.002
c = 2
d = 0.002
k = 20000

def dx1(x1,x2):
    ret = (a * x1) - ((a/k) * x1**2) - (b * x1 * x2)
    return ret

def dx2(x1,x2):
    ret = -(c * x2) + (d * x1 * x2)
    return ret

# Funktion, die dx/dt berechnet (2d)
def model3(x, t):
    x1 = x[0]
    x2 = x[1]
    dx1dt = (a * x1) - ((a/k) * x1**2) - (b * x1 * x2)
    dx2dt = -(c * x2) + (d * x1 * x2)
    return [dx1dt, dx2dt]

# Anfangswert
x0 = [1000, 2500]

# Zeitschritte
t = np.linspace(0, 100, 2500)
# Graph der Funktion
y15 = odeint(model3, x0, t)

beute = y15[:,0]
rauber= y15[:,1]

# --- zeitabhängige Darstellung ---
plt.plot(t,beute,'r--',linewidth=1,label='Beutepopulation')
plt.plot(t,rauber,'g:',linewidth=1,label='Räuberpopulation')
plt.legend(loc='best')
plt.xlabel('Zeit')
plt.ylabel('Populationsgröße')
plt.show()

x1 = np.linspace(0,20000,200)
x2 = np.linspace(0,4000, 200)

plt.plot(beute,rauber,linewidth=1)
plt.show()

