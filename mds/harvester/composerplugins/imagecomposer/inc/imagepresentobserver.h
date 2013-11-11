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
* Description:  image composer listener for setting pending image objects present
 *
*/


#ifndef IMAGEPRESENTOBSERVER_H_
#define IMAGEPRESENTOBSERVER_H_

#include "mdeharvestersession.h"

// forward declarations
class CMdENamespaceDef;
class CComposerImagePlugin;

/**
 *  CImagePresentObserver
 * 
 */
class CImagePresentObserver : public CActive, public MMdEObjectPresentObserver
	{
public:

	enum TRequest
		{
		ERequestIdle,
		ERequestWaitPendingCount,
		ERequestWaitPending,
		ERequestProcess
		} 
	;
	/**
	 * Destructor.
	 */
	~CImagePresentObserver();

	/**
	 * Two-phased constructor.
	 */
	static CImagePresentObserver* NewL( CMdESession* aSession, CComposerImagePlugin* aComposer, 
			CMdEHarvesterSession* aMdEHarvesterSession );

    /**
     * Set next request (=state) of this active object.
     * @param aRequest  State enumeration.
     */       
    void SetNextRequest( TRequest aRequest );	
	
    /**
     * Standard RunL-method.
     * From CActive.
     */       
    void RunL();

    /**
     * From CActive.
     */       
    void DoCancel();

    /**
     * From CActive.
     * @param aError  Leave error code.
     * @return Always KErrNone.
     */       
    TInt RunError( TInt aError );
		
	
protected:

	/**
	 * Called to notify the observer that objects has been set
	 * to present or not present state in the metadata engine database.
	 *
	 * @param aSession session
	 * @param aPresent state: ETrue - present or  EFales - not present
	 * @param aObjectIdArray object IDs which are set to present state
	 */
	void HandleObjectPresentNotification(CMdESession& aSession, 
			TBool aPresent, const RArray<TItemId>& aObjectIdArray);
	
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CImagePresentObserver();

	/**
	 * default constructor for performing 2nd stage construction
	 */
	void ConstructL( CMdESession* aSession, CComposerImagePlugin* aComposer, CMdEHarvesterSession* aMdEHarvesterSession );
	
	void CleanUp();
	

	
private:

	/**
	 * Pointer to default Mde namespace.
	 */
	CMdENamespaceDef* iDefaultNamespace;

	/**
	 * Pointer to Mde session.
	 */
	CMdESession* iSession;
	
	CMdEHarvesterSession* iMdEHarvesterSession;

	/**
	 * Pointer to a composer.
	 */
	CComposerImagePlugin* iComposer;	
	
	RArray<TItemId> iPresentObjectIds;
	RArray<TItemId> iPendingObjectIds;
	RArray<TItemId> iComposeObjectIds;
	
    TRequest iNextRequest;
    
    TInt iPendingCount;
    
    CMdCSerializationBuffer* iPendingBuffer;
    TBool iStarted;
	
	};

#endif /*IMAGEPRESENTOBSERVER_H_*/

