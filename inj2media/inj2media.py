import sys
import argparse
import random
import string

def get_args():
	text_mode = False
	supported_media = ['jpg', 'png', 'bmp']
	parser = argparse.ArgumentParser()
	parser.add_argument('--file', required=False, dest='filename', help='Path to file containing text to inject')
	parser.add_argument('--text', required=False, dest='text', help='Text to inject')
	parser.add_argument('--ext', required='True', dest='ext', choices=supported_media, type=str, help='Extension')
	args = parser.parse_args()
	
	if not bool(args.filename) ^ bool(args.text):
		print('Args supplied to both file and text, need just 1')
		sys.exit(0)

	if bool(args.text):
		print(args.text, '\nIs this what you want to embed?[y/n]')
		op = input()
		print('\n')
		if str(op).lower() == 'n':
			print('If the quotes are messed up, switch the first and last quotes')
			print('eg. "print(\'pythontest\')"    \'printf("ctest");\'')	
			sys.exit(0)
	
	return args

def getRandomFileName():
	return ''.join(random.choices(string.ascii_uppercase + string.digits, k=10))

def inj2jpg(args):
	rfname = getRandomFileName()
	with open(rfname + '.' + args.ext, 'wb') as f:
		f.write(b'\xff\xd8\x20\x03' + args.text.encode())
	print(rfname + '.' + args.ext, ' was written.')


# messed around with the hex payloads bigtime, figured out the bytes responsible for width, height and bit depth of image
# hardcoded to 256x256, 24-bit depth
# appends the text at end
def inj2png(args):
	rfname = getRandomFileName()
	with open(rfname + '.' + args.ext, 'wb') as f:
		f.write(b'\x89\x50\x4e\x47\x0d\x0a\x1a\x0a\x00\x00\x00\x0d\x49\x48\x44\x52\x00\x00\x01\x00\x00\x00\x01\x00\x18\x45\x4e\x44\xae\x42\x60\x82' + args.text.encode())
	print(rfname + '.' + args.ext, ' was written.')

# messed around with the hex bytes, figured out bytes responsible for height, width, depth, windows formatting version
# hardcoded for 64x64, bit depth = 24, windows format version = 3.x
def inj2bmp(args):
	rfname = getRandomFileName()
	with open(rfname + '.' + args.ext, 'wb') as f:
		f.write(b'\x42\x4d\xe2\x4b\x02\x00\x00\x00\x00\x00\x36\x00\x00\x00\x28\x00\x00\x00\x40\x00\x00\x00\x40\x00\x00\x00\x01\x00\x18\x00' + args.text.encode())
	print(rfname + '.' + args.ext, ' was written')

def inject(args):
	if args.ext == 'jpg':
		return inj2jpg(args)
	elif args.ext == 'png':
		return inj2png(args)
	elif args.ext == 'bmp':
		return inj2bmp(args)
	
def main(args):
	ret_string = inject(args)


if __name__ == '__main__':	
	main(get_args())
	sys.exit(0)
