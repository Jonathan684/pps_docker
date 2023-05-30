echo "====================================================="
make clean
sleep 0.3
make
echo "====================================================="
# CARPETA DE TRABAJO /Jonathan
sshpass -p "analog" scp transmitter_v1 root@192.168.1.37:/jonathan/
sshpass -p "analog" scp receiver_v1 root@192.168.1.37:/jonathan/
sshpass -p "analog" scp transmition_config.txt root@192.168.1.37:/jonathan/


sshpass -p "analog" ssh -t root@192.168.1.37  "cd /jonathan/; exec \$SHELL --login"

#sshpass -p "analog" ssh -t -f root@192.168.1.37 "cd /jonathan/; exec ./transmitter"
#sshpass -p "analog" ssh -t root@192.168.1.37 "cd /jonathan/; exec ./receiver_v1"

#sshpass -p "analog" scp  root@192.168.1.37:/jonathan/output.txt ../receiver_and_transmitter
#sleep 10
#make clean

#sshpass -p "analog" scp 2_sdr root@192.168.1.32:/tmp/
#sshpass -p "analog" ssh -l root 192.168.1.37
#sshpass -p "analog" scp filter.txt root@192.168.1.37:/tmp/