#!/bin/bash
# Leer la dirección IP pasada como argumento
ip=$1
# Hacer algo con la dirección IP
echo "Dirección IP: $ip"
# Escribir la dirección IP en un archivo de texto
echo "$ip" > ip.txt
# Resto del script...
cd ../ADALM-PLUTO-SDR/run
./TX_RX.sh
cd ../../test
pwd
/usr/bin/python3 sdr_test.py
exit 1