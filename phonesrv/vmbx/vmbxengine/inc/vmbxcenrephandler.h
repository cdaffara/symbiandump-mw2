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
* Description:  Access class to central repository
*  Interface   : Private, CVmbxCenRepHandler
*
*/


#ifndef C_VMBXCENREPHANDLER_H
#define C_VMBXCENREPHANDLER_H

//  INCLUDES
#include <e32base.h>

#include <voicemailboxdefs.h>
#include "mvmbxcenrephandler.h"


// FORWARD DECLARATIONS
class CRepository;
class CVoiceMailboxEntry;

// CLASS DECLARATION
/**
*  Creates connection to central repository and handles data flow from/to it
*
*  @lib vmbxengine.lib
*/
NONSHARABLE_CLASS( CVmbxCenRepHandler ) : public CBase,
                                          public MVmbxCenrepHandler
    {
public: // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static CVmbxCenRepHandler* NewL( );

    /**
     * Destructor.
     */
    virtual ~CVmbxCenRepHandler();

public: // New functions

    /**
     * Get saved mailbox number
     *
     * @param aEntry saved vmbx number data
     * @return Symbian OS error code (KErrNone if successful)
     */
    TInt GetVmbxNumber( CVoiceMailboxEntry& aEntry );

    /**
     * Saves the mailbox number
     *
     * @param aEntry vmbx number data to save
     * @return Symbian OS error code (KErrNone if successful)
     */
    TInt Save( const CVoiceMailboxEntry& aEntry );

    /**
     * Fetches active store type
     *
     * @return aStoreType Active store in use
     */
    TVmbxMemoryLocation StoreType();

    /**
     * Returns state of the video support
     *
     * @return ETrue if supported
     */
    TBool VideoSupported();

    /**
     * Returns ETrue if CenRep setting Sim read only
     *
     * @return ETrue if CenRep setting Sim read only
     */
    TBool IsSimReadOnly();

     /**
     * Returns ETrue if number allowed to user change
     *
     * @return ETrue if from Sim
     */
    TBool IsAllowedUserEdit();

private: // New functions

    /**
     * C++ default constructor.
     * @param aStoreHandler Reference to StoreHandler
     */
    CVmbxCenRepHandler();

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

    /**
     * Resolves Voice CenRep key to be used
     *
     * @param in aLine current ALS lne
     * @return CenRep key
     */
    TUint32 VoiceCenRepKey( const TVmbxAlsLineType& aLine );

    /**
     * Resolves Video CenRep key to be used
     *
     * @param in aLine current ALS lne
     * @return CenRep key
     */
    TUint32 VideoCenRepKey( const TVmbxAlsLineType& aLine );

   /**
     * Resolves CenRep key to be used
     *
     * @param aEntry entry type
     * @return CenRep key
     */
    TUint32 ResolveKeyByEntry( const CVoiceMailboxEntry& aEntry );

private: //data

    /**
     * Central repository session
     * Own.
     */
    CRepository* iCRSession;
    };

#endif // C_VMBXCENREPHANDLER_H
