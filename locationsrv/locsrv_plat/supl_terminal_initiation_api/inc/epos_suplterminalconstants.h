/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Constant definitions for SUPL Terminal Initiation API 
*
*/

/*! \file epos_suplterminalconstants.h
    \brief Constant values for SUPL Terminal Initiation API
*/

#ifndef __EPOS_SUPLTERMINALCONSTANTS_H__
#define __EPOS_SUPLTERMINALCONSTANTS_H__


/**
 * POS Technologies 
 */
/** SET Assisted A-GPS */
const TUint	KGpsSETAssisted	=   0x0001;
/** SET Based A-GPS */
const TUint	KGpsSETBased  	=  	0x0002;
/** Autonomous GPS */
const TUint	KAutonomousGps 	=  	0x0004;
/** Advanced Forward Link Trilateration */
const TUint	KAFLT 			=  	0x0008;
/** Enhanced Cell ID */
const TUint	KECID   		=  	0x0010;
/** Enhanced Observed Time Difference */
const TUint	KEOTD  			= 	0x0020;
/** Observed Time Difference of Arrival */
const TUint	KOTDOA 			=   0x0040;
/** Cell ID */
const TUint	KCID 			=   0x0080;

/** HSLP Address Max Length*/
const TUint KMaxHslpAddressLength       =   255;

/**
 * Completion codes to give extended information about location fix.
 */
const TInt KSuplPositionQualityLoss = -32524;
/**
 * Completion code if Position information is present but SUPL Session returned error.
 */

const TInt KSuplPositionPresent = 100;

/**
 * Undefined values for QoP Parameters.
 */
/** Undefined value for Horizontal Accurary. This value is used to reset Horizontal Accuracy in QoP */
const TInt KUndefinedHorizontalAccuracy =   -1;
/** Undefined value for Vertical Accurary. This value is used to reset Vertical Accuracy in QoP  */
const TInt KUndefinedVerticalAccuracy =     -1;
/** Undefined value for Maximum Location Age. This value is used to reset Maximum Location Age in QoP  */
const TInt KUndefinedMaxLocationAge =       -1;
/** Undefined value for Delay. This value is used to reset delay in QoP  */
const TUint KUndefinedDelay =               0;

#endif // __EPOS_SUPLTERMINALCONSTANTS_H__
