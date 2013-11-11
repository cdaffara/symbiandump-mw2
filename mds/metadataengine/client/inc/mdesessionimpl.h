/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Metadata engine client session implementation*
*/


#ifndef __MDESESSIONIMPL_H__
#define __MDESESSIONIMPL_H__

#include "mdesession.h"
#include "mdeenginesession.h"
#include "mdscommoninternal.h"


// forward declarations
class CMdENotifierAO;
class CMdEDataBuffer;
class CMdCSerializationBuffer;
class CMdESessionStartupAO;
class CMdESessionAsyncHandler;

class RMdESessionAsyncRequest
	{
		friend class CMdESessionAsyncHandler;
	private:
		enum TRequestType
		{
			EAddRequest,
			EUpdateRequest,
			ERemoveRequest
		};
	
		RMdESessionAsyncRequest(TRequestType aRequestType, 
			CMdCSerializationBuffer* aBuffer, 
			CMdCSerializationBuffer& aResultBuffer,
			TRequestStatus& aRequestStatus);
		
		void Close();

	private:
		TRequestType iRequestType;
		CMdCSerializationBuffer* iBuffer;
		CMdCSerializationBuffer* iResultBuffer;
		TRequestStatus* iRequestStatus;
	};

NONSHARABLE_CLASS(CMdESessionAsyncHandler) : public CActive
{
public:
	void AddRequest( CMdCSerializationBuffer* aBuffer,
			CMdCSerializationBuffer& aResultBuffer, 
			TRequestStatus& aRequestStatus );

	void UpdateRequest( CMdCSerializationBuffer * aBuffer,
			CMdCSerializationBuffer& aResultBuffer,
			TRequestStatus& aRequestStatus );

	void RemoveRequest( CMdCSerializationBuffer* aBuffer,
			CMdCSerializationBuffer& aResultBuffer,
			TRequestStatus& aRequestStatus );
		
	static CMdESessionAsyncHandler* NewL(CMdESessionImpl& aSession, 
			RMdEEngineSession &aEngineSession);
		
	static CMdESessionAsyncHandler* NewLC( CMdESessionImpl& aSession, 
			RMdEEngineSession &aEngineSession );
		
	~CMdESessionAsyncHandler();
		
	
private:	
	void RunL();

	TInt RunError(TInt aError);

	void DoCancel();

	CMdESessionAsyncHandler(CMdESessionImpl& aSession, 
			RMdEEngineSession &aEngineSession);

	void ConstructL();

private:
	RArray<RMdESessionAsyncRequest> iRequests;
	CMdESessionImpl& iSession;
	RMdEEngineSession& iEngineSession;
};

/**
 * Metadata engine session implementation.
 */
NONSHARABLE_CLASS(CMdESessionImpl) : public CMdESession
	{
	friend class CMdESessionStartupAO;

    public: // Constructors and destructor.

    	/**
    	 * Constructor.
    	 *
    	 * @param aObserver observer to notify when opening the session has 
    	 *                  been completed
    	 */
    	CMdESessionImpl( MMdESessionObserver& aObserver );

    	/**
    	 * Second-phase constructor.
    	 */
    	void ConstructL();

    	/**
    	 * Destructor.
    	 */
    	virtual ~CMdESessionImpl();

		/** From MdESession */
		TInt NamespaceDefCount() const;

		/** From MdESession */
		CMdENamespaceDef& NamespaceDefL(TInt aIndex);

		/** From MdESession */
		CMdENamespaceDef& GetNamespaceDefL(const TDesC& aName);

		CMdENamespaceDef& GetNamespaceDefL(TDefId aId);

		/** From MdESession */
		CMdENamespaceDef& GetDefaultNamespaceDefL();

		/** From MdESession */
		CMdEObject* NewObjectL( CMdEObjectDef& aDef, const TDesC& aUri, 
                TUint32 aMediaId = 0 );

		/** From MdESession */
		CMdEObject* NewObjectLC( CMdEObjectDef& aDef, const TDesC& aUri, 
				TUint32 aMediaId = 0 );

		/** From MdESession */
	    TInt AddObjectsL(RPointerArray<CMdEObject>& aObjects);

		/** From MdESession */
	    void CommitObjectL(CMdEObject& aObject);

		/** From MdESession */
	    void CommitObjectsL(RPointerArray<CMdEObject>& aObjects);

		/** From MdESession */
	    TItemId CancelObjectL(CMdEObject& aObject);

		/** From MdESession */
		CMdERelation* NewRelationLC(CMdERelationDef& aDef, 
				TItemId aLeftObjectId, TItemId aRightObjectId, 
				TInt32 aParameter);

		/** From MdESession */
		CMdERelation* NewRelationL(CMdERelationDef& aDef, 
				TItemId aLeftObjectId, TItemId aRightObjectId, 
				TInt32 aParameter);

		/** From MdESession */
		CMdEEvent* NewEventLC(CMdEEventDef& aDef, TItemId aObjectId, 
				TTime aTime, const TDesC* aSource, const TDesC* aParticipant);

		/** From MdESession */
		CMdEEvent* NewEventL(CMdEEventDef& aDef, TItemId aObjectId, 
				TTime aTime, const TDesC* aSource, const TDesC* aParticipant);

		/** From MdESession */
		void AddSchemaObserverL(MMdESchemaObserver& aObserver);

		/** From MdESession */
		void RemoveSchemaObserverL(MMdESchemaObserver& aObserver);

		/** From MdESession */
		void AddRelationDefL(const CMdENamespaceDef &aNamespaceDef, 
				const TDesC &aName);

		/** From MdESession */
		void AddEventDefL(const CMdENamespaceDef &aNamespaceDef, 
				const TDesC &aName);

		/* From MdESession */
    	TItemId AddObjectL(CMdEObject& aObject);
    	
    	/* From MdESession */
		TInt DeserializeIdsL( RMdEDataBuffer& aSerializedItemIds, 
				RArray<TItemId>* aResultObjects = NULL,
				RArray<TItemId>* aResultEvents = NULL, 
				RArray<TItemId>* aResultRelations = NULL );

    	/* From MdESession */
   		TInt DeserializeItemsL( RMdEDataBuffer& aSerializedItems, 
   				RPointerArray<CMdEInstanceItem>& aItems );

    	/* From MdESession */
    	TItemId RemoveObjectL( TItemId aId, CMdENamespaceDef* aNamespaceDef );
    	
    	/* From MdESession */
    	TItemId RemoveObjectL( const TDesC& aUri, 
    			CMdENamespaceDef* aNamespaceDef );

    	/* From MdESession */
		TInt RemoveObjectsL( const RArray<TItemId>& aId, 
				RArray<TItemId>& aResult, CMdENamespaceDef* aNamespaceDef );
	
    	/* From MdESession */
		TInt RemoveObjectsL( const RPointerArray<TDesC>& aUri, 
				RArray<TItemId>& aResult, CMdENamespaceDef* aNamespaceDef );

    	/* From MdESession */
		void RemoveObjectsAsyncL( 
				const RArray<TItemId>& aId, TRequestStatus& aStatus, 
				RMdEDataBuffer& aSerializedObjectIds, 
				CMdENamespaceDef* aNamespaceDef );

    	/* From MdESession */
		void RemoveObjectsAsyncL( 
				const RPointerArray<TDesC>& aUri, TRequestStatus& aStatus, 
				RMdEDataBuffer& aSerializedObjectIds, 
				CMdENamespaceDef* aNamespaceDef );

    	/* From MdESession */
    	CMdEObject* GetObjectL( const TItemId aId, 
    			CMdEObjectDef& aObjectDef );

    	/* From MdESession */
    	CMdEObject* OpenObjectL( const TItemId aId, 
    			CMdEObjectDef& aObjectDef );

    	/* From MdESession */
    	CMdEObject* GetObjectL( 
    			const TInt64 aGuidHigh, const TInt64 aGuidLow, 
    			CMdEObjectDef& aObjectDef );

    	/* From MdESession */
    	CMdEObject* OpenObjectL( 
    			const TInt64 aGuidHigh, const TInt64 aGuidLow, 
    			CMdEObjectDef& aObjectDef );
    	
    	/* From MdESession. */
    	CMdEObject* GetObjectL( const TDesC& aUri, 
    			CMdEObjectDef& aObjectDef );

    	/* From MdESession. */
    	CMdEObject* OpenObjectL( const TDesC& aUri, 
    			CMdEObjectDef& aObjectDef );

    	/* From MdESession */
    	CMdEObject* GetObjectL( const TItemId aId, 
    			CMdENamespaceDef* aNamespaceDef );

    	/* From MdESession */
    	CMdEObject* OpenObjectL( const TItemId aId, 
    			CMdENamespaceDef* aNamespaceDef );

    	/* From MdESession */
    	CMdEObject* GetObjectL(
    			const TInt64 aGuidHigh, const TInt64 aGuidLow, 
    			CMdENamespaceDef* aNamespaceDef );

    	/* From MdESession */
    	CMdEObject* OpenObjectL( 
    			const TInt64 aGuidHigh, const TInt64 aGuidLow, 
    			CMdENamespaceDef* aNamespaceDef );
    	
    	/* From MdESession. */
    	CMdEObject* GetObjectL( const TDesC& aUri, 
    			CMdENamespaceDef* aNamespaceDef );

    	/* From MdESession. */
    	CMdEObject* OpenObjectL( const TDesC& aUri, 
    			CMdENamespaceDef* aNamespaceDef );

    	/* From MdESession */
    	CMdEObject* GetFullObjectL( const TItemId aId, 
    			CMdENamespaceDef* aNamespaceDef );

    	/* From MdESession */
    	CMdEObject* OpenFullObjectL( const TItemId aId, 
    			CMdENamespaceDef* aNamespaceDef );

    	/* From MdESession */
    	CMdEObject* GetFullObjectL( 
    			const TInt64 aGuidHigh, const TInt64 aGuidLow, 
    			CMdENamespaceDef* aNamespaceDef );

    	/* From MdESession */
    	CMdEObject* OpenFullObjectL( 
    			const TInt64 aGuidHigh, const TInt64 aGuidLow, 
    			CMdENamespaceDef* aNamespaceDef );
    	
    	/* From MdESession. */
    	CMdEObject* GetFullObjectL( const TDesC& aUri, 
    			CMdENamespaceDef* aNamespaceDef );

    	/* From MdESession. */
    	CMdEObject* OpenFullObjectL( const TDesC& aUri, 
    			CMdENamespaceDef* aNamespaceDef );

    	void CheckObjectL( 	TMdEObject& aObject, const TDesC& aUri, 
    			CMdENamespaceDef* aNamespaceDef );

    	void CheckObjectL( TMdEObject& aObject, TItemId aId, 
    			CMdENamespaceDef* aNamespaceDef );

    	void CheckObjectL( RArray<TMdEObject>& aObjects, 
    			const RArray<TItemId>& aIds, 
    			CMdENamespaceDef* aNamespaceDef );

    	/* From MdESession. */
     	TItemId AddRelationL( CMdERelation& aRelation );

    	/* From MdESession. */
     	TItemId UpdateRelationL( CMdERelation& aRelation );

    	/* From MdESession. */
        TInt AddItemsL( RPointerArray<CMdEInstanceItem>& aItems );

    	/* From MdESession. */
        TInt UpdateItemsL( RPointerArray<CMdEInstanceItem>& aItems );

		/* From MdESession. */
        void AddItemsAsyncL( 
        		RPointerArray<CMdEInstanceItem>& aItems, 
        		TRequestStatus& aStatus,
        		RMdEDataBuffer& aSerializedItemIds );

		/* From MdESession. */
        void UpdateItemsAsyncL( 
        		RPointerArray<CMdEInstanceItem>& aItems, 
        		TRequestStatus& aStatus,
        		RMdEDataBuffer& aSerializedItemIds );

    	/* From MdESession. */
        CMdERelation* GetRelationL(TItemId aId, 
        		CMdENamespaceDef* aNamespaceDef);

    	/* From MdESession. */
    	TItemId RemoveRelationL(TItemId aId, 
    			CMdENamespaceDef* aNamespaceDef);
    	
    	/* From MdESession. */
    	TInt RemoveRelationsL(const RArray<TItemId>& aId, 
    			RArray<TItemId>& aSuccessful, CMdENamespaceDef* aNamespaceDef);

    	/* From MdESession. */
    	void RemoveRelationsAsyncL(
    			const RArray<TItemId>& aId, TRequestStatus& aStatus, 
    			RMdEDataBuffer& aSerializedRelationIds, 
    			CMdENamespaceDef* aNamespaceDef );

    	/* From MdESession. */
        CMdEObjectQuery* NewObjectQueryL(CMdENamespaceDef& aNamespaceDef, 
        		CMdEObjectDef& aObjectDef, MMdEQueryObserver* aObserver);

        /* From MdESession. */
        CMdEObjectQuery* NewObjectQueryL(CMdEObjectDef& aObjectDef, 
        		RPointerArray<CMdEObjectDef>* aObjectDefs, 
        		MMdEQueryObserver* aObserver);
        
    	/* From MdESession. */
        CMdEEvent* GetEventL(TItemId aId, CMdENamespaceDef* aNamespaceDef);

    	/* From MdESession. */
        TItemId AddEventL( CMdEEvent& aEvent );   
    						
    	/* From MdESession. */		   
		TItemId RemoveEventL( TItemId aId, CMdENamespaceDef* aNamespaceDef );
		
		/* From MdESession. */
		TInt RemoveEventsL( const RArray<TItemId>& aId, 
				RArray<TItemId>& aSuccessful, 
				CMdENamespaceDef* aNamespaceDef );
		
		/* From MdESession. */
		void RemoveEventsAsyncL( 
				const RArray<TItemId>& aId, TRequestStatus& aStatus, 
				RMdEDataBuffer& aSerializedEventIds, 
				CMdENamespaceDef* aNamespaceDef );
	
    	/* From MdESession. */
        CMdERelationQuery* NewRelationQueryL(CMdENamespaceDef& aNamespaceDef, 
        		MMdEQueryObserver* aObserver);

    	/* From MdESession. */
        CMdEEventQuery* NewEventQueryL(CMdENamespaceDef& aNamespaceDef, 
        		MMdEQueryObserver* aObserver);

        /* From MdESession. */
    	void AddObjectObserverL(MMdEObjectObserver& aObserver,
    							CMdELogicCondition* aCondition,
    							TUint32 aNotificationType,
    							CMdENamespaceDef* aNamespaceDef);

        /* From MdESession. */
    	void RemoveObjectObserverL(MMdEObjectObserver& aObserver, 
    							  CMdENamespaceDef* aNamespaceDef);

		/* From MdESession. */
		void AddObjectPresentObserverL(
				MMdEObjectPresentObserver& aObserver);

		/* From MdESession. */
		void RemoveObjectPresentObserverL(
				MMdEObjectPresentObserver& aObserver);
	
        /* From MdESession. */
    	void AddRelationObserverL(MMdERelationObserver& aObserver,
    							  CMdECondition* aCondition,
    							  TUint32 aNotificationType,
    							  CMdENamespaceDef* aNamespaceDef);

        /* From MdESession. */
    	void RemoveRelationObserverL(MMdERelationObserver& aObserver,
    								CMdENamespaceDef* aNamespaceDef);

    	/* From MdESession. */
    	void AddRelationItemObserverL(MMdERelationItemObserver& aObserver,
    								  CMdECondition* aCondition,
    								  TUint32 aNotificationType,
    								  CMdENamespaceDef* aNamespaceDef);

    	/* From MdESession. */
    	void RemoveRelationItemObserverL(MMdERelationItemObserver& aObserver, 
    									CMdENamespaceDef* aNamespaceDef);

    	/* From MdESession. */
		void AddRelationPresentObserverL(
				MMdERelationPresentObserver& aObserver);

        /* From MdESession. */
		void RemoveRelationPresentObserverL(
				MMdERelationPresentObserver& aObserver);

        /* From MdESession. */
    	void AddEventObserverL(MMdEEventObserver& aObserver,
    						   CMdECondition* aCondition, 
 							   TUint32 aNotificationType,
    						   CMdENamespaceDef* aNamespaceDef);

        /* From MdESession. */
    	void RemoveEventObserverL(MMdEEventObserver& aObserver, 
    							 CMdENamespaceDef* aNamespaceDef);

        /** Finds an existing observer notifier */
        TInt FindNotifier( TUint32 aNotifyType, TAny* aObserver, 
        		CMdENamespaceDef& aNamespaceDef );

        /** With this call Notifier announces it is in error state */
        void NotifierInError( CMdENotifierAO* aNotifier );

        /* From MdESession. */
        void ImportSchemaL( const TDesC& aFileName );

        /* From MdESession. */
        TInt ImportMetadataL(const TDesC& aFileName );

        /* From MdESession. */
        void ImportMetadata( const TDesC& aFileName, TPckgBuf<TInt>& aResult, 
        		TRequestStatus& aStatus );

        /* From MdESession. */
        void ExportMetadataL( const TDesC& aFileName, 
        					  const CMdENamespaceDef* aNamespaceDef = NULL, 
	                		  const RPointerArray<CMdEObjectDef>* aObjectDefs = NULL, 
                    		  const RPointerArray<CMdERelationDef>* aRelationDefs = NULL, 
                        	  const RPointerArray<CMdEEventDef>* aEventDefs = NULL );

        /* From MdESession. */
        void ExportMetadataL( const TDesC& aFileName, TRequestStatus& aStatus, 
        					  RMdEDataBuffer& aBuffer, 
        					  const CMdENamespaceDef* aNamespaceDef = NULL, 
	                		  const RPointerArray<CMdEObjectDef>* aObjectDefs = NULL, 
                    		  const RPointerArray<CMdERelationDef>* aRelationDefs = NULL, 
                        	  const RPointerArray<CMdEEventDef>* aEventDefs = NULL );

		void LoadSchemaL();

		RMdEEngineSession& EngineSession();

		/* From MdESession. */
    	void GetSchemaVersionL(TInt& aMajorVersion, TInt& aMinorVersion);

    	/* From MdESession. */
    	void SetObjectToPresentByGuidL( 
    			const TInt64& aGuidHigh, const TInt64& aGuidLow );
    	
        void GetCountL( CMdCSerializationBuffer* aBuffer, TUint32& aResult );

        void GetItemIdL( CMdCSerializationBuffer* aBuffer, 
        		RArray<TItemId>& aIdArray );

        void GetDistinctValuesL( CMdCSerializationBuffer& aBuffer, 
        		CDesCArray& aResults );

		/**
		 * Should be called to notify the base class about errors, which are 
		 * not a direct consequence of the operations initiated by the client 
		 * but caused by some external source (e.g., other clients). Attempts 
		 * to recover from the error have failed. All on-going operations 
		 * initiated by the client should be aborted before calling this 
		 * method. After calling this method any attempts to use the session 
		 * will cause a panic.
		 *
		 * @param aError  one of the system-wide error codes
		 */
		void NotifyError(TInt aError);

        /* From MdESession. */
        void AddObjectObserverWithUriL( MMdEObjectObserverWithUri& aObserver,
                                CMdELogicCondition* aCondition,
                                TUint32 aNotificationType,
                                CMdENamespaceDef* aNamespaceDef );
        
        /* From MdESession. */
        void RemoveObjectObserverWithUriL( MMdEObjectObserverWithUri& aObserver, 
                                CMdENamespaceDef* aNamespaceDef );
    protected:

        /*
        * Loads the schema
        */
        void DoLoadSchemaL();

       	void Close();

		/* Methods to notify the base class about different events. */

		/**
		 * Should be called to notify the base class that opening the session 
		 * has been completed and, if the opening succeeded, the session is 
		 * ready for use.
		 *
		 * @param aError <code>KErrNone</code>, if opening the session 
		 *               succeeded; or one of the system-wide error codes, 
		 *               if opening the session failed
		 */
		void NotifySessionOpened(TInt aError);

		/* Utility methods for concrete session implementations. */	

		/**
		 * Panics if the session has not been succesfully opened.
		 */
		void CheckOpened() const;

	public:

		void DeserializeQueryResultL( CMdCSerializationBuffer& aBuffer, 
				RPointerArray<CMdEInstanceItem>& aItems );

	protected:						   		       
		CMdCSerializationBuffer* SerializeItemsL( 
				RPointerArray<CMdEInstanceItem>& aItems );					   		       
		
		TItemId AddItemL( CMdEInstanceItem& aItem );
		
		void DeserializeAddingResultsL( 
				RPointerArray<CMdEInstanceItem>& aItems, 
				CMdCSerializationBuffer& aResultBuffer );
		
		/**
		 * Creates a serialized buffer with id marked to be removed
		 *
		 * @param aNamespaceDef namespace where items should be removed
		 * @param aObjects objects id to remove (could be NULL)
		 * @param aEvents events id to remove (could be NULL)
		 * @param aRelations relation id to remove (could be NULL)
		 * @return serialized buffer
		 */
		CMdCSerializationBuffer* RemoveCommonL( 
				CMdENamespaceDef& aNamespaceDef, 
				const RPointerArray<TDesC16>* aObjects,
				const RArray<TItemId>* aEvents, 
				const RArray<TItemId>* aRelations );
		
	 	/**
		 * Creates a serialized buffer with id marked to be removed
		 *
		 * @param aNamespaceDef namespace where items should be removed
		 * @param aObjects objects uri to remove (could be NULL)
		 * @param aEvents events id to remove (could be NULL)
		 * @param aRelations relation id to remove (could be NULL)
		 * @return serialized buffer
		 */
		CMdCSerializationBuffer* RemoveCommonL( 
				CMdENamespaceDef& aNamespaceDef, 
				const RArray<TItemId>* aObjects, 
				const RArray<TItemId>* aEvents, 
				const RArray<TItemId>* aRelations );

		/**
		 * Creates a serialized buffer with defs id items to be exported
		 *
		 * @param aNamespaceDef namespace from where items should be removed 
		 *                      (NULL means from every)
		 * @param aObjectDefs objectDefs to export
		 * @param aRelationDefs
		 * @param aEventDefs
		 * @return serialized items ids
		 */
		CMdCSerializationBuffer* ExportCommonL( 
				const CMdENamespaceDef* aNamespaceDef,
				const RPointerArray<CMdEObjectDef>* aObjectDefs,
                const RPointerArray<CMdERelationDef>* aRelationDefs,
                const RPointerArray<CMdEEventDef>* aEventDefs );

	private:
		/**
		 * If aId is KNoId it isn't used. If aUri is KNullDesC, it isn't used.
		 */
		CMdEObject* GetObjectL( CMdEObjectDef& aObjectDef, const TItemId aId, 
				                const TInt64 aGuidHigh, const TInt64 aGuidLow, 
				                const TDesC& aUri, TMdCQueryLockType aLocktype,
				                TBool aIncludeFreetexts );

		/**
		 * Return namespace definition or default namespace definition
		 * 
		 * @param aNamespaceDef namespace definition or NULL
		 * @return default namespace definition if parameter was NULL, 
		 *         othrewise namespace definition which was given as parameter
		 */
		CMdENamespaceDef* GetNamespaceDefL( CMdENamespaceDef* aNamespaceDef );
		
    private: // data members

		/**
		*
		*/
		CMdESessionStartupAO* iSessionStartupAO;

        /**
        * array of observer notifier active objects
        */
        RPointerArray<CMdENotifierAO> iNotifiers;
                
		/** Open observer. */
		MMdESessionObserver* iSessionObserver;
		
		RChunk iSchemaChunk;
		
		CMdCSerializationBuffer* iSchemaBuffer;
		
		RPointerArray<CMdENamespaceDef> iNamespaceDefs;
		
		CMdESessionAsyncHandler* iAsyncHandler;

		TUint32 iNextQueryId;
		
	protected:

		/**
	 	* iSession, client server session 
	 	*/
	    RMdEEngineSession iSession;
	
		enum TSessionState
			{
				EMdESessionClosed = 0,
				EMdESessionOpen,
				EMdESessionError,
			};
		TSessionState iSessionState;
	};

#endif  // __MDESESSIONIMPL_H__
