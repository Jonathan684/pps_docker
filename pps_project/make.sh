echo "====================================================="
make clean
sleep 0.5
make
echo "====================================================="
sshpass -p "analog" scp 1_sdr root@192.168.1.37:/tmp/
sshpass -p "analog" ssh -l root 192.168.1.37