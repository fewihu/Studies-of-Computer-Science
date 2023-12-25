import math
from matplotlib import pyplot as plt

k = [2, 3, 5, 10, 20, 48]


def P_rest(P, k):
    erg = 1 - (
        (1 - P) +                   # Fehlerwkt das der Kanal richtig arbeitet
        (P * math.pow((1-P), k))    # Fehler das ein paket kaputt ist und der rest nicht kaputt ist
    )
    return erg

x = range(0, 500)
x_promill = [n / 1000 for n in x]


k2 = [P_rest(P,2) for P in x_promill]
k3 = [P_rest(P,3) for P in x_promill]
k5 = [P_rest(P,5) for P in x_promill]
k10 = [P_rest(P,10) for P in x_promill]
k20 = [P_rest(P,20) for P in x_promill]
k48 = [P_rest(P,48) for P in x_promill]

#plt.plot(x_promill, x_promill, color='black')
plt.plot(x_promill, k2, color='red')
plt.plot(x_promill, k3, color='blue')
plt.plot(x_promill, k5, color='yellow')
plt.plot(x_promill, k10, color='green')
plt.plot(x_promill, k20, color='purple')
plt.plot(x_promill, k48, color='orange')
plt.plot([0.1,0.2,0.3,0.4,0.5], [0.0217,0.0797,0.1650,0.2553,0.3695], 'k+')
plt.plot([0.1,0.2,0.3,0.4,0.5], [0.1046,0.2048,0.3062,0.3962,0.4922], 'k^')
plt.legend(["2","3","5", "10", "20", "48", "2 (gemessen)", "48 (gemessen)"], loc='best')
plt.grid()

plt.show()
