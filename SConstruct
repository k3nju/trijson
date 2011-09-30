#! /usr/bin/python
# -*- coding:utf-8 -*-

import os;

CXXFLAGS = [
	"-g",
	"-O2",
	"-fpermissive",
	"-Wpointer-arith",
	"-pedantic",
	"-Wall",
	"-Wno-variadic-macros",
	"-std=c++0x",
	];

LIBS = [ "gtest", "pthread" ]

TOP_DIR = os.getcwd() + "/";
Export( "TOP_DIR" );

CXXFLAGS.append( "-I" + TOP_DIR + "src/" );

ENV = Environment( CXXFLAGS = CXXFLAGS, LIBS=LIBS );
Export( "ENV" );

#ENV.SConscript( dirs = "./src" );
ENV.SConscript( dirs = "./test", variant_dir = "build/" );


# EOF
