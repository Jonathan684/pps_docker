
echo "====================================================="
echo "                | MAKEFILE TX 33|                    "
echo "                ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                    "
make clean
rm output_pulse.txt
make
echo "====================================================="

#echo "====================================================="
# CARPETA DE TRABAJO /Jonathan
sshpass -p "analog" scp transmition_config.txt root@192.168.1.33:/jonathan/
sshpass -p "analog" scp tx_rx_pulse root@192.168.1.33:/jonathan/

sshpass -p "analog" ssh -t root@192.168.1.33  "cd /jonathan/; exec \$SHELL --login"
#ingresar a la sdr 31
#sshpass -p "analog" ssh -l root 192.168.1.31