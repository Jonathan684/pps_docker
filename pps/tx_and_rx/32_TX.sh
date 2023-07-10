echo "====================================================="
echo "                | MAKEFILE TX 32|                    "
echo "                ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                    "
make clean
sleep 0.5

if ! make; then
    echo "Make failed. Exiting the script."
    exit 1
fi

echo "====================================================="
#echo "====================================================="
# CARPETA DE TRABAJO /Jonathan

sshpass -p "analog" scp prueba_tx root@192.168.1.32:/jonathan/
sshpass -p "analog" ssh -t root@192.168.1.32  "cd /jonathan/; exec \$SHELL --login"
echo "FIN SIMULACION"
