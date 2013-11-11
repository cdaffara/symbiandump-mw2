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
* Description:  This is the implementation of the CConfigurationComponentsFactory class.
*
*/



// INCLUDE FILES
#include <ConfigurationComponentsFactory.h>
#include "ConfigurationComponentsFactoryImpl.h"



// Two-phased constructor.
EXPORT_C TInt CConfigurationComponentsFactory::CreateFactoryL(CConfigurationComponentsFactory*& aFactory)
    {	
    CConfigurationComponentsFactory* self = new (ELeave)CConfigurationComponentsFactory;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    aFactory = self;
    return KErrNone;
    }
    
CConfigurationComponentsFactory::CConfigurationComponentsFactory()
	{

	}    
// -----------------------------------------------------------------------------
// CConfigurationComponentsFactory::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CConfigurationComponentsFactory::ConstructL()
    {
    iBody = CBody::NewL();
    iBody->ConstructL();
    }
       
// -----------------------------------------------------------------------------
// CMMFDevSoundAdaptation::DestructL
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CConfigurationComponentsFactory::~CConfigurationComponentsFactory()
    {
    delete iBody;
    }
            
// ---------------------------------------------------------
// CConfigurationComponentsFactory::CreateRestrictedAudioOutput
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CConfigurationComponentsFactory::CreateRestrictedAudioOutput(CMMFDevSound& aDevSound, CRestrictedAudioOutput*& aAudioOutput)
    {
    return iBody->CreateRestrictedAudioOutput(aDevSound, aAudioOutput);
    }
    
// ---------------------------------------------------------
// CConfigurationComponentsFactory::CreateAudioOutputControlUtility
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CConfigurationComponentsFactory::CreateAudioOutputControlUtility(CAudioOutputControlUtility*& aAudioOutputControlUtility)
    {
    return iBody->CreateAudioOutputControlUtility(aAudioOutputControlUtility);
    }    
    
// End of file

