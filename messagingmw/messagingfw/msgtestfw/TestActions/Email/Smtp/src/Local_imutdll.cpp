// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// IMUTDLL.CPP
// 
//

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

GLDEF_C void OpenResourceFileL(RResourceFile& rFile, RFs& anFs)
	{
	// open resource file
	TFileName fileName(KImEngineResourceFile);
	MsvUtils::AddPathAndExtensionToResFileL(fileName);
	BaflUtils::NearestLanguageFile( anFs, fileName );
	if (BaflUtils::FileExists(anFs,fileName))
 		{
		rFile.OpenL( anFs, fileName ); // Not a Leavescan error
 		return;
 		}
 
	User::Leave(KErrNotFound);
	}

GLDEF_C void gPanic( TImutErrorReason aReason)
	{
	User::Panic( KMTFName,aReason);
	}

GLDEF_C void CloseResourceFile( TAny* file )
	{
	((RResourceFile*)file)->Close();
	}
