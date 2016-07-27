PWD2=$PWD
TrackLocation=/usr/src/TrackSensor
echo "stopping currently running server"
sudo service TrackSensor stop
echo "removing old Settings";
rm -r $TrackLocation
echo "creating source location in /usr/src/TrackSensor";
mkdir $TrackLocation
echo "moving files to source location"
cp final/socket_server.c $TrackLocation/
cp final/socket_client.c $TrackLocation/
cp final/maker.sh $TrackLocation/
cp final/TrackSensor $TrackLocation/
cd $TrackLocation/
echo "Compiling program"
./maker.sh
echo "removing sources";
rm socket_client.c
rm socket_server.c
rm maker.sh
echo "removing old links";
rm -r /etc/rc*.d/K*TrackSensor
rm -r /etc/rc*.d/S*TrackSensor
rm /etc/init.d/TrackSensor
echo "creating new links";
cp TrackSensor /etc/init.d/TrackSensor
for i in 0 1 2 3 4 5 6 S
do
	ln -s TrackSensor /etc/rc$i.d/K99TrackSensor
done
echo "updating rc*.d location"
update-rc.d TrackSensor enable
sudo service TrackSensor start
