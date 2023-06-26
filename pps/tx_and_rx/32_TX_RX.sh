echo "====================================================="
echo "                | MAKEFILE TX 31|                    "
echo "                ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                    "
make clean
sleep 0.5
make
echo "====================================================="
echo -n > output_pulse.txt
#echo "====================================================="
# CARPETA DE TRABAJO /Jonathan
sshpass -p "analog" scp tx_config.txt root@192.168.1.32:/jonathan/
sshpass -p "analog" scp rx_config.txt root@192.168.1.32:/jonathan/

sshpass -p "analog" scp tx_rx root@192.168.1.32:/jonathan/
sshpass -p "analog" ssh -t root@192.168.1.32 "cd /jonathan/; exec ./tx_rx"

sshpass -p "analog" scp  root@192.168.1.32:/jonathan/output_pulse.txt ../tx_and_rx

echo "fin"
#sshpass -p "analog" ssh -t root@192.168.1.31  "cd /jonathan/; exec \$SHELL --login"



#ejecutar
#sshpass -p "analog" ssh -t root@192.168.1.31 "cd /jonathan/; exec ./2_receive_pulse"
#buscar
#sshpass -p "analog" scp  root@192.168.1.31:/jonathan/output_pulse.txt ../new_idea