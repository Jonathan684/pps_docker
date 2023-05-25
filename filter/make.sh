echo "====================================================="
make clean
sleep 0.5
make
echo "====================================================="
#0 Crear un espacio de trabajo llamado jonathan
#Copiar el filter
sshpass -p "analog" scp filter_jupyter.txt root@192.168.1.37:/jonathan/
sshpass -p "analog" scp filter root@192.168.1.37:/jonathan/
#ejecuta
#sshpass -p "analog" ssh -t root@192.168.1.37 /jonathan/filter
sshpass -p "analog" ssh -t root@192.168.1.37 "cd /jonathan/; exec ./filter"
#sshpass -p "analog" ssh -t root@192.168.1.37 /jonathan/receive
#Acceso
#sshpass -p "analog" ssh -t root@192.168.1.37  "cd /jonathan/; exec \$SHELL --login"
## Acceso a device
#sshpass -p "analog" ssh -t root@192.168.1.37  "cd /sys/bus/iio/devices/iio\:device0/; exec \$SHELL --login"
#sshpass -p "analog" ssh -l -t root@192.168.1.37 "cd /jonathan/"

#sshpass -p "analog" scp filter.txt root@192.168.1.37:/sys/bus/iio/devices/iio\:device0
#/sys/bus/iio/devices/iio\:device0
#ssh -t root@192.168.82.209 "cd /data/www/triki/application; exec \$SHELL --login"
#Acceso normal
#ssh -l root 192.168.1.37
make clean