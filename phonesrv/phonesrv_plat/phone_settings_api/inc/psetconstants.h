/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Various Constants
*
*
*/


#ifndef     PSETCONSTANTS_H
#define     PSETCONSTANTS_H

//Common
const TUid KPSetUidPset = { 0x100058E3 };
const TInt KPSetUnusedValue = 255; //unknown basic service group
const TInt KPSetNumberOfBsc = 13;
const TInt KPsetRequestAlreadySet = -1;
const TInt KPsetPhoneNumberLength = 100;
typedef TBuf<KPsetPhoneNumberLength> TTelNumber;

//Divert object
const TInt KPSetDefaultNumberMax = 3;
const TInt KPSetDefaultDelayTimeValue = 0;
const TInt KPSetFirstDefNumber = 1;
const TInt KPSetSecondDefNumber = 2;
const TInt KPSetThirdDefNumber = 3;
const TInt KPsetDelayTimeDivider = 5;
const TInt KPsetDefaultTimerValue = 30;
const TInt KPsetMatchingChars = 7;
_LIT( KPSetAsterisk, "*" );

//Barring
const TInt KPsetPwdLength = 10;
typedef TBuf<KPsetPwdLength> TBarringPassword; 

// Network object
const TInt KPSetNetworkInfoArrayGranularity = 10;

// Service codes for setting passwords
const TInt KPsetAllSSServices = 0;
const TInt KPsetBarringService = 330;

//ETEL connection
const TInt KPSetRetryCount = 7;
const TInt KPSetRetryTimeout = 100000; 
const TInt KPSetNoLinesAvailable = -1;

#endif //PSETCONSTANTS_H
// End of file
