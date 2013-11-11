// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
// cinstalledmtmgroup.h 
//
/**
 @file
 @internalComponent
 @released
*/
#ifndef __CINSTALLEDMTMGROUP_H__
#define __CINSTALLEDMTMGROUP_H__

class CInstalledMtmGroup;

class CInstalledMtmGroupArray : public CArrayPtrFlat<CInstalledMtmGroup>
/**
@internalComponent
@released
*/
	{
public:
	CInstalledMtmGroupArray();
	~CInstalledMtmGroupArray();
	void AddInstalledMtmGroupL(CInstalledMtmGroup* aInstalledMtmGroup);
	};

#endif //__CINSTALLEDMTMGROUP_H__