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
* Description:  Public APIs of VmbxUiUtilities to internal class
*  Interface   : Internal, MVmbxUiutilities
*
*/


#ifndef M_VMBXUIUTILITIES_H
#define M_VMBXUIUTILITIES_H

// INCLUDES
#include <cvoicemailbox.h>
#include "voicemailboxdefsinternal.h"

// CLASS DECLARATION

/**
 *  MVmbxUiUtilities
 *
 *  @lib vmbxengine.lib
 *
 */
NONSHARABLE_CLASS ( MVmbxUiUtilities )
    {

public:
    /**
     * Show mailbox query
     *
     * @param in aType TVmbxType type
     * @param in aMode TVmbxQueryMode mode
     * @param out aNumber the voice mailbox number
     */
    virtual void ShowQueryL( const TVmbxType& aType,
                    const TVmbxQueryMode& aMode,
                    TDes& aNumber ) = 0;

    /**
     * Show mailbox query dialog
     * Leaves if user cancel selected.
     *
     * @param aType in TVmbxType type
     * @param aMode Tin VmbxQueryMode mode
     * @param aNumber the voice mailbox number
     * @param aResult out the result of dialog
     */
    virtual void ShowQueryDialogL( const TVmbxType& aType,
        const TVmbxQueryMode & aMode, TDes& aNumber, TInt& aResult ) = 0;

    /**
     * Show informationd note
     *
     * @param aType in TVmbxNoteType type
     */
    virtual void ShowInformationdNoteL(const TVmbxNoteType aType) = 0;


    /**
     * Show define number in selection dialog
     *
     * @param in aType vmbx type
     * @param out aResult the result of dialog
     */
    virtual void ShowDefineSelectionDialogL( TVmbxType& aType, TInt& aResult ) = 0;

    /**
     * Show call number in selection dialog
     * Leaves if user cancel selected.
     *
     * @param in aArray array of the defined voice mailbox entry
     * @param in aIcons icons of the defined voice mailbox
     * @param out aParams the type TVoiceMailboxParams which should include
     *          the service id and the type of seclected TVmbxType
     * @param out aResult the result user seclected
     */
    virtual void ShowCallSelectionDialogL( 
                const RPointerArray<CVoiceMailboxEntry>& aArray, 
                    TVoiceMailboxParams& aParams, TInt& aResult ) = 0;

    /**
     * Tries to close all open dialogs
     *
     */
    virtual void DismissDialogL() = 0;


    /**
     * Get Mailbox type default image.
     * If get unseccessful, Leaves with Symbian OS error code
     * 
     * @param aParams Mailbox type for which image is needed.
     * @return aImage New image. Ownership is transferred.
     */
    //virtual CGulIcon* GetVmbxImageL( const TVoiceMailboxParams& aParams ) = 0;

    /**
     * Show not allowed editing dialog
     *
     */
    virtual void ShowNotAllowedEditingDialogL() = 0;

    /**
     * Show Invalid Warning dialog.
     *
     */
    virtual void ShowInvalidWarningDialogL() = 0;
    };

#endif // M_VMBXUIUTILITIES_H
