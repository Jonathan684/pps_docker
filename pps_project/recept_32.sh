make clean
make
sshpass -p "analog" scp 2_receive_pulse root@192.168.1.32:/jonathan/
sshpass -p "analog" ssh -t root@192.168.1.32 "cd /jonathan/; exec ./2_receive_pulse"
sshpass -p "analog" scp  root@192.168.1.32:/jonathan/output_pulse.txt ../pps_project

# file_path="output_pulse.txt"  # Ruta al archivo que deseas verificar y eliminar

# if [ -f "$file_path" ]; then
#     echo "El archivo existe. Eliminando..."
#     rm "$file_path"
#     echo "Archivo eliminado."
# else
#     echo "El archivo no existe."
# fi
