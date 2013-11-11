/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  External, CVoiceMailboxEntry
*
*/

#ifndef C_VOICEMAILBOXENTRY_H
#define C_VOICEMAILBOXENTRY_H

#include <e32base.h>
#include <voicemailboxdefs.h>

/**
 *  Container class for Voice Mailbox info.
 *  @lib vmbxengine.lib
 */

class CVoiceMailboxEntry : public CBase
    {
public:

    /**
     * Static constructor.
     * @return New instance of the object. Ownership transferred.
     */
    IMPORT_C static CVoiceMailboxEntry* NewL();

    /**
     * Static constructor.
     * @return New instance of the object. Ownership transferred.
     */
    IMPORT_C static CVoiceMailboxEntry* NewLC();

    /**
     * Destructor
     *
     */
    virtual ~CVoiceMailboxEntry();

    /**
     * Gets the Voice Mailbox service.
     *
     * @return Mailbox service.
     */
    IMPORT_C virtual TServiceId ServiceId() const;

    /**
     * Sets the Voice Mailbox service to container.
     *
     * @param in aVmbxServiceId Voice Mailbox service.
     */
    virtual void SetServiceId(
                            const TServiceId& aVmbxServiceId );

    /**
     * Gets the Voice Mailbox type.
     *
     * @return Mailbox type.
     */
    IMPORT_C virtual TVmbxType VoiceMailboxType() const;

    /**
     * Sets the Voice Mailbox type to container.
     *
     * @param in aVmbxType Voice Mailbox type.
     */
    IMPORT_C virtual void SetVoiceMailboxType(
                            const TVmbxType& aVmbxType );

    /**
     * Gets the Voice Mailbox line type.
     *
     * @return Mailbox line type.
     *      EVmbxAlsLineDefault is returned if mailbox does not
     *      support several lines.
     */
    IMPORT_C virtual TVmbxAlsLineType VmbxAlsLineType() const;

    /**
     * Sets the Voice Mailbox line type to container.
     * EVmbxAlsLineDefault is used by default.
     *
     * @param in aLine Selected Voice Mailbox line type.
     */
    virtual void SetVmbxAlsLineType(
                            const TVmbxAlsLineType& aLine );

    /**
     * Gets the Voice Mailbox number or address.
     *
     * @param out aVmbxNumber If successful, contains the returned 
     *   mailbox number or address.
     * @return One of the system wide error codes.
     */
    IMPORT_C virtual TInt GetVmbxNumber( TPtrC& aVmbxNumber ) const;

    /**
     * Sets the Voice Mailbox number or address to container.
     *
     * @param in aVmbxNumber New Voice Mailbox number or address.
     * @return One of the system wide error codes.
     */
    IMPORT_C virtual TInt SetVmbxNumber( const TDesC& aVmbxNumber );

    /**
     * Gets the Voice Mailbox brand id.
     *
     * @param out aBrandId If successful, contains the returned Brand id.
     * @return One of the system wide error codes.
     */
    IMPORT_C virtual TInt GetBrandId( TPtrC8& aBrandId ) const;

    /**
     * Sets the Voice Mailbox Brand Id to container.
     *
     * @param in aBrandId New Brand Id.
     * @return One of the system wide error codes.
     */
    virtual TInt SetBrandId(
                            const TDesC8& aBrandId );

    /**
     * Gets the VoIp name.
     *
     * @param out aVmbxName If successful, contains the returned mailbox name.
     * @return One of the system wide error codes.
     */
    IMPORT_C virtual TInt GetVmbxName( TPtrC& aVmbxName ) const;

    /**
     * Sets the VoIp name to container.
     *
     * @param in aVmbxName New Voice Mailbox name.
     * @return One of the system wide error codes.
     */
    IMPORT_C virtual TInt SetVmbxName( const TDesC& aVmbxName );

    /**
     * Gets the type of memory location.
     *
     * @return Memory location type.
     */
    IMPORT_C virtual TVmbxMemoryLocation UsingMemoryLocation( ) const;

    /**
     * Resets the container to initial state.
     *
     */
    IMPORT_C virtual void Reset();

    /**
     * Sets the using memory location to entry.
     *
     * @param in aType Memory location type.
     */
    virtual void SetUsingMemoryLocation( 
                                     const TVmbxMemoryLocation& aType );

protected:

    /**
     * Second phase constructor.
     *
     */
    void ConstructL();

private:

     /**
     * Default constructor.
     *
     */

    CVoiceMailboxEntry();

private: // data

    /**
     * Mailbox service id.
     */
    TServiceId iVmbxServiceId;

    /**
     * Mailbox type.
     */
    TVmbxType iVmbxType;

    /**
     * Mailbox als line type.
     */
    TVmbxAlsLineType iVmbxLineType;

    /**
     * Number of vmbx.
     * Own.
     */
    HBufC* ivmbxNumber;

    /**
     * Brand id of vmbx.
     * Own.
     */
    HBufC8* iVmbxBrandId;

    /**
     * Name of vmbx.
     * Own.
     */
    HBufC* ivmbxName;

    /**
     * Mailbox Memory location.
     */
    TVmbxMemoryLocation iUsingMemory;

    };

#endif // C_VOICEMAILBOXENTRY_H
