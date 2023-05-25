echo "====================================================="
make clean
sleep 0.5
make
echo "====================================================="
sshpass -p "analog" scp transmitter root@192.168.1.37:/tmp/
#sshpass -p "analog" scp 2_sdr root@192.168.1.32:/tmp/
sshpass -p "analog" ssh -l root 192.168.1.37