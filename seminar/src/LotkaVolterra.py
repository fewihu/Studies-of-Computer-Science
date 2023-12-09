import numpy as np
from scipy.integrate import odeint
import matplotlib.pyplot as plt

# Parameter

a = 2
b = 0.002
c = 0.2
d = 0.0000002

# --- Lotka-Volterra ---
# Funktion, die dx/dt berechnet (2d)
def model3(x, t):
    x1 = x[0]
    x2 = x[1]
    dx1dt = (a * x1) - (b * x1 * x2)
    dx2dt = -(c * x2) + (d * x1 * x2)
    return [dx1dt, dx2dt]

# Anfangswert
x0 = [300000, 1250]

# Zeitschritte
t = np.linspace(0, 100, 2500)
# Graph der Funktion
y15 = odeint(model3, x0, t)

beute = y15[:,0]
rauber= y15[:,1]

# --- zeitabhängige Darstellung ---
plt.semilogy(t,beute,'r--',linewidth=1,label='Beutepopulation')
plt.semilogy(t,rauber,'g:',linewidth=1,label='Räuberpopulation')
plt.legend(loc='best')
plt.xlabel('Zeit')
plt.ylabel('Populationsgröße')
plt.show()

# --- Höhenlinie im Richtungsfeld ---
dx = np.ones((10,10))
dy = np.ones((10,10))
ax = plt.subplot()

ww = np.arange(0, 2500, 50)
vv = np.arange(0, 150000, 3000)
(VV, WW) = np.meshgrid(vv, ww)

ax.quiver(beute, rauber, VV, WW,width=0.001,scale=20000000)
plt.show()
