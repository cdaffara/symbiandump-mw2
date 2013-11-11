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
#ifndef LOCATIONDATATYPE_H_
#define LOCATIONDATATYPE_H_

#include <lbsposition.h>
#include <etel3rdparty.h>

const TInt KMaxCountryName = 128;
typedef TBuf<KMaxCountryName> TCountryName;
/*
 * TLocationData encapsulates all location data.
 */
struct TLocationData
    {
    TPosition iPosition;
    CTelephony::TNetworkInfoV1 iNetworkInfo;
    TCourse iCourse;
    TCountryName iCountry;
    TUint iSatellites;
    TReal32 iQuality;
    };
	    
#endif /*LOCATIONDATATYPE_H_*/

