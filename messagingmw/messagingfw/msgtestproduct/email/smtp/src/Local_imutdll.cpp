// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

/**
 @file
*/

#include <e32std.h>
#include <f32file.h>	// RFs
#include <barsc.h>		// RResourceFile
#include <bautils.h>	// Bafl::
#include "msvutils.h"
#include <imcvtext.h>
#include "local_imutdll.h"

#include <miuthdr.h>	// resource file name
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "timrfc822datefield.h"
#endif

_LIT(KMTFName,"Messaging Test Framework");


/**
OpenResourceFileL()
Opens a file using the file server session

@param rFile
@param anFs
*/
GLDEF_C void OpenResourceFileL(RResourceFile& rFile, RFs& anFs)
	{
	// open resource file
	TFileName fileName(KImEngineResourceFile);
	MsvUtils::AddPathAndExtensionToResFileL(fileName);
	BaflUtils::NearestLanguageFile( anFs, fileName );
	if (BaflUtils::FileExists(anFs,fileName))
 		{
		rFile.OpenL( anFs, fileName );
 		return;
 		}
 
	User::Leave(KErrNotFound);
	}


/**
gPanic()

@param aReason
@panic 
*/
GLDEF_C void gPanic( TImutErrorReason aReason)
	{
	User::Panic( KMTFName,aReason);
	}


/**
CloseResourceFile()
Closes a opened file

@param file
*/
GLDEF_C void CloseResourceFile( TAny* file )
	{
	((RResourceFile*)file)->Close();
	}
