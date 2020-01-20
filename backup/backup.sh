#!/bin/bash

# script to mount hard disk and copy contents of SSD onto the backup disk
# run manually daily to create backups, create folders by date
# run 'sudo fdisk -l' this shows the drives in the machine
# the backup drive is named /dev/sdb with a filesystem on /dev/sdb1  
# mount this to /media/SSDBackup
# create a new folder in drive whose name is - current time
# copy contents
# - Anish Sujanani (asi7296)

echo "Running ..."

# if not already mounted, mount drive here
# mount disk onto /media/SSDBackup
if ! grep -qs '/media/SSDBackup' /proc/mounts; then
echo 'Drive not mounted, mounting /dev/sdb1 now'
mount /dev/sdb1 /media/SSDBackup
fi

echo '/dev/sdb1 mounted'
cd /media/SSDBackup
# create folder named dd-mm-yy
folder_name=`date +%d-%m-%y`
mkdir $folder_name
chmod 777 $folder_name

# copy contents
echo 'copying Desktop'
cp -R /home/asi/Desktop $folder_name && cd $folder_name/Desktop && tar -czf ../Desktop.tar.gz * && rm -r ../Desktop &
echo 'copying Documents'
cp -R /home/asi/Documents $folder_name && cd $folder_name/Documents && tar -czf ../Documents.tar.gz * && rm -r ../Documents &
echo 'copying Pictures'
cp -R /home/asi/Pictures $folder_name && cd $folder_name/Pictures && tar -czf ../Pictures.tar.gz * && rm -r ../Pictures &
echo 'copying var/html'
cp -R /var/www/html $folder_name && cd $folder_name/html && tar -czf ../html.tar.gz * && rm -r ../html &
echo 'copying Projects'
cp -R /home/asi/Projects $folder_name && cd $folder_name/Projects && tar -czf ../Projects.tar.gz * && rm -r ../Projects &
echo 'copying Code'
cp -R /home/asi/Code $folder_name && cd $folder_name/Code && tar -czf ../Code.tar.gz * && rm -r ../Code &
#echo 'copying Datasets'
#cp -R /home/asi/datasets $folder_name && cd $folder_name/datasets && tar -czf ../datasets.tar.gz * && rm -r ../datasets &

wait
# change permissions for all copied contents

chmod 777 -R $folder_name 

echo 'Compressing files ... '
tar -czf $folder_name.tar.gz $folder_name &
wait
rm -r $folder_name &
wait

# unmount -lazy HDD before exiting to prevent data corruption
umount -l /media/SSDBackup
echo "Drive has been unmounted."
