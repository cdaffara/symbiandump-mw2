/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of positioning settings plugin helper private class. 
*
*/


#include "possettingspluginhelper_p.h"
#include "possettingspluginhelper.h"
#include <EPos_CPosModules.h>
#include <lbs/epos_cposmoduleidlist.h>
#include <featmgr.h> 

//---------------------------------------------------------------------
// PosSettingsPluginHelperPrivate::isPositionPsyAvailable()
// (other items were commented in a header).
//---------------------------------------------------------------------
bool PosSettingsPluginHelperPrivate::isPositionPsyAvailable()
    {
    TInt cnt = 0;	
    TInt err2 = KErrGeneral;
    CPosModules*  posModules;
    TRAPD(err1, posModules = CPosModules::OpenL(););
    if(!posModules)
      {
    	User::Leave( KErrGeneral );
      }
    else
    	{
    	TRAP(err2,
      CPosModuleIdList* idList = posModules->ModuleIdListLC();    
      cnt = idList->Count();
      CleanupStack::PopAndDestroy( idList ););
      }
    if(err1==KErrNone && err2 ==KErrNone && cnt) //if no error and there 
    	return true;
    else
    	return false;	
    }

//---------------------------------------------------------------------
// PosSettingsPluginHelperPrivate::isFeatureFlagEnabled()
// (other items were commented in a header).
//---------------------------------------------------------------------
bool PosSettingsPluginHelperPrivate::isFeatureFlagEnabled()    
    {   
    TBool flag = EFalse;
    TRAPD(err, 
		FeatureManager::InitializeLibL();	    
    if ( FeatureManager::FeatureSupported( KFeatureIdFfLocationsysuiHidePositioningMenu ) )
        {
        flag = ETrue;
        }    
		FeatureManager::UnInitializeLib();		
		);
		if(err==KErrNone && flag)
			return true;
		else
			return false;
		}
		
// End of file
