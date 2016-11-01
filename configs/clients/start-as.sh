#!/bin/bash

SERVER=192.168.25.10

# Wait until the desktop is fully loaded
sleep 5
cd /home/pi/Desktop/

# Disable the screensaver and power saving option
xset s off
xset -dpms

# Start the client script
echo "Starting AdmissionSurveyClient script..."

# If the directory doesn't exist, we create the directory
if [ ! -d "AdmissionSurvey" ]; then
        echo "Creating AdmissionSurvey directory..."
        mkdir AdmissionSurvey
fi

# We mount the directory hosted on the server as "RemoteVersion" which contains the latest client files
mkdir RemoteVersion 2> /dev/null
sudo mount -o intr,soft,timeo=5,retrans=1,retry=1 $SERVER:/home/pi/Desktop/AdmissionSurveyClient RemoteVersion

echo "Remote repository mounted..."

# We check the remote version is newer than the current version
localVersion=0
if [ -e "AdmissionSurvey/version" ]; then
        localVersion=`cat AdmissionSurvey/version`
fi

remoteVersion=0
if [ -e "RemoteVersion/version" ]; then
        remoteVersion=`cat RemoteVersion/version`
fi

echo "Local version : $localVersion | RemoteVersion $remoteVersion"

if [ "$remoteVersion" -gt "$localVersion" ]; then
        echo "Performing update..."

        rm -rf AdmissionSurvey/*
        cp -R RemoteVersion/* AdmissionSurvey/
        chmod +x AdmissionSurvey/AdmissionSurvey

        if [ -e AdmissionSurvey/update.sh ]; then
                echo "Running update.sh..."

                chmod +x AdmissionSurvey/update.sh
                AdmissionSurvey/update.sh
                rm AdmissionSurvey/update.sh
        fi

        echo "Client updated"
fi

sudo umount RemoteVersion
rmdir RemoteVersion

echo "Remote repository unmounted..."

# We start the client
echo "Starting the client..."

cd AdmissionSurvey
./AdmissionSurvey survey.xml $SERVER
