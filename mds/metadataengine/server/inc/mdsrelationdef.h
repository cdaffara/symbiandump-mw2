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
* Description:  Class to hold description about one relation
*
*/

#ifndef __MDSRELATIONDEF_H__
#define __MDSRELATIONDEF_H__

#include <e32base.h>

#include "mdsitemdef.h"


class CMdCSerializationBuffer;


/**
 *  Metadata relationdef class
 *
 *  This class is responsible for holding information about one relation
 *
 */
class CMdsRelationDef : public CMdsItemDef
    {
    friend class CMdsNamespaceDef;
public:
	/**
	 * Create new relation object
	 * @param aId id of relation
	 * @param aName name of relation
	 * @return pointer to new created relation
	 */
    static CMdsRelationDef* NewL( const TDesC& aName );

	/**
	 * Create new relation object
	 * @param aId id of relation
	 * @param aName name of relation
	 * @return pointer to new created relation
	 */
    static CMdsRelationDef* NewLC( const TDesC& aName );

    virtual ~CMdsRelationDef();
    

    TBool operator==( const CMdsRelationDef& aRelationDef ) const;

	inline TBool operator!=( const CMdsRelationDef& aRelationDef ) const;

	void StoreToDBL( const TDefId aNamespaceDefId );

	/**
     * Get required size of serialized buffer when this is serialized.
     *
     * @return required size of serialized buffer
     */
    TUint32 RequiredBufferSize();

    /**
     * Serialize own data to serialized buffer (correct position must be set 
     * before calling) and return new position of serialized buffer.
	 *
     * @param aBuffer  serialized buffer.
     */
    TMdCOffset SerializeL(CMdCSerializationBuffer& aBuffer, TMdCOffset aFreeSpace);

private:
	/**
	 * 2nd phase construction
	 * @param aName name of event
	 */
    void ConstructL( const TDesC& aName );

private: // data

    };

#include "mdsrelationdef.inl"

#endif // __MDSRELATIONDEF_H__
