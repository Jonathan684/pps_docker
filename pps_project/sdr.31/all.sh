echo "====================================================="
echo "                | MAKEFILE TX 31|                    "
echo "                ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                    "
make clean
sleep 0.5
make
echo "====================================================="

#echo "====================================================="
# CARPETA DE TRABAJO /Jonathan
sshpass -p "analog" scp transmition_config.txt root@192.168.1.31:/jonathan/
sshpass -p "analog" scp tx_rx_pulse root@192.168.1.31:/jonathan/
sshpass -p "analog" ssh -t root@192.168.1.31  "cd /jonathan/; exec \$SHELL --login"
#ingresar a la sdr 31
#sshpass -p "analog" ssh -l root 192.168.1.31