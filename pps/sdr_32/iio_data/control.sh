#/sys/bus/iio/devices/iio:device0
echo "=========================================="
sshpass -p "analog" ssh -t root@192.168.1.32 "cd /sys/bus/iio/devices/iio:device0; 
echo -n "out_altvoltage1_TX_LO_frequency-:" ; cat out_altvoltage1_TX_LO_frequency
echo -n "out_altvoltage0_RX_LO_frequency-:" ; cat out_altvoltage0_RX_LO_frequency
echo -n "out_voltage_rf_bandwidth--------:" ; cat out_voltage_rf_bandwidth
echo -n "in_voltage_rf_bandwidth---------:" ; cat in_voltage_rf_bandwidth
echo -n "out_voltage_sampling_frequency--:" ; cat out_voltage_sampling_frequency
echo -n "in_voltage_sampling_frequency---:" ; cat in_voltage_sampling_frequency
echo -n "filter_fir_config---------------:" ; cat filter_fir_config
echo -n "tx_path_rates-------------------:" ; cat tx_path_rates
echo -n "in_voltage0_hardwaregain rx-----:" ; cat in_voltage0_hardwaregain
echo -n "out_voltage0_hardwaregain tx----:" ; cat out_voltage0_hardwaregain
echo -n "in_voltage0_gain_control_mode --:" ; cat in_voltage0_gain_control_mode
echo -n "out_voltage_sampling_frequency_available --:" ; cat out_voltage_sampling_frequency_available
echo -n "in_voltage_sampling_frequency_available --:" ; cat in_voltage_sampling_frequency_available"

echo "=========================================="