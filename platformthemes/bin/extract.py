#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# ============================================================================
#  Name        : extract.py
#  Part of     : Hb
#  Description : Hb themes script for extracting theme archives
#  Version     : %version: %
#
#  Copyright (c) 2008-2010 Nokia.  All rights reserved.
#  This material, including documentation and any related computer
#  programs, is protected by copyright controlled by Nokia.  All
#  rights are reserved.  Copying, including reproducing, storing,
#  adapting or translating, any or all of this material requires the
#  prior written consent of Nokia.  This material also contains
#  confidential information which may not be disclosed to others
#  without the prior written consent of Nokia.
# ============================================================================

import os
import re
import sys
import shutil
import zipfile
import fnmatch
import optparse

# ============================================================================
# Globals
# ============================================================================
VERBOSE = False
INPUT_DIR = os.getcwd()
OUTPUT_DIR = os.getcwd()
INCLUDE = None
EXCLUDE = None
CLEAN = False

# ============================================================================
# OptionParser
# ============================================================================
class OptionParser(optparse.OptionParser):
    def __init__(self):
        optparse.OptionParser.__init__(self)
        self.add_option("-v", "--verbose", action="store_true", dest="verbose",
                        help="print verbose information about each step of the sync process")
        self.add_option("-q", "--quiet", action="store_false", dest="verbose",
                        help="do not print information about each step of the sync process")
        self.add_option("-i", "--input", dest="input", metavar="dir",
                        help="specify the input <dir> (default %s)" % INPUT_DIR)
        self.add_option("-o", "--output", dest="output", metavar="dir",
                        help="specify the output <dir> (default %s)" % OUTPUT_DIR)
        self.add_option("--include", dest="include", action="append", metavar="pattern",
                        help="specify the include <pattern> (default %s)" % INCLUDE)
        self.add_option("--exclude", dest="exclude", action="append", metavar="pattern",
                        help="specify the exclude <pattern> (default %s)" % EXCLUDE)
        self.add_option("--clean", dest="clean", action="store_true",
                        help="clean intermediate files in the output directory")

# ============================================================================
# Utils
# ============================================================================
if not hasattr(os.path, "relpath"):
    def relpath(path, start=os.curdir):
        abspath = os.path.abspath(path)
        absstart = os.path.abspath(start)
        if abspath == absstart:
            return "."
        i = len(absstart)
        if not absstart.endswith(os.path.sep):
            i += len(os.path.sep)
        if not abspath.startswith(absstart):
            i = 0
        return abspath[i:]
    os.path.relpath = relpath

def extract(archive):
    global INPUT_DIR, OUTPUT_DIR

    path, filename = os.path.split(archive)
    relpath = os.path.relpath(path, INPUT_DIR)
    outpath = os.path.join(OUTPUT_DIR, relpath)

    # extract
    zip = zipfile.ZipFile(archive)
    for name in zip.namelist():
        data = zip.read(name)
        outfile = os.path.join(outpath, name)
        # overwrite only if different size
        if not os.path.exists(outfile) or os.path.getsize(outfile) != len(data):
            file = open(outfile, "wb")
            file.write(data)
            file.close()

def include_exclude(filepath):
    global INCLUDE, EXCLUDE
    result = True
    if INCLUDE != None:
        for pattern in INCLUDE:
            if not fnmatch.fnmatch(filepath, pattern):
                result = False
    if EXCLUDE != None:
        for pattern in EXCLUDE:
            if fnmatch.fnmatch(filepath, pattern):
                result = False
    return result

# ============================================================================
# main()
# ============================================================================
def main():
    global VERBOSE, INPUT_DIR, OUTPUT_DIR, INCLUDE, EXCLUDE, CLEAN

    parser = OptionParser()
    (options, args) = parser.parse_args()

    if options.verbose != None:
        VERBOSE = options.verbose
    if options.input != None:
        INPUT_DIR = options.input
    if options.output != None:
        OUTPUT_DIR = options.output
    if options.include != None:
        INCLUDE = options.include
    if options.exclude != None:
        EXCLUDE = options.exclude
    if options.clean != None:
        CLEAN = options.clean

    extracted = 0
    copied = 0
    omitted = 0
    workpath = None
    newline = False
    if CLEAN:
        shutil.rmtree(OUTPUT_DIR, ignore_errors=True)
    sys.stdout.write("Processing: ")
    for root, dirs, files in os.walk(INPUT_DIR):
        for file in files:
            filepath = os.path.join(root, file)
            extension = os.path.splitext(file)[1]
            if include_exclude(filepath):
                # ensure that output dir exists
                relfilepath = os.path.relpath(filepath, INPUT_DIR)
                relpath = os.path.split(relfilepath)[0]
                outpath = os.path.join(OUTPUT_DIR, relpath)
                if not os.path.exists(outpath):
                    os.makedirs(outpath)

                # output processing dir info
                tmppath = os.path.split(filepath)[0]
                if tmppath != workpath:
                    if workpath != None:
                        newline = True
                        sys.stdout.write("\n            ")
                    workpath = tmppath
                    sys.stdout.write(os.path.relpath(workpath, INPUT_DIR).replace("\\", "/"))
                    sys.stdout.write(".")
                else:
                    sys.stdout.write(".")

                # extract zips
                if extension == ".zip":
                    extracted += 1
                    extract(filepath)
                # copy others
                else:
                    copied += 1
                    shutil.copy(filepath, outpath)
            else:
                omitted += 1
    if newline:
        sys.stdout.write("\n")
    print "        ==> %s archives(s) extracted" % extracted
    print "        ==> %s file(s) copied" % copied
    print "        ==> %s file(s) omitted" % omitted

    return 0

if __name__ == "__main__":
    sys.exit(main())
