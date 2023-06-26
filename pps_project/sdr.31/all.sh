
echo "====================================================="
echo "                | MAKEFILE TX 31|                    "
echo "                ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                    "
make clean
#rm output_pulse.txt
echo -n > output_pulse.txt
make
echo "====================================================="

#echo "====================================================="
# CARPETA DE TRABAJO /Jonathan
#Configuraciones
sshpass -p "analog" scp transmition_config.txt root@192.168.1.31:/jonathan/
sshpass -p "analog" scp receive_config.txt root@192.168.1.31:/jonathan/
#Programa
sshpass -p "analog" scp tx_rx_pulse root@192.168.1.31:/jonathan/
#Ejecutar
sshpass -p "analog" ssh -t -t root@192.168.1.31 "cd /jonathan/; exec ./tx_rx_pulse"
#Obtener resultados
sshpass -p "analog" scp  root@192.168.1.31:/jonathan/output_pulse.txt ../sdr.31

#sshpass -p "analog" ssh -t root@192.168.1.31  "cd /jonathan/; exec \$SHELL --login"
#ingresar a la sdr 31
#sshpass -p "analog" ssh -l root 192.168.1.31