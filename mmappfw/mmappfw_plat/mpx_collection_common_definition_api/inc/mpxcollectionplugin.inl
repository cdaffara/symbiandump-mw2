/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Inline implementation of collection plugin constructor/destructor
*
*/


// ======================= INLINE MEMBER FUNCTIONS ============================
#include <mpxtaskqueue.h>

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
inline CMPXCollectionPlugin* CMPXCollectionPlugin::NewL(
    const TUid& aUid,
    MMPXCollectionPluginObserver& aObs)
    {
    CMPXCollectionPlugin* self = reinterpret_cast<CMPXCollectionPlugin*>(
                REComSession::CreateImplementationL(aUid,
                                         _FOFF(CMPXCollectionPlugin,iDtorKey)));
    CleanupStack::PushL(self);
    self->InitializeL();
    CleanupStack::Pop(self);
    self->iObs = &aObs;
    self->iUid = aUid;
    return self;
    }

// ----------------------------------------------------------------------------
// plugin destruction
// ----------------------------------------------------------------------------
//
inline CMPXCollectionPlugin::~CMPXCollectionPlugin()
    {
    delete iTaskQueue;
    REComSession::DestroyedImplementation(iDtorKey);
    }

// ----------------------------------------------------------------------------
// Set observer of current request
// ----------------------------------------------------------------------------
//
inline void CMPXCollectionPlugin::SetObserver(MMPXCollectionPluginObserver& aObs)
    {
    iObs = &aObs;
    }

// ----------------------------------------------------------------------------
// Returns implemenation uid of the plugin
// ----------------------------------------------------------------------------
//
inline const TUid& CMPXCollectionPlugin::Uid()
    {
    return iUid;
    }

// ----------------------------------------------------------------------------
// Returns current async task in the task queue
// ----------------------------------------------------------------------------
//
inline TInt CMPXCollectionPlugin::Task()
    {
    return iTaskQueue->Task();
    }

// ----------------------------------------------------------------------------
// Set observer of current request
// ----------------------------------------------------------------------------
//
inline void CMPXCollectionPlugin::AddTaskL(
    TInt aTask,
    TAny* aCallback,
    MMPXTaskQueueObserver* aHandler,
    TInt aParamData/*=0*/,
    CBufBase* aBuf/*=NULL*/,
    TAny* aPtrData/*=NULL*/,
    CBase* aCObject1/*=NULL*/,
    CBase* aCObject2/*=NULL*/)
    {
    iTaskQueue->AddTaskL(aTask, aCallback, aHandler, aParamData,
                         aBuf, aPtrData, aCObject1, aCObject2);
    }

// ----------------------------------------------------------------------------
// Set observer of current request
// ----------------------------------------------------------------------------
//
inline void CMPXCollectionPlugin::CompleteTask()
    {
    iTaskQueue->CompleteTask();
    }

// ----------------------------------------------------------------------------
// CMPXCollectionPlugin::CompleteAllTasks
// ----------------------------------------------------------------------------
//
inline void CMPXCollectionPlugin::CompleteAllTasks(
    TInt aError)
    {
    iTaskQueue->CompleteAllTasks(aError);
    }

// ----------------------------------------------------------------------------
// Set observer of current request
// ----------------------------------------------------------------------------
//
inline void CMPXCollectionPlugin::RemoveTask(TAny* aCallback)
    {
    iTaskQueue->RemoveTask(aCallback);
    }

// ----------------------------------------------------------------------------
// Initialize the data memeber
// ----------------------------------------------------------------------------
//
inline void CMPXCollectionPlugin::InitializeL()
    {
    iTaskQueue = CMPXActiveTaskQueue::NewL();
    }

// ----------------------------------------------------------------------------
// Returns callback of current active task
// ----------------------------------------------------------------------------
//
inline MMPXCollectionEngineObserver* CMPXCollectionPlugin::Callback()
    {
    return iCallback;
    }

// ----------------------------------------------------------------------------
// Set call back when current active task completed. NULL if there is
// no active task for this plugin
// ----------------------------------------------------------------------------
//
inline void CMPXCollectionPlugin::SetCallback(
    MMPXCollectionEngineObserver* aCallback)
    {
    iCallback = aCallback;
    }
// End of file
