echo "====================== compile ======================"
make clean
#sleep 0.5
make
sshpass -p "analog" scp all_attr_read root@192.168.1.37:/tmp/
sshpass -p "analog" ssh -t root@192.168.1.37 /tmp/all_attr_read
sshpass -p "analog" scp root@192.168.1.37:/root/output.txt ../all_attr_read
echo "====================== finish ======================"