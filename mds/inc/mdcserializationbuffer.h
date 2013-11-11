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
* Description:  Serialize and deserialize data between client and server
*
*/

#ifndef __MDCSERIALIZATIONBUFFER_H__
#define __MDCSERIALIZATIONBUFFER_H__

// INCLUDE FILES
#include <e32base.h>
#include "mdscommoninternal.h"

class TMdCSerializationType;

typedef TUint16 TSerializedDesLength;

const TUint8 KPadding8 = 0xBD;
const TInt32 KSerializedDesMaxLength = KMaxTUint16;
const TUint32 KNoOffset = 0;

/**
* An automatic serializtion buffer class. 
* The class provides insertion functionality for multiple variable types.
*/
NONSHARABLE_CLASS(CMdCSerializationBuffer): public CBase
    {
    public:

		#ifndef _DEBUG
			const static TUint32 KRequiredSizeForTypeCheck = 0;
		#else
			const static TUint32 KRequiredSizeForTypeCheck = sizeof( TUint16 );
		#endif

    	const static TUint32 KRequiredSizeForTBool   = sizeof( TInt32 ) + 2*KRequiredSizeForTypeCheck;
    	const static TUint32 KRequiredSizeForTInt8   = sizeof( TInt16 ) + 2*KRequiredSizeForTypeCheck;
    	const static TUint32 KRequiredSizeForTUint8  = sizeof( TUint16 ) + 2*KRequiredSizeForTypeCheck;
    	const static TUint32 KRequiredSizeForTInt16  = sizeof( TInt16 ) + KRequiredSizeForTypeCheck;
    	const static TUint32 KRequiredSizeForTUint16 = sizeof( TUint16 ) + KRequiredSizeForTypeCheck;
    	const static TUint32 KRequiredSizeForTInt32  = sizeof( TInt32 ) + KRequiredSizeForTypeCheck;
    	const static TUint32 KRequiredSizeForTUint32 = sizeof( TUint32 ) + KRequiredSizeForTypeCheck;
    	const static TUint32 KRequiredSizeForTInt64  = sizeof( TInt64 ) + KRequiredSizeForTypeCheck;
    	const static TUint32 KRequiredSizeForTReal32 = sizeof( TReal32 ) + KRequiredSizeForTypeCheck;
    	const static TUint32 KRequiredSizeForTReal64 = sizeof( TReal64 ) + KRequiredSizeForTypeCheck;
    	const static TUint32 KRequiredSizeForTTime   = sizeof( TInt64 ) + KRequiredSizeForTypeCheck;

		// artificial types
		const static TUint32 KRequiredSizeForTItemId   = sizeof( TItemId ) + KRequiredSizeForTypeCheck;
		const static TUint32 KRequiredSizeForTDefId    = sizeof( TDefId ) + KRequiredSizeForTypeCheck;
		const static TUint32 KRequiredSizeForEmptyText = sizeof( TSerializedDesLength ) + KRequiredSizeForTypeCheck;
		const static TUint32 KRequiredSizeForTMdCValueUnion = sizeof( TMdCValueUnion ) + KRequiredSizeForTypeCheck;

    public: // public definitions
    	/**
        * Returns required buffer size for given 8-bit descriptor
        * @param aDes 8-bit descriptor
        * @return required size for descriptor (descriptor's length is limited to KSerializedDesMaxLength)
    	*/
    	IMPORT_C static TUint32 RequiredSize( const TDesC8& aDes );

    	/**
        * Returns required buffer size for given 16-bit descriptor
        * @param aDes 16-bit descriptor
        * @return required size for descriptor (descriptor's length is limited to KSerializedDesMaxLength)
    	*/
    	IMPORT_C static TUint32 RequiredSize( const TDesC16& aDes );
    
        /**
        * Static constructor
        * @param aSize size of the buffer
        * @return buffer instance
        */
        IMPORT_C static CMdCSerializationBuffer* NewL( TInt32 aSize );

        /**
        * Static constructor
        * @param aSize size of the buffer
        * @return buffer instance
        */
        IMPORT_C static CMdCSerializationBuffer* NewLC( TInt32 aSize );

        /**
         * Static constructor
         * @param aBuffer pointer to the buffer
         * @param aSize size of the buffer
         * @return buffer instance
         */
        IMPORT_C static CMdCSerializationBuffer* NewL( TUint8* aBuffer, TInt32 aSize );

        /**
         * Static constructor
         * @param aBuffer pointer to the buffer
         * @param aSize size of the buffer
         * @return buffer instance
         */
        IMPORT_C static CMdCSerializationBuffer* NewLC( TUint8* aBuffer, TInt32 aSize );

        /**
         * Static constructor
         * @param aMessage received message
         * @param aIndex index of wanted message
         * @return buffer instance
         */
        IMPORT_C static CMdCSerializationBuffer* NewL(const RMessage2 &aMsg, TInt aIndex );
         
        /**
         * Static constructor
         * @param aMessage received message
         * @param aIndex index of wanted message
         * @return buffer instance
         */
        IMPORT_C static CMdCSerializationBuffer* NewLC(const RMessage2 &aMsg, TInt aIndex);

        /**
         * Static constructor
         * @param aBuffer buffer to copy data from
         * @return buffer instance
         */
        IMPORT_C static CMdCSerializationBuffer* NewL( const CMdCSerializationBuffer& aBuffer );

        /**
         * Static constructor
         * @param aBuffer buffer to copy data from
         * @return buffer instance
         */
        IMPORT_C static CMdCSerializationBuffer* NewLC( const CMdCSerializationBuffer& aBuffer );

      

        virtual ~CMdCSerializationBuffer();

        /**
        * Get pointer to serialized buffer.
        * @return serialized buffer pointer
        */
		IMPORT_C TPtr8 Buffer() const;

        /**
        * Get pointer to serialized buffer.
        * @return serialized buffer pointer
        */
		IMPORT_C TPtr8* BufferPtr() const;

        /**
        * Get pointer to serialized buffer.
        * @return serialized buffer pointer
        */
		IMPORT_C const TPtr8* BufferConstPtr() const;

    	/**
    	* Return size of the buffer.
    	* @return size of the buffer
    	*/
    	IMPORT_C TUint32 Size() const;

        /**
        * Change position of the serialized buffer.
        * @param aPosition value to be insert to current position
        * @return new buffer position
        */
		IMPORT_C void PositionL( TUint32 aPosition );

        /**
        * Get current position of the serialized buffer.
        * @return current buffer position
        */
		IMPORT_C TUint32 Position() const;

        /**
        * Insert TBool value to serialized buffer and move buffer's position 
        * to after inserted value.
        * @param aValue value to be insert to current position
        * @return new buffer position
        */
		IMPORT_C TUint32 InsertL(TBool aValue);

        /**
        * Insert TInt8 value to serialized buffer and move buffer's position 
        * to after inserted value.
        * @param aValue value to be insert to current position
        * @return new buffer position
        */
		IMPORT_C TUint32 InsertL(TInt8 aValue);

        /**
        * Insert TUint8 value to serialized buffer and move buffer's position 
        * to after inserted value.
        * @param aValue value to be insert to current position
        * @return new buffer position
        */
		IMPORT_C TUint32 InsertL(TUint8 aValue);

        /**
        * Insert TInt16 value to serialized buffer and move buffer's position 
        * to after inserted value.
        * @param aValue value to be insert to current position
        * @return new buffer position
        */
		IMPORT_C TUint32 InsertL(TInt16 aValue);

        /**
        * Insert TUint16 value to serialized buffer and move buffer's position 
        * to after inserted value.
        * @param aValue value to be insert to current position
        * @return new buffer position
        */
		IMPORT_C TUint32 InsertL(TUint16 aValue);

        /**
        * Insert TInt32 value to serialized buffer and move buffer's position 
        * to after inserted value.
        * @param aValue value to be insert to current position
        * @return new buffer position
        */
		IMPORT_C TUint32 InsertL(TInt32 aValue);

        /**
        * Insert TUint32 value to serialized buffer and move buffer's position 
        * to after inserted value.
        * @param aValue value to be insert to current position
        * @return new buffer position
        */
		IMPORT_C TUint32 InsertL(TUint32 aValue);

        /**
        * Insert TUint value to serialized buffer and move buffer's position 
        * to after inserted value.
        * @param aValue value to be insert to current position
        * @return new buffer position
        */
		IMPORT_C TUint32 InsertL(TUint aValue);

        /**
        * Insert TInt64 value to serialized buffer and move buffer's position 
        * to after inserted value.
        * @param aValue value to be insert to current position
        * @return new buffer position
        */
		IMPORT_C TUint32 InsertL(TInt64 aValue);

        /**
        * Insert TReal32 value to serialized buffer and move buffer's position 
        * to after inserted value.
        * @param aValue value to be insert to current position
        * @return new buffer position
        */
		IMPORT_C TUint32 InsertL(TReal32 aValue);

        /**
        * Insert TReal64 value to serialized buffer and move buffer's position 
        * to after inserted value.
        * @param aValue value to be insert to current position
        * @return new buffer position
        */
		IMPORT_C TUint32 InsertL(TReal64 aValue);

        /**
        * Insert TTime value to serialized buffer and move buffer's position 
        * to after inserted value.
        * @param aValue value to be insert to current position
        * @return new buffer position
        */
		IMPORT_C TUint32 InsertL(TTime aValue);

        /**
        * Insert TDesC8 value to serialized buffer and move buffer's position 
        * to after inserted value.
        * @param aValue value to be insert to current position
        * @return new buffer position
        */
		IMPORT_C TUint32 InsertL(TDesC8& aValue);

        /**
        * Insert TDesC16 value to serialized buffer and move buffer's position 
        * to after inserted value.
        * @param aValue value to be insert to current position
        * @return new buffer position
        */
		IMPORT_C TUint32 InsertL(TDesC16& aValue);

		/**
         * Insert TMdCValueUnion value to serialized buffer and move buffer's position 
         * to after inserted value.
         * @param aValue value to be insert to current position
         * @return new buffer position
         */
 		IMPORT_C TUint32 InsertL(TMdCValueUnion& aValue);

		/**
         * Insert TMdCSerializationType value to serialized buffer and move buffer's position 
         * to after inserted value.
         * @param aValue value to be insert to current position
         * @param aSize size of value
         * @return new buffer position
         */
 		IMPORT_C TUint32 InsertL(TMdCSerializationType& aValue, TUint32 aSize);

        /**
        * Receive TBool value from serialized buffer's current position and 
        * move buffer's position to after received value.
        * @param aDestination value received from serialized buffer
        * @return new buffer position
        */
		IMPORT_C TUint32 ReceiveL(TBool& aDestination);

        /**
        * Receive TInt8 value from serialized buffer's current position and 
        * move buffer's position to after received value.
        * @param aDestination value received from serialized buffer
        * @return new buffer position
        */
		IMPORT_C TUint32 ReceiveL(TInt8& aDestination);

        /**
        * Receive TUint8 value from serialized buffer's current position and 
        * move buffer's position to after received value.
        * @param aDestination value received from serialized buffer
        * @return new buffer position
        */
		IMPORT_C TUint32 ReceiveL(TUint8& aDestination);

        /**
        * Receive TInt16 value from serialized buffer's current position and 
        * move buffer's position to after received value.
        * @param aDestination value received from serialized buffer
        * @return new buffer position
        */
		IMPORT_C TUint32 ReceiveL(TInt16& aDestination);

        /**
        * Receive TUint16 value from serialized buffer's current position and 
        * move buffer's position to after received value.
        * @param aDestination value received from serialized buffer
        * @return new buffer position
        */
		IMPORT_C TUint32 ReceiveL(TUint16& aDestination);

        /**
        * Receive TInt32 value from serialized buffer's current position and 
        * move buffer's position to after received value.
        * @param aDestination value received from serialized buffer
        * @return new buffer position
        */
		IMPORT_C TUint32 ReceiveL(TInt32& aDestination);

        /**
        * Receive TUint32 value from serialized buffer's current position and 
        * move buffer's position to after received value.
        * @param aDestination value received from serialized buffer
        * @return new buffer position
        */
		IMPORT_C TUint32 ReceiveL(TUint32& aDestination);

        /**
        * Receive TInt64 value from serialized buffer's current position and 
        * move buffer's position to after received value.
        * @param aDestination value received from serialized buffer
        * @return new buffer position
        */
		IMPORT_C TUint32 ReceiveL(TInt64& aDestination);

        /**
        * Receive TUint value from serialized buffer's current position and 
        * move buffer's position to after received value.
        * @param aDestination value received from serialized buffer
        * @return new buffer position
        */
		IMPORT_C TUint32 ReceiveL(TUint& aDestination);

        /**
        * Receive TReal32 value from serialized buffer's current position and 
        * move buffer's position to after received value.
        * @param aDestination value received from serialized buffer
        * @return new buffer position
        */
		IMPORT_C TUint32 ReceiveL(TReal32& aDestination);

        /**
        * Receive TReal64 value from serialized buffer's current position and 
        * move buffer's position to after received value.
        * @param aDestination value received from serialized buffer
        * @return new buffer position
        */
		IMPORT_C TUint32 ReceiveL(TReal64& aDestination);

        /**
        * Receive TTime value from serialized buffer's current position and 
        * move buffer's position to after received value.
        * @param aDestination value received from serialized buffer
        * @return new buffer position
        */
		IMPORT_C TUint32 ReceiveL(TTime& aDestination);

        /**
         * Receive TMdCValueUnion value from serialized buffer's current position and 
         * move buffer's position to after received value.
         * @param aDestination value received from serialized buffer
         * @return new buffer position
         */
 		IMPORT_C TUint32 ReceiveL(TMdCValueUnion& aDestination);

		/**
         * Receive TMdCSerializationType value from serialized buffer's current position and 
         * move buffer's position to after received value.
         * @param aDestination value received from serialized buffer
         * @param aSize size of value
         * @return new buffer position
         */
 		IMPORT_C TUint32 ReceiveL(TMdCSerializationType& aDestination, TUint32 aSize);

        /**
        * Receive the length of descriptor which is in the current position of
        * the serialized buffer. (Current position of the buffer will not change.)
        * @return the length of descriptor
        */
		IMPORT_C TSerializedDesLength ReceiveDesciptorLength() const;

        /**
        * Receive 8-bit descriptor value from serialized buffer's current 
        * position and move buffer's position to after received value.
        * @param aDestination value received from serialized buffer
        * @return new buffer position
        */
		IMPORT_C TUint32 ReceiveL(TDes8& aDestination);

        /**
        * Receive 16-bit descriptor value from serialized buffer's current 
        * position and move buffer's position to after received value.
        * @param aDestination value received from serialized buffer
        * @return new buffer position
        */
		IMPORT_C TUint32 ReceiveL(TDes16& aDestination);

        /**
        * Receive 8-bit descriptor value from serialized buffer's current 
        * position and move buffer's position to after received value.
        * @return descriptor buffer, which must be destroyed by receiver
        */
		IMPORT_C HBufC8* ReceiveDes8L();

        /**
        * Receive 16-bit descriptor value from serialized buffer's current 
        * position and move buffer's position to after received value.
        * @return descriptor buffer, which must be destroyed by receiver
        */
		IMPORT_C HBufC16* ReceiveDes16L();

        /**
        * Receive pointer to 8-bit descriptor value from serialized buffer's
        * current position and move buffer's position to after received value.
        * @return descriptor pointer
        */
		IMPORT_C TPtrC8 ReceivePtr8L();

        /**
        * Receive pointer to 16-bit descriptor value from serialized buffer's
        * current position and move buffer's position to after received value.
        * @return descriptor pointer
        */
		IMPORT_C TPtrC16 ReceivePtr16L();

    private:

        /**
        * Default constructor
        */
        CMdCSerializationBuffer();

        /**
        * 2nd phase constructor
        * @param aInitialSize size of the buffer
        */
        void ConstructL( TInt32 aSize );

        /**
        * 2nd phase constructor
        * @param aInitialSize pointer to the buffer
        * @param aInitialSize size of the buffer
        */
        void ConstructL( TUint8* aBuffer, TInt32 aSize );

	#ifdef _DEBUG
        void InsertTypeL( TUint16 aType );
		#define INSERTTYPE(A) InsertTypeL( (TUint16)A );

        void CheckTypeL( TUint16 aType );
		#define CHECKTYPE(A) CheckTypeL( (TUint16)A );
        
        const TDesC& GetPropertyName( TUint16 aType );
	#else
	#define INSERTTYPE(A)
	#define CHECKTYPE(A)
	#endif

    private:

        /**
        * The heap buffer
        */
        HBufC8* iHeapBuffer;

        /**
        * The pointer to buffer
        */
        TPtr8 iBuffer;

        /**
        * Current position of the buffer
        */
        TUint32 iPosition;
	};

	
/**
 * definitions belongs to querycriteria(de)serialization
 * files: mdsquerycriteriadeserialization.h, mdequerycriteriaserialization.h
 */
const TUint32 KOffsetResultMode = 0; // The begin of the buffer
const TUint32 KOffsetQueryType = KOffsetResultMode + CMdCSerializationBuffer::KRequiredSizeForTInt32;
const TUint32 KOffsetNamespaceDefId = KOffsetQueryType + CMdCSerializationBuffer::KRequiredSizeForTDefId;
const TUint32 KOffsetObjectDefId = KOffsetNamespaceDefId + CMdCSerializationBuffer::KRequiredSizeForTDefId;
const TUint32 KOffsetObjectDefsIds = KOffsetObjectDefId + CMdCSerializationBuffer::KRequiredSizeForTUint32;
const TUint32 KOffsetLimit = KOffsetObjectDefsIds + CMdCSerializationBuffer::KRequiredSizeForTUint32;
const TUint32 KOffsetOffset = KOffsetLimit + CMdCSerializationBuffer::KRequiredSizeForTUint32;
const TUint32 KOffsetOptimizationFlags = KOffsetOffset + CMdCSerializationBuffer::KRequiredSizeForTUint32;
const TUint32 KOffsetRootCondition = KOffsetOptimizationFlags + CMdCSerializationBuffer::KRequiredSizeForTUint32;
const TUint32 KOffsetOrderRules = KOffsetRootCondition + CMdCSerializationBuffer::KRequiredSizeForTUint32;
const TUint32 KOffsetPropertyFilters = KOffsetOrderRules + CMdCSerializationBuffer::KRequiredSizeForTUint32;

	

#endif // __MDCSERIALIZATIONBUFFER_H__
