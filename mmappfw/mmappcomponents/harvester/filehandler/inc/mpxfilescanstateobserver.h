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
* Description:  Generic file scanning state change observer
*
*/


#ifndef MMPXFILESCANSTATEOBSERVER_H
#define MMPXFILESCANSTATEOBSERVER_H
    
/**
 *  MMPXFileScanStateObserver 
 *
 *  Interface class to handle changes in the scanning state
 *
 *  @lib mpxfilehandler.lib
 *  @since S60 3.0
 */
class MMPXFileScanStateObserver 
    {
public: 
    
    // ENUMS
    enum TScanState
        {
        EScanNone,
        EScanFiles,
        ECleanupBrokenLink,
        EScanPlaylists,
        EScanMetadata
        };
public:

    /**
     * Handle a change in the scanning state
     *
     * @since S60 3.0
     * @param aState, state we are on
     * @param aErr, any error
     */
    virtual void HandleScanStateCompleteL( TScanState aState, TInt aErr ) = 0;

    };


#endif // MMPXFILESCANSTATEOBSERVER_H
