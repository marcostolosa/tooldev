import praw
import requests
import getpass
import datetime
import sys
import os

client_key = ''
client_secret = ''
username = ''
password = getpass.getpass()
useragent = ''
op_dir = './downloads/'


reddit = praw.Reddit(client_id = client_key,
					 client_secret = client_secret, 
					 username = username,
					 password = password,
					 user_agent = useragent)

#print(reddit.user.me())


num_posts = 1
if len(sys.argv) < 2:
	print('# posts not entered. Defaulting to 1 post per sub')

else:
	try:
		num_posts = int(sys.argv[1])
		if num_posts < 0:
			print('-ve entered. Defaulting to 1 post per sub')
			num_posts = 1
	except ValueError:
		print('Enter integer. Defaulting to 1 post per sub.')



subs = ['ProgrammerHumor']


try:
	for sub in subs:
		ctr = 0
		print('\nDOWNLOADING FROM r/', sub)
		posts = reddit.subreddit(sub).new(limit=num_posts)
		for post in posts:
			if post.stickied:
				continue
			#print(dir(post))
			if post.url.split('.')[-1] == 'jpg':
				ctr += 1
				print(ctr, ' ', post.url)
				r = requests.get(post.url)
				if r.status_code == 200:
					randstr = datetime.datetime.now().strftime('%Y%m%d%H%M%s')
					with open(op_dir + randstr + post.url.split('/')[-1], 'wb') as f:
						f.write(r.content)
except Exception as e:
	print('Invalid creds - ', e)

# need to have 'fdupes' installed
print('\nNow removing duplicates ...')
os.system('fdupes -d -N %s' % op_dir)
print('Done')


sys.exit(0)
