#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# ============================================================================
#  Name        : installs.py
#  Part of     : Hb
#  Description : Hb themes script for generating installs.pri
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
import fnmatch
import optparse
import posixpath

# ============================================================================
# Globals
# ============================================================================
INPUT_DIR = os.getcwd()
OUTPUT_DIR = os.getcwd()
INCLUDE = None
EXCLUDE = None

# ============================================================================
# OptionParser
# ============================================================================
class OptionParser(optparse.OptionParser):
    def __init__(self):
        optparse.OptionParser.__init__(self)
        self.add_option("-i", "--input", dest="input", metavar="dir",
                        help="specify the input <dir> (default %s)" % INPUT_DIR)
        self.add_option("-o", "--output", dest="output", metavar="dir",
                        help="specify the output <dir> (default %s)" % OUTPUT_DIR)
        self.add_option("--include", dest="include", action="append", metavar="pattern",
                        help="specify the include <pattern> (default %s)" % INCLUDE)
        self.add_option("--exclude", dest="exclude", action="append", metavar="pattern",
                        help="specify the exclude <pattern> (default %s)" % EXCLUDE)

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

def make_target(path):
    # generate a compatible make target name from path
    target = os.path.splitdrive(path)[1].strip("\\/")
    return "_".join(re.split("[\\\/\.]+", target))

def write_pri(filepath, input_dir):
    outpath = os.path.dirname(filepath)
    if not os.path.exists(outpath):
        os.makedirs(outpath)
    out = open(filepath, "w")

    roots = []
    for root, dirs, files in os.walk(input_dir):
        for file in files:
            filepath = os.path.abspath(root + "/" + file).replace("\\", "/")
            filepath = os.path.splitdrive(filepath)[1]
            if include_exclude(filepath):
                target = make_target(root)
                relpath = os.path.relpath(root, input_dir).replace("\\", "/")
                if os.path.splitext(file)[1] == ".zip":
                    out.write("symbian:BLD_INF_RULES.prj_exports += \":zip %s $${EPOCROOT}epoc32/data/z/resource/hb/themes/%s/\"\n" % (filepath, relpath))
                    out.write("symbian:BLD_INF_RULES.prj_exports += \":zip %s $${EPOCROOT}epoc32/winscw/c/resource/hb/themes/%s/\"\n" % (filepath, relpath))
                    out.write("!isEmpty(%s.commands): %s.commands += &&\n" % (target, target))
                    out.write("%s.commands += $$QMAKE_UNZIP %s -d $$(HB_THEMES_DIR)/themes/%s\n" % (target, filepath, relpath))
                else:
                    out.write("symbian:BLD_INF_RULES.prj_exports += \"%s $${EPOCROOT}epoc32/data/z/resource/hb/themes/%s/\"\n" % (filepath, relpath))
                    out.write("symbian:BLD_INF_RULES.prj_exports += \"%s $${EPOCROOT}epoc32/winscw/c/resource/hb/themes/%s/\"\n" % (filepath, relpath))
                    out.write("%s.files += %s\n" % (target, filepath))
                if root not in roots:
                    out.write("%s.CONFIG += no_build\n" % target)
                    out.write("%s.path = $$(HB_THEMES_DIR)/themes/%s\n" % (target, relpath))
                    out.write("INSTALLS += %s\n" % target)
                    roots.append(root)
                out.write("QMAKE_CLEAN += %s\n" % filepath)
    out.close()
    return 0

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
    global INPUT_DIR, OUTPUT_DIR, INCLUDE, EXCLUDE

    parser = OptionParser()
    (options, args) = parser.parse_args()

    if options.input != None:
        INPUT_DIR = options.input
    if options.output != None:
        OUTPUT_DIR = options.output
    if options.include != None:
        INCLUDE = options.include
    if options.exclude != None:
        EXCLUDE = options.exclude

    print "Generating: %s/installs.pri" % os.path.basename(OUTPUT_DIR)
    return write_pri(OUTPUT_DIR + "/installs.pri", INPUT_DIR)

if __name__ == "__main__":
    sys.exit(main())
