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

#ifndef __MSVSECURITYCAPABILITYSET_H__
#define __MSVSECURITYCAPABILITYSET_H__


class TResourceReader;
class TCapabilitySet;
class RWriteStream;
class RReadStream;

class MsvSecurityCapabilitySetUtils
	{
public:
	static void ReadFromResourceL(TResourceReader& aResourceReader, TCapabilitySet& aCaps);
	static void InternalizeL(RReadStream& aReadStream, TCapabilitySet& aCaps);
	static void ExternalizeL(RWriteStream& aWriteStream, const TCapabilitySet& aCaps);
	};
	
#endif //__MSVSECURITYCAPABILITYSET_H__
