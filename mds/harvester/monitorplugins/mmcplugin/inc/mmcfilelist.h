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

#ifndef __MMCFILELIST_H__
#define __MMCFILELIST_H__


#include <e32base.h>
#include <f32file.h>
#include "harvestermediaidutil.h"

// FORWARD DECLARATIONS
class CHarvesterEventManager;
class CPlaceholderData;
class CMdEHarvesterSession;
class CHarvesterPluginFactory;

class CMmcFileList : public CBase
{
public:

	static CMmcFileList* NewL();	
	virtual ~CMmcFileList();
	
private:	
	CMmcFileList();
	void ConstructL();
	
	static TBool IsDescInArray(const TPtrC& aSearch, const RPointerArray<HBufC>& aArray);
	
public:	
	void BuildFileListL(RFs& aFs, const TDesC& aDrivePath, 
			RPointerArray<CPlaceholderData>& aEntryArray );
	void HandleFileEntryL( CMdEHarvesterSession& aMdeSession,
			RPointerArray<CPlaceholderData>& aEntryArray,
			RPointerArray<CPlaceholderData>& aHarvestEntryArray,
			TUint32 aMediaID,
			CHarvesterPluginFactory* aPluginFactory );

private:
	CHarvesterEventManager* iHem;
    // not own
    CHarvesterMediaIdUtil* iMediaIdUtil;
    
    RPointerArray<HBufC> iIgnorePaths;
};

#endif /*MMCFILELIST_H_*/
