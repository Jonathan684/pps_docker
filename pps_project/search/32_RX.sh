make clean
make
sshpass -p "analog" scp receive_config.txt root@192.168.1.32:/jonathan/
sshpass -p "analog" scp 2_receive_pulse root@192.168.1.32:/jonathan/
#ejecutar
sshpass -p "analog" ssh -t root@192.168.1.32 "cd /jonathan/; exec ./2_receive_pulse"
#buscar
sshpass -p "analog" scp  root@192.168.1.32:/jonathan/output_pulse.txt ../pps_project/data