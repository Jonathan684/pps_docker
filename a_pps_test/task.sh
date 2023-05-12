echo "====================== compile ======================"
make clean
#sleep 0.5
make
sshpass -p "analog" scp sdr_transmitter root@192.168.1.37:/tmp/
sshpass -p "analog" ssh -t root@192.168.1.37 /tmp/sdr_transmitter
echo "====================== finish ======================"