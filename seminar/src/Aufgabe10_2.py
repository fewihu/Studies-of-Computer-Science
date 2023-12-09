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
sum = 0
sum_buf = 100000
for a in my_range(0.0, 1.0, 0.01):
    print("===")
    print(a)
    for b in my_range(0.0, 0.2, 0.0001):
        sum = 0
        y = odeint(model3, x0, t)
        i = y[:, 1]
        for index in range(0, 5, 1):
            sum += (i[ref_t[index]] - ref_i[index])**2

        if(sum < sum_buf):
            print(sum)
            print(a)
            print(b)
            print("---------------------")
            sum_buf = sum