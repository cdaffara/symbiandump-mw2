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
* Description:  Voice mailbox engine implementation
*  Interface   : Private, CVmbxCsVoiceEngine
*
*/

#ifndef C_VMBXCSVOICEENGINE_H
#define C_VMBXCSVOICEENGINE_H

// INCLUDES
#include "mvmbxresourceprovider.h"

// FORWARD DECLARATIONS
class CVmbxEngineBase;
class CVmbxSimHandler;


// CLASS DECLARATION
/**
*  Implements functionality specifig to voice mailbox entry
*
*  @lib vmbxengine.lib
*/
NONSHARABLE_CLASS( CVmbxCsVoiceEngine ) : public CVmbxEngineBase
    {

public:  // Constructors and destructor

    /**
     * Two-phased constructor
     * @param aProvider Reference to MVmbxResourceProvider.
     * @return New instance of the object. Ownership transferred.
     */
    static CVmbxCsVoiceEngine* NewL( MVmbxResourceProvider& aProvider );

    /**
     * Destructor.
     */
    virtual ~CVmbxCsVoiceEngine();

// from base class CVmbxEntryBase

    /**
     * @see CVmbxEntryBase::GetL
     *
     * @param aEntry Reference to CVoiceMailboxEntry
     */
    void GetL( CVoiceMailboxEntry*& aEntry );

    /**
     * @see CVmbxEntryBase::SaveL
     *
     * @param aEntry 
     * @param aShowNotesAllowed Provision mode is used to decide whether show
     *          saved notification UI dialog. If aShowNotesAllowed is EFalse,
     *          saved notification UI dialog will not show. 
     */
    void SaveL( const CVoiceMailboxEntry& aEntry, 
                TBool aShowNotesAllowed );

    /**
     * To save mailbox Provisioned entry value
     *
     * @param aEntry Provisioned Entry data
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

    /**
     * Check Entry writable
     *
     * @param aParams params of Entry data
     * @return ETrue if entry is writable
     */
    TBool IsWritable( const TVoiceMailboxParams& aParams );
 
private:

    /**
     * C++ default constructor.
     *
     * @param aProvider Reference to MVmbxResourceProvider
     */
    CVmbxCsVoiceEngine( MVmbxResourceProvider& aProvider );

    /**
     * By default Symbian 2nd phase constructor is private.
     *
     */
    void ConstructL();

    /**
     * Queries where to save and then saves
     *
     * @param in aEntry Entry data
     * @param aShowNotesAllowed Provision mode is used to decide whether show
     *          saved notification UI dialog. If aShowNotesAllowed is EFalse,
     *          saved notification UI dialog will not show. 
     */
    void SaveEntryToPhoneL( const CVoiceMailboxEntry& aEntry,
                            TBool aShowNotesAllowed );

    /**
     * For saving to place that user selects
     *
     * @param in aEntry Entry data
     * @param aShowNotesAllowed Provision mode is used to decide whether show
     *          saved notification UI dialog. If aShowNotesAllowed is EFalse,
     *          saved notification UI dialog will not show. 
     */
    void SaveEntryToSimL( const CVoiceMailboxEntry& aEntry, 
                          TBool aShowNotesAllowed );

    /**
     * Returns Sim writable or not
     *
     * @return ETrue if writable
     */
    TBool IsSimWritable();

private: // data

    /**
     * CVmbxSimHandler
     * Own.
     */
    CVmbxSimHandler* iSimHandler;
    };

#endif  // C_VMBXCSVOICEENGINE_H
