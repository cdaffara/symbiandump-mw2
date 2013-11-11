#
# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
# 
# Description:
# 
#


# wrap mifconv to figure out all the inputs and options
#
# when run with --make=BITMAP all the files are generated.
# when run with --make=CLEAN the temporary files are removed
#                                (Raptor removes the releasables itself).

# a list of releasable files is written to the file passed as --manifest
# this file is guaranteed to be in a unique directory for each component,
# so the directory can be used for intermediate build files (SVGB files
# and the mifconv command file, for example).

import optparse
import os
import shutil
import subprocess
import sys

parser = optparse.OptionParser()
	  
parser.add_option("--debug", default="")
parser.add_option("--epocroot", default=".")
parser.add_option("--extroot", default=".")
parser.add_option("--header", default="")
parser.add_option("--make", default="BITMAP")
parser.add_option("--manifest", default="")
parser.add_option("--platforms", default="ARMV5 urel")
parser.add_option("--sourcedir", default=".")
parser.add_option("--sourcefile", default="")
parser.add_option("--sources", default="")
parser.add_option("--svgversion", default="")
parser.add_option("--target", default="")

# parse the command-line arguments
(options, leftover_args) = parser.parse_args()

# there should not be any leftover_args
for leftover in leftover_args:
	sys.stderr.write("warning: unexpected argument '%s'\n" % leftover)

# keep track of all the releasable output files
releasables = []
# and which files need to be copied to all the target directories
copiers = []

# work out the absolute paths of our main target (the MIF file)
if not options.target:
	sys.stderr.write("error: no target defined")
	sys.exit(1)

# the output directory(s) depends on both the platforms passed in and on the
# actual target name. if the target name is an absolute path then it is used
# as-is: otherwise if the target name contains a "/" then the outputs
# are actually constructed in the source tree (why?): and if the target name
# is simply a filename then it is relative to a platform-specific directory.
if "/" in options.target:
	if os.path.isabs(options.target):
		parts = os.path.split(options.target)
		targetdirs = [parts[0]]
		options.target = parts[1]
	else:
		targetdirs = [options.extroot]
else:
	targetdirs = []
	if "ARMV5" in options.platforms:
		targetdirs.append(options.epocroot + "/epoc32/data/z/resource/apps")
	if "WINSCW urel" in options.platforms:
		targetdirs.append(options.epocroot + "/epoc32/release/winscw/urel/z/resource/apps")
	if "WINSCW udeb" in options.platforms:
		targetdirs.append(options.epocroot + "/epoc32/release/winscw/udeb/z/resource/apps")

if not targetdirs:
	sys.stderr.write("error: no known platforms given (ARMV5, WINSCW urel, WINSCW udeb)\n")
	sys.exit(1)
	
# mifconv ignores the target extension and adds .mif anyway, so we need
# to do the same here otherwise the releasables will be reported wrongly.
targetname = os.path.splitext(options.target)[0] + ".mif"

for dir in targetdirs:
	targetpath = dir + "/" + targetname
	releasables.append(targetpath)
	
	if options.debug:
		print "target path =", targetpath

copiers.append(targetdirs[0] + "/" + targetname)

# if we are generating a header (MBG file) then work out its path.
# again if the header name is not an absolute path but contains a "/"
# then the output is actually written in the source tree (why?)
if options.header:
	if "/" in options.header:
		if os.path.isabs(options.header):
			headerpath = options.header
		else:
			headerpath = options.extroot + "/" + options.header
	else:
		headerpath = options.epocroot + "/epoc32/include/" + options.header
	releasables.append(headerpath)
	
if options.debug and options.header:
	print "header path =", headerpath

# gather the list of source names
sources = []

def parse_sources(str, list):
	# split the string into (arg, filename) tuples and add to the list
	items = str.split()
	list.extend(zip(items[::2], items[1::2]))

if options.sources:
	parse_sources(options.sources, sources)
	
if options.sourcefile:
	sourcefile = options.extroot + "/" + options.sourcefile
	try:
		file = open(sourcefile, "r")
		for line in file:
			parse_sources(line, sources)
		file.close()
	except Exception, ex:
		sys.stderr.write("error: %s\n" % str(ex))
		sys.exit(1)
		
if not sources:
	sys.stderr.write("error: no source files specified\n")
	sys.exit(1)
	
if options.debug:
	for s in sources:
		print "source =", s

# take the source names and search for the actual file paths.
# the search list is an ordered sequence of (directory, extension) pairs.
# the first file found is the one used for each source name.

img = []    # everything, to retain the ordering
bmp = []
svg = []

anyextsourcedir = options.extroot + "/" + options.sourcedir
bitmapsourcedir = options.epocroot + "/epoc32/s60/bitmaps"
vectorsourcedir = options.epocroot + "/epoc32/s60/icons"

search = [ (anyextsourcedir, ".svg"),
           (anyextsourcedir, ".bmp"),
           (vectorsourcedir, ".svg"),
           (vectorsourcedir + "/nss", ".svg"),
           (vectorsourcedir + "/nokia", ".svg"),
           (vectorsourcedir + "/oem", ".svg"),
           (bitmapsourcedir, ".bmp") ]

for (opt, src) in sources:
	src = os.path.splitext(src)[0]    # strip any extension from src
	found = ""
	for (dir, ext) in search:
		path = dir + "/" + src + ext
		if os.path.isfile(path):
			found = path
			break	# stick with the first match we get
	if found:
		img.append((opt, found))
		if found.endswith(".bmp"):
			bmp.append((opt, found))
		else:
			svg.append((opt, found))
	else:	
		# warn if there were no matches (or error?)
		sys.stderr.write("warning: no image file found from name '%s'\n" % src)

if not bmp and not svg:
	sys.stderr.write("error: no bmp and no svg files found\n")
	sys.exit(1)
	
if options.debug:
	for b in bmp:
		print "bmp =", b
	for s in svg:
		print "svg =", s

# if an MBM file will be generated then add it to the releasables.
# there will be an MBM if any BMP files are used.
# the MBM name is the MIF name with the extension changed.
if bmp:
	for r in releasables:
		if r.endswith(".mif"):
			mbm = r[0:-4] + ".mbm"
			releasables.append(mbm)
			if options.debug:
				print "mbm =", mbm

	for c in copiers:
		if c.endswith(".mif"):
			copiers.append(c[0:-4] + ".mbm")

# write the releasables out into the manifest file
if options.manifest:
	try:
		file = open(options.manifest, "w")
		for r in releasables:
			file.write(r + "\n")
		file.close()
	except Exception, ex:
		sys.stderr.write("error: %s\n" % str(ex))
		sys.exit(1)
		
# the SVG files are copied to a temporary location because svgtbinencode does
# not have a separate input and output option. This is crazy and should be
# fixed, since unnecessary copying reduces build performance.
svgb = []

# put temporary files in the same directory as the manifest file, since it
# is known to be unique to this component.
tmpdir = os.path.dirname(options.manifest)

# on windows the tools end with .exe
if sys.platform.lower().startswith("win"):
	exe = ".exe"
else:
	exe = ""
	
# the offending tool...
svgoptions = [os.getenv("SVGTBINENCODE", options.epocroot + "/epoc32/tools/svgtbinencode" + exe)]
if options.svgversion:
	svgoptions.extend( ["-v", options.svgversion] )

# track whether anything was regenerated or not
regenerated_svgb = False

for (opt, src) in svg:
	svgcopy = tmpdir + "/" + os.path.basename(src)
	if options.debug:
		print "svg copy =", svgcopy
		
	if options.make == "CLEAN":
		# remove svgcopy and svgcopy + "b" if they exist
		try:
			os.unlink(svgcopy)
			os.unlink(svgcopy + "b")
		except:
			pass
	else:
		svgb.append(svgcopy + "b")
		
		if not os.path.isfile(src):
			sys.stderr.write("error: source file '%s' does not exist\n" % src)
			sys.exit(1)
			
		# copy the SVG if no copy exists or if the copy is old
		if not os.path.isfile(svgcopy) \
		   or os.path.getmtime(svgcopy) < os.path.getmtime(src):
			try:
				shutil.copy(src, svgcopy)
			except Exception, ex:
				sys.stderr.write("error: %s\n" % str(ex))
				sys.exit(1)
				
		# run svgtbinencode to generate the SVGB file if we need to
		if not os.path.isfile(svgcopy + "b") \
		   or os.path.getmtime(svgcopy + "b") < os.path.getmtime(svgcopy):
			command = svgoptions + [svgcopy]
			if options.debug:
				print "command =", " ".join(command)
				
			returncode = subprocess.call(command)
			if returncode != 0:
				sys.stderr.write("error: '%s' failed\n" % " ".join(command))
				sys.exit(1)
				
			regenerated_svgb = True

# if we are CLEANing then all we have left to do is remove any mifconv
# command file that exists in the same directory as the manifest file.
commandfile = options.manifest + ".options"

if options.make == "CLEAN":
	try:
		os.unlink(commandfile)
	except:
		pass    # didn't exist
	
	# all done
	sys.exit(0)
	
# process the BMP files list to add in any requested mask files.
# we need a complete list to determine whether the MIF file is out of date.
all_bmp = []
for (opt, b) in bmp:		
	all_bmp.append(b)
	mask = ""
	if ",1" in opt:
		mask = b.replace(".bmp", "_mask.bmp")
	elif ",8" in opt:
		mask = b.replace(".bmp", "_mask_soft.bmp")
	if mask:
		all_bmp.append(mask)
		if options.debug:
			print "mask =", mask
	
# track whether any BMP file is newer than the target file(s).
# and make sure all the input BMP files exist.
new_bmp = False
changed = 0
for r in releasables:
	try:
		t = os.path.getmtime(r)
		if t > changed:
			changed = t
	except:
		new_bmp = True    # target doesn't exist so we must build it

for b in all_bmp:
	if not os.path.isfile(b):
		sys.stderr.write("error: source file '%s' does not exist\n" % b)
		sys.exit(1)
	if os.path.getmtime(b) > changed:
		new_bmp = True
								
# if all the bitmaps are older than the targets then we have nothing to do
if not new_bmp and not regenerated_svgb:
	if options.debug:
		print "all the targets are up to date."
	sys.exit(0)

# write all the mifconv options into the command file
if options.debug:
	print "commandfile =", commandfile
try:
	file = open(commandfile, "w")
	if options.header:
		file.write("-H%s\n" % headerpath)
	file.write("-E\n")
	file.write("-I%s\n" % anyextsourcedir)

	# add the images in the original order, swapping in SVGB for SVG
	s = 0
	for (opt, name) in img:
		if name.endswith(".svg"):
			name = svgb[s]
			s += 1
		file.write(opt.replace('/', '-') + " " + name + "\n")
		
	file.close()
except Exception, ex:
	sys.stderr.write("error: %s\n" % str(ex))
	sys.exit(1)

# run the mifconv tool
command = [ os.getenv("MIFCONV", options.epocroot + "/epoc32/tools/mifconv" + exe),
            releasables[0], # the first MIF target
            "-F" + commandfile ]

if options.debug:
	print "command =", " ".join(command)
				
returncode = subprocess.call(command)
if returncode != 0:
	sys.stderr.write("error: '%s' failed\n" % " ".join(command))
	sys.exit(1)		

# finally, copy the built files to the additional target directories
if len(targetdirs) > 1:
	destdirs = targetdirs[1:]
	errors = 0
	for c in copiers:
		for dir in destdirs:
			try:
				shutil.copy(c, dir)
				if options.debug:
					print "copy =", c, "to", dir
			except Exception, ex:
				sys.stderr.write("error: %s\n" % str(ex))
				errors += 1
	if errors:
		sys.exit(1)
				
# we made it to the end.
