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
* Description:  Handler ui related.
*  Interface   : Private, CVmbxUiHandler
*
*/

#ifndef VMBXENGINEQTUI_H
#define VMBXENGINEQTUI_H


#include <voicemailboxdefs.h>
#include "mvmbxuihandler.h"

// FORWARD DECLARATION
class VmbxQtUiHandler;

// CLASS DECLARATION

/**
*
*  @lib qvmbxengine.lib
*/
NONSHARABLE_CLASS( CVmbxUiHandler ):public CBase,
                     public MVmbxUiHandler
    {

public: // Constructors and destructor
    /**
     * Two-phased constructor
     */
    static CVmbxUiHandler* NewL();

    /**
    * Destructor.
    */
     ~CVmbxUiHandler();

//From MVmbxUiHandler

    /**
     * Show mailbox query dialog
     * Leaves if user cancel selected.
     *
     * @param aType in TVmbxType type
     * @param aNumber the voice mailbox number
     * @param aResult out the result of dialog
     */
    virtual void ShowVmbxQueryDialogL( const TVmbxType& aType,
        TDes& aNumber, TInt& aResult );

    /**
     * Show define number in selection dialog
     *
     * @param in aType vmbx type
     * @param out aResult the result of dialog
     */
    virtual void ShowDefineSelectionDialogL( TVmbxType& aType, TInt& aResult );

    /**
     * Show call number in selection dialog
     * Leave if aArray < 1
     *
     * @param in aArray array of the defined voice mailbox entry
     * @param in aIcons icons of the defined voice mailbox
     * @param out aParams the type TVoiceMailboxParams which should include
     *          the service id and the type of seclected TVmbxType
     * @param out aResult the result user seclected
     */
    virtual void ShowCallSelectionDialogL(
                const RPointerArray<CVoiceMailboxEntry>& aArray,
                //CArrayPtr<CGulIcon>* aIcons,
                TVoiceMailboxParams& aParams, TInt& aResult );

    /**
     * Show informationd note
     *
     * @param aType in TVmbxNoteType type
     */
    virtual void ShowInformationdNoteL(const TVmbxNoteType aType);

    /**
     * show an error dialog for invalid number
     *
     */
    virtual void ShowInvalidWarningNoteL();

private:

    /** C++ default constructor.
    *
    */
    CVmbxUiHandler();

    /**
     * C++  Two-phased constructor.
     */
    void ConstructL();

private:  // data

    /**
     * Dialog manage
     * Own
     */
    VmbxQtUiHandler* iHandler;
    };

#endif  // VMBXENGINEQTUI_H
