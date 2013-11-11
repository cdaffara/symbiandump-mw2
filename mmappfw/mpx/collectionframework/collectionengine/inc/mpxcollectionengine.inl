/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Inline implementation of collection engine
*
*/


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Handle collection item change event
// ----------------------------------------------------------------------------
//
inline void CMPXCollectionEngine::HandleMessage(CMPXMessage* aMsg, TInt aError)
    {
    MPX_FUNC("CMPXCollectionEngine::HandleMessage");
    for (TInt i=0; i<iContexts.Count(); ++i)
        {
        iContexts[i]->DoHandleMessage(aMsg, aError, EFalse);
        }
    }

// ----------------------------------------------------------------------------
// Handle open event
// ----------------------------------------------------------------------------
//
inline void CMPXCollectionEngine::HandleOpen(
    CMPXMedia* /*aMedia*/,
    TInt /*aErr*/)
    {
    MPX_DEBUG1("WARNING!!! - CMPXCollectionEngine::HandleOpen aMedia should never be called");
    }

// ----------------------------------------------------------------------------
// Handle open event
// ----------------------------------------------------------------------------
//
inline void CMPXCollectionEngine::HandleOpen(
    CMPXMedia* /*aMedia*/,
    const CMPXCollectionPath* /*aPath*/,
    TInt /*aErr*/)
    {
    MPX_DEBUG1("WARNING!!! - CMPXCollectionEngine::HandleOpen aMedia should never be called");
    }

 // ----------------------------------------------------------------------------
// Handle open event
// ----------------------------------------------------------------------------
//
inline void CMPXCollectionEngine::HandleOpen(
   CMPXCollectionPath* /*aPath*/,
   TInt /*aErr*/)
    {
    MPX_DEBUG1("WARNING!!! - CMPXCollectionEngine::HandleOpen aPath should never be called");
    }

 // ----------------------------------------------------------------------------
// Callback of retrieving extended media property
// ----------------------------------------------------------------------------
//
inline void CMPXCollectionEngine::HandleMedia(
    CMPXMedia* /*aMedia*/,
    TInt /*aError*/)
    {
    MPX_DEBUG1("WARNING!!! - CMPXCollectionEngine::HandleMedia should never be called");
    }

// ----------------------------------------------------------------------------
// Callback of async CommandL
// ----------------------------------------------------------------------------
//
inline void CMPXCollectionEngine::HandleCommandComplete(
    CMPXCommand* /*aCommandResult*/,
    TInt /*aError*/)
     {

     }

// ----------------------------------------------------------------------------
// Handle find all
// ----------------------------------------------------------------------------
//
inline void CMPXCollectionEngine::HandleFindAll(
    CMPXMedia* /*aMedia*/,
    TInt /*aError*/)
    {
    MPX_DEBUG1("WARNING!!! - CMPXCollectionEngine::HandleFindAll should never be called");
    }

// ----------------------------------------------------------------------------
// Handle delete all
// ----------------------------------------------------------------------------
//
inline void CMPXCollectionEngine::HandleRemove(
    const CDesCArray& /*aUriArray*/,
    TInt /*aError*/)
    {
    MPX_DEBUG1("WARNING!!! - CMPXCollectionEngine::HandleRemove should never be called");
    }

// ----------------------------------------------------------------------------
// Check if refreshing is ongoing
// ----------------------------------------------------------------------------
//
inline TBool CMPXCollectionEngine::IsRefreshing()
    {
    return iRefreshing;
    }

// End of file
