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
* Description:
*    Provides different utility functions for CBSUI.
*
*/


#ifndef CBSUIUTILITY_H
#define CBSUIUTILITY_H

//  INCLUDES

// FORWARD DECLARATIONS
class MSaveMessageController;
class MCbs;


// CLASS DECLARATION

/**
* Offers utility functions for CBSUI.
*
*/
class CbsUiUtility
    {

    public: // New functions
        
        /**
        * Shows confirmation note with text accordingly to aResourceId.
        *
        * @param aResourceId resource id
        * @param aWaitingDialog if ETrue dialog waits before continues
        */
        LOCAL_C void ShowConfirmationNoteL(
            const TInt aResourceId, TBool aWaitingDialog = EFalse );

        /**
        * Shows error note with text accordingly to aResourceId.
        *
        * @param aResourceId resource id
        */
        LOCAL_C void ShowErrorNoteL( const TInt aResourceId );

        /**
        * Shows warning note with text accordingly to aResourceId.
        *
        * @param aResourceId resource id
        */
        LOCAL_C void ShowWarningNoteL( const TInt aResourceId );

        /**
        * Shows a note accordingly to parameters.
        * 
        * @param aResource note resource id
        * @param aTimeout duration
        * @param aTone sound
        */
        /*LOCAL_C void ShowNoteL( const TInt aResource, 
                                const CAknNoteDialog::TTimeout aTimeout,
                                const CAknNoteDialog::TTone aTone );*/

        /**
        * Returns a new navigation pane. Remember to delete a memory area!
        *
        * @return new empty decorated label
        */
        //LOCAL_C CAknNavigationDecorator* CreateEmptyNavigationPaneL();

        /**
        * Returns ETrue if message is saved
        *
        * @param aHandle message to be checked
        * @param aServer is needed to get information
        * @return ETrue if current message is saved
        */
        LOCAL_C TBool IsSavedL( const TCbsMessageHandle aHandle, MCbs& aServer );

        /**
        * Saves the message and shows a error message in case
        * something went wrong.
        *
        * @param aHandle message to be saved
        * @param aSaver saves the message and shows global error note if needed
        * @return Error code
        */
        LOCAL_C TInt SaveCurrentMessageL(
            const TCbsMessageHandle aHandle, 
            MSaveMessageController& aSaver );

    };

#endif      // CBSUIUTILITY_H

// End of File
