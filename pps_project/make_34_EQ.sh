echo "====================================================="
echo "                | MAKEFILE |                         "
echo "                ¯¯¯¯¯¯¯¯¯¯¯¯                         "
make clean
sleep 0.5
make
echo "====================================================="

# #echo "====================================================="
# # CARPETA DE TRABAJO /Jonathan
# sshpass -p "analog" scp transmition_config.txt root@192.168.1.34:/jonathan/
# sshpass -p "analog" scp 1_transmit_pulse root@192.168.1.34:/jonathan/
# sshpass -p "analog" ssh -t root@192.168.1.34  "cd /jonathan/; exec \$SHELL --login"

# CARPETA DE TRABAJO /Jonathan
sshpass -p "analog" scp transmition_config.txt root@192.168.1.34:/jonathan/
sshpass -p "analog" scp 1_transmit_pulse root@192.168.1.34:/jonathan/
sshpass -p "analog" scp receive_config.txt root@192.168.1.34:/jonathan/
sshpass -p "analog" scp 2_receive_pulse root@192.168.1.34:/jonathan/
echo "ejecutar"
sshpass -p "analog" ssh -t -f root@192.168.1.34 "cd /jonathan/; exec ./1_transmit_pulse"
sshpass -p "analog" ssh -t root@192.168.1.34 "cd /jonathan/; exec ./2_receive_pulse"

sshpass -p "analog" scp  root@192.168.1.34:/jonathan/output_pulse.txt ../pps_project

sleep 11
#make clean


#sshpass -p "analog" scp receive_pulse root@192.168.1.34:/jonathan/
#sshpass -p "analog" ssh -l root 192.168.1.34

#sshpass -p "analog" scp 2_sdr root@192.168.1.32:/tmp/
#sshpass -p "analog" ssh -l root 192.168.1.34
#sshpass -p "analog" scp filter.txt root@192.168.1.34:/tmp/