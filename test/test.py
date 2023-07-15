from os import remove
import numpy as np
import matplotlib.pyplot as plt
import ast

# Read the file pps_docker/pps/sdr_36/output/datos.txt pps_docker/pps/sdr_36/output/datos.txt
with open("datos.txt", "r") as file:
    content = file.read()

Longitud_del_pulso = 1
PRI = 2**15-1
count = 0
amplitud = 4080# 2 ** 14
N = 2**18-1  # Replace N with the desired value of your signal length buffer rx
signal = np.zeros(N)
signal_tx = np.zeros((1, N), dtype=complex)  # Inicializar el array de pulsos

for n in range(N):
    if 0 <= count < Longitud_del_pulso:
        signal[n] = 1 / amplitud
        signal_tx[0,n] = 100+100j
       
    elif Longitud_del_pulso <= count < PRI:
        signal[n] = 0.0
        signal_tx[0,n] = 0+0j
    count += 1
    if count == PRI:
        count = 0

signal_tx = signal_tx[0]


def plotSignal(signal_tx,signal_rx,max):
    print("graficar")
    #------------------------------- Transmitted I component -------------------------------
    # Plot time domain
    fig = plt.figure(figsize=(20, 8), dpi=80, facecolor='w', edgecolor='k')

    # Subplot 1 - Transmitted I component
    ax1 = fig.add_subplot(2, 2, 1)
    ax1.plot(np.real(signal_rx),color = "blue", label = "recibida")
    ax1.plot(np.real(signal_tx), color="red", label = "transmitida")
    
    num_samples = len(signal_rx)
    x_ticks = np.linspace(0, num_samples-1, 11, dtype=int)
    ax1.set_xticks(x_ticks)
    ax1.legend()
    ax1.set_ylabel('Amplitude')
    ax1.set_xlabel('samples')
    ax1.set_title('In-phase Component of the Signal_rx and tx')
    ax1.grid()

    # Subplot 2 - Transmitted Q component
    ax2 = fig.add_subplot(2, 2, 2)
    ax2.plot(np.imag(signal_rx), color = "blue", label = "recibida")
    ax2.plot(np.real(signal_tx), color="red", label = "transmitida")
    ax2.set_ylabel('Amplitude')
    ax2.set_xlabel('samples')
    ax2.set_title('Quadrature Component of the Signal and tx')
    ax2.legend()
    ax2.grid()
    
    #ZOOM
    medio = max# int((2**18)/2) 
    #medio = PRI
    mini = medio - 20000
    maxim = medio + 20000

    # Subplot 3 - Component in quadrature (first 1000 samples)
    ax3 = fig.add_subplot(2, 2, 3)
    ax3.set_xlim(mini,maxim)
    ax3.plot(np.real(signal_rx),color="blue", label = "recibida")
    ax3.plot(np.real(signal_tx),color="red", label = "transmitida")
    ax3.set_ylabel('Amplitude')
    ax3.set_xlabel('samples')
    ax3.set_title('In-phase Component of the Signal_rx and tx (zoom centro)')
    ax3.grid()
    ax3.legend()
    #x_min = 0
    #x_max = 1000
    
    # Subplot 4 - Component in quadrature (first 1000 samples)
    ax4 = fig.add_subplot(2, 2, 4)
    ax4.set_xlim(mini,maxim)
    ax4.plot(np.imag(signal_rx),color="blue", label = "recibida")
    ax4.plot(np.imag(signal_tx),color="red", label = "transmitida")
    ax4.set_ylabel('Amplitude')
    ax4.set_xlabel('samples')
    ax4.set_title('Quadrature Component of the Signal and tx (zoom centro)')
    ax4.grid()
    ax4.legend()
    plt.tight_layout()
    plt.show()

signal_rx = ast.literal_eval(content)


max = np.real(signal_rx).argmax()
min = np.real(signal_rx).argmin()

val_max = np.real(signal_rx)[max]
val_min = np.real(signal_rx)[min]

print("Maximo :",np.real(signal_rx)[max])
print("Min :",np.real(signal_rx)[min])
print("Posicion max:",max)
print("Posicion min:",min)


plotSignal(signal_tx,signal_rx,max)


#plotSignal(txSignal)
min = 0
max = 200000

#print(np.real(txSignal[10000:10000]))
flag = 0
for i in np.real(signal_rx[min:max]):
    if(i  > 0.20):
        #print(i)
        flag += 1
        break
    flag += 1

# print("flag:",flag)
# print(np.real(signal_rx[(min+flag)]))
# print("1->",np.real(signal_tx[:10]))
# print("1 IMG->",np.imag(signal_tx[:10]))
# print("2->",np.real(signal_rx[:10]))


# def plotSignal(signal):
# #------------------------------- Transmitted I component -------------------------------
# # Plot time domain
#     plt.figure(figsize=(20,8), dpi= 80, facecolor='w', edgecolor='k')
#     plt.subplot(2,2,2)
#     plt.plot(np.real(signal))
#     num_samples = len(signal)
#     x_ticks = np.linspace(0, num_samples-1, 11, dtype=int)
#     plt.xticks(x_ticks)
#     plt.xlabel('n')
#     plt.ylabel('Amplitud')
#     plt.title('Componente en fase de la señal')
#     plt.grid()
# #------------------------------- Transmitted Q component -------------------------------
# # Plot time domain
#     plt.subplot(2,2,1)
#     plt.plot(np.imag(signal))
#     plt.xlabel('n')
#     plt.ylabel('Amplitud')
#     plt.title('Componente en cuadratura de la señal')
#     plt.grid()


#     plt.subplot(2,1,1)
#     cant = 1000
#     plt.plot(np.imag(signal[:cant]))
#     plt.xlabel('n')
#     plt.ylabel('Amplitud')
#     plt.title('Componente en cuadratura de la señal')
#     plt.grid()

#     plt.subplot(2,1,2)
#     plt.plot(np.imag(signal[:cant]))
#     plt.xlabel('n')
#     plt.ylabel('Amplitud')
#     plt.title('Componente en cuadratura de la señal')
#     plt.grid()
#     plt.show()




# #plotSignal(txSignal[1:1000])
# #plotSignal(txSignal[1:100])

# # Datos de las muestras y la frecuencia de muestreo
# muestras = np.real(txSignal[:100])  # Muestras de la señal
# frecuencia_muestreo = 60000000  # Frecuencia de muestreo en Hz

# # Calcular el tiempo correspondiente a cada muestra en microsegundos
# tiempo_micros = np.arange(len(muestras)) * (1 / frecuencia_muestreo) * 1e6

# # Graficar la señal en función del tiempo en microsegundos
# plt.figure()
# plt.plot(tiempo_micros, muestras)
# plt.xlabel('Tiempo (μs)')
# plt.ylabel('Amplitud')
# plt.title('Gráfico de la señal en función del tiempo en microsegundos')
# plt.grid(True)
# plt.show()