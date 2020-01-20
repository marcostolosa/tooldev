A collection of scripts I use on a daily basis for basic automation and maintenance.

Some scripts have been added to the root crontab. eg. backup.sh requires root priveleges to run, I have it set up to run at 8:00 p.m on every second day.

sudo crontab -e (in file): 0 20 */2 * * /path/to/backup.sh ( Mount point - /media/SSDBackup has to be explicitly created )
