#touch salida.txt
#iio_info >> salida.txt
#Buscar el archivo.
#touch salida.txt
#iio_info >> salida.txt
sshpass -p "analog" ssh -t root@192.168.1.32 "cd /jonathan/;rm iio_info.txt; touch iio_info.txt ; iio_info >> iio_info.txt"
echo -n > iio_info.txt
sshpass -p "analog" scp  root@192.168.1.32:/jonathan/iio_info.txt ../iio_data