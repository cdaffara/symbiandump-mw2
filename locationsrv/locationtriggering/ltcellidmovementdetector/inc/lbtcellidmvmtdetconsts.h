/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Contains definition of all the constants used by movement detection 
*                component.				 
*
*/



#ifndef __LBTCELLIDMVMTDETCONSTS_H__
#define __LBTCELLIDMVMTDETCONSTS_H__


/**
 * This structure contains GSM information retrieved.
 */
struct 	TGsmData
	{
	// Absolute Radio Frequency Channel Number.
	TInt  	iARFCN[5];
	// Base Station Identity Code.
	TInt  	iBSIC[5];
	// The average signal strength received
	TInt  	iRxLEV[5];
	// Timing advance.
	TInt16  iTimingAdvance;
	};
	
	
/*
 *
 */
struct TGsmNMR
    {
    TInt    BSIC;
    TInt    ARFCN;
    TInt    RxLEV;
    };
	

/**
 * This structure contains Wcdma information retrieved.
 */ 	
struct TWcdmaNMR
    {
    TInt iCarrierRSSI;
	TInt iCpichEcN0;
	TInt iCpichRscp;
	TInt iPathloss;
	}; 
#endif // __LBTCELLIDMVMTDETCONSTS_H__
