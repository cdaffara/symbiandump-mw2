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
* Description:  Implementation of positioning settings engine private class. 
*
*/

#include "possettingsengineprivate.h"
#include "possettingsengine.h"
#include <qglobal.h>
#include <EPos_CSuplSettings.h>
#include <lbtcommon.h>
#include <EPos_CPosModules.h>
#include <EPos_CPosModuleIdList.h>
#include <EPos_CPosModuleUpdate.h>

// AGPS psy module UID
const TUid KAGPSPsyModuleID =
    {
    0x101fe98c
    };
// NPP Psy module UID
const TUid KNPPPsyModuleID =
    {
    0x10206915
    };

//---------------------------------------------------------------------
// PosSettingsEnginePrivate::PosSettingsEnginePrivate()
// (other items were commented in a header).
//---------------------------------------------------------------------
PosSettingsEnginePrivate::PosSettingsEnginePrivate( 
                                      PositionTechnologyChangeObserver& 
                                      posTechChangeObserver):
                                      mPosTechChangeObserver(
                                      posTechChangeObserver )
    {
    // Open session to LBT server.
    int error = mLbtServer.Connect();
    if( error != KErrNone )
        {
        throw( error );
        }
    //Open subsession to LBT server.
    error = mLbtManager.Open( mLbtServer );
    if( error != KErrNone )
        {
        throw( error );
        }
    QT_TRAP_THROWING( mSuplSettings = CSuplSettings::NewL();
                      mPosModules = CPosModules::OpenL();
                
                      populatePsyModuleInfoListL();
                      // Get back ground position status
                      GetBackGroundPositioingStateL();
                      // Register for settings change.
                      mPosModules->SetObserverL( *this )
                    );
    }


//---------------------------------------------------------------------
// PosSettingsEnginePrivate::~PosSettingsEnginePrivate()
// (other items were commented in a header).
//---------------------------------------------------------------------
PosSettingsEnginePrivate::~PosSettingsEnginePrivate()
    {
    mLbtManager.Close();
    mLbtServer.Close();
    delete mSuplSettings;
    mSuplSettings = NULL;
    // remove observer
    mPosModules->RemoveObserver();
    delete mPosModules;
    mPosModules = NULL;
    
    mPsyModuleInfoList.Close();
    }


//---------------------------------------------------------------------
// PosSettingsEnginePrivate::isPositionTechnologyAvailable()
// (other items were commented in a header).
//---------------------------------------------------------------------
bool PosSettingsEnginePrivate::isPositionTechnologyAvailable( 
                                       PositionTechnology posTechnology )
    {
    // Iterate through the PSY list to find the specified position 
    // technology.
    TInt cnt = mPsyModuleInfoList.Count();
    for( TInt i=0;i<cnt;++i )
        {
        if( mPsyModuleInfoList[i].mPosTech == posTechnology )
            {
            return true;
            }
        }
    return false;
    }

//---------------------------------------------------------------------
// PosSettingsEnginePrivate::setPositionTypeState()
// (other items were commented in a header).
//---------------------------------------------------------------------
int PosSettingsEnginePrivate::setPositionTechnologyState( 
                                      State posTechnologyState,
                                      PositionTechnology posTechnology )
    {
    // If the specified position technology is unavailable, return error.
    if( !isPositionTechnologyAvailable( posTechnology ) )
        {
        return KErrNotFound;
        }
    
    int error = KErrNone;
    TRAP( error,setPositionTechnologyStateL( posTechnologyState,posTechnology ) );
    return error;
    }

//---------------------------------------------------------------------
// PosSettingsEnginePrivate::isPositionTechnologyEnabled()
// (other items were commented in a header).
//---------------------------------------------------------------------
bool PosSettingsEnginePrivate::isPositionTechnologyEnabled( 
                                       PositionTechnology posTechnology )
    {
    // Iterate through the PSY info list to find specified position 
    // technology and its corresponding state.
    TInt cnt = mPsyModuleInfoList.Count();
    for( TInt i=0;i<cnt;++i )
        {
        if( mPsyModuleInfoList[i].mPosTech == posTechnology && 
            mPsyModuleInfoList[i].mState == StateEnable )
            {
            return true;
            }
        }
    return false;
    }
                                         
//---------------------------------------------------------------------
// PosSettingsEnginePrivate::setBackGroundPositioningState()
// (other items were commented in a header).
//---------------------------------------------------------------------
int PosSettingsEnginePrivate::setBackGroundPositioningState( 
                                             State backGroundPosState )
    {    
    // Check if any of the position technology are available.
    if( !mPsyModuleInfoList.Count() )
        {
        return KErrNotSupported;
        }
    int error = KErrNone;
    // Change the state of back ground positioning only if its current
    // state is different from the requested state.
    if( backGroundPosState != mBackGroundPositioningState )
        {
        TRAP( error,setBackGroundPositioningStateL( backGroundPosState ) );
        }
    return error;
    }

//---------------------------------------------------------------------
// PosSettingsEnginePrivate::isBackGroundPositioningEnabled()
// (other items were commented in a header).
//---------------------------------------------------------------------
bool PosSettingsEnginePrivate::isBackGroundPositioningEnabled()
    {
    if( mBackGroundPositioningState == StateEnable )
        {
        return true;
        }
    return false;
    }

//---------------------------------------------------------------------
// PosSettingsEnginePrivate::HandleSettingsChangeL()
// (other items were commented in a header).
//---------------------------------------------------------------------
void PosSettingsEnginePrivate::HandleSettingsChangeL( 
                                       TPosModulesEvent aEvent )
    {    
    // Reset the module info list and repopulate again.
    mPsyModuleInfoList.Reset();
    populatePsyModuleInfoListL();
    
    if( aEvent.iType == EPosModulesEventModuleInstalled || aEvent.iType == EPosModulesEventModuleRemoved
    	|| aEvent.iType == EPosModulesEventVisibilityChanged ) 
        {       
    	mPosTechChangeObserver.handlePositionTechnologyChange();
    	}
    }

//---------------------------------------------------------------------
// PosSettingsEnginePrivate::setPositionTechnologyStateL()
// (other items were commented in a header).
//---------------------------------------------------------------------
void PosSettingsEnginePrivate::setPositionTechnologyStateL( 
                                        State posTechnologyState,
                                        PositionTechnology posTechnology )
    {
    CPosModuleUpdate* updateParams = CPosModuleUpdate::NewLC();
    
    if( posTechnologyState == StateEnable )
        {
        updateParams->SetUpdateAvailability( ETrue );
        }
    else
        {
        updateParams->SetUpdateAvailability( EFalse );
        }
    TInt cnt = mPsyModuleInfoList.Count();    
    for( TInt i=0;i<cnt;++i )
        {
        // Change the state only if position technology of PSY matches
        // with the position technology specified and if the state of 
        // the PSY is different from the requested state.
        if( mPsyModuleInfoList[i].mPosTech == posTechnology &&
            mPsyModuleInfoList[i].mState != posTechnologyState )
            {
            mPosModules->UpdateModuleL( mPsyModuleInfoList[i].mPsyModuleId, 
                                        *updateParams );
            mPsyModuleInfoList[i].mState = posTechnologyState;
            }
        }
    CleanupStack::PopAndDestroy( updateParams );
    }

//---------------------------------------------------------------------
// PosSettingsEnginePrivate::setBackGroundPositioningStateL()
// (other items were commented in a header).
//---------------------------------------------------------------------
void PosSettingsEnginePrivate::setBackGroundPositioningStateL( 
                                           State backGroundPosState )
    {
    if( backGroundPosState == StateEnable )
        {
        mLbtManager.SetTriggeringMechanismStateL( 
                     ETriggeringMechanismOn );
        User::LeaveIfError( mSuplSettings->SetSuplTriggeredServiceStatus( 
                                CSuplSettings::ESuplTriggerOn  ) );
        mBackGroundPositioningState = StateEnable;
        }
    else
        {
        mLbtManager.SetTriggeringMechanismStateL( 
                     ETriggeringMechanismOff );
        User::LeaveIfError( mSuplSettings->SetSuplTriggeredServiceStatus( 
                                CSuplSettings::ESuplTriggerOff ) );
        mBackGroundPositioningState = StateDisable;
        }
    }

//---------------------------------------------------------------------
// PosSettingsEnginePrivate::populatePsyModuleInfoListL()
// (other items were commented in a header).
//---------------------------------------------------------------------
void PosSettingsEnginePrivate::populatePsyModuleInfoListL()
    {
    TBool agpsState = EFalse;
    TBool nppState = EFalse;
    CPosModuleIdList* idList = mPosModules->ModuleIdListLC();
    TInt cnt = idList->Count();
    for ( TInt i = 0; i < cnt; ++i )
        {
        TPositionModuleInfo moduleInfo;
        mPosModules->GetModuleInfoL( (*idList)[i], moduleInfo );
        
        // Psy module info hold the information regarding PSY that are
        // essential for our operation.
        PsyModuleInfo psyModuleInfo;
        
        // Set the module Id.
        psyModuleInfo.mPsyModuleId = moduleInfo.ModuleId();
        
        // Set position technology type.
        if( moduleInfo.TechnologyType() == TPositionModuleInfo::ETechnologyTerminal ||
            moduleInfo.TechnologyType() == TPositionModuleInfo::ETechnologyAssisted )
            {
            psyModuleInfo.mPosTech = PositionTechnologyGps;
            }
        else if ( moduleInfo.TechnologyType() == TPositionModuleInfo::ETechnologyNetwork )
            {
            psyModuleInfo.mPosTech = PositionTechnologyNetwork;
            }
        
        // Set state of Psy.
        if( moduleInfo.IsAvailable() )
            {
            psyModuleInfo.mState = StateEnable;
            // capture the state of the agps psy module
            if (moduleInfo.ModuleId() == KAGPSPsyModuleID)
                {
                agpsState = ETrue;
                }
            // capture the state of npp psy module
            if (moduleInfo.ModuleId() == KNPPPsyModuleID)
                {
                nppState = ETrue;
                }

            }
        else 
            {
            psyModuleInfo.mState = StateDisable;
            // capture the state of the agps psy module
            if (moduleInfo.ModuleId() == KAGPSPsyModuleID)
                {
                agpsState = EFalse;
                }
            // capture the state of npp psy module
            if (moduleInfo.ModuleId() == KNPPPsyModuleID)
                {
                nppState = EFalse;
                }
            }
        
        mPsyModuleInfoList.AppendL( psyModuleInfo );
        }
    // Check if the state of both agps & npp psy are disabled then turn off the supl trigger status
    // else turn it back on again even if either is enabled
    if (!agpsState && !nppState)
        {
        User::LeaveIfError(mSuplSettings->SetSuplTriggeredServiceStatus(
                CSuplSettings::ESuplTriggerOff));
        }
    else
        {
        User::LeaveIfError(mSuplSettings->SetSuplTriggeredServiceStatus(
                CSuplSettings::ESuplTriggerOn));
        }

    CleanupStack::PopAndDestroy(idList);
    }


//---------------------------------------------------------------------
// PosSettingsEnginePrivate::GetBackGroundPositioingStateL()
// (other items were commented in a header).
//---------------------------------------------------------------------
void PosSettingsEnginePrivate::GetBackGroundPositioingStateL()
    {
    // Get the SUPL service status.
    CSuplSettings::TSuplTriggerStatus suplTriggerStatus;
    User::LeaveIfError( mSuplSettings->GetSuplTriggeredServiceStatus( 
                        suplTriggerStatus ) );
    
    // Get LBT service status.
    TLbtTriggeringSystemManagementSettings triggeringSystemSettings;
    mLbtManager.GetTriggeringSystemSettingsL( triggeringSystemSettings );
    
    // Back ground positioning state is considered to be enabled only 
    // if both LBT and SUPL services are enabled.
    if( suplTriggerStatus == CSuplSettings::ESuplTriggerOn && 
        triggeringSystemSettings.TriggeringMechanismState() ==
        ETriggeringMechanismOn )
        {
        mBackGroundPositioningState = StateEnable;
        }
    else
        {
        mBackGroundPositioningState = StateDisable;
        } 
    }

// End of file
