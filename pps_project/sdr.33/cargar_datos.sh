
echo "====================================================="
echo "                | MAKEFILE TX 33|                    "
echo "                ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                    "
make clean
#rm output_pulse.txt
echo -n > output_pulse.txt
make
echo "====================================================="

#echo "====================================================="
# CARPETA DE TRABAJO /Jonathan
#Configuraciones
sshpass -p "analog" scp transmition_config.txt root@192.168.1.33:/jonathan/
sshpass -p "analog" scp receive_config.txt root@192.168.1.33:/jonathan/
sshpass -p "analog" scp tx_rx_pulse root@192.168.1.33:/jonathan/

