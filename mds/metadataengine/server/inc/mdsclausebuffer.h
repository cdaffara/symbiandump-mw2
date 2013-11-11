/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  A unicode des buffer with automatic memory*
*/

#ifndef __MDSCLAUSEBUFFER_H__
#define __MDSCLAUSEBUFFER_H__

// INCLUDE FILES
#include <e32base.h>


/**
 * An automatic clause buffer class. The class provides Descriptor 
 * functionality, plus automatic memory handling so that it reserves
 * the required space automatically as the size grows.
 */
class CMdsClauseBuffer: public CBase
    {
    public: // public definitions

        /**
         * Static constructor
         * @param aInitialSize size of the buffer
         * @return buffer instance
         */
        IMPORT_C static CMdsClauseBuffer* NewL( TInt aInitialSize );

        /**
         * Static constructor
         * @param aInitialSize size of the buffer
         * @return buffer instance
         */
        IMPORT_C static CMdsClauseBuffer* NewLC( TInt aInitialSize );

        virtual ~CMdsClauseBuffer();

    private:

        /**
         * Default constructor
         */
        CMdsClauseBuffer();

        /**
         * 2nd phase constructor
         * @param aInitialSize size of the buffer
         */
        void ConstructL( TInt aInitialSize );

    public:

        /**
         * Appends a descriptor at the end of this descriptor.
         * Increases the size of the descriptor if needed.
         * @param aDes descriptor which is appended
         */
        void AppendL( const TDesC16& aDes );

        /**
         * Appends a descriptor at the end of this descriptor.
         * Increases the size of the descriptor if needed.
         * @param aDes descriptor which is appended
         * @param aAdditionl 
         */
        void AppendL( const TDesC16& aDes, const TInt aAdditional );

        /**
         * Makes sure the buffer max size is AT LEAST given required bytes.
         * @param aRequired required buffer total size
         * @return ETrue if the buffer was re-allocated
         */
        TBool ReserveSpaceL( TInt aRequired );
    
        /**
         * Return buffer.
         * 
         * @return buffer
         * 
		 * @leave KErrNotFound Buffer is NULL.
         */
        TPtr BufferL() const;
        
        /**
         * Return const reference to buffer.
         * 
         * @return const reference to buffer
         * 
		 * @leave KErrNotFound Buffer is NULL.
         */
        const TDesC& ConstBufferL() const;
        
    private:

        /**
         * The heap buffer
         */
        HBufC* iBuffer;

        /**
         * Current size of the buffer
         */
        TInt iCurrentBufSize;
    };


#endif // __MDSCLAUSEBUFFER_H__
