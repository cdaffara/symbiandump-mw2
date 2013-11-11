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
* Description:  Class to hold description about one item definition
*
*/

#ifndef __MDSITEMDEF_H__
#define __MDSITEMDEF_H__

#include "mdscommoninternal.h"

class CMdCSerializationBuffer;

/*
 * All item definitions should inherit from this one
 */
class CMdsItemDef : public CBase
	{
	public:
	 	/**
	 	 * Destructor
	 	 */
	    virtual ~CMdsItemDef();
	
		/**
		 * Get object name
		 * @return object name
		 */
		inline const TDesC& GetName() const;
	
		/**
		 * Get object Id
		 * @return object Id
		 */
		inline TDefId GetId() const;
	
		/**
		 * Sets object Id
		 * param aId id which is assigned to event
		 */		
		inline void SetId(const TDefId aId);
	
		/*
		 * Set that item is already stored in DB
		 */	
		inline void SetAllNotStoredInDB();

		/**
		 * Sets EStoredFlagsDB flag on.
		 */
		inline void SetStoredInDB();
		
		/**
		 * @return ETrue if EStoredFlagsDB flag is set.
		 */
		inline TBool GetStoredInDB() const;

		/**
		 * Sets EStoredFlagsTable flag on.
		 */
		inline void SetTableStoredInDB();
		
		/**
		 * @return ETrue if EStoredFlagsTable flag is set.
		 */
		inline TBool GetTableStoredInDB() const;

		/**
		 * stores defined flag
		 */
		inline void SetFlag( TStoredFlags aFlag );
		inline TBool GetFlag( TStoredFlags aFlag ) const;
		
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
	    TMdCOffset SerializeL(CMdCSerializationBuffer& aBuffer);
			
	protected:
		/**
		 * Default constructor
		 */
	    inline CMdsItemDef()
	    	{}
	
		/**
		 * 2nd phase construction
		 * @param aName name of event
		 * @param aPriority priority of event
		 */
	    void ConstructL( const TDesC& aName );
	    
		/**
		 * Check name of schema definition against allowed characters 
		 * (A-Z, a-z, 0-9). First character can't be number. 
		 *
		 * Leaves with error code not equal to KErrNone if aName contains 
		 * forbidden character(s).
		 *
		 * @param aName name of schema definition
		 * @param aAllowNumbers are numbers allowed
		 */
	    void CheckAllowerCharatersL( const TDesC& aName, TBool aAllowNumbers );

	private:
		/**
		 * Check name of schema definition against reserved word. 
		 * 
		 * Leaves with error code not equal to KErrNone if aName is reserved 
		 * word.
		 * 
		 * @param aName name of schema definition
		 */
		void CheckReservedWordsL( const TDesC& aName );

		/**
		 * Check that character is allowed alphabet (A-Z, a-z).
		 * 
		 * @param aChar character to be checked
		 */
		void CheckAlphabetL( TUint16 aChar );

		/**
		 * Check that character is allowed alphabet (A-Z, a-z) or number (0-9).
		 * 
		 * @param aChar character to be checked
		 */
		void CheckAlphabetOrNumberL( TUint16 aChar );

	private: // data
	
		/**
		 * ObjectDef Name
		 */
		HBufC* iName;
	
		/**
		 * ItemDef Id
		 */
		TDefId iId;
	
		/**
		 * Tells if object is stored in DB
		 */
		TUint32 iStoredFlags;
	};

#include "mdsitemdef.inl"
	
#endif /*__MDSITEMDEF_H__*/
