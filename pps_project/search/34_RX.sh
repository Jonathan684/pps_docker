
make clean
make
sshpass -p "analog" scp receive_config.txt root@192.168.1.34:/jonathan/
sshpass -p "analog" scp 2_receive_pulse root@192.168.1.34:/jonathan/
sshpass -p "analog" ssh -t root@192.168.1.34 "cd /jonathan/; exec ./2_receive_pulse"
sshpass -p "analog" scp  root@192.168.1.34:/jonathan/output_pulse.txt ../pps_project