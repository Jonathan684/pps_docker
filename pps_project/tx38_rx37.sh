make clean
make
echo "Enviando..."
sshpass -p "analog" scp receive_config.txt root@192.168.1.37:/jonathan/
sshpass -p "analog" scp 2_receive_pulse root@192.168.1.37:/jonathan/
sshpass -p "analog" scp transmition_config.txt root@192.168.1.38:/jonathan/
sshpass -p "analog" scp 1_transmit_pulse root@192.168.1.38:/jonathan/

echo "Transmitiendo"
sshpass -p "analog" ssh -t -f root@192.168.1.38 "cd /jonathan/; exec ./1_transmit_pulse"
echo "Recibiendo"
sshpass -p "analog" ssh -t root@192.168.1.37 "cd /jonathan/; exec ./2_receive_pulse"
sshpass -p "analog" scp  root@192.168.1.37:/jonathan/output_pulse.txt ../pps_project
sleep 12
echo "Fin"