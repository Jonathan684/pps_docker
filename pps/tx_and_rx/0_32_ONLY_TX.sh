echo "====================================================="
echo "                | MAKEFILE TX 32|                    "
echo "                ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                    "
make clean
sleep 0.5
make
echo "====================================================="
echo -n > output_pulse.txt
#echo "====================================================="
# CARPETA DE TRABAJO /Jonathan
sshpass -p "analog" scp filter_1.txt root@192.168.1.32:/jonathan/
sshpass -p "analog" scp filter_2.txt root@192.168.1.32:/jonathan/
sshpass -p "analog" scp filter_3.txt root@192.168.1.32:/jonathan/
sshpass -p "analog" scp filter_4.txt root@192.168.1.32:/jonathan/

sshpass -p "analog" scp tx_config.txt root@192.168.1.32:/jonathan/
sshpass -p "analog" scp rx_config.txt root@192.168.1.32:/jonathan/

sshpass -p "analog" scp tx root@192.168.1.32:/jonathan/
sshpass -p "analog" ssh -t root@192.168.1.32  "cd /jonathan/; exec \$SHELL --login"

echo "FIN SIMULACION"
#sshpass -p "analog" ssh -t root@192.168.1.31  "cd /jonathan/; exec \$SHELL --login"



#ejecutar
#sshpass -p "analog" ssh -t root@192.168.1.31 "cd /jonathan/; exec ./2_receive_pulse"
#buscar
#sshpass -p "analog" scp  root@192.168.1.31:/jonathan/output_pulse.txt ../new_idea