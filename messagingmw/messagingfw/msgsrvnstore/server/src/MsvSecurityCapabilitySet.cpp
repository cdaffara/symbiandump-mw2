// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "MsvSecurityCapabilitySet.h"
#include <msvstd.hrh>
#include <s32strm.h>
#include <barsread.h>


void MsvSecurityCapabilitySetUtils::ReadFromResourceL(TResourceReader& aResourceReader, TCapabilitySet& aCaps)
	{
	aResourceReader.ReadInt32(); // Version. Not used yet
	aCaps.SetEmpty();
	const TInt count = aResourceReader.ReadInt16();
	for (TInt ii=0; ii<count; ++ii)
		{
		aCaps.AddCapability(static_cast<TCapability>(aResourceReader.ReadInt32()));
		}
	}

void MsvSecurityCapabilitySetUtils::ExternalizeL(RWriteStream& aWriteStream, const TCapabilitySet& aCaps)
	{
	aWriteStream.WriteInt32L(KMsvCurrentSecurityCapabilitySetVersion); // Always write out the current version
	// Create a non-modifiable pointer descriptor over aCaps and write to the stream
	TPckgC<TCapabilitySet> capSetPckgC(aCaps);
	aWriteStream << capSetPckgC;
	}
	
void MsvSecurityCapabilitySetUtils::InternalizeL(RReadStream& aReadStream, TCapabilitySet& aCaps)
	{
	aReadStream.ReadInt32L(); // Version. Not used yet
	// Create a modifiable pointer descriptor over aCaps and read directly into it from the stream 
	TPckg<TCapabilitySet> capSetPckg(aCaps);
	aReadStream >> capSetPckg;
	}
