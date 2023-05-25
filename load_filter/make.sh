echo "====================================================="
make clean
sleep 0.5
make
echo "====================================================="
sshpass -p "analog" scp filter_jupyter.txt root@192.168.1.37:/tmp/
sshpass -p "analog" scp filter root@192.168.1.37:/tmp/
#sshpass -p "analog" scp 2_sdr root@192.168.1.32:/tmp/
sshpass -p "analog" ssh -l root 192.168.1.37
#sshpass -p "analog" scp filter.txt root@192.168.1.37:/sys/bus/iio/devices/iio\:device0
#/sys/bus/iio/devices/iio\:device0