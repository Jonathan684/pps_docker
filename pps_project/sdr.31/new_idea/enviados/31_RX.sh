make clean
make
sshpass -p "analog" scp receive_config.txt root@192.168.1.31:/jonathan/
sshpass -p "analog" scp 2_receive_pulse root@192.168.1.31:/jonathan/
#ejecutar
sshpass -p "analog" ssh -t root@192.168.1.31 "cd /jonathan/; exec ./2_receive_pulse"
#buscar
sshpass -p "analog" scp  root@192.168.1.31:/jonathan/output_pulse.txt ../new_idea