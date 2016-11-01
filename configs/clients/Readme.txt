# Update
apt-get update

# Install packages
apt-get install chkconfig
apt-get install nfs-kernel-server
apt-get install vim
apt-get install qt4-dev-tools
apt-get install x11-server-utils

# Define the IP address
# 192.168.25.1x/24

# Copy files
cp admissionsurvey.desktop /home/pi/Desktop
cp admissionsurvey.desktop /etc/xdg/autostart
cp start-as.sh /home/pi/Desktop
chmod +x /home/pi/Desktop/start-as.sh
chown pi.pi /home/pi/Desktop/start-as.sh

mkdir /home/pi/Desktop/AdmissionSurvey
chown pi.pi /home/pi/Desktop/AdmissionSurvey

# Edit /etc/hosts for each box
#  127.0.0.1  Box<n>

# Hostname
# /etc/hostname -> box<n>

# Copy the NTP configuration
# see ntp.org

# Start NTP client
chkconfig ntp on

# Reboot
reboot