echo "====================================================="
echo "                | MAKEFILE TX 35|                    "
echo "                ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                    "

cd ../sources
make clean
if ! make; then
    echo "Make failed. Exiting the script."
    exit 1
fi

echo "====================================================="
cd ../output
echo -n > datos.txt
#echo "====================================================="
cd ../run
# CARPETA DE TRABAJO /Jonathan
sshpass -p "analog" scp ../filters/filter_1.txt root@192.168.1.35:/jonathan/
sshpass -p "analog" scp ../filters/filter_2.txt root@192.168.1.35:/jonathan/
sshpass -p "analog" scp ../filters/filter_3.txt root@192.168.1.35:/jonathan/
sshpass -p "analog" scp ../filters/filter_4.txt root@192.168.1.35:/jonathan/
sshpass -p "analog" scp ../output/datos.txt root@192.168.1.35:/jonathan/

sshpass -p "analog" scp ../configuration/tx_config.txt root@192.168.1.35:/jonathan/
sshpass -p "analog" scp ../configuration/rx_config.txt root@192.168.1.35:/jonathan/

#echo "FIN ENVIO"
sshpass -p "analog" scp ../sources/tx_rx root@192.168.1.35:/jonathan/
echo "FIN ENVIO"
sshpass -p "analog" ssh -t root@192.168.1.35 "cd /jonathan/; exec ./tx_rx"
sshpass -p "analog" scp  root@192.168.1.35:/jonathan/datos.txt ../output
echo "FIN SIMULACION"




#ejecutar
#sshpass -p "analog" ssh -t root@192.168.1.31 "cd /jonathan/; exec ./2_receive_pulse"
#buscar
#sshpass -p "analog" scp  root@192.168.1.31:/jonathan/output_pulse.txt ../new_idea