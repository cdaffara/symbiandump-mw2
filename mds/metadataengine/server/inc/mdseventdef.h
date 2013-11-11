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
* Description:  Class to hold description about one event definition
*
*/

#ifndef __MDSEVENTDEF_H__
#define __MDSEVENTDEF_H__

#include "mdsitemdef.h"


/**
 *  Metadata eventdef class
 *
 *  This class is responsible for holding information one event
 *
 */
class CMdsEventDef : public CMdsItemDef
    {
    friend class CMdsNamespaceDef;
public:
	/**
	 * Create new event object
	 * @param aId id of event
	 * @param aName name of event
	 * @param aPriority priority of event
	 * @return pointer to new created event
	 */
    static CMdsEventDef* NewL( const TDesC& aName, const TInt32 aPriority );

	/**
	 * Create new event object
	 * @param aId id of event
	 * @param aName name of event
	 * @param aPriority priority of event
	 * @return pointer to new created event
	 */
    static CMdsEventDef* NewLC( const TDesC& aName, const TInt32 aPriority );

    /**
     * Stores event definition to DB
     * @param aNamespaceDefId namespace definition
     * @param aConnection connection to database
     */
 	void StoreToDBL( const TInt aNamespaceDefId );

 	/**
 	 * Destructor
 	 */
    virtual ~CMdsEventDef();

	TBool operator==( const CMdsEventDef& aEventDef ) const;

	inline TBool operator!=( const CMdsEventDef& aEventDef ) const;

	/**
	 * Get object priority
	 * @return object priority
	 */
	inline TInt32 GetPriority() const;

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
	 * Default constructor
	 */
    inline CMdsEventDef()
    	{}

	/**
	 * 2nd phase construction
	 * @param aName name of event
	 * @param aPriority priority of event
	 */
    void ConstructL( const TDesC& aName, TInt32 aPriority );

private: // data
	
	/**
	 * event priority
	 */
	TInt32 iPriority;
    };

#include "mdseventdef.inl"
    
#endif // __MDSEVENTDEF_H__
