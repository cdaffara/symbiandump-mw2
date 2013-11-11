/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
*  Interface   : Internal, MVmbxUiHandler
*
*/


#ifndef M_MVMBXUIHANDLER_H
#define M_MVMBXUIHANDLER_H

// INCLUDES

// FORWARD DECLARATION


// CLASS DECLARATION

/**
 *  MVmbxUiHandler
 *
 *  @lib vmbxengine.lib
 *
 */
NONSHARABLE_CLASS ( MVmbxUiHandler )
    {

public:

    /**
     * Show mailbox query dialog
     *
     * @param aType in TVmbxType type
     * @param aNumber the voice mailbox number
     * @param aResult out the result of dialog
     */
    virtual void ShowVmbxQueryDialogL( const TVmbxType& aType,
        TDes& aNumber, TInt& aResult )= 0;


    /**
     * Show define number in selection dialog
     *
     * @param in aType vmbx type
     * @param out aResult the result of dialog
     */
    virtual void ShowDefineSelectionDialogL( 
        TVmbxType& aType, TInt& aResult ) = 0;

    /**
     * Show call number in selection dialog
     * Leave if aArray < 1
     *
     * @param in aArray array of the defined voice mailbox entry
     * @param out aParams the type TVoiceMailboxParams which should include
     *          the service id and the type of seclected TVmbxType
     * @param out aResult the result user seclected
     */
    virtual void ShowCallSelectionDialogL(
                const RPointerArray<CVoiceMailboxEntry>& aArray,
                TVoiceMailboxParams& aParams, TInt& aResult ) = 0;

    /**
     * show an error dialog for invalid number
     *
     */
    virtual void ShowInvalidWarningNoteL() = 0;

    };

#endif // M_MVMBXUIHANDLER_H
