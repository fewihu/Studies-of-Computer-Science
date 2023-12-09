import numpy as np
from scipy.integrate import odeint
import matplotlib.pyplot as plt

# --- SIS-Modell ---

#Paramter

b = 5
g = 3
m = 0.2
N = 80000000

# Funktion, die dx/dt berechnet (2d)
def model3(x, t):
    x1 = x[0]
    x2 = x[1]
    dx1dt = (m*(x1+x2)) - (b*x1*(x2/(x1+x2))) - (m*x1) + (g*x2)
    dx2dt = (b*x1*(x2/(x1+x2))) - ((m+g)*x2)
    return [dx1dt, dx2dt]

# Anfangswert
inf = 5000
x0 = [N-inf, inf]

# Zeitschritte
t = np.linspace(0, 100, 2500)
# Graph der Funktion
y = odeint(model3, x0, t)

s = y[:,0]
i = y[:,1]

# --- zeitabhängige Darstellung ---
plt.plot(t,s,'r--',linewidth=1,label='Infizierbare')
plt.plot(t,i,'g:',linewidth=1,label='Infizierte')
plt.legend(loc='best')
plt.xlabel('Zeit')
plt.ylabel('Anzahl Menschen')
plt.show()


plt.plot(s,i,linewidth=1)
plt.show()

