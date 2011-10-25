#! /usr/bin/python
# -*- coding:utf-8 -*-

from struct import *;
from json import *;

def write( filename, value ):
	open( filename, "wb" ).write( value );

# 1 byte
write( "./utf8_1.raw", "\x7f" );
write( "./utf8_1.enc", "\"\\u007f\"" );

# 2 bytes
write( "./utf8_2.raw", "\x03\xff" );
write( "./utf8_2.enc", "\"\\u03ff\"" );

# 3 bytes
write( "./utf8_3.raw", "\xff\xff" );
write( "./utf8_3.enc", "\"\\uffff\"" );
