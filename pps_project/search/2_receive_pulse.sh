echo "====================================================="
echo "                | MAKEFILE RX |                      "
echo "                ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                      "
make clean
sleep 0.5
make
echo "====================================================="

#echo "====================================================="
# CARPETA DE TRABAJO /Jonathan
sshpass -p "analog" scp receive_config.txt root@192.168.1.37:/jonathan/
sshpass -p "analog" scp 2_receive_pulse root@192.168.1.37:/jonathan/
sshpass -p "analog" ssh -t root@192.168.1.37  "cd /jonathan/; exec \$SHELL --login"