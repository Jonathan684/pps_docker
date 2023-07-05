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
sshpass -p "analog" scp filter_1.txt root@192.168.1.34:/jonathan/
sshpass -p "analog" scp filter_2.txt root@192.168.1.34:/jonathan/
sshpass -p "analog" scp filter_3.txt root@192.168.1.34:/jonathan/
sshpass -p "analog" scp filter_4.txt root@192.168.1.34:/jonathan/

sshpass -p "analog" scp tx_config.txt root@192.168.1.34:/jonathan/
sshpass -p "analog" scp rx_config.txt root@192.168.1.34:/jonathan/

sshpass -p "analog" scp tx_rx root@192.168.1.34:/jonathan/
sshpass -p "analog" ssh -t root@192.168.1.34 "cd /jonathan/; exec ./tx_rx"

sshpass -p "analog" scp  root@192.168.1.34:/jonathan/output_pulse.txt ../tx_and_rx

echo "FIN SIMULACION"
