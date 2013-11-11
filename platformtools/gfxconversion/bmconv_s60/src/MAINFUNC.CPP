/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/

#if defined(__MSVCDOTNET__) || defined(__TOOLS2__) || defined(__CW32__)
#include <ostream>
#include <iostream>
using namespace std;
#else //!__MSVCDOTNET__ && !__TOOLS2__ && !__CW32__
#pragma warning(disable: 4100)
#pragma warning(disable: 4511)
#pragma warning(disable: 4512)
#pragma warning(disable: 4530)
#pragma warning(disable: 4663)
#pragma warning(disable: 4710)
#pragma warning(disable: 4786)
#pragma warning(disable: 4800)
#include <ostream.h>
#endif //__MSVCDOTNET__

#include "TOOLSVER.H"
#include "BMCONV.H"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string>

const int KMaxPaletteFileLen = 256;

#ifdef __linux__
const char* DefaultPaletteFileSearchLocations[] =
{
    "/epoc32/include/mw/ThirdPartyBitmap.pal",
    "/epoc32/include/middleware/ThirdPartyBitmap.pal",
    "/epoc32/include/ThirdPartyBitmap.pal"
};
#else
const char* DefaultPaletteFileSearchLocations[] =
{
    "\\epoc32\\include\\mw\\ThirdPartyBitmap.pal",
    "\\epoc32\\include\\middleware\\ThirdPartyBitmap.pal",
    "\\epoc32\\include\\ThirdPartyBitmap.pal"
};
#endif

/**
Returns an informative error message, the result of the program actions performed.
@return Informative error string
@param aErrorNumber The error returned from the actions performed
@param aDestfile The multiple bitmap store file name
@param aDestCreated True if the multiple bitmap store has been created/modified
*/

char* ErrorMessage(int aErrorNumber, char* aDestfile=NULL, bool aDestCreated=false)
	{
	// Remove the multiple bitmap store if it has been created/modified during an fstream session and there has been an error
	if(aDestfile && (aErrorNumber != NoError) && (aDestCreated == true))
		{
		remove(aDestfile);
		}

	switch(aErrorNumber)
		{
		case NoError:
			return "Success.";
		case NoMemory:
			return "Out of memory.";
		case Arg:
			return "Bad argument.";
		case Files:
			return "File does not exist";
		case SourceFile:
			return "Bad source file(s).";
		case DestFile:
			return "Bad destination file(s).";
		case CommandFile:
			return "Bad command file.";
		case OutOfRange:
			return "Number of sources/targets mismatch.";
		case TooManyArgs:
			return "Too many arguments.";
		case UnknownCompression:
			return "Unknown source compression type.";
		case CompressionError:
			return "Compression error.";
		case DecompressionError:
			return "Decompression error.";
		case Bpp:
			return "Invalid bitmap mode specified.";
		case PaletteFile:
			return "Bad palette file.";
		case PaletteSupportNotImplemented:
			return "Palettes not supported";
		case AlphaFiles:
			return "Alpha bitmap file does not exist";
		case AlphaDimensions:
			return "Alpha channel bitmap's dimensions don't match pixel bitmap's dimensions.";
		case AlphaBpp:
			return "Alpha channel bitmap must be 8bpp.";
		default:
			return "Unknown error!";
		};
	}

void Header()
	{
	cout << "\n";
	cout << "\n";
	cout << "BMCONV S60 version "<< version << ".\n";
	}
	
void Report(int aError)
	{
	Header();
	cout << ErrorMessage(aError) << "\n";
	}

/**
Compiliation information to print to the user at the end of the program.
@param aQuiet Flag if the user selected quiet output mode
@param aError The error returned from the actions performed
@param aType The multiple bitmap store type created
@param aDestfile The multiple bitmap store file name
@param aBitmapFiles The array of bitmaps used
@param aNumFiles The amount of bitmaps used
@param aDestCreated True if the multiple bitmap store has been created/modified
*/

void CompilationReport(int aQuiet,int aError,TStoreType aType,char* aDestfile,TSourceFile* aBitmapFiles,int aNumFiles, bool aDestCreated)
	{	
	if(!aQuiet || aError)
		{
		Header();
		cout << "Compiling...\n";
		if(aType!=ENoStore)
			cout << "Multiple bitmap store type: ";
		if(aType==EFileStore)
			cout << "File store" << "\n";
		else if(aType==ERomStore)
			cout << "ROM image store" << "\n";
		else if(aType==ECompressedRomStore)
			cout << "Compressed ROM image store" << "\n";
		if(aDestfile!=NULL)
			cout << "Epoc file: " << aDestfile << "\n\n";
		for(int count=0;count<aNumFiles;count++)
			{
			cout << "Bitmap file " << count+1 << "	: ";
			aBitmapFiles[count].WriteCompileInfo(cout);
			cout << endl;
			}
		cout << ErrorMessage(aError, aDestfile, aDestCreated) << "\n";
		}
	}
	
void DecompilationReport(int aError,char* aDestfile,char** aBitmapFiles,int aNumFiles)
	{	
	Header();
	cout << "Decompiling...\n";
	if(aDestfile!=NULL)
		cout << "Epoc file: " << aDestfile << "\n\n";
	for(int count=0;count<aNumFiles;count++)
		{
		cout << "Bitmap file " << count+1 << "	: ";
		cout << aBitmapFiles[count] << "\n";
		}
	cout << ErrorMessage(aError) << "\n";
	}
	
void Usage()
    {
	cout << "\n";
	cout << "BMCONV S60 version "<< version << ".\n";
	cout << "Symbian OS multiple bitmap file/rom store conversion program.\n";
	cout << "Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.";
	cout << "\n";
	cout << "\n";
	cout << "Usage:\n";
	cout << "BMCONV [-r|-s|-n] [-hfilename] [-q] [-pfilename] epocfile [OPT]bmp_1 ... [OPT]bmp_n\n";
	cout << "BMCONV [-r|-s|-n] [-q] [-pfilename] epocfile -mepocfile2\n";
	cout << "BMCONV -u epocfile bmp_1 [... bmp_n]\n";
	cout << "BMCONV -v epocfile\n";
	cout << "BMCONV commandfile\n";
	cout << "\n";
	cout << " -r specifies a ROM image destination file,\n";
	cout << " -s specifies a compressed ROM image file,\n";
	cout << " -n disables bitmap File Store compression,\n";
	cout << " the default is a compressed File Store file.\n\n";
	cout << " -q specifies quiet mode - only errors are reported.\n\n";
	cout << " -hfilename specifies the filename for the automatic\n";
	cout << " generation of a header file for inclusion into code.\n\n";
	cout << " -pfilename gives the filename of a palette file containing 256 hex\n";
	cout << " numbers (0x00BBGGRR) specifying the palette for 8bpp colour bitmaps.\n";
	cout << " (Omission results in the use of a default palette.)\n\n";
	cout << " OPT may be one of -1, -2, -4, -8, -c4, -c8, -c12, -c16, -c24 -c32 -c32a\n";
	cout << " specifying bits per pixel and grey-scale-colour, or -mepocfile2\n";
	cout << " to specify an existing multiple bitmap file. default is -2.\n\n";
	cout << " To avoid ambiguity when specifying -c32 with a bitmap file whose name\n";
	cout << " begins with an 'a', use a relative or direct directory reference\n";
	cout << " e.g. -c32.\\abitmap.bmp or -c32c:\\abitmap.bmp\n";
	cout << " Directory names must not include spaces.\n\n";
	cout << " -c32a specifies use of an alpha channel in a 32bpp bitmap. Alpha data\n";
	cout << " is supplied in a separate 8bpp bmp file with identical dimensions to\n";
	cout << " the pixel data. This file must be named as bmp_n with the suffix '-alpha'\n";
	cout << " e.g. if bmp_1 is 'my.bmp' then the file 'my-alpha.bmp' is required in the\n";
	cout << " same directory.  The alpha file does not need to be specified.\n\n";
	cout << " epocfile specifies the epoc multi-bitmap file name.\n";
	cout << " bmp_n specifies the nth bitmap file name.\n\n";
	cout << " -u decompiles epocfile to bmp_1,...,bmp_n.\n";
	cout << " If an alpha channel is present then a further, 8bpp file is output for \n";
	cout << " the alpha data, named with an '-alpha' suffix as described above.\n\n";
	cout << " -v displays a summary of the bitmaps in epocfile\n";
	cout << " otherwise bmp_1,...,bmp_n are compiled to epocfile\n\n";
	cout << " commandfile specifies a file containing the commandline\n";
	cout << " with commands separated by spaces or newlines.\n\n";
	cout << " When bmconv is used on Windows, options may start with '/' or '-'\n";
	}

int IsWhiteSpace(char aCharacter)
	{
	return(aCharacter==' ' || aCharacter=='\n' || aCharacter=='\r' || aCharacter==0x1a);
	}

int ProcessCommandFile(char* aCommandFileName,char** aArgPtrs,int& aNumArgs)
    {
	struct stat fileinfo;
	if (stat(aCommandFileName,&fileinfo)==-1)
		return CommandFile;

	int filesize=fileinfo.st_size;
	if (filesize==0)
		return NoError;
#if defined(__MSVCDOTNET__) || defined(__TOOLS2__) || defined(__CW32__)
	fstream commandfile(aCommandFileName, ios::in | ios::binary);
#else //!__MSVCDOTNET__
	fstream commandfile(aCommandFileName, ios::in | ios::binary | ios::nocreate);
#endif //__MSVCDOTNET__
	if(!commandfile.is_open())
		return CommandFile;

	char* commandData=new char[filesize+1];
	if(commandData==NULL)
		return NoMemory;

	memset(commandData,0,filesize+1);
	commandfile.read(commandData,filesize);
	commandData[filesize]='\0';

	char* commandptr = (char*)commandData;
	char* commandptrLimit = (char*)(commandData + filesize);
	while (commandptr < commandptrLimit)
		{
		if(*commandptr=='/' && *(commandptr+1)=='/')
			while(*commandptr!='\n' && *commandptr!='\r' && commandptr < commandptrLimit)
				*commandptr++=' ';
		else if (*commandptr==0x1a)
			*commandptr++=' ';
		commandptr++;
		}

	commandptr = (char*)commandData;
	while (commandptr < commandptrLimit)
		{
		while(IsWhiteSpace(*commandptr) && commandptr < commandptrLimit)
			*commandptr++='\0';
		if (commandptr == commandptrLimit)
			break;
		aArgPtrs[aNumArgs]=commandptr;
        if ( (*commandptr == '/') && (*(commandptr+1) == 'p'||*(commandptr+1) == 'P') && (*(commandptr+2) == '"') )
            {
            commandptr+=3;
            while(*commandptr != '"')
                commandptr++;
            *commandptr=' ';
            
            }
		while(!IsWhiteSpace(*commandptr) && commandptr < commandptrLimit)
			commandptr++;
		if (commandptr == commandptrLimit)
			break;
		aNumArgs++;
		}

	commandfile.close();
	return NoError;
    }

int Decompile(int aArgc,int aNumArgs,char** aArgPtrs)
	{
	int ret=OutOfRange;
	char* destfilename=aArgPtrs[1];

	if(aArgc>=4 || aArgc==2)
		{
		for(int count=2;count<aNumArgs;count++)
			{
			EpocLoader pl;
			ret=pl.LoadEpocBitmap(destfilename,count-2);
			if(!ret) ret=pl.SaveBitmap(aArgPtrs[count]);
			if(ret) break;
			}
		DecompilationReport(ret,destfilename,&aArgPtrs[2],aNumArgs-2);
		}
	else
		DecompilationReport(ret,NULL,NULL,0);

	return ret;
	}

const char* AddEpocRootTo(std::string& aBuf, const std::string& aName)
	{ 
	int nameLen = aName.length();
	int rootLen = 0;
	aBuf = ""; // Clear old contents
	char* epocroot = getenv("EPOCROOT");
	if (epocroot)
		{
		rootLen = strlen(epocroot);
		if (nameLen+rootLen < KMaxPaletteFileLen)
			{
			if( rootLen > 0 && epocroot[rootLen-1] == '\\')
			    {
			    aBuf = std::string(epocroot, rootLen-1);
			    }
			else if( rootLen > 0 )
			    {
			    aBuf = std::string(epocroot);
			    }
			}
		}
	aBuf += aName;
	return aBuf.c_str();
	}

bool FileExists( const char* aFileName )
{
  struct stat fileInfo;   
  int retVal = 0; 

  // Try to get file attributes to see if the file exists or not:
  retVal = stat( aFileName, &fileInfo); 
  return retVal == 0;
}

const char* DefaultPaletteFileName(std::string& aBuf)
    {
    const char* palettefilename = NULL;    
    int numOfSearchLocations = sizeof(DefaultPaletteFileSearchLocations)/sizeof(char*);
    
    for( int i = 0; i < numOfSearchLocations; ++i )
        {
        palettefilename = AddEpocRootTo(aBuf, DefaultPaletteFileSearchLocations[i]);
        if( FileExists( palettefilename))
            {
            return palettefilename;
            }
        }
    
    return NULL;
    }

int Compile(int aNumArgs,int aArgArraySize, char** aArgPtrs)
	{
	TStoreType storeType = EFileStore;
	int compression = 1;
	int quiet = 0;
	char* headerfilename = NULL;
	char paletteBuf[KMaxPaletteFileLen];
	memset(paletteBuf, 0, KMaxPaletteFileLen);
	std::string paletteStr;	
	const char* defaultPalettefile = DefaultPaletteFileName(paletteStr);
	char* palettefilename = NULL;
	char* destfilename = NULL;
	int ret = OutOfRange;
	bool aDestCreated = false;

	for(int argnum=0;argnum<aNumArgs;argnum++)
		{
		if(aArgPtrs[argnum] && (aArgPtrs[argnum][0] == OPTCHAR || aArgPtrs[argnum][0]==ALTERNATE_OPTCHAR))
			{
			if(aArgPtrs[argnum][1]=='r' || aArgPtrs[argnum][1]=='R')
				{
				if(storeType==ECompressedRomStore)
					{
					ret=TooManyArgs;
					CompilationReport(quiet,ret,storeType,NULL,NULL,0,aDestCreated);
					return ret;
					}
				storeType=ERomStore;
				aArgPtrs[argnum] = NULL;
				}
			else if(aArgPtrs[argnum][1]=='s' || aArgPtrs[argnum][1]=='S')
				{
				if(storeType==ERomStore)
					{
					ret=TooManyArgs;
					CompilationReport(quiet,ret,storeType,NULL,NULL,0,aDestCreated);
					return ret;
					}
				storeType=ECompressedRomStore;
				aArgPtrs[argnum] = NULL;
				}
			else if(aArgPtrs[argnum][1]=='n' || aArgPtrs[argnum][1]=='N')
				{
				compression=0;
				aArgPtrs[argnum] = NULL;
				}
			else if(aArgPtrs[argnum][1]=='h' || aArgPtrs[argnum][1]=='H')
				{
				headerfilename = &aArgPtrs[argnum][2];
				aArgPtrs[argnum] = NULL;
				}
			else if(aArgPtrs[argnum][1]=='q' || aArgPtrs[argnum][1]=='Q')
				{
				if (getenv("BMCONV_NOT_QUIET") == 0)
					quiet=1;
				aArgPtrs[argnum] = NULL;
				}
			else if(aArgPtrs[argnum][1]=='p' || aArgPtrs[argnum][1]=='P')
				{
				palettefilename = &aArgPtrs[argnum][2];
                if (*palettefilename=='"')
                    {
                    palettefilename+=1;
                    }
				aArgPtrs[argnum] = NULL;
				}
			}
		else
			break;		// the RNHQP arguments must precede the output filename
		}

	if( palettefilename == NULL )
	    {
	    // Palette file not given in arguments. If the default palette file
	    // was found, use it, otherwise return error.
	    if( defaultPalettefile != NULL )
	        {
	        memcpy(paletteBuf, defaultPalettefile, strlen(defaultPalettefile));
	        palettefilename = paletteBuf;
	        }
	    else
	        {
	        // Palettefile not given in arguments nor found from default locations:
	        ret = PaletteFile;
	        CompilationReport(quiet,ret,storeType,NULL,NULL,0,aDestCreated);
	        return ret;
	        }
	    }
	int firstsource=0;
	while(firstsource<aArgArraySize && aArgPtrs[firstsource]==NULL)
		firstsource++;
	if(firstsource==aArgArraySize) firstsource=0;
	destfilename=aArgPtrs[firstsource];
	firstsource++;
	int numsources=firstsource;
	while(numsources<aArgArraySize && aArgPtrs[numsources]!=NULL)
		numsources++;
	if(numsources==aArgArraySize) numsources=0;
	numsources-=firstsource;

	TSourceFile* sources = 0;

	if (numsources > 0)
		{
		sources = new TSourceFile[numsources];
		for (int ii=0; ii<numsources; ii++)
			sources[ii].FileName() = aArgPtrs[firstsource + ii];
		BitmapCompiler mp(sources,numsources);
		ret = mp.Compile(storeType,compression,destfilename,headerfilename,palettefilename);
		aDestCreated = true;		// The multiple bitmap store has been created/modified
		}

	CompilationReport(quiet,ret,storeType,destfilename,sources,aNumArgs-firstsource,aDestCreated);
	delete [] sources;

	return ret;
	}

void GetInfo(char* aSourceFile)
	{
	Header();

	EpocLoader pl;
	int numSources=-1;
	int romFormat=0;
	int ret = pl.EpocBitmapCount(aSourceFile, numSources, romFormat);
	if (ret)
		{
		cout << "Problem reading number of bitmaps \n";
		cout << ErrorMessage(ret) << "\n";
		return;
		}

	cout << aSourceFile << " is a " << (romFormat? "ROM image":"File store") 
		<< " containing " << numSources << ((numSources==1)? " bitmap\n":" bitmaps\n");

	for (int count = 0;count<numSources;count++)
		{
		ret = pl.LoadEpocBitmap(aSourceFile,count);
		if (ret == OutOfRange)
			break;
		cout << "\n";
		if (ret)
			{
			cout << "Problem loading bitmap number " << count << "\n";
			cout << ErrorMessage(ret) << "\n";
			break;
			}
		else
			{
			SEpocBitmapHeader h = pl.Header();
			cout << "Bitmap " << count + 1 << " information:\n";
			cout << "Pixel size " << h.iWidthInPixels << " x " << h.iHeightInPixels << "\n";
			cout << "Twips size " << h.iWidthInTwips << " x " << h.iHeightInTwips << "\n";
			cout << h.iBitsPerPixel << " Bpp ";
			if (h.iColor == EColorBitmap)
				cout << "Colour";
			else if (h.iColor == EColorBitmapAlpha)
				cout << "Colour with alpha channel";
			else if(h.iColor == EMonochromeBitmap)
				cout << "Monochrome";
			else
				cout << "Unknown colour format";
			cout << "\n";
			if (h.iPaletteEntries > 0)
				cout << "Palette entries " << h.iPaletteEntries;

			int byteSize = BitmapUtils::ByteWidth(h.iWidthInPixels,h.iBitsPerPixel) * h.iHeightInPixels;
			int compressionRatio = 0;
			if (byteSize > 0)
				compressionRatio = (h.iBitmapSize - sizeof(SEpocBitmapHeader)) * 100 / byteSize;

			switch (h.iCompression)
				{
			case ENoBitmapCompression:
				cout << "No compression\n";
				break;
			case EByteRLECompression:
				cout << "Bytewise RLE compression " << compressionRatio << "%\n";
				break;
			case ETwelveBitRLECompression:
				cout << "12 bit RLE compression " << compressionRatio << "%\n";
				break;
			case ESixteenBitRLECompression:
				cout << "16 bit RLE compression " << compressionRatio << "%\n";
				break;
			case ETwentyFourBitRLECompression:
				cout << "24 bit RLE compression " << compressionRatio << "%\n";
				break;
			case EThirtyTwoUBitRLECompression:
				cout << "unsigned 32 bit RLE compression (no alpha channel) " << compressionRatio << "%\n";
				break;
			case EThirtyTwoABitRLECompression:
				cout << "unsigned 32 bit RLE compression (with alpha channel) " << compressionRatio << "%\n";
				break;
		//	case ERLECompressionLast: // Added to supress unhandled switch warning
			default:
				break;
				}
			}
		}

	cout << "\n";
	}

class TAutoPtr
	{
public:
	TAutoPtr(char** aPtr) :
		iPtr(aPtr)
		{
		}
	~TAutoPtr()
		{
		delete iPtr;
		}
private:
	char** iPtr;
	};

int main(int argc,char* argv[],char* [])
    {
	if (argc <= 1)
		{
		Usage();
		return 0;
		}

	int optMaxCnt = argc;

	if(argc==2) // The single argument must be a command file name
		{
		struct stat fileinfo;
		if (stat(argv[1],&fileinfo)==-1)
			{
			Report(CommandFile);
			return 0;
			}
		optMaxCnt = fileinfo.st_size;
		}

	char** argptrs = new char*[optMaxCnt];
	if(!argptrs)
		{
		Report(NoMemory);
		return 0;
		}
	TAutoPtr autoPtr(argptrs);
	memset(argptrs, 0, optMaxCnt * sizeof(char*));

	int numargs = 0;
	if(argc>2) // Explicit arguments are present
		{
		for(int count=0;count<argc-1;count++)
			argptrs[count]=argv[count+1];
		numargs = argc-1;
		}
	else // The single argument must be a command file name
		{
		int ret = ProcessCommandFile(argv[1],argptrs,numargs);
		if (ret)
			{
			Report(ret);
			return 0;
			}
		}

	if ((argptrs[0]!=NULL && (argptrs[0][0]==OPTCHAR || argptrs[0][0]==ALTERNATE_OPTCHAR)) && (argptrs[0][1]=='u' || argptrs[0][1]=='U')) {
		return Decompile(argc,numargs,argptrs); }

	if ((argptrs[0]!=NULL && (argptrs[0][0]==OPTCHAR || argptrs[0][0]==ALTERNATE_OPTCHAR)) && (argptrs[0][1]=='v' || argptrs[0][1]=='V'))
		{
		GetInfo(argptrs[1]);
		return 0;
		}

	return Compile(numargs,optMaxCnt,argptrs);
    }

