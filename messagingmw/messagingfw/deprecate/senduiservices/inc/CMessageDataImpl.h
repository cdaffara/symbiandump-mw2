/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Encapsulates the message data for sending services.
*
*/


#ifndef C_MESSAGEDATAIMPL_H
#define C_MESSAGEDATAIMPL_H

#include <e32base.h>
#include <badesca.h>
#include <f32file.h>
#include <msvstd.h>
#include <CMessageAddress.h>

class MDesC16Array;
class CRichText;

/**
*  Encapsulates the message data for sending services.
*
*  Encapsulates the message data for sending services. Class supports
*  externalazation and internalization, which are used to relay message 
*  to SendUI server application.
*
*  @lib sendui
*  @since S60 v3.2
*/ 
NONSHARABLE_CLASS( CMessageDataImpl ) : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @return Pointer to object of CMessageDataImpl.
        */
        static CMessageDataImpl* NewL();

        /**
        * Two-phased constructor.
        *
        * @return Pointer to object of CMessageDataImpl.
        */
        static CMessageDataImpl* NewLC();

        /**
        * Destructor.
        */
        virtual ~CMessageDataImpl();

    public: // New functions

        /**
        * Set message subject.
        *
        * @since Series 60 3.0
        * @param aSubject Sets message subject. Does not take ownership.
        * @return None.
        */
        void SetSubjectL( const TDesC* aSubject );

        /**
        * Returns message subject.
        *
        * @since Series 60 3.0
        * @return Message subject.
        */
        const TPtrC Subject() const;

        /**
        * Appends attachment file path to the array of attachment file paths. Only a link 
        * to the attachment is stored and compared to AppendAttachmentHandleL less resources 
        * is used during the send operation.
        * 
        * NOTE: Do not add any temporary files as an attachment. Always use 
        * AppendAttachmentHandleL for temporary files.
        *
        * @since Series 60 3.2
        * @param aFilePath File path of the attachment. 
        *                  
        * @return None.
        */
        void AppendAttachmentL( const TDesC& aFilePath );

        /**
        * Returns the array of attachment file paths.
        *
        * @since Series 60 3.2
        * @return Array of attachment file paths.
        */
        const CDesCArray& AttachmentArray() const;

        /**
        * Clears the array of attachment file paths.
        *
        * @since Series 60 3.2
        * @return Array of attachment file paths.
        */        
        void ClearAttachmentArray();

        /**
        * Encapsulates address and alias to CMessageAddress and adds to array.
        *
        * @since Series 60 3.2
        * @param aAddress Real address.
        * @param aAlias Alias for the real address.
        * @return None.
        */
        void AppendToAddressL(
            const TDesC& aAddress,
            const TDesC& aAlias = KNullDesC);

        /**
        * Returns array of addresses and aliases.
        *
        * @since Series 60 3.2
        * @return Array of addresses and aliases.
        */
        const CMessageAddressArray& ToAddressArray() const;

        /**
        * Encapsulates CC address and alias to CMessageAddress and adds to 
        * array.
        *
        * @since Series 60 3.2
        * @param aCcAddress Real CC-address.
        * @param aCcAlias Alias for the real CC-address.
        * @return None.
        */
        void AppendCcAddressL(
            const TDesC& aCcAddress,
            const TDesC& aCcAlias = KNullDesC);

        /**
        * Returns array of CC addresses and aliases.
        *
        * @since Series 60 3.2
        * @return Array of cc addresses and aliases.
        */
        const CMessageAddressArray& CcAddressArray() const;

        /**
        * Encapsulates Bcc address and alias to CMessageAddress and adds to
        * array.
        *
        * @since Series 60 3.2
        * @param aBccAddress Real Bcc-address.
        * @param aBccAlias Alias for the real Bcc-address.
        * @return None.
        */
        void AppendBccAddressL(
            const TDesC& aBccAddress,
            const TDesC& aBccAlias = KNullDesC);

        /**
        * Returns array of Bcc addresses and aliases.
        *
        * @since Series 60 3.2
        * @return Array of Bcc addresses and aliases.
        */
        const CMessageAddressArray& BccAddressArray() const;

        /**
        * Set message body text.
        *
        * @since Series 60 3.2
        * @param aBodyText Message body text.
        * @return None.
        */
        void SetBodyTextL( const CRichText* aBodyText );

        /**
        * Returns message body text.
        *
        * @since Series 60 3.2
        * @return Message body text.
        */
        const CRichText* BodyText() const;

        /**
        * Appends attachment handle to the array of attachment handles. 
        *
        * NOTE: Attachment handles should be opened using ShareProtected()
        * file session, otherwise handles can not be transfered through
        * client-server interface.
        *
        * NOTE: When adding a temporary file as an attachment, always use this method,
        *
        * @since Series 60 3.2
        * @param aFileHandle File handle of the attachment.
        * @return None.
        */
        void AppendAttachmentHandleL( const RFile& aFileHandle );

        /**
        * Returns the array of attachment handles.
        *
        * @since Series 60 3.2
        * @return Array of attachment handles.
        */
        const RArray<RFile>& AttachmentHandleArray() const;

        /**
        * Returns data type id.
        *
        * This is used for static casting.
        * @since Series 60 3.2
        * @return Data type id.
        */
        TUid DataType() const;

        /**
        * Clears all addressee arrays.
        *
        * @since Series 60 3.2
        * @return None.
        */ 
        void ClearAddresses();

        /**
        * Externalizes message data to a stream
        *
        * @since Series 60 3.2
        * @return None.
        */ 

        void ExternalizeL( RWriteStream& aStream ) const;

        /**
        * Internalizes message data to from a stream
        *
        * @since Series 60 3.2
        * @return None.
        */ 
        void InternalizeL( RReadStream& aStream );
        
        /**
        * Set opaque data
        *
        * @since Series 60 3.2
        * @param aData Opaque data.
        * @return None.
        */
        void SetOpaqueDataL( const TDesC8* aData );
        
        /**
        * Get opaque data
        *
        * @since Series 60 3.2
        * @return aData Opaque data.
        */
        const TPtrC8 OpaqueData() const;

    private:

        /**
        * C++ default constructor.
        */
        CMessageDataImpl();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Externalizes an address array
        *
        * @since Series 60 3.2
        * @param anArray Addresses to be externalized
        * @param aStream Destination stream
        * @return None.
        */ 
        void ExternalizeArrayL(   CMessageAddressArray* anArray,    RWriteStream& aStream ) const;

        /**
        * Internalizes an address array
        *
        * @since Series 60 3.2
        * @param anArray Destination of the internalized addresses
        * @param aStream Source stream
        * @return None.
        */ 
        void InternalizeArrayL( CMessageAddressArray* anArray,  RReadStream& aStream );

        /**
        * Creates temporary path
        */
        void CreateTempPathL( TDes16& aPath, RFs& aFs ) const;

    protected:  // Data

        /**
        * Data type of this message. Normally this is KSenduiMsgDataBase
        */
        TUid                            iDataType;

        private:  // Data

        /**
        *  Message subject. 
        *  Owned.
        */
        HBufC*                          iSubject;

        /**
        *  Message body text. 
        *  Owned.
        */
        CRichText*                      iBodyText;

        /**
        *  Attachment filepaths. 
        *  Owned.
        */
        CDesCArray*                     iAttachments;

        /**
        *  Array of real addresses and aliases. 
        *  Owned.
        */
        CMessageAddressArray*           iToAddresses;

        /**
        *  Array of real CC addresses and aliases. 
        *  Owned.
        */
        CMessageAddressArray*           iCcAddresses;

        /**
        *  Array of real BC addresses and aliases.
        *  Owned.
        */
        CMessageAddressArray*           iBccAddresses;

        /**
        *  Attachment handles. 
        *  Owned.
        */
        RArray<RFile>                   iAttachmentHandles;
        
        /**
        *  Opaque data. Interpretation depends on service handler.
        *  Owned.
        */
        HBufC8*                          iOpaqueData;
    };

#endif      // C_MESSAGEDATAIMPL_H

// End of File

