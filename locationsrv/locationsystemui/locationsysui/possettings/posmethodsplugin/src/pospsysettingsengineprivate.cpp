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
 * Description:  Implementation of psy settings engine private class. 
 *
 */


#include "pospsysettingsengineprivate.h"
#include "pospsysettingsengine.h"

#include <qglobal.h>
#include <EPos_CPosModules.h>
#include <EPos_CPosModuleIdList.h>
#include <EPos_CPosModuleUpdate.h>

// constants
const TInt KNameLength = 50;

//---------------------------------------------------------------------
// PosPsySettingsEnginePrivate::PosPsySettingsEnginePrivate()
// Constructor
//---------------------------------------------------------------------
PosPsySettingsEnginePrivate::PosPsySettingsEnginePrivate(
        PositionTechnologyChangeObserver& posTechChangeObserver) :
    mPosTechChangeObserver(posTechChangeObserver)
    {
    
    // initialize the CPosModules pointer
    QT_TRAP_THROWING ( mPosModules = CPosModules::OpenL();
            // Register for settings change.
            mPosModules->SetObserverL( *this ));
    }

//---------------------------------------------------------------------
// PosPsySettingsEnginePrivate::~PosPsySettingsEnginePrivate()
// Destructor
//---------------------------------------------------------------------
PosPsySettingsEnginePrivate::~PosPsySettingsEnginePrivate()
    {
    // delete mPosModules
    if (mPosModules)
        {
        // remove the observer
    		mPosModules->RemoveObserver();
        delete mPosModules;
        mPosModules = NULL;
        }
    // delete the psyname 
    TInt cnt = mNameList.count();
    for(TInt i=0;i<cnt;++i)
        {
        delete mNameList[i];
        mNameList[i] = NULL;
        }
    mNameList.clear();
    
    // delete the list of psy info
    mPsyInfoList.Reset();
    }

//---------------------------------------------------------------------
// PosPsySettingsEnginePrivate::listPsys( QList<PosPsyInfo>& psyList )
// 
//---------------------------------------------------------------------
int PosPsySettingsEnginePrivate::listPsys(QList<PosPsyInfo>& psyList)
    {
    // Get the list of available psy
    TRAPD( error, listPsysL() );

    if (error == KErrNone)
        {        
        // copy the psy from RArray to the Qlist
        TInt cnt = mPsyInfoList.Count();
        for (TInt i = 0; i < cnt; ++i)
            {
            psyList.append(mPsyInfoList[i]);
            }
        return KErrNone;        
        }
    else
        {
        return error;
        }

    }

//---------------------------------------------------------------------
// PosPsySettingsEnginePrivate::changePsyState( TUid psyModuleId, PosPsyState psyState )
// 
//---------------------------------------------------------------------
int PosPsySettingsEnginePrivate::changePsyState(TUid psyModuleId,
        PosPsyState psyState)
    {
    // update the psy state
    TRAPD( error, changePsyStateL( psyModuleId, psyState ) );
    return error;
    }

//---------------------------------------------------------------------
// PosPsySettingsEnginePrivate::listPsysL( )
// 
//---------------------------------------------------------------------
void PosPsySettingsEnginePrivate::listPsysL()
    {
    // reset the list of psyinfo
    mPsyInfoList.Reset();

    // get the list of psy module ids
    CPosModuleIdList* idList = mPosModules->ModuleIdListLC();
    // for each psy in the list,do the following :
    // - get module info
    // - check visibility of psy,if true then do the following :
    //     -> store the uid,name, state of psy in the PosPsyInfo structure
    //     -> append to the list of psys.
    TInt cnt = idList->Count();
    for (TInt i = 0; i < cnt; ++i)
        {
        TPositionModuleInfo moduleInfo;
        mPosModules->GetModuleInfoL((*idList)[i], moduleInfo);

        // check visibility
        if (mPosModules->IsModuleVisibleL((*idList)[i]))
            {

            PosPsyInfo psyInfo;
            // Set the module Id.
            psyInfo.mPsyModuelId = moduleInfo.ModuleId();

            // Set module name
            TBuf<KNameLength> moduleName;
            moduleInfo.GetModuleName(moduleName);
            // converting Tbuf to qstring
            QString* name = new QString((QChar*) moduleName.Ptr(),
                    moduleName.Length());
            mNameList.append( name );
            psyInfo.mPsyName = *name;

            // Set the state of the psy
            if (moduleInfo.IsAvailable())
                {
                psyInfo.mPsyState = PsyEnable;
                }
            else
                {
                psyInfo.mPsyState = PsyDisable;
                }

            // append the psyinfo to the list
            mPsyInfoList.AppendL(psyInfo);
            }
        }

    CleanupStack::PopAndDestroy(idList);
    }

//---------------------------------------------------------------------
// PosPsySettingsEnginePrivate::changePsyStateL( TUid psyModuleId, PosPsyState psyState )
// 
//---------------------------------------------------------------------
void PosPsySettingsEnginePrivate::changePsyStateL(TUid psyModuleId,
        PosPsyState psyState)
    {
    // create an instance of CPosModuleUpdate to help in updating the attributes
    //  of Psy
    CPosModuleUpdate* updateParams = CPosModuleUpdate::NewLC();
    // Depending on the changed state update the availability of the psy
    // i.e enable/disable
    if (psyState == PsyEnable)
        {
        updateParams->SetUpdateAvailability(ETrue);
        }
    else
        {
        updateParams->SetUpdateAvailability(EFalse);
        }
    // update the state of the psy whose id is mentioned
    mPosModules->UpdateModuleL(psyModuleId, *updateParams);

    CleanupStack::PopAndDestroy(updateParams);

    }

//---------------------------------------------------------------------
// PosPsySettingsEnginePrivate::changePsyStateL( TUid psyModuleId, PosPsyState psyState )
// 
//---------------------------------------------------------------------
void PosPsySettingsEnginePrivate::HandleSettingsChangeL(
        TPosModulesEvent aEvent)
    {
    // Check the event type,only for installation,removal,
    // change in  visibility,
    // get the newly available list of psy
    if (aEvent.iType == EPosModulesEventModuleInstalled || 
        aEvent.iType == EPosModulesEventModuleRemoved || 
        aEvent.iType == EPosModulesEventVisibilityChanged)
        {
        listPsysL();
        // call the engine's handlePositionTechnologyChange
        mPosTechChangeObserver.handlePositionTechnologyChange();
        }
    }

