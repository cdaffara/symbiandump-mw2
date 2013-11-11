#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# ============================================================================
#  Name        : svg2nvg.py
#  Part of     : Hb
#  Description : Hb themes script for converting SVG to NVG
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
import sys
import optparse

# ============================================================================
# Globals
# ============================================================================
VERBOSE = False
ENCODER = "svgtbinencode"
INPUT_DIR = os.getcwd()

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
        self.add_option("-e", "--encoder", dest="encoder", metavar="encoder",
                        help="specify the encoder (default %s)" % ENCODER)
        self.add_option("-i", "--input", dest="input", metavar="dir",
                        help="specify the input <dir> (default %s)" % INPUT_DIR)

# ============================================================================
# Functions
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

def _print_summary(succeed, failed, omitted):
    print "        ==> %s file(s) successfully converted" % succeed
    print "        ==> %s file(s) failed to convert" % failed
    print "        ==> %s file(s) omitted" % omitted

def encode(path):
    global VERBOSE, INPUT_DIR, ENCODER

    failed = 0
    succeed = 0
    omitted = 0
    workpath = None
    newline = False

    for root, dirs, files in os.walk(path):
        i = 0
        for file in files:
            filepath = os.path.join(root, file)
            basepath, extension = os.path.splitext(filepath)

            if extension == ".svg":
                tmppath = os.path.split(filepath)[0]
                if tmppath != workpath:
                    if workpath != None:
                        newline = True
                        sys.stdout.write("\n")
                        _print_summary(succeed, failed, omitted)
                    failed = 0
                    succeed = 0
                    omitted = 0
                    workpath = tmppath
                    sys.stdout.write("Converting: %s" % os.path.relpath(workpath, INPUT_DIR).replace("\\", "/"))
                else:
                    i += 1
                    if i % 10 == 0:
                        sys.stdout.write(".")

                command = "%s -v 6 -e .nvg %s" % (ENCODER, filepath)
                os.system(command)
                exists = os.path.exists(basepath + ".nvg")
                if exists:
                    # cleanup conversion source (.svg)
                    os.remove(filepath)
                    succeed += 1
                else:
                    failed += 1
                if VERBOSE:
                    print "            %s: %s" % (file, exists)
            elif extension == ".nvg":
                # cleanup, from previous conversion
                os.remove(filepath)
            else:
                omitted += 1

    if newline:
        sys.stdout.write("\n")
    _print_summary(succeed, failed, omitted)

# ============================================================================
# main()
# ============================================================================
def main():
    global VERBOSE, ENCODER, INPUT_DIR

    parser = OptionParser()
    (options, args) = parser.parse_args()

    if options.verbose != None:
        VERBOSE = options.verbose
    if options.encoder != None:
        ENCODER = options.encoder
    if options.input != None:
        INPUT_DIR = options.input

    encode(INPUT_DIR)

    return 0

if __name__ == "__main__":
    sys.exit(main())
