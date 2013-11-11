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
* Description:  This is the implementation of the CAudioOutputControlUtility class.
*
*/



// INCLUDE FILES

#include <AudioOutputControlUtility.h>
#include "AudioOutputControlUtilityImpl.h"


// Two-phased constructor. To be called by derived classes.
EXPORT_C void CAudioOutputControlUtility::ConstructL(CAudioOutputControlUtilityImpl* aImpl)
    {
	iImpl = aImpl; 
    }
    
EXPORT_C CAudioOutputControlUtility::CAudioOutputControlUtility()

	{
	}    
     
// -----------------------------------------------------------------------------
// CAudioOutputControlUtility::DestructL
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CAudioOutputControlUtility::~CAudioOutputControlUtility()
    {
#ifdef _DEBUG    
	RDebug::Print(_L("CAudioOutputControlUtility[%x]::~AudioOutputControlUtility\n"),this); 
#endif
    delete iImpl;	
    }

// ---------------------------------------------------------
// CAudioOutputControlUtility::Configure
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CAudioOutputControlUtility::Configure(CMMFDevSound& aDevSound)
    {
    TInt status(KErrNotReady);    
		if (iImpl)
		{    
			return iImpl->Configure(aDevSound);
		}
   	return status;		
    }   
    
// ---------------------------------------------------------
// CAudioOutputControlUtility::ConfigureL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CAudioOutputControlUtility::SetDRMConfigIntfc(CDRMConfigIntfc*& aDRMConfigIntfc)
    {
    TInt status(KErrNotReady);    
	if (iImpl)
		{    
    	return iImpl->SetDRMConfigIntfc(aDRMConfigIntfc);
		}
   	return status;		
    }       
    
// ---------------------------------------------------------
// CAudioOutputControlUtility::SetDataSource
// Implementation given by CAudioOutputControlUtilityImpl class
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CAudioOutputControlUtility::SetDataSource(MDataSource* aDataSource)
    {
    TInt status(KErrNotReady);    
    if (iImpl)
        {    
        return iImpl->SetDataSource(aDataSource);
        }
    return status;      
    } 

// ---------------------------------------------------------
// CAudioOutputControlUtility::SetDataSource
// Implementation given by CAudioOutputControlUtilityImpl class
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CAudioOutputControlUtility::SetDataSource(CMultimediaDataSource* aDataSource)
    {
    TInt status(KErrNotReady);    
    if (iImpl)
        {    
        return iImpl->SetDataSource(aDataSource);
        }
    return status;      
    } 
// End of file

