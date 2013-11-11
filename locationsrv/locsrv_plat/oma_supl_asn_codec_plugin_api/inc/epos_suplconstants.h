/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Constants related to Supl
*
*/

#ifndef T_SUPCONSTANTS_H
#define T_SUPCONSTANTS_H

#include <data_caging_path_literals.hrh>
#include <rmmcustomapi.h>

const TInt KIPv4Address(4);
const TInt KIPv6Address(16);
const TInt KImsiAddress(8);
const TInt KMacLen(6);
const TInt KSLPSessionID(4);
const TInt KIPAddress(5);
const TInt KImsi(2);
const TInt KFqdn(2);
const TInt KVer(8);
const TInt KNoOfSatellite(32);
const TInt KRANDStrLen(16);
const TInt KMaxKeyIdentityLength = 16;
const TInt KMaxLongSPCKey = 32;

const TInt KMncMin(0);
const TInt KMncMax(999);
const TInt KMaxStrLen = 255;
const TInt KMccMin(0);
const TInt KMccMax(999);

const TInt KRefUcMin(0);
const TInt KRefUcMax(268435455);

const TInt KRefLacMin(0);
const TInt KRefLacMax(65535);

const TInt KRefCIMin(0);
const TInt KRefCIMax(65535);

const TInt KVersionMin(0);
const TInt KVersionMax(255);

const TInt KMessageLengthMin(0);
const TInt KMessageLengthMax(65535);

const TInt KSETSessionIDMin(0);
const TInt KSETSessionIDMax(65535);

const TInt KLatitudeMin(0);
const TInt KLatitudeMax(8388607);

const TInt KLongitudeMin(-8388608);
const TInt KLongitudeMax(8388607);

const TInt KUncertSemiMajorMin(0);
const TInt KUncertSemiMajorMax(127);

const TInt KUncertSemiMinorMin(0);
const TInt KUncertSemiMinorMax(127);

const TInt KOrientMajorAxisMin(0);
const TInt KOrientMajorAxisMax(180);

const TInt KConfidenceMin(0);
const TInt KConfidenceMax(100);

const TInt KAltitudeMin(0);
const TInt KAltitudeMax(32767);

const TInt KAltitudeUncertMin(0);
const TInt KAltitudeUncertMax(127);

const TInt KSatIdMin(0);
const TInt KSatIdMax(63);

const TInt KIodeMin(0);
const TInt KIodeMax(255);

const TInt KGPSWeekMin(0);
const TInt KGPSWeekMax(1023);

const TInt KGPSToeMin(0);
const TInt KGPSToeMax(167);

const TInt KNSatMin(0);
const TInt KNSatMax(31);

const TInt KToeLimitMin(0);
const TInt KToeLimitMax(10);

const TInt KHorizAccuracyMin(0);
const TInt KHorizAccuracyMax(127);

const TInt KVertAccuracyMin(0);
const TInt KVertAccuracyMax(127);

const TInt KMaxLocationAgeMin(0);
const TInt KMaxLocationAgeMax(65535);

const TInt KDelayMin(0);
const TInt KDelayMax(7);
const TInt KErrOMASuplInvalidMessage(-32509);

const TInt KErrOMASuplManParamMissing(-32510);

const TInt KErrOMASuplDecodeSessionFailed(-32511);

const TInt KErrOMASuplDataMissing(-32525);

const TInt KErrOMASuplShortFrame(-32527);

const TInt KErrOMASuplOutOfRangeParameter(-32529);

const TInt KErrOMASuplMessageLengthMismatch(-32530);

const TInt KErrOMASuplMessageUnsupportedParam(-32531);

#define LEFTSHIFT(i)        (1<<(i));

/*
// Cell info constant
const TInt KOMASUPLMaxTimeSlotIscpAmount		= 1; // Changed from 14 to 1 ...as its got changed in CustomAPI
const TInt KOMASUPLMaxCellMeasuredResults		= 32;
const TInt KOMASUPLMaxNetworkMeasureReports	= 8;
const TInt KOMASUPLMaxNmrAmount				= 15;
*/

// Cell info constant-- Changed due to changes in CustomAPI
const TInt KOMASUPLMaxTimeSlotIscpAmount		= KMaxTimeSlotIscpAmount; // Changed from 14 to 1 ...as its got changed in CustomAPI
const TInt KOMASUPLMaxCellMeasuredResults		= KMaxCellMeasuredResults;
const TInt KOMASUPLMaxNetworkMeasureReports	= KMaxNetworkMeasureReports;
const TInt KOMASUPLMaxNmrAmount				= KMaxNmrAmount;

const TInt KTAMin = 0;
const TInt KTAMax = 255;

const TInt KARFCNMin = 0;  
const TInt KARFCNMax = 1023 ; 

const TUint8 KBSICMin = 0;
const TUint8 KBSICMax = 63;

const TUint8 KRxLEVMin = 0;  
const TUint8 KRxLEVMax = 63;

const TInt KCIDMin = 0;
const TInt KCIDMax = 268435455;

const TInt KPrimaryScrambilingCodeMin = 0;
const TInt KPrimaryScrambilingCodeMax = 511;

const TInt KFddDLMin = 0;
const TInt KFddDLMax = 16383;

const TInt KFddULMin = 0;
const TInt KFddULMax = 16383;

const TInt KTddDtMin = 0;
const TInt KTddDtMax = 16383;

const TInt KOMASuplOptionalParamNotSet = -1; 

const TInt KPathlossMin = 46;
const TInt KPathlossMax = 173;

const TInt KPrimaryCcpchRscpMin = 0 ;
const TInt KPrimaryCcpchRscpMax = 127;

const TInt KProposedTGSNMin = 0;
const TInt KProposedTGSNMax = 14;

const TInt KCellParamIDMin = 0;
const TInt KCellParamIDMax = 127;

const TInt KTimeslotISCPMin = 0;
const TInt KTimeslotISCPMax = 127;

const TInt KPrimaryCPICHMin = 0;
const TInt KPrimaryCPICHMax = 127;

const TInt KCpichEcN0Min = 0;
const TInt KCpichEcN0Max = 63;

const TInt KCichRscpMin = 0;
const TInt KCichRscpMax = 127;

const TInt KCarrierRSSIMin = 0;
const TInt KCarrierRSSIMax = 127;

const TInt KMaxLocDataValueLen = 128;
const TInt KMaxNMRListElem = 32;
const TInt KThirdPartyOctetLen = 8;
const TInt KThirdPartyMinLen = 8;
const TInt KSPCSETKeyLen = 16;
const TInt KSectorIdLen = 16;
const TInt KWiMaxAreaBitLen = 3;
const TInt KBasicProtKeyIdLen = 8;
const TInt KBasicProtMacLen = 4;
#endif // T_SUPCONSTANTS_H