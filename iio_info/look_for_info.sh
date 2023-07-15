#touch salida.txt
#iio_info >> salida.txt
#Buscar el archivo.
#touch salida.txt
#iio_info >> salida.txt
sshpass -p "analog" ssh -t root@192.168.1.32 "cd /jonathan/; touch salida.txt ; iio_info >> salida.txt"
sshpass -p "analog" scp  root@192.168.1.32:/jonathan/salida.txt ../iio_info