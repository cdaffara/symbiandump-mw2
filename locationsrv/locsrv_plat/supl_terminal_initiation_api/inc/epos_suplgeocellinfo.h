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
* Description:  Data structure to hold cell information for geo tagging purposes.
*
*/

/*! \file epos_suplgeocellinfo.h
    \brief Triggering classes 
*/

#ifndef __EPOS_SUPLGEOCELLINFO_H__
#define __EPOS_SUPLGEOCELLINFO_H__

// ==========================================

/**
 * Cell types
 */
enum TGeoCellType
	{
	/** GSM Cell. */
	EGeoGsmCell = 0,
	/** WCDMA cell. */
	EGeoWcdmaCell = 1
	};

/**
 * Class defines detailed information for gsm/wcdma cell.
 *
 * @since S60 5.2
 */
struct TGeoCellInfo
    {
    TInt iMNC;
    TInt iMCC;
    TInt iCid;
    TInt iLac;
    TBool iConnectionPrompt;
    TGeoCellType iGeoCellType;
    };

#endif //__EPOS_SUPLGEOCELLINFO_H__
