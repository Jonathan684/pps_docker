file_path="output_pulse.txt"  # Ruta al archivo que deseas verificar y eliminar
make clean
if [ -f "$file_path" ]; then
    echo "El archivo existe. Eliminando..."
    rm "$file_path"
    echo "Archivo eliminado."
else
    echo "El archivo no existe."
fi

make
sshpass -p "analog" scp receive_pulse root@192.168.1.37:/jonathan/
sshpass -p "analog" ssh -t root@192.168.1.37 "cd /jonathan/; exec ./receive_pulse"
sshpass -p "analog" scp  root@192.168.1.37:/jonathan/output_pulse.txt ../pulse_generator
