echo "====================================================="
echo "                | MAKEFILE TX |                      "
echo "                ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                      "
make clean
sleep 0.5
make
echo "====================================================="

#echo "====================================================="
# CARPETA DE TRABAJO /Jonathan
sshpass -p "analog" scp transmition_config.txt root@192.168.1.38:/jonathan/
sshpass -p "analog" scp 1_transmit_pulse root@192.168.1.38:/jonathan/
sshpass -p "analog" ssh -t root@192.168.1.38  "cd /jonathan/; exec \$SHELL --login"