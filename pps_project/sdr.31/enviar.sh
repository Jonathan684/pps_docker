
echo "====================================================="
echo "                | MAKEFILE TX 31|                    "
echo "                ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                    "
make clean
#rm output_pulse.txt
echo -n > output_pulse.txt
make
echo "====================================================="

#echo "====================================================="
# CARPETA DE TRABAJO /Jonathan
#Configuraciones
sshpass -p "analog" scp transmition_config.txt root@192.168.1.31:/jonathan/
sshpass -p "analog" scp receive_config.txt root@192.168.1.31:/jonathan/
#Programa
sshpass -p "analog" scp new root@192.168.1.31:/jonathan/
sshpass -p "analog" ssh -t root@192.168.1.31  "cd /jonathan/; exec \$SHELL --login"


#ingresar a la sdr 31
#sshpass -p "analog" ssh -l root 192.168.1.31