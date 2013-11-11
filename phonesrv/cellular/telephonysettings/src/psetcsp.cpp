/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*      Customer Service Profile wrapper layer for SsSettings. 
*      PsetCustomerServiceProfile is a class that access CSP through
*      SSsettings. It is in Phone Settings to provide easier access 
*      to CSP values. Whole class should only be used if CSP is supported.                                                         
*
*
*/


//INCLUDE FILES
#include "psetcsp.h" 
#include <etelmm.h>
#include "phonesettingslogger.h" 

//  MEMBER FUNCTIONS
// ---------------------------------------------------------------------------
// 
// Symbian OS 1st phase constructor.
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetCustomerServiceProfile* CPsetCustomerServiceProfile::NewL()
    {
    CPsetCustomerServiceProfile* self = new ( ELeave ) CPsetCustomerServiceProfile();
    return self;
    }

// ---------------------------------------------------------------------------
// 
// Destructor
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetCustomerServiceProfile::~CPsetCustomerServiceProfile()
    {
    //If GS has failed to create CSP due to low memory, check it.
    if ( iCsp )
        {
        iCsp->Close();
        }
    delete iCsp;
    iCsp = NULL;
    }

// ---------------------------------------------------------------------------
// 
// Checks if Call Waiting is to be supported.
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CPsetCustomerServiceProfile::IsCWSupported( TBool& aSupport )
    {
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    __PHSLOGSTRING("[PHS]--> CPsetCustomerServiceProfile::IsCWSupported");
    RMobilePhone::TCspCallCompletion params;    
    TInt retVal = iCsp->CspCallCompletion( params );
    aSupport = EFalse;

    //if bit7 is 1, call waiting is not available through CSP
    if ( retVal == KErrNone )
        {
        aSupport = CheckIfCWSupported( params );
        }

    __PHSLOGSTRING1("[PHS]   CPsetCustomerServiceProfile::IsCWSupported: aSupport: %d", aSupport);
    __PHSLOGSTRING("[PHS] <--CPsetCustomerServiceProfile::IsCWSupported");
    return retVal;
    }

// ---------------------------------------------------------------------------
// 
// Checks if Call Barring is to be supported.
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CPsetCustomerServiceProfile::IsCBSupported( TBool& aSupport )
    {
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    __PHSLOGSTRING("[PHS]--> CPsetCustomerServiceProfile::IsCBSupported");
    RMobilePhone::TCspCallRestriction params;
    TInt retVal = iCsp->CspCallRestriction( params );    
    aSupport = EFalse;

    //if params more than 1, barring is available through CSP
    if ( retVal == KErrNone )
        {
        aSupport = CheckIfCBSupported( params );
        }

    __PHSLOGSTRING1("[PHS]   CPsetCustomerServiceProfile::IsCBSupported: aSupport: %d", aSupport);
    __PHSLOGSTRING("[PHS] <--CPsetCustomerServiceProfile::IsCBSupported");
    return retVal;
    }

// ---------------------------------------------------------------------------
// 
// Checks if Call Forwarding is to be supported.
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CPsetCustomerServiceProfile::IsCFSupported( TBool& aSupport )
    {
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    __PHSLOGSTRING("[PHS]--> CPsetCustomerServiceProfile::IsCFSupported");
    RMobilePhone::TCspCallOffering params;
    TInt retVal = iCsp->CspCallOffering( params );
    aSupport = EFalse;

    //if all of first five bits are 1, divert is available through CSP
    if ( retVal == KErrNone )
        {
        aSupport = CheckIfCFSupported( params );
        }

    __PHSLOGSTRING1("[PHS]   CPsetCustomerServiceProfile::IsCFSupported: aSupport: %d", aSupport);
    __PHSLOGSTRING("[PHS] <--CPsetCustomerServiceProfile::IsCFSupported");
    return retVal;
    }

// ---------------------------------------------------------------------------
// 
// Checks if Alternate Line is to be supported.
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CPsetCustomerServiceProfile::IsALSSupported( TBool& aSupport )
    {
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    __PHSLOGSTRING("[PHS]--> CPsetCustomerServiceProfile::IsALSSupported");
    RMobilePhone::TCspCPHSTeleservices params;
    TInt retVal = iCsp->CspCPHSTeleservices( params );
    aSupport = EFalse;

    if ( retVal == KErrNone )
        {
        aSupport = CheckIfAlsSupported( params );
        }

    __PHSLOGSTRING1("[PHS]   CPsetCustomerServiceProfile::IsALSSupported: aSupport: %d", aSupport);
    __PHSLOGSTRING("[PHS] <--CPsetCustomerServiceProfile::IsALSSupported");
    return retVal;
    }


// ---------------------------------------------------------------------------
// 
// Open CSP from SIM.
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPsetCustomerServiceProfile::OpenCSProfileL()
    {
    __PHSLOGSTRING("[PHS]--> CPsetCustomerServiceProfile::OpenCSProfileL");
    if ( !iCsp )
        {
        iCsp = new (ELeave) RCustomerServiceProfileCache;    
        }
    TInt error = iCsp->Open();

    __PHSLOGSTRING1("[PHS]   CPsetCustomerServiceProfile::OpenCSProfileL: error: %d", error);
    __PHSLOGSTRING("[PHS] <--CPsetCustomerServiceProfile::OpenCSProfileL");
    return error;
    }

// ---------------------------------------------------------------------------
// 
// Checks if Manual Network Selection is to be supported.
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CPsetCustomerServiceProfile::IsNetworkSelectionSupported( 
    TBool& aSupport )
    {
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    __PHSLOGSTRING("[PHS]--> CPsetCustomerServiceProfile::IsNetworkSelectionSupported");
    RMobilePhone::TCspValueAdded params;
    TInt retVal = iCsp->CspCPHSValueAddedServices( params );
    aSupport = EFalse;

    if ( retVal == KErrNone )
        {
        aSupport = CheckIfNetworkSelectionSupported( params );
        }

    __PHSLOGSTRING1("[PHS]   CPsetCustomerServiceProfile::IsNetworkSelectionSupported: aSupport: %d", aSupport);
    __PHSLOGSTRING("[PHS] <--CPsetCustomerServiceProfile::IsNetworkSelectionSupported");
    return  retVal;
    }



// ---------------------------------------------------------------------------
// CPsetCustomerServiceProfile::CheckIfCWSupported
// 
// 
// ---------------------------------------------------------------------------
// 
TBool CPsetCustomerServiceProfile::CheckIfCWSupported(
    const RMobilePhone::TCspCallCompletion aContainer ) const
    {
    return IsSupported( aContainer, RMobilePhone::KCspCW );
    }

// ---------------------------------------------------------------------------
// CPsetCustomerServiceProfile::CheckIfCBSupported
// 
// 
// ---------------------------------------------------------------------------
// 
TBool CPsetCustomerServiceProfile::CheckIfCBSupported(
    const RMobilePhone::TCspCallRestriction aContainer ) const
    {
    return ( 
        IsSupported( aContainer, RMobilePhone::KCspBICRoam )||
        IsSupported( aContainer, RMobilePhone::KCspBAIC ) ||
        IsSupported( aContainer, RMobilePhone::KCspBOICexHC ) ||
        IsSupported( aContainer, RMobilePhone::KCspBOIC ) ||
        IsSupported( aContainer, RMobilePhone::KCspBOAC ) );
    }

// ---------------------------------------------------------------------------
// CPsetCustomerServiceProfile::CheckIfCFSupported
// 
// 
// ---------------------------------------------------------------------------
// 
TBool CPsetCustomerServiceProfile::CheckIfCFSupported(
    const RMobilePhone::TCspCallOffering aContainer ) const
    {
    return ( 
        IsSupported( aContainer, RMobilePhone::KCspCFU )   ||
        IsSupported( aContainer, RMobilePhone::KCspCFB )   ||
        IsSupported( aContainer, RMobilePhone::KCspCFNRc ) ||
        IsSupported( aContainer, RMobilePhone::KCspCFNRy ) );
    }

// ---------------------------------------------------------------------------
// CPsetCustomerServiceProfile::CheckIfAlsSupported
// 
// 
// ---------------------------------------------------------------------------
// 
TBool CPsetCustomerServiceProfile::CheckIfAlsSupported(
    const RMobilePhone::TCspCPHSTeleservices aContainer ) const
    {
    return IsSupported( aContainer, RMobilePhone::KCspALS );
    }

// ---------------------------------------------------------------------------
// CPsetCustomerServiceProfile::CheckIfNetworkSelectionSupported
// 
// 
// ---------------------------------------------------------------------------
// 
TBool CPsetCustomerServiceProfile::CheckIfNetworkSelectionSupported(
    const RMobilePhone::TCspValueAdded aContainer ) const
    {
    return IsSupported( aContainer, RMobilePhone::KCspPLMNMode );
    }


// ---------------------------------------------------------------------------
// CPsetCustomerServiceProfile::IsSupported
// 
// 
// ---------------------------------------------------------------------------
// 
TBool CPsetCustomerServiceProfile::IsSupported(
    const TInt aContainer,
    const TInt aFeature ) const
    {
    __PHSLOGSTRING("[PHS]--> CPsetCustomerServiceProfile::IsSupported");
    TBool supported = EFalse;

    if ( ( aContainer & aFeature ) != 0 )
        {
        supported = ETrue;
        }

    __PHSLOGSTRING1("[PHS]   CPsetCustomerServiceProfile::IsSupported: supported: %d", supported);
    __PHSLOGSTRING("[PHS] <--CPsetCustomerServiceProfile::IsSupported");
    return supported;
    }


// End of File
