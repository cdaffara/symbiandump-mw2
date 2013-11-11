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
#include "AudioOutputControlUtilityImpl.h"
#include <e32debug.h>

#ifdef _DEBUG
#define DEBPRN0(str)                RDebug::Print(str, this)
#define DEBPRN1(str)    RDebug::Printf( "%s %s", __PRETTY_FUNCTION__, str );
#else
#define DEBPRN0
#define DEBPRN1(str)
#endif


// -----------------------------------------------------------------------------
// CAudioOutputControlUtilityImpl::~CAudioOutputControlUtilityImpl
// Destructor
// -----------------------------------------------------------------------------
//
inline CAudioOutputControlUtilityImpl::~CAudioOutputControlUtilityImpl()
    {
	DEBPRN0(_L("CAudioOutputControlUtilityImpl[0x%x]::~CAudioOutputControlUtilityImpl\n"));     
    } 
                  
  
// ---------------------------------------------------------
// CAudioOutputControlUtilityImpl::Configure
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline TInt CAudioOutputControlUtilityImpl::Configure(CMMFDevSound& /*aDevSound*/)
    {
    return KErrNotReady;
    }    
    
// ---------------------------------------------------------
// CAudioOutputControlUtilityImpl::SetDRMConfigIntfc
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline TInt CAudioOutputControlUtilityImpl::SetDRMConfigIntfc(CDRMConfigIntfc* /*aDRMConfigIntfc*/)
    {
    return KErrNotReady;
    }
    
// ---------------------------------------------------------
// CAudioOutputControlUtilityImpl::SetDataSource
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline TInt CAudioOutputControlUtilityImpl::SetDataSource(MDataSource* /*aDataSource*/)
    {
    return KErrNotReady;
    }

// ---------------------------------------------------------
// CAudioOutputControlUtilityImpl::SetDataSource
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
inline TInt CAudioOutputControlUtilityImpl::SetDataSource(CMultimediaDataSource* /*aDataSource*/)
    {
    return KErrNotReady;
    }
// End of file

