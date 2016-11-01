#!/bin/bash

# One argument needed : the device
if [ $# != 1 ] ;then
	echo "[ERROR] Device is missing"
	exit 1
fi

# The provided device must exist
if ! [ -e "/dev/$1" ]; then
	echo "[ERROR] /dev/$1 not found !"
	exit 1
fi

# Global vars
DB_PATH="/home/pi/Desktop/AdmissionSurveyServer/"
DB_SEAMLESSCARE="results_seamlesscare.csv"
DB_TABAC="results_tabac.csv"
DEST="/media/mount_tmp_$1"

# Some checks
if ! [ -e "$DB_PATH/$DB_SEAMLESSCARE" ]; then
	echo "[ERROR]Seamless care database doesn't exist"
	exit 1
fi

if ! [ -e "$DB_PATH/$DB_TABAC" ]; then
	echo "[ERROR]Tabac database doesn't exist"
	exit 1
fi

# The led is turned on to notify the operator that the process is running
gpio -g write 17 1

# We create the mount point
mkdir "$DEST" 2> /dev/null

# Mounting
mount /dev/$1 "$DEST"

# We create directories
mkdir "$DEST/AdmissionSurvey" 2> /dev/null
mkdir "$DEST/AdmissionSurvey/Seamless care" 2> /dev/null
mkdir "$DEST/AdmissionSurvey/Tabac" 2> /dev/null

# Full backup :
# - remove old backups
# - take the current version as backup
# - backup the current database
rm "$DEST/AdmissionSurvey/Seamless care/$DB_SEAMLESSCARE.bak" 2> /dev/null
mv "$DEST/AdmissionSurvey/Seamless care/$DB_SEAMLESSCARE" "$DEST/AdmissionSurvey/Seamless care/$DB_SEAMLESSCARE.bak" 2> /dev/null
cp "$DB_PATH/$DB_SEAMLESSCARE" "$DEST/AdmissionSurvey/Seamless care/"

rm "$DEST/AdmissionSurvey/Tabac/$DB_TABAC.bak" 2> /dev/null
mv "$DEST/AdmissionSurvey/Tabac/$DB_TABAC" "$DEST/AdmissionSurvey/Tabac/$DB_TABAC.bak" 2> /dev/null
cp "$DB_PATH/$DB_TABAC" "$DEST/AdmissionSurvey/Tabac/"

# Incremential backup :
# - we find the current position
# - we copy all lines after the current position
# - we update the file with the position pointer
curPos=0;
if [ -e "$DEST/AdmissionSurvey/Seamless care/currentpos" ]; then
	curPos=`cat "$DEST/AdmissionSurvey/Seamless care/currentpos"`
	curPos=`expr $curPos + 1`
fi

tail -n +$curPos "$DB_PATH/$DB_SEAMLESSCARE" > "$DEST/AdmissionSurvey/Seamless care/incremental_$DB_SEAMLESSCARE"
wc -l "$DEST/AdmissionSurvey/Seamless care/$DB_SEAMLESSCARE" | awk '{print $1}' > "$DEST/AdmissionSurvey/Seamless care/currentpos"

curPos=0;
if [ -e "$DEST/AdmissionSurvey/Tabac/currentpos" ]; then
	curPos=`cat "$DEST/AdmissionSurvey/Tabac/currentpos"`
	curPos=`expr $curPos + 1`
fi

tail -n +$curPos "$DB_PATH/$DB_TABAC" > "$DEST/AdmissionSurvey/Tabac/incremental_$DB_TABAC"
wc -l "$DEST/AdmissionSurvey/Tabac/$DB_TABAC" | awk '{print $1}' > "$DEST/AdmissionSurvey/Tabac/currentpos"

# Unmount
umount /dev/$1 2> /dev/null

# We wait until the device is free 
while [ $? != 0 ]; do
	sleep 1 # Waiting 1 second before the next try
	
	sync 
	umount /dev/$1
done

# Cleaning 
rmdir "$DEST" 2> /dev/null

# We turn off the light to notify the operator that the backup is ended
gpio -g write 17 0
