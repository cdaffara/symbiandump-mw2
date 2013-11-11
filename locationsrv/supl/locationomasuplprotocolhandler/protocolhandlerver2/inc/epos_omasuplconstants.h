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
* Description:  Contains common constants
*
*/



#ifndef EPOS_OMASUPLCONSTANTS_H
#define EPOS_OMASUPLCONSTANTS_H

// HSLP Port No
const TUint KOMASuplHSLPPort = 7275;

// SUPL Version
const TUint KSuplMajorVersion = 2;
const TUint KSuplMinorVersion = 0; 
const TUint KSuplRevision = 0;

//POS MSG Plugin Version
const TReal KSuplPOSPluginVersion = 1.0;

const TInt KMaxPosPayloadLength = 8192;
const TUint KSecond  = 	 1000000; // 1 SECS 6 0s

// OMA SUPL Errors

const TInt KErrOMAUnknownState = -32512;
const TInt KErrOMASuplProtocolError = -32513;
const TInt KErrOMANonProxyModeNotSupported = -32514;
const TInt KErrOMASuplVersionNotMatching = -32515;
const TInt KErrOMASETCapMisMatch = -32516;
const TInt KErrOMAManParamMissing = -32517;
const TInt KErrOMAInvalidHSLPAddress = -32518;
const TInt KErrOMASuplDenied = -32519;
const TInt KErrOMASuplAccepted = -32520;
const TInt KErrOMASuplInformationOnly = -32521;
const TInt KErrOMASuplNoPosition = -32522;
const TInt KErrSuplEndProtocolError = -32523;
const TInt KErrSuplposMethodMismatch = -32526;
const TInt KErrOMASuplUnInitilized =  -32528; 
const TInt KErrSuplInvalidSessionID = -32529;
//const TInt KErrSuplTimeout =  -32530; //also used by connection requestor...
const TInt KErrSuplServiceNotSupported = -32531;

//UID's
const TUid KCRUidOMASuplSettings = {0x102073D6 };  
//const TUid KCRUidOMASuplConfiguration = {0x102750A0 };  
const TUid KOMAPOSHandlerInterfaceUid = {0x102073D4};  


//General Constanst
const TUint KMaxSessionCount = 65535;
const TUint KIP4AddressLength = 4;
const TUint KIP6AddressLength = 16;
const TUint KHSLPAddressLength = 256;
const TInt KMaxIPAddressLength = 256;
const TInt KMaxIapNameLength = 256;
const TInt KMaxSessionNameLength = 256;
const TInt KMaxImsiLength = 15;
const TInt KIMSILength = 8;
const TInt KInvalidIAPID = -1;
const TReal32 KOMAMaxVerticalAccuracy = 991;
//Toe Limit to use or ignore pos data
const TInt KMaxCellIdChangeToeLimit = 2;

//Chi square values to calculate vertical accuracy from altitude uncertainty
const TReal32 chisquare[21] = {0.000000000,  /* For confidence 0 */
                               0.003933000,  /* For confidence 5 */
                               0.015790000,  /* For confidence 10 */
                               0.035770000,  /* For confidence 15 */
                               0.064180000,  /* For confidence 20 */
                               0.101500000,  /* For confidence 25 */
                               0.148500000,  /* For confidence 30 */
                               0.205900000,  /* For confidence 35 */
                               0.275000000,  /* For confidence 40 */
                               0.357300000,  /* For confidence 45 */
                               0.455000000,  /* For confidence 50 */
                               0.570600000,  /* For confidence 55 */
                               0.708300000,  /* For confidence 60 */
                               0.873500000,  /* For confidence 65 */
                               1.074000000,  /* For confidence 70 */
                               1.323000000,  /* For confidence 75 */
                               1.642000000,  /* For confidence 80 */
                               2.072000000,  /* For confidence 85 */
                               2.706000000,  /* For confidence 90 */
                               3.841000000,  /* For confidence 95 */
                               6.635000000,  /* For confidence 99 */
                               };
//Uncertainty conversion factor
const TReal KCEP68ConversionFactor = 0.7547;
const TReal KUncertaintyConversionFactor = 0.01;
const TUint32 KOMASuplCurrentCellid =    0x0000000c;

#endif //EPOS_OMASUPLCONSTANTS_H
