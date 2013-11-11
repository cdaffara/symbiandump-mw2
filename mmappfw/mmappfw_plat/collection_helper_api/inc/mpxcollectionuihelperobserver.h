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
* Description:  Collection Ui Helper common observer interface
*
*/


#ifndef MMPXCOLLECTIONUIHELPEROBSERVER_H
#define MMPXCOLLECTIONUIHELPEROBSERVER_H

#include <mpxmediageneraldefs.h>

/**
* Different operations handled by the collection helper
*/
enum TCHelperOperation
    {
    EAddOp,
    EDeleteOp,
    ESetOp,
    EEmbeddedOpenOp,
    EMoveOp,
    EExportPlaylistOp,
    ERenameOp,
    EReorderPlaylistOp,
    EDeleteStatusOp
    };

    
/**
* Generic Observer class for the ui helper
* A single callback function, but usable for all collection helper callbacks
*/
NONSHARABLE_CLASS( MMPXCHelperObserver )
    {
public:
    // ADD Operation
        /**
        * Handles the completion of adding a media event.
		* @since S60 3.2.3
        * @param aOperation = EAddOp.
        * @param aArgument a media object representing the added media.
        *        If this is the callback for adding a playlist media,
        *        the content of this object is the same as what's
        *        supplied in AddL, except that URI for the playlist
        *        is added to the object upon successful processing
        *        of AddL. Client should take over the ownership of
        *        this object. NULL if an error has occured during
        *        processing of AddL.
        * @param aErr the error code for AddL processing error.
        */

    // SET Operation
        /**
        * Handles the completion of setting a media event.
		* @since S60 3.2.3
        * @param aOperation = ESetOp.
        * @param aArgument a media object representing the updated media.
        *        Client takes over the ownershop of this object.
        * @param aErr the error code for SetL processing error.
        */

    // RENAME Operation
        /**
        * Handles the completion of renaming a media event.
		* @since S60 3.2.3
        * @param aOperation = EDeleteOp.
        * @param aArgument a media object representing the renamed media.
        * @param aErr the error code for RenameL processing error
        */
    virtual void HandleOperationCompleteL( TCHelperOperation aOperation,
                                           TInt aErr,
                                           void* aArgument ) = 0;
    };

/**
* Observer class for the ui helper, embedded open mode
*/
NONSHARABLE_CLASS( MMPXCHelperEmbeddedOpenObserver )
    {
public:
    /**
    * Callback to the observer for embedded OpenL() result.
	* @since S60 3.2.3
    * @param aErr error for the operation KErrNone if successful.
    * @param aCategory category of the file opened
    */
    virtual void HandleEmbeddedOpenL( TInt aErr, TMPXGeneralCategory aCategory ) = 0;
    };


#endif // MMPXCOLLECTIONUIHELPEROBSERVER_H
