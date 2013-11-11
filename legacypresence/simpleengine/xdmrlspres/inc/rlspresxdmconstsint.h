/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    RLS and Presence XDM, This is internal constants.
*
*/



#ifndef __RLSPRESXDMCONSTSINT_H__
#define __RLSPRESXDMCONSTSINT_H__

#include <xcapappusagedef.h>

// this is total length of rule id i.e. KPresRuleIDPrefix + random number
const TInt KPresRuleIDLength(7);
const TInt KPresRuleIDPrefixLength(3);
const TInt KPresRuleIDNumLength(4);
const TInt KPresRuleIdCalc(16770000); // basically we want only last four digits

const TInt KPresDateTimeBufLength(60);
const TInt KPresTransCompAttrLength = 25;
const TInt KPresRLSElemAttrNameLen  = 10;

const TInt KPresRLSDesArrayInitlVal = 10;
const TInt KPresRLSChildConstBufLen = 5;

//Validity constants
const TInt KPresDTWithoutUTCLength = 24;
const TInt KPresDTSeperatorPos     = 10; 


// document names
_LIT(KRLSDocumentName, "index");
_LIT(KPresDocumentName, "pres-rules");

// directory model names
_LIT(KRLSDirectoryModel, "rls-services");
_LIT(KPresRulesDirectoryModel, "org.openmobilealliance.pres-rules");

//Logging constants
_LIT(KPresRLSLogDir, "rlspresxdmapi");
_LIT(KPresRLSLogFile, "rlspresxdm.txt");

// Time formatting strings
_LIT(KPresDateTimeFormatString, "%Y%M%D%/0%1%/1%2%/2%3%/3%J%:1%T%:2%S%.%*C3%:3");
_LIT(KPresUTCFormatString, "%J%:1%T");
_LIT(KPresSpaceString, " ");


// Async operation states for RlsXDM
enum ERLSXDMAsyncOperation
    {
    ERLSXDMNoRequestMade = 0,
    ERLSXDMUpdateToServer,
    ERLSXDMUpdateFromServer,
    ERLSXDMDeletingEmptyLists,
    ERLSXDMDeletedEmptyLists,
    ERLSXDMRequestCancelling
    };

// Async operation states for PresXDM    
enum EPresXDMAsyncOperation
    {
    EPresXDMNoRequestMade = 0,
    EPresXDMUpdateToServer,
    EPresXDMUpdateFromServer,
    EPresXDMRequestCancelling        
    };

// The first rule id from which random generation begins   
_LIT(   KPresRuleIDPrefix,                                  "nok0000");


#endif // __RLSPRESXDMCONSTSINT_H__