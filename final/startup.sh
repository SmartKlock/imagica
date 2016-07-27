#sleep 20
echo "ifconfig"
#ifconfig
echo "iwlist wlan0 scan | grep SSID"
#iwlist wlan0 scan | grep SSID
echo "checking connection"
#cp /boot/wifi/wpa_supplicant.back /boot/wifi/wpa_supplicant.conf
connection=$(echo $(iwgetid) "" | cut -d '"' -f 2)
echo $connection
if [ ! -z "$connection" ]; then
	echo "remaining on connection";
else
	echo "switching to hotspot";
	sudo cp /etc/network/interfaces /etc/network/interfaces.back;
	sudo cp /etc/network/interfaces.hot /etc/network/interfaces;
	ifdown wlan0;
	ifup wlan0;
	sudo service hostapd start;
	sudo service udhcpd start;
	sudo cp /etc/network/interfaces.back /etc/network/interfaces;
fi

