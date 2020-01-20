from bs4 import BeautifulSoup
import requests
import socket

res = BeautifulSoup(requests.get('https://www.whatsmyip.com').text, 'lxml')
print('Live IP addr: ', res.find_all('p')[0].getText())

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.connect(('8.8.8.8', 80))
print('Local IP on machine: ', s.getsockname()[0])
s.close()
