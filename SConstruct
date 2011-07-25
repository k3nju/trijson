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
	];

TOP_DIR = os.getcwd() + "/";
CXXFLAGS.append( "-I" + TOP_DIR + "src/" );

ENV = Environment( CXXFLAGS = CXXFLAGS );
Export( "ENV" );

ENV.SConscript( dirs = "./src" );
ENV.SConscript( dirs = "./test", variant_dir = "build/" );


# EOF
