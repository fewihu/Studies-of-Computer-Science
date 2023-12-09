import numpy as np
from scipy.integrate import odeint
from scipy.optimize import least_squares
import matplotlib.pyplot as plt
import sys
from scipy.stats.stats import pearsonr
from decimal import Decimal

# Beipiel: lsq_agn.py 0.2 0.6 125 0.5 0.5 5000.0
# alpha = 0.2, beta = 0.6, 125 Zeitschritte, Gewicht I = 0.5 Gewicht R = 0.5
# Varianz für Störung = 5000.0

# Argumente
# ! sys.argv[0] ist der Programmname
alphaOrig 	= float(sys.argv[1]) #Parameter für ungestörte Lösung
betaOrig	= float(sys.argv[2])
timeSteps	= int(sys.argv[3])	 #Anzahl Zeitschritte
weightI 	= float(sys.argv[4]) #Gewicht I
weightR 	= float(sys.argv[5]) #Gewicht R
var			= float(sys.argv[6]) #Varianz der Störung
ts = np.linspace(0,timeSteps-1,num=timeSteps)

#Startwerte ODE
S0 = 80000000;I0 = 1 ;R0 = 0
x0=[S0,I0,R0]

#Differentialgleichungsmodell
def model(x, ts, alpha, beta):

    # Werte
    S = x[0];I = x[1];R = x[2]

    # Ableitung
    dSdt = (-beta/S0) * S * I
    dIdt = ((beta/S0) * S * I) - (alpha * I)
    dRdt = alpha * I
    
    return [dSdt, dIdt, dRdt]
    
# erzeugt ungestörte Vektoren der Länge steps des Modells
# mit Parametern alpha und beta und Startwertvektor x
def genGenuineSIR(x, steps, alpha, beta):
	
	ts = np.linspace(0,steps-1,steps)
	return odeint(model, x0, ts, args=(alpha,beta))
	
	
# erzeugt Fehlervektor der Länge steps
# normalverteilt E=0 Varianz=var
def genNoise(var, steps):	
	return np.random.normal(0,var,steps)


# addiert normalverteilten Fehler auf Vektor data 	
# Werte kleiner 0 werden 0 
def addNoise(var, steps, data):
	
	noisyData = data + genNoise(var, steps)
	return noisyData


# berechne Modeldaten zu Parametervektor theta und
# Zeiteinteilung time
def modelValues(theta,time):
    alpha = theta[0]; beta = theta[1]
    modelValues = odeint(model, x0, time,args=(alpha, beta))
    return modelValues

# Residuumsfunktion nutzt Messwerte für I und R
# gewichtet
def residuum(theta):
		I_res = weightI * (noisyI - modelValues(theta, ts)[:, 1])
		R_res = weightR * (noisyR - modelValues(theta, ts)[:, 2])
		total = np.concatenate([I_res,R_res])
		return I_res


# kompletter Prozess
# erzeugt ungestörte Lösung 
# fügt Störung hinzu
# schätzt aus gestörten Werten Parameter
def genDataAndOptimize(): 
	def residuum(theta):
		I_res = weightI * (noisyI - modelValues(theta, ts)[:, 1])
		R_res = weightR * (noisyR - modelValues(theta, ts)[:, 2])
		total = np.concatenate([I_res,R_res])
		return I_res
     
        
	# erzeuge Daten und störe diese
	genuineSIR 	= genGenuineSIR(x0,timeSteps,alphaOrig,betaOrig)
	noisyI		= addNoise(var, timeSteps, genuineSIR[:, 1])
	noisyR		= addNoise(var, timeSteps, genuineSIR[:, 2])	

	# Optimierung
	theta0 = [0.2,0.6]
	res = least_squares(residuum, theta0, verbose=0,
                    ftol= 1e-12,
                    xtol= 1e-12,
                    gtol= 1e-12,
                    #bounds=((0,0),(4,4)),
  	                max_nfev=200)     
	return res
  
  	   
# Scheleife x-fache Ausführung genDataAndOptimize          
list_alpha = []
list_beta  = []
i = 0  	          
while i < 200:
	result = genDataAndOptimize()  
	if(result.status > 0): 
		#print(result.status)
		i = i + 1
		list_alpha.append(result.x[0])
		list_beta.append(result.x[1])


corelation = pearsonr(np.array(list_alpha), np.array(list_beta))
print(corelation)

#Scatter Plot alpha - beta
plt.rcParams.update({'font.size': 18})	
plt.title("alpha0: " + str(alphaOrig) + ",  beta0: " + str(betaOrig) + ",  var: " + '%.0E' % Decimal(var) + ",  Zeitschritte: " + str(timeSteps));
plt.scatter(np.array(list_alpha), np.array(list_beta), marker='x', label='Optimierungsergebnis')
plt.scatter(alphaOrig, betaOrig, marker='*', color='red', s=70, label='ursprüngliche Parameter')
plt.xlim([0.17, 0.24])
plt.ylim([0.565, 0.64])
plt.legend(loc='best')
plt.xlabel('alpha')
plt.ylabel('beta')
plt.grid()
plt.show()

#-------------------------------------------------------------------------
# beispielhaft einen zeitabhängigen Plot für I und R, 
# um gestörte Werte und Lage der optimierten Funktion zu zeigen

orig	= genGenuineSIR(x0,timeSteps,alphaOrig,betaOrig)
noisyI	= addNoise(var, timeSteps, orig[:, 1])
noisyR	= addNoise(var, timeSteps, orig[:, 2])

theta0 = [0.2,0.6]
res = least_squares(residuum, theta0, verbose=0,
                    ftol= 1e-12,
                    xtol= 1e-12,
                    gtol= 1e-12,
  	                max_nfev=200)

optSIR = odeint(model, x0, ts, args=(res.x[0], res.x[1]))
s = optSIR[:, 0]; i = optSIR[:, 1]; r = optSIR[:, 2]

#Plot für I
plt.plot(orig[:, 1], color='orange' ,linewidth=3,label='ungestörte Lösung für I')
plt.plot(noisyI,'bo' , label='gestörte Messdaten für I')
#plt.plot(i,color='black', linewidth = 2, linestyle=':', label='opt')
plt.legend(loc='best')
plt.xlabel('t')
plt.ylabel('I(t)')
plt.grid()
plt.show()

#Plot für R
plt.plot(orig[:, 2], color='orange', linewidth=3,label='ungestörte Lösung für R')
plt.plot(noisyR, 'bo', label='gestörte Messdaten für R')
#plt.plot(r,color='black', linewidth = 2, linestyle=':', label='opt')
plt.legend(loc='best')
plt.xlabel('t')
plt.ylabel('R(t)')
plt.grid()
plt.show()

plt.rcParams.update({'font.size': 22})
plt.plot(orig[:, 0], color='orange', linewidth=3, label='S(t)')
plt.plot(orig[:, 1], color='blue', linewidth=3,   label='I(t)')
plt.plot(orig[:, 2], color='green', linewidth=3,  label='R(t)')
plt.legend(loc='best')
plt.xlabel('t')
plt.ylabel('Anzahl Individuen in S(t), I(t), R(t)')
plt.grid()
plt.show()

