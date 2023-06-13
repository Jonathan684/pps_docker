echo "====================================================="
echo "                | MAKEFILE TX 31|                    "
echo "                ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                    "
make clean
sleep 0.5
make
echo "====================================================="

#echo "====================================================="
# CARPETA DE TRABAJO /Jonathan
sshpass -p "analog" scp transmition_config.txt root@192.168.1.32:/jonathan/
sshpass -p "analog" scp 1_transmit_pulse root@192.168.1.32:/jonathan/
sshpass -p "analog" ssh -t root@192.168.1.32  "cd /jonathan/; exec \$SHELL --login"
