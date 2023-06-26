echo "====================================================="
echo "                | MAKEFILE TX 37|                    "
echo "                ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                    "
make clean
sleep 0.5
make
echo "====================================================="

#echo "====================================================="
# CARPETA DE TRABAJO /Jonathan
sshpass -p "analog" scp transmition_config.txt root@192.168.1.34:/jonathan/
sshpass -p "analog" scp 1_transmit_pulse root@192.168.1.34:/jonathan/
sshpass -p "analog" ssh -t root@192.168.1.34  "cd /jonathan/; exec \$SHELL --login"