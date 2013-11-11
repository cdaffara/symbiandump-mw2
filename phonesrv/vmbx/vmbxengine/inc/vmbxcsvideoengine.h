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
* Description:  Video mailbox engine implementation
*  Interface   : Private, CVmbxCsVideoEngine
*
*/

#ifndef C_VMBXCSVIDEOENGINE_H
#define C_VMBXCSVIDEOENGINE_H

// INCLUDES
#include "mvmbxresourceprovider.h"

// FORWARD DECLARATIONS
class CVmbxEngineBase;


// CLASS DECLARATION

/**
*  Implements functionality specifig to video mailbox implementation
*
*  @lib vmbxengine.lib
*/
NONSHARABLE_CLASS( CVmbxCsVideoEngine ) : public CVmbxEngineBase
    {

public:  // Constructors and destructor

    /**
     * Two-phased constructor
     * @param in aProvider Reference to Ui Utilities
     * @return New instance of the object. Ownership transferred.
     */
    static CVmbxCsVideoEngine* NewL( MVmbxResourceProvider& aProvider );

    /**
     * Destructor.
     */
    virtual ~CVmbxCsVideoEngine();

// from base class CVmbxEntryBase

    /**
     * @see CVmbxEntryBase::GetL
     *
     * @param in aEntry a pointer reference of CVoiceMailboxEntry
     */
    void GetL( CVoiceMailboxEntry*& aEntry );

    /**
     * @see CVmbxEntryBase::SaveL
     *
     * @param aEntry Reference to CVoiceMailboxEntry
     * @param aShowNotesAllowed Provision mode is used to decide whether show
     *          saved notification UI dialog. If aShowNotesAllowed is EFalse, 
     *          saved notification UI dialog will not show. 
     */
    void SaveL( const CVoiceMailboxEntry& aEntry,
                TBool aShowNotesAllowed );

    /**
     * To save mailbox Provisioned entry value
     *
     * @param in aEntry Reference to CVoiceMailboxEntry
     */
    void SaveProvisionedEntryL( const CVoiceMailboxEntry& aEntry );
    
    /**
     * @see CVoiceMailbox::CheckConfiguration
     *
     * @param aParams Specifies which mailbox capabilities,
     *          the caller wants to check.
     * @param aFlags Use TVmbxFeatureCapabilityFlag values for this parameter.
     *          Specifies what features client wants to check.

     * @return True if feature(s) enabled.
     */
    TBool CheckConfiguration( const TVoiceMailboxParams& aParams,
                                 const TInt aFlags );

private:

    /**
     * C++ default constructor.
     *
     * @param in aProvider Reference to Ui Utilities
     */
    CVmbxCsVideoEngine( MVmbxResourceProvider& aProvider );

    /**
     * By default Symbian 2nd phase constructor is private.
     *
     */
    void ConstructL();

    };

#endif  // C_VMBXCSVIDEOENGINE_H
