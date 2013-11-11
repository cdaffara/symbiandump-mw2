/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the CCbsRecDecoder class
*
*/


#ifndef CCBSRECDECODER_H
#define CCBSRECDECODER_H

//  INCLUDES

#include <e32base.h>
#include <f32file.h>

// FORWARD DECLARATIONS

class CCnvCharacterSetConverter;
class CCbsRecMessage;
class CCbsRecWcdmaMessage;
class CCbsMessage;

// CLASS DECLARATION

/**
*   CCbsRecDecoder handles decoding of message contents.
*
*   CbsServer receives messages stored in 8-bit descriptors
*   from ETel. This class converts them into 16-bit descriptors
*   and decodes the message representation into UCS-2 (which
*   is used by Symbian OS internally). 
*  
*/
class CCbsRecDecoder : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        *   Two-phased constructor.
        *
        *   @returns                    New instance of CCbsRecDecoder.
        */
         static CCbsRecDecoder* NewL();
        
        /**
        *   Destructor.
        */
        virtual ~CCbsRecDecoder();

    public:  // New functions

        /**
        *   Decodes message page into Unicode representation.
        *
        *   If the message has a language indication prefixed
        *   in the message body, the indication is removed.
        *
        *   Compressed messages are not supported.
        *   
        *   Leave reasons:
        *   KErrNotSupported    Message compressed or of unsupported character
        *                       coding.
        *   KErrNoMemory        OOM.
        *   KErrGeneral         Failed due to other reasons.
        *
        *   @param aMessage Message page.
        */
        void DecodeL( CCbsMessage& aMessage );
      
    private:  // New functions

        // Default constructor.
        CCbsRecDecoder();

        // Prohibited copy constructor.
        CCbsRecDecoder( const CCbsRecDecoder& );

        // Prohibited assignment operator.
        CCbsRecDecoder& operator=( const CCbsRecDecoder& );

        /**
        *   2nd-phase constructor. 
        */
        void ConstructL();

        /**
        *   Decodes the given message's content.
        *
        *   @param aMessage             CB message
        */
        void DoDecodeL( CCbsMessage& aMessage );

        /**
        *   Decodes 8-bit and Unicode message representations.
        *
        *   @param aMsg                 CB message encoded in UCS2
        */
        void UnicodeDecodeL( CCbsMessage& aMsg );

        /**
        *   Decodes 7-bit message representation.
        *
        *   @param aMsg                 CB message encoded in 7-bit
        */
        void DefaultAlphabetDecodeL( CCbsMessage& aMsg );
        
        /**
        *   Decodes 8-bit message representation.
        *
        *   @param aMsg                 CB message encoded in 8-bit
        */
        void EightbitAlphabetDecodeL( CCbsMessage& aMsg );

        /**
        *   Removes the trailing CRs from a message.
        *
        *   @param aText                Text from which the CR is to be removed
        */
        TPtrC RemoveTrailingCR( const TDesC& aText );
        
        /**
        *   Removes trailing CRs and LFs from a 7-bit Cell info message.
        *
        *   @param aText                Text from which the CR is to be removed
        *   @return                     Text with no CRs or LFs        
        */
        TPtrC8 RemoveTrailingControlChars( const TDesC8& aText );
        
   private:    // Data
        // Own: character converter for 8-bit and UCS2 encodings
        CCnvCharacterSetConverter* iConverter;

        // File server session
        RFs iFs;
    };

#endif      // CBSRECDECODER_H
            
// End of File
