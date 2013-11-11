/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This is the implementation of the CConfigurationComponentsFactoryImpl class.
*
*/



// INCLUDE FILES
#include <e32svr.h>
#include "ConfigurationComponentsFactoryImpl.h"
#include <RestrictedAudioOutputProxy.h>
#include <AudioOutputControlUtilityProxy.h>

#ifdef _DEBUG
#define DEBPRN0         RDebug::Printf( "%s", __PRETTY_FUNCTION__);
#define DEBPRN1(str)    RDebug::Printf( "%s %s", __PRETTY_FUNCTION__, str );
#else
#define DEBPRN0
#define DEBPRN1(str)
#endif

// -----------------------------------------------------------------------------
// CConfigurationComponentsFactory::CBody::CBody
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CConfigurationComponentsFactory::CBody::CBody()
    {
    }


// -----------------------------------------------------------------------------
// CConfigurationComponentsFactory::CBody::ConstructL
// Symbian 2nd phase constructor can leave.
// assumes that iParent has already been set up properly
// -----------------------------------------------------------------------------
//
void CConfigurationComponentsFactory::CBody::ConstructL()
    {

    }


// Two-phased constructor.
CConfigurationComponentsFactory::CBody* CConfigurationComponentsFactory::CBody::NewL()
    {
    DEBPRN0;
    
	CConfigurationComponentsFactory::CBody* self = new(ELeave) CBody();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;  

    }
    
// -----------------------------------------------------------------------------
// CConfigurationComponentsFactory::CBody::~CBody
// Destructor
// -----------------------------------------------------------------------------
//
CConfigurationComponentsFactory::CBody::~CBody()
    {

    } 
       
// ---------------------------------------------------------
// CConfigurationComponentsFactory::CBody::CreateRestrictedAudioOutput
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CConfigurationComponentsFactory::CBody::CreateRestrictedAudioOutput(CMMFDevSound& aDevSound, CRestrictedAudioOutput*& aAudioOutput)
    {
    DEBPRN0;
    
    TInt err = KErrNone;
    TRAP(err, iAudioOutput = CRestrictedAudioOutputProxy::NewL(aDevSound));
    if (err != KErrNone)
    	return KErrNoMemory;
    
    aAudioOutput = iAudioOutput;
    
    return err;
    }

// ---------------------------------------------------------
// CConfigurationComponentsFactory::CBody::CreateAudioOutputControlUtility
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CConfigurationComponentsFactory::CBody::CreateAudioOutputControlUtility(CAudioOutputControlUtility*& aAudioOutputControlUtility)
    {
    DEBPRN0;
    
    TInt err = KErrNone;    
    TRAP(err, iAudioOutputControlUtility = CAudioOutputControlUtilityProxy::NewL());
    if (err != KErrNone)
    	return KErrNoMemory;
    
    aAudioOutputControlUtility = iAudioOutputControlUtility;
    
    return err;
    }
    
// End of file

