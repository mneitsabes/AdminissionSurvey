# Update
apt-get update

# Install packages
apt-get install chkconfig
apt-get install nfs-kernel-server
apt-get install vim
apt-get install qt4-dev-tools

# Download and compile WiringPI 
git clone git://git.drogon.net/wiringPi
cd wiringPi/
git pull origin
./build

# Define the server IP
# 192.168.25.10/24 gw 192.168.25.1

# Create the user "admissionsurvey"
useradd -M -N -s /bin/bash admissionsurvey

# Create the log file
touch /var/log/asserver.log
chown admissionsurvey /var/log/asserver.log

# Copy files
mv asserver /etc/init.d/
mv gpioinit /etc/init.d/
mv AdmissionSurveyCopyResults.sh /usr/local/bin/
mv 50-admission-survey-copy-results.rules /etc/udev/rules.d/

# Edit /etc/hosts for each box
#  127.0.0.1  server

# Edit the hostname
echo "server" > /etc/hostname

# Config NTP to act as server  
# see ntp.conf

# Configure the NFS share
echo "/home/pi/Desktop/AdmissionSurveyClient (ro,async)" >> /etc/exports

# Start on boot
chkconfig asserver on
chkconfig ssh-ondemand on
chkconfig gpioinit on
chkconfig rcpbind on
chkconfig nfs-kernel-server on
chkconfig ntp on

# Reboot
reboot