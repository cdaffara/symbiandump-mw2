/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Ext Phone Dial Data.
*
*/



// INCLUDE FILES
#include <cphcltdialdata.h>
#include "cphcltextphonedialdata.h" // For correct default values 

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CPhCltDialData::CPhCltDialData
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhCltDialData::CPhCltDialData():
    iTelNumber( KNullDesC ),
    iCallType( EPhCltVoice ),
    iName( KNullDesC ), 
    iWindowGroup( KPhCltGoToIdle ), 
    iRedial( KPhCltRedialDefault ), 
    iSubAddress( KNullDesC ),
    iShowNumber( ETrue ), 
    iAllowMatch( KPhCltDefaultAllowMatch ),
    iSATCall( EFalse ),
    iEndOtherCalls( EFalse ),
    iBearer( KNullDesC8 ),
    iSendKey( EFalse ),
    iReturnToNumberEntryOnFailure( KPhCltDefaultNumberEntry ), 
    iInitCall ( EFalse ), 
    iFlags( CPhCltExtPhoneDialData::EPhCltCallFlagsDefault ),
    iServiceId( NULL )
    { 
    iRedialMaximumDuration = 0;
    }


// Destructor
CPhCltDialData::~CPhCltDialData()
    {  
    delete iContactLink;
    iContactLink = NULL;
    delete iUUI; 
    iUUI = NULL;
    }

void CPhCltDialData::ConstructL()
    {
    iContactLink = KNullDesC8().AllocL();
    iUUI = KNullDesC().AllocL();
    }

//  End of File  
