echo "====================================================="
echo "                | MAKEFILE |                         "
echo "                ¯¯¯¯¯¯¯¯¯¯¯¯                         "
make clean
sleep 0.5
make
echo "====================================================="

#echo "====================================================="
# CARPETA DE TRABAJO /Jonathan
sshpass -p "analog" scp transmit_pulse root@192.168.1.37:/jonathan/
#sshpass -p "analog" scp receive_pulse root@192.168.1.37:/jonathan/
#sshpass -p "analog" ssh -l root 192.168.1.37
sshpass -p "analog" ssh -t root@192.168.1.37  "cd /jonathan/; exec \$SHELL --login"
#sshpass -p "analog" scp 2_sdr root@192.168.1.32:/tmp/
#sshpass -p "analog" ssh -l root 192.168.1.37
#sshpass -p "analog" scp filter.txt root@192.168.1.37:/tmp/