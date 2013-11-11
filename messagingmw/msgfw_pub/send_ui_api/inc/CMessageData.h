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


#ifndef C_MESSAGEDATA_H
#define C_MESSAGEDATA_H

#include <e32base.h>
#include <badesca.h>
#include <f32file.h>
#include <msvstd.h>
#include "CMessageAddress.h"

class MDesC16Array;
class CRichText;
class CMessageDataImpl;

/**
 *  Encapsulates the message data for sending services.
 *
 *  Encapsulates the message data for sending services. Class supports
 *  externalazation and internalization, which are used to relay message 
 *  to SendUI server application.
 *
 *  @lib sendui
 *  @since S60 v3.0
 */ 
class CMessageData : public CBase
{
public:  // Constructors and destructor

   /**
    * Two-phased constructor.
    *
    * @return Pointer to object of CMessageData.
    */
    IMPORT_C static CMessageData* NewL();
    
   /**
    * Two-phased constructor.
    *
    * @return Pointer to object of CMessageData.
    */
    IMPORT_C static CMessageData* NewLC();

   /**
    * Destructor.
    */
    IMPORT_C virtual ~CMessageData();

public: // New functions

   /**
    * Set message subject.
    *
    * @since Series 60 3.0
    * @param aSubject Sets message subject. Does not take ownership.
    * @return None.
    */
    IMPORT_C void SetSubjectL( const TDesC* aSubject );

   /**
    * Returns message subject.
    *
    * @since Series 60 3.0
    * @return Message subject.
    */
    IMPORT_C const TPtrC Subject() const;

   /**
    * Appends attachment file path to the array of attachment file paths. Only a link 
    * to the attachment is stored and compared to AppendAttachmentHandleL less resources 
    * is used during the send operation.
    * 
    * NOTE: Do not add any temporary files as an attachment. Always use 
    * AppendAttachmentHandleL for temporary files.
    *
    * @since Series 60 3.0
    * @param aFilePath File path of the attachment. 
    *                  
    * @return None.
    */
    IMPORT_C void AppendAttachmentL( const TDesC& aFilePath );

   /**
    * Returns the array of attachment file paths.
    *
    * @since Series 60 3.0
    * @return Array of attachment file paths.
    */
    IMPORT_C const CDesCArray& AttachmentArray() const;
    
    /**
    * Clears the array of attachment file paths.
    *
    * @since Series 60 3.0
    * @return Array of attachment file paths.
    */        
    IMPORT_C void ClearAttachmentArray();

   /**
    * Encapsulates address and alias to CMessageAddress and adds to array.
    *
    * @since Series 60 3.0
    * @param aAddress Real address.
    * @param aAlias Alias for the real address.
    * @return None.
    */
    IMPORT_C void AppendToAddressL(
        const TDesC& aAddress,
        const TDesC& aAlias = KNullDesC);

   /**
    * Returns array of addresses and aliases.
    *
    * @since Series 60 3.0
    * @return Array of addresses and aliases.
    */
    IMPORT_C const CMessageAddressArray& ToAddressArray() const;

   /**
    * Encapsulates CC address and alias to CMessageAddress and adds to 
    * array.
    *
    * @since Series 60 3.0
    * @param aCcAddress Real CC-address.
    * @param aCcAlias Alias for the real CC-address.
    * @return None.
    */
    IMPORT_C void AppendCcAddressL(
        const TDesC& aCcAddress,
        const TDesC& aCcAlias = KNullDesC);

   /**
    * Returns array of CC addresses and aliases.
    *
    * @since Series 60 3.0
    * @return Array of cc addresses and aliases.
    */
    IMPORT_C const CMessageAddressArray& CcAddressArray() const;

   /**
    * Encapsulates Bcc address and alias to CMessageAddress and adds to
    * array.
    *
    * @since Series 60 3.0
    * @param aBccAddress Real Bcc-address.
    * @param aBccAlias Alias for the real Bcc-address.
    * @return None.
    */
    IMPORT_C void AppendBccAddressL(
        const TDesC& aBccAddress,
        const TDesC& aBccAlias = KNullDesC);

   /**
    * Returns array of Bcc addresses and aliases.
    *
    * @since Series 60 3.0
    * @return Array of Bcc addresses and aliases.
    */
    IMPORT_C const CMessageAddressArray& BccAddressArray() const;

   /**
    * Set message body text.
    *
    * @since Series 60 3.0
    * @param aBodyText Message body text.
    * @return None.
    */
    IMPORT_C void SetBodyTextL( const CRichText* aBodyText );

   /**
    * Returns message body text.
    *
    * @since Series 60 3.0
    * @return Message body text.
    */
    IMPORT_C const CRichText* BodyText() const;
    
   /**
    * Appends attachment handle to the array of attachment handles. 
    *
    * NOTE: Attachment handles should be opened using ShareProtected()
    * file session, otherwise handles can not be transfered through
    * client-server interface.
    *
    * NOTE: When adding a temporary file as an attachment, always use this method,
    *
    * @since Series 60 3.0
    * @param aFileHandle File handle of the attachment.
    * @return None.
    */
    IMPORT_C void AppendAttachmentHandleL( const RFile& aFileHandle );

    /**
    * Returns the array of attachment handles.
    *
    * @since Series 60 3.0
    * @return Array of attachment handles.
    */
    IMPORT_C const RArray<RFile>& AttachmentHandleArray() const;

   /**
    * Returns data type id.
    *
    * This can be used for interpreting the opaque data.
    * @since Series 60 3.0
    * @return Data type id.
    */
    IMPORT_C TUid DataType() const;

   /**
    * Clears all addressee arrays.
    *
    * @since Series 60 3.0
    * @return None.
    */ 
    IMPORT_C void ClearAddresses();

   /**
    * Externalizes message data to a stream
    *
    * @since Series 60 3.2
    * @return None.
    */ 
   
   IMPORT_C void ExternalizeL( RWriteStream& aStream ) const;
    
   /**
    * Internalizes message data to from a stream
    *
    * @since Series 60 3.2
    * @return None.
    */ 
    IMPORT_C void InternalizeL( RReadStream& aStream );

   /**
    * Set opaque data
    *
    * @since Series 60 3.2
    * @param aData Opaque data.
    * @param aDataType, Identifier for the opaque data.
    * @return None.
    */
    IMPORT_C void SetOpaqueDataL( const TDesC8* aData, TUid aDataType );
    
   /**
    * Get opaque data
    *
    * @since Series 60 3.2
    * @return aData Opaque data.
    */
    IMPORT_C const TPtrC8 OpaqueData() const;

 protected:

   /**
    * C++ default constructor.
    */
    IMPORT_C CMessageData();

   /**
    * By default Symbian 2nd phase constructor is private.
    */
    IMPORT_C void ConstructL();

protected:  // Data

    /**
     * Data type of this message. Normally this is KSenduiMsgDataBase
     * This can be used for interpreting the opaque data.
     */
    TUid iDataType;

private:  // Data

    /**
     *  Message data implementation.
     *  Owned.
     */
    CMessageDataImpl* iMessageDataImpl;

    /**
     *  Reserved.
     */
    TAny* iReserved1;
    TAny* iReserved2;
    TAny* iReserved3;
    TAny* iReserved4;
    TAny* iReserved5;
    RArray<TAny*> iReserved6;

};

#endif      // C_MESSAGEDATA_H

// End of File

