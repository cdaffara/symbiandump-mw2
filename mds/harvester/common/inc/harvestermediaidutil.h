/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Utility class to keep a list of mediaid's
*/

#ifndef HARVESTERMEDIAIDUTIL_H
#define HARVESTERMEDIAIDUTIL_H

#include <e32base.h>
#include <f32file.h>
#include <e32svr.h>

const TInt KHarvesterMediaIdTLSKey = 0x200009FE;

class CHarvesterMediaIdUtil : public CBase
	{
	friend class RMediaIdUtil;
	
public: 
	
	IMPORT_C TInt GetMediaId( const TDesC& aUri, TUint32& aMediaId );
	
	IMPORT_C TInt GetDriveLetter( TUint32 aMediaId, TChar& aChar );
	
	IMPORT_C void RemoveMediaId( TUint32 aMediaId );
	
private:
	
	static CHarvesterMediaIdUtil* NewL();
		
	~CHarvesterMediaIdUtil();
		
	CHarvesterMediaIdUtil();
	
	void ConstructL();
	
private:
    TUint32 iMediaId[KMaxDrives];
	RFs iFs;
	TVolumeInfo iVolumeInfo;
	
	};

struct TMediaIdUtilInfo
	{
	CHarvesterMediaIdUtil* iMediaIdUtil;
    TInt iRefCount;
	};


class RMediaIdUtil 
    {
public:
   	IMPORT_C static CHarvesterMediaIdUtil& GetInstanceL();

   	IMPORT_C static void ReleaseInstance();
    };

#endif
