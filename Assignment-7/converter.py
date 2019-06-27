#!/usr/bin/python

#Filename: converter.py
#Author: Jonathan So
#Student ID: SLAE-1470
#SLAE-Exercise 7

shellcode = ("\x31\xdb\x53\x68\x43\x43\x90\x90\x68\x42\x42\x43\x43\x68\x68\x41\x42\x42\x68\x2f\x62\x61\x73\x68\x2f\x62\x69\x6e\x89\xe6\x88\x5e\x09\x89\x76\x0a\x89\x5e\x0e\x8d\x1e\x8d\x4e\x0a\x8d\x56\x0e\x31\xc0\xb0\x0b\xcd\x80")
encoded = 'unsigned char shellCode[] = {'
padding = ''
print('Encoded shellcode ...')
count = 1

for x in bytearray(shellcode) :
	encoded += '0x'
	encoded += '%02x' %x
	if(x != bytearray(shellcode)[len(bytearray(shellcode))-1]):
		encoded += ','
if len(bytearray(shellcode)) % 8 !=0:
	for i in range(8-(len(bytearray(shellcode)) % 8)):
		padding += ',0x00'

print(encoded + padding + '};')

print('Len: %d' % len(bytearray(shellcode)))
