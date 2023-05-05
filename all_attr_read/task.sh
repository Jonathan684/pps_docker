echo "====================== compile ======================"
make clean
sleep 0.5
make
echo "====================== finish ======================"
scp all_attr_read root@192.168.1.37:/tmp/
ssh -t root@192.168.1.37 /tmp/all_attr_read
scp  root@192.168.1.37:/root/output.txt ../all_attr_read