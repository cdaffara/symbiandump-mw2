#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# ============================================================================
#  Name        : rom.py
#  Part of     : Hb
#  Description : Hb themes script for generating IBY files
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
import fnmatch
import zipfile
import optparse
import posixpath
import ConfigParser

# ============================================================================
# Globals
# ============================================================================
VERBOSE = False
INCLUDE = None
EXCLUDE = None
INPUT_DIR = os.getcwd()
OUTPUT_DIR = os.getcwd()
SOURCE_PREFIX = "ZRESOURCE/hb/themes"
TARGET_PREFIX = "RESOURCE_FILES_DIR/hb/themes"
EXIT_STATUS = 0

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

        group = optparse.OptionGroup(self, "Input/output options")
        group.add_option("-i", "--input", dest="input", metavar="dir",
                         help="specify the input <dir> (default %s)" % INPUT_DIR)
        group.add_option("-o", "--output", dest="output", metavar="dir",
                         help="specify the output <dir> (default %s)" % OUTPUT_DIR)
        group.add_option("--include", dest="include", action="append", metavar="pattern",
                         help="specify the include <pattern> (default %s)" % INCLUDE)
        group.add_option("--exclude", dest="exclude", action="append", metavar="pattern",
                         help="specify the exclude <pattern> (default %s)" % EXCLUDE)
        self.add_option_group(group)

        group = optparse.OptionGroup(self, "Prefix options")
        group.add_option("--source-prefix", dest="sourceprefix", metavar="prefix",
                         help="specify the source <prefix> (default %s)" % SOURCE_PREFIX)
        group.add_option("--target-prefix", dest="targetprefix", metavar="prefix",
                         help="specify the target <prefix> (default %s)" % TARGET_PREFIX)
        self.add_option_group(group)

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

def zip_filelist(filepath):
    files = list()
    archive = zipfile.ZipFile(filepath)
    for entry in archive.namelist():
        if not entry.endswith("/"):
            files.append(entry)
    return files

class Theme:
    def __init__(self, name):
        self.name = name
        self.paths = []
        self.files = {}
        self.archives = {}

    def add_file(self, filepath):
        if self._include(filepath):
            filepath = filepath.replace("\\", "/")
            path = os.path.split(filepath)[0]
            extension = os.path.splitext(filepath)[1]
            if extension == ".zip":
                if path not in self.archives:
                    self.archives[path] = list()
                if filepath not in self.archives[path]:
                    self.archives[path].append(filepath)
            else:
                if path not in self.files:
                    self.files[path] = list()
                if filepath not in self.files[path]:
                    self.files[path].append(filepath)

    def initialize(self):
        for path in self.paths:
            for root, dirs, files in os.walk(path):
                for file in files:
                    self.add_file(os.path.join(root, file))

    def write_iby(self, ibypath):
        global SOURCE_PREFIX, TARGET_PREFIX, EXIT_STATUS
        outpath = os.path.dirname(ibypath)
        if not os.path.exists(outpath):
            os.makedirs(outpath)
        out = open(ibypath, "w")
        out.write("#ifndef __%s_IBY__\n" % self.name.upper())
        out.write("#define __%s_IBY__\n" % self.name.upper())
        out.write("\n")
        out.write("#include <bldvariant.hrh>\n")
        out.write("\n")
        out.write("data=%s/%s.themeindex\t%s/%s.themeindex\n" % (SOURCE_PREFIX, self.name, TARGET_PREFIX, self.name))
        written_entries = list()
        for path, files in self.files.iteritems():
            relpath = os.path.relpath(path, INPUT_DIR).replace("\\", "/")
            for filepath in files:
                filename = os.path.basename(filepath)
                entry = posixpath.join(relpath, filename)
                if entry not in written_entries:
                    written_entries.append(filepath)
                    out.write("data=%s/%s\t%s/%s\n" % (SOURCE_PREFIX, entry, TARGET_PREFIX, entry))
                else:
                    print "ERROR: %s duplicate entry %s" % (ibypath, entry)
                    EXIT_STATUS = -1
        for path, archives in self.archives.iteritems():
            relpath = os.path.relpath(path, INPUT_DIR).replace("\\", "/")
            for archive in archives:
                files = zip_filelist(archive)
                for filepath in files:
                    entry = posixpath.join(relpath, filepath)
                    if entry not in written_entries:
                        written_entries.append(entry)
                        out.write("data=%s/%s\t%s/%s\n" % (SOURCE_PREFIX, entry, TARGET_PREFIX, entry))
                    else:
                        print "ERROR: %s duplicate entry %s" % (ibypath, entry)
                        EXIT_STATUS = -1
        out.write("\n")
        out.write("#endif __%s_IBY__\n" % self.name.upper())
        out.close()

    def write_thx(self, thxpath):
        global SOURCE_PREFIX, TARGET_PREFIX, EXIT_STATUS
        outpath = os.path.dirname(thxpath)
        if not os.path.exists(outpath):
            os.makedirs(outpath)
        archive = zipfile.ZipFile(thxpath, "w")
        os.chdir(INPUT_DIR)
        written_entries = list()
        for path, files in self.files.iteritems():
            relpath = os.path.relpath(path, INPUT_DIR).replace("\\", "/")
            for filepath in files:
                filename = os.path.basename(filepath)
                entry = posixpath.join(relpath, filename)
                if entry not in written_entries:
                    written_entries.append(entry)
                    archive.write(entry)
                else:
                    print "ERROR: %s duplicate entry %s" % (thxpath, entry)
                    EXIT_STATUS = -1
        archive.close()

    def hidden(self):
        result = False
        config = ConfigParser.ConfigParser()
        indexthemepath = INPUT_DIR + '/icons/' + self.name + '/index.theme'
        if os.path.exists(indexthemepath):
            config.read(indexthemepath)
            result = config.getboolean('Icon Theme', 'Hidden')
        return result

    def _include(self, filepath):
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

def lookup_themes(path):
    themes = {}
    if os.path.exists(path):
        # base: effects, icons...
        for base in os.listdir(path):
            basepath = posixpath.join(path, base)
            if os.path.isdir(basepath):
                # theme: footheme, bartheme...
                for theme in os.listdir(basepath):
                    themepath = posixpath.join(basepath, theme)
                    if os.path.isdir(themepath):
                        if theme not in themes:
                            themes[theme] = Theme(theme)
                        themes[theme].paths.append(themepath)
                        # special case: themeindex
                        themeindex = os.path.join(path, theme + ".themeindex")
                        if os.path.exists(themeindex):
                            themes[theme].add_file(themeindex)
    return themes

# ============================================================================
# main()
# ============================================================================
def main():
    global VERBOSE, INPUT_DIR, OUTPUT_DIR, INCLUDE, EXCLUDE, SOURCE_PREFIX, TARGET_PREFIX

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
    if options.sourceprefix != None:
        SOURCE_PREFIX = options.sourceprefix
    if options.targetprefix != None:
        TARGET_PREFIX = options.targetprefix

    themes = lookup_themes(INPUT_DIR)
    for name, theme in themes.iteritems():
        theme.initialize()
        if theme.hidden():
            print "Generating: %s.iby" % name
            theme.write_iby(posixpath.join(OUTPUT_DIR, "%s.iby" % name))
        else:
            print "Generating: %s.thx" % name
            theme.write_thx(posixpath.join(OUTPUT_DIR, "%s.thx" % name))

    return EXIT_STATUS

if __name__ == "__main__":
    sys.exit(main())
