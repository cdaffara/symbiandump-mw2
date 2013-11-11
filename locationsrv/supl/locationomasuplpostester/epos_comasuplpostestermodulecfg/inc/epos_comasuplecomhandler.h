/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Intracts with ECom Framework.
*
*/

#ifndef C_COMASUPLECOMHANDLER_H
#define C_COMASUPLECOMHANDLER_H

#include <e32base.h>
#include <e32def.h>
#include <ecom/implementationinformation.h>

/**
 *  Intracts with ECom framework.
 *
 *  Identifies all implementations associated with a particular
 *  Interface UId and stores it in an array.
 *
 *  @lib PosTesterModuleCfg.lib
 *  @since S60 v3.1u
 */
class COMASuplEComHandler : public CBase
	{
	public:
		static	COMASuplEComHandler* NewL(const TInt aInterfaceUid);
		virtual ~COMASuplEComHandler();
		/**
		 * Gets the list of all implementations associated with
		 * an Interface UId
		 *
		 * @since S60 v3.1u
		 * @return RImplInfoPtrArray&: Contains all implementations
		 * associated with the UId.
		 */
		const RImplInfoPtrArray& GetImplementationList();

		/**
		 * Initializes the list of all implementations associated with
		 * an Interface UId.
		 *
		 * @since S60 v3.1u
		 * @return void
		 */
		void InitializeImplementationListL();

		/**
		 * Closes session with ECom (FinalClose())
		 *
		 * @since S60 v3.1u
		 * @return void
		 */
		void UnLoadImplementation();

	protected:
		/**
		 * Constructor
		 *
		 * @since S60 v3.1
		 * @param aInterfaceUid: InterfaceUid 
		 * @return 
		 */
		COMASuplEComHandler(const TInt aInterfaceUid);

	private:
		/**
	     * Two Phase constructor
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void ConstructL();
	
	private:
	    /**
	     * Contains list of Implementations identified by ECom Framework.
	     */
		RImplInfoPtrArray iImplementationList;

	    /**
	     * Contains Interface Uid.
	     */	
		TUid iInterfaceUid;

	    /**
	     * identifer returned by ECom to identify an instance to the framework.
     	 */
		TUid iDtorIdKey;
	};
#endif
