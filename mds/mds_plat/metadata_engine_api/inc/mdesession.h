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
* Description:  Metadata engine client session
*
*/


#ifndef __MDESESSION_H__
#define __MDESESSION_H__


#include <e32base.h>
#include <badesca.h>

#include <mdccommon.h>


/* Forward declarations. */
class CMdESession;
class CMdEObjectDef;
class CMdERelationDef;
class CMdEEventDef;
class CMdEObject;
class TMdEObject;
class CMdERelation;
class TMdERelation;
class CMdEEvent;
class CMdEObjectQuery;
class CMdERelationQuery;
class CMdEEventQuery;
class CMdELogicCondition;
class CMdEInstanceItem;
class RMdEEngineSession;
class MMdEQueryObserver;
class CMdENamespaceDef;
class RMdEDataBuffer;
class CMdECondition;



/**
 * Observer interface for a metadata engine session.
 * 
 * 
 * 
 * Examples of MdE session usage:
 * 
 * The class that needs a MdE session needs to implement MMdESessionObserver interface to get
 * a notification of completed session opening or an error that has occurred.
 * 
 * class CMdESessionExample : public MMdESessionObserver
 * {
 * 		void HandleSessionOpened(CMdESession& aSession, TInt aError);
 *   	void HandleSessionError(CMdESession& aSession, TInt aError);
 * 		...
 * 
 * 		CMdESession* iMdeSession;
 * };
 * 
 * The session is opened simply by creating an instance of CMdESession.
 * 
 * void CMdESessionExample::OpenSession()
 * {
 * 		iMdeSession = CMdESession::NewL( *this );
 * }
 * 
 * void CMdESessionExample::HandleSessionOpened(CMdESession& aSession, TInt aError)
 *	{
 *		if ( KErrNone != aError ) {
 * 			// Error occurred when opening session. iMdeSession must be deleted and new
 * 			// session opened if we wish to use MdE.
 * 
 * 			delete iMdeSession; iMdeSession = NULL;
 * 			return;
 * 		}
 * 		
 * 		// The session was opened successfully.
 * 		...
 *	}
 * 
 * void CMdESessionError::HandleSessionError(CMdESession& aSession, TInt aError)
 * {
 * 		// This function is called when an error has occurred in the session by some
 * 		// external source (e.g. other clients). It’s important to notice, that 
 * 		// this method is called when the session has already been opened, not when 
 * 		// error happens during opening. Session can't be used anymore and it must be deleted.
 * 
 * 		delete iMdeSession; iMdeSession = NULL;
 * }
 */
class MMdESessionObserver
	{
public:

	/**
	 * Called to notify the observer that opening the session has been 
	 * completed and, if the opening succeeded, the session is ready for use.
	 *
	 * @param aSession session
	 * @param aError   <code>KErrNone</code>, if opening the session succeeded;
	 *                 or one of the system-wide error codes, if opening the 
	 *                 session failed
	 */
	virtual void HandleSessionOpened(CMdESession& aSession, TInt aError) = 0;

	/**
	 * Called to notify the observer about errors, which are not a direct 
	 * consequence of the operations initiated by the client but caused by 
	 * some external source (e.g., other clients). The error cannot be 
	 * recovered and all on-going operations initiated by the client have been 
	 * aborted. Any attempts to continue using the session will cause a panic. 
	 * The client should close the session immediately and try to open a new 
	 * session, if it needs to continue using the metadata engine.
	 *
	 * @param aSession session
	 * @param aError one of the system-wide error codes
	 */
	virtual void HandleSessionError(CMdESession& aSession, TInt aError) = 0;
    };


/**
 * Observer interface for modifications of the metadata engine schema.
 */
class MMdESchemaObserver
	{
public:

	/**
	 * Called to notify the observer that the schema has been modified.
	 *
	 * @param none
	 */
	virtual void HandleSchemaModified() = 0;
    };
    
/**
 * Observer interface for modifications of the objects in the metadata engine 
 * database.
 * 
 * Examples of observers.
 * A class that is interested in observing events in the DB must implement observer interfaces
 * and register as observer. Following examples show how this is done for objects but observing
 * relations and events works the same way.
 * 
 * class CMdeObserverExample : public MMdEObjectObserver, public MMdEObjectPresentObserver
 * {
 *		void HandleObjectNotification(CMdESession& aSession, TObserverNotificationType aType,
 *						const RArray<TItemId>& aObjectIdArray);
 *		void HandleObjectPresentNotification(CMdESession& aSession, TBool aPresent,
 *		 				const RArray<TItemId>& aObjectIdArray);
 * 		...
 * 		CMdESession* iMdeSession;
 * };
 * 
 * void CMdeObserverExample::ConstructL()
 * {
 * 		// Register this class as observer and start listening to object remove events.
 * 		// The logic condition can be as complicated as is necessary. In this example
 * 		// the condition as simple as possible.
 *		CMdELogicCondition* condition = CMdELogicCondition::NewL( ELogicConditionOperatorAnd );
 *		iMdeSession->AddObjectObserverL( *this, condition, ENotifyRemove, 
 *   		&iMdeSession->GetDefaultNamespaceDefL() );
 * 
 * 		// Start listening to changes in object present status.
 * 		iMdeSession->AddObjectPresentObserverL( *this );
 * }
 * 
 * @see MMdEObjectObserver::HandleObjectNotification
 * void CMdeObserverExample::HandleObjectNotification(CMdESession& aSession, TObserverNotificationType aType,
 *						const RArray<TItemId>& aObjectIdArray)
 * {
 *	 	if ( aType == ENotifyAdd )
 * 		{	
 * 		// object was added to DB
 * 		} else if ( aType == ENotifyModify )
 * 		{
 * 		// object was modified
 * 		} else if ( aType == ENotifyRemove )
 * 		{
 * 		// object was removed from DB
 * 		}
 * 
 * 		// aObjectIdArray contains ids for all objects that were added/modified/removed
 * }
 * 
 * void CMdeObserverExample::HandleObjectPresentNotification(CMdESession& aSession, 
 *			TBool aPresent, const RArray<TItemId>& aObjectIdArray)
 *	{
 * 		if ( aPresent )
 * 		{
 * 			// objects in aObjectIdArray were set as present
 * 		}
 *  }
 */
class MMdEObjectObserver
	{
public:

	/**
	 * Called to notify the observer that new objects has been
	 * added/modified/removed in the metadata engine database.
	 * 
	 * @param aSession session
	 * @param aType defines if object was added/modified/remove
	 * @param aObjectIdArray IDs of added object
	 * @see CMdESession::AddObjectObserverL
	 * @see CMdELogicCondition
	 */
	virtual void HandleObjectNotification(CMdESession& aSession, 
						TObserverNotificationType aType,
						const RArray<TItemId>& aObjectIdArray) = 0;
    };

class MMdEObjectObserverWithUri
    {
public:

    /**
     * Called to notify the observer that new objects has been
     * added/modified/removed in the metadata engine database.
     * 
     * @param aSession session
     * @param aType defines if object was added/modified/remove
     * @param aObjectIdArray IDs of added object
     * @param aUriArray Uris of added object
     * @see CMdESession::AddObjectObserverL
     * @see CMdELogicCondition
     */
    virtual void HandleUriObjectNotification(CMdESession& aSession, 
                        TObserverNotificationType aType,
                        const RArray<TItemId>& aObjectIdArray,
                        const RPointerArray<HBufC>& aObjectUriArray) = 0;
    };
    
/**
 * Observer interface for modifications of the objects in the metadata engine 
 * database.
 */
class MMdEObjectPresentObserver
	{
public:

	/**
	 * Called to notify the observer that objects has been set
	 * to present or not present state in the metadata engine database.
	 *
	 * @param aSession session
	 * @param aPresent state: ETrue - present or  EFales - not present
	 * @param aObjectIdArray object IDs which are set to present state
	 */
	virtual void HandleObjectPresentNotification(CMdESession& aSession, 
			TBool aPresent, const RArray<TItemId>& aObjectIdArray) = 0;
    };

/**
 * Observer interface for modifications of the relations in the metadata 
 * engine database.
 */
class MMdERelationPresentObserver
	{
public:

	/**
	 * Called to notify the observer that objects has been set
	 * to present or not present state in the metadata engine database.
	 *
	 * @param aSession session
	 * @param aPresent state: ETrue - present or  EFales - not present
	 * @param aObjectIdArray object IDs which are set to present state
	 */
	virtual void HandleRelationPresentNotification(CMdESession& aSession,
			TBool aPresent, const RArray<TItemId>& aRelationIdArray) = 0;
    };


/**
 * Observer interface for modifications of relations in the metadata engine 
 * database. This observer returns only relations IDs.
 */
class MMdERelationObserver
	{
public:

	/**
	 * Called to notify the observer that new relations has been
	 * added/modified/removed in the metadata engine database.
	 *
	 * @param aSession session
	 * @param aType defines if relation was added/modified/remove
	 * @param aRelationIdArray IDs of relations
	 */
	virtual void HandleRelationNotification(CMdESession& aSession, 
			TObserverNotificationType aType,
			const RArray<TItemId>& aRelationIdArray) = 0;
    };

/**
 * Observer interface for modifications of relations in the metadata engine 
 * database. This observer returns relations (not only IDs).
 */
class MMdERelationItemObserver
	{
public:

	/**
	 * Called to notify the observer that new relations has been
	 * added/modified/removed in the metadata engine database.
	 *
	 * @param aSession session
	 * @param aType if relation was added/modified/remove
	 * @param aRelationArray relations
	 */
	virtual void HandleRelationItemNotification(CMdESession& aSession, 
			TObserverNotificationType aType,
			const RArray<TMdERelation>& aRelationArray) = 0;
    };

/**
 * Observer interface for additions or removes of new events to the metadata 
 * engine database.
 */
class MMdEEventObserver
	{
public:

	/**
	 * Called to notify the observer that new events has been
	 * added or removed in the metadata engine database.
	 *
	 * @param aSession session
	 * @param aType if event was added or removed
	 * @param aEventIdArray IDs of added events
	 */
	virtual void HandleEventNotification(CMdESession& aSession, 
			TObserverNotificationType aType,
			const RArray<TItemId>& aEventIdArray) = 0;

    };

/**
 * Metadata engine session.
 */
NONSHARABLE_CLASS(CMdESession) : public CBase
	{
public:

	/* Constructors and destructor. */

	/**
	 * Opens a new metadata engine session.
	 *
	 * @param aObserver observer to notify when opening the session has been 
	 *                  completed
	 *
	 * @return metadata engine session
	 */
	IMPORT_C static CMdESession* NewL(MMdESessionObserver& aObserver);

	/**
	 * Opens a new metadata engine session and leaves the session in the 
	 * cleanup stack.
	 *
	 * @param aObserver observer to notify when opening the session has been 
	 *                  completed
     *
	 * @return  metadata engine session
	 */
	IMPORT_C static CMdESession* NewLC(MMdESessionObserver& aObserver);

	/**
	 * Destructor.
	 */
	virtual ~CMdESession();

	/* Methods for managing namespace definitions. */

	/**
	 * Returns the number of namespace definitions.
	 *
	 * @return number of namespace definitions
	 */
	virtual TInt NamespaceDefCount() const = 0;

	/**
	 * Returns the namespace definition at the specified index.
	 *
	 * @param aIndex index
	 *
	 * @return namespace definition
	 */
	virtual CMdENamespaceDef& NamespaceDefL(TInt aIndex) = 0;

	/**
	 * Returns the namespace definition with the specified name.
	 *
	 * @param aName name
	 *
	 * @return namespace definition; or 0, if there is no object definition
	 *         with the specified name
	 */
	virtual CMdENamespaceDef& GetNamespaceDefL(const TDesC& aName) = 0;

	 /* Returns the default namespace definition.
	 *
	 * @return  the default namespace definition.
	 */
	virtual CMdENamespaceDef& GetDefaultNamespaceDefL() = 0;

	/**
	 * Adds a new relation definition to namespace,
	 *
	 * Example:
	 *  void AddRelationL()
	 *  {
	 *    _LIT( TestRelation, "TestRelation" );
	 *    TBuf <15> relname( TestRelation );
	 * 	  iMdeSession->AddRelationDefL( *iDefaultNamespaceDef, relname );
	 *  }
	 * 
	 * @param aNamespaceDef namespace definition to which relation belongs
	 * @param aName         relation definitions name
	 */
	virtual void AddRelationDefL(const CMdENamespaceDef &aNamespaceDef, 
			const TDesC &aName) = 0;

	/**
	 * Adds a new event definition to namespace.
	 * 
	 * Example:
	 *  void AddEventDefL()
	 *  {
	 *    _LIT( TestEvent, "TestEvent" );
	 *    TBuf <10> eventName( TestEvent );
	 * 	  iMdeSession->AddRelationDefL( *iDefaultNamespaceDef, eventName );
	 *  }
	 *
	 * @param aNamespaceDef namespace definition to which event belongs
	 * @param aName         event definitions name
	 */
	virtual void AddEventDefL(const CMdENamespaceDef &aNamespaceDef, 
			const TDesC &aName) = 0;

	/* Methods for managing objects. */

    /**
     * Adds multiple instance items to metadata database.
     * The array contains the items, any other type than object, relation or 
     * event causes a leave.
	 *
     * @param aItems list of items to add
     * @return first item error
     */
	virtual TInt AddItemsL( RPointerArray<CMdEInstanceItem>& aItems ) = 0;

    /**
     * Commits multiple instance items to metadata database.
     * The array contains the items, any other type than object, relation or 
     * event causes a leave.
     * 
     * @param aItems list of items to add
     * @return first item error
     */
	virtual TInt UpdateItemsL( RPointerArray<CMdEInstanceItem>& aItems ) = 0;

   /**
     * Adds multiple instance items asynchronously to metadata database.
     * The array contains the items, any other type than object, relation or 
     * event causes a leave. Returned serialized list of item IDs must be 
     * deserialized with DeserializeItemsL method.
     * 
     * Example:
     *  class CExampleActiveObject : public CActive
     *  {
     *    void CActiveObject::AddItemsL();
     *    ...
     *    RPointerArray<CMdEInstanceItem> iItems;
     *    RMdEDataBuffer iResultBuffer;
     *  };
     * 
     *  void CExampleActiveObject::AddItemsL()
     *  {
     *    iMdeSession->AddItemsAsyncL( iItems, iStatus, iResultBuffer );
     *    SetActive();
     *  }
     * 
     * When adding items is finished, RunL() will be called.
     * 
     * @param aItems List of item to add.
     * @param aStatus Returns the result code after the asynchronous call 
     *                completes.
     * @param aSerializedItemIds Returned serialized list of item IDs.
     */
	virtual void AddItemsAsyncL( 
			RPointerArray<CMdEInstanceItem>& aItems, 
			TRequestStatus& aStatus,
			RMdEDataBuffer& aSerializedItemIds) = 0;

   /**
     * Commits multiple instance items asynchronously to metadata database.
     * The array contains the items, any other type than object, relation or 
     * event causes a leave. Returned serialized list of item IDs must be 
     * deserialized with DeserializeItemsL method.
     * 
     * Example:
     *  class CExampleActiveObject : public CActive
     *   {
     *     void CActiveObject::UpdateItemsL();
     *     ...
     *     RPointerArray<CMdEInstanceItem> iItems;
     *     RMdEDataBuffer iResultBuffer;
     *   };
     * 
     *   void CExampleActiveObject::UpdateItemsL()
     *   {
     *     iMdeSession->UpdateItemsAsyncL( iItems, iStatus, iResultBuffer );
     *     SetActive();
     *   }
     * 
     * @param aItems List of item to add.
     * @param aStatus Returns the result code after the asynchronous call 
     *                completes.
     * @param aSerializedItemIds Returned serialized list of item IDs.
     */
	virtual void UpdateItemsAsyncL( 
			RPointerArray<CMdEInstanceItem>& aItems, 
			TRequestStatus& aStatus,
			RMdEDataBuffer& aSerializedItemIds ) = 0;

	/**
	 * Constructs a new empty object. Note that the object is not inserted in 
	 * the database. The ownership of the new object is passed to the caller 
	 * (that is, the caller is responsible for deleting the object).
	 *
	 * Example:
	 *  _LIT( KObjectDef, "ObjectDefName" );
	 *  TBuf<13> objDefStr( KObjectDef );
	 *  CMdENamespaceDef& defNS = iMdESession->GetDefaultNamespaceDefL();
	 *	CMdEObjectDef& mdeObjectDef = defNS.GetObjectDefL( objDefStr );
	 *  CMdEObject* mdeObject = iMdESession->NewObjectL( mdeObjectDef, aUri );
	 * 
	 * @param aDef definition of the new object
	 * @param aUri URI of the new object
	 * @param aMediaId media ID of the new object (default 0)
	 *
	 * @return new object
	 */
	virtual CMdEObject* NewObjectL( CMdEObjectDef& aDef, const TDesC& aUri, 
                                    TUint32 aMediaId = 0 ) = 0;

	/**
	 * Constructs a new empty object and leaves it in the cleanup stack. 
	 * Note that the object is not inserted in the database. The ownership of 
	 * the new object is passed to the caller (that is, the caller is 
	 * responsible for deleting the object).
	 * 
 	 * Example:
	 *  _LIT( KObjectDef, "ObjectDefName" );
	 *  TBuf<13> objDefStr( KObjectDef );
	 *  CMdENamespaceDef& defNS = iMdESession->GetDefaultNamespaceDefL();
	 *	CMdEObjectDef& mdeObjectDef = defNS.GetObjectDefL( objDefStr );
	 *  CMdEObject* mdeObject = iMdESession->NewObjectLC( mdeObjectDef, aUri );
	 *
	 * @param aDef definition of the new object
	 * @param aUri URI of the new object
	 * @param aMediaId media ID of the new object (default 0)
	 *
	 * @return new object
	 */
	virtual CMdEObject* NewObjectLC( CMdEObjectDef& aDef, const TDesC& aUri, 
                                     TUint32 aMediaId = 0 ) = 0;

    /**
     * Adds the specified new object to metadata database.
     * aObject is modified so that it has the new item ID.
     * If object adding fails object's ID is KNoId, 
     * otherwise object adding is successful.
     *
     * @param aObject object to be added
     *
     * @return item ID of the added object
     */
    virtual TItemId AddObjectL(CMdEObject& aObject) = 0;

    /**
     * Adds multiple object items to the metadata engine database.
     * The array contains the object items. aObjects are modified 
     * so that those has the new item IDs. If object adding fails
     * object's ID is KNoId, otherwise object adding is successful.
     *
     * @param aObjects  list of objects to be added
     * @return first object error
     */
    virtual TInt AddObjectsL(RPointerArray<CMdEObject>& aObjects) = 0;

	/**
	 * Deserialize serialized list of item IDs
	 *
	 * @param aSerializedItemIds serialized list of item IDs
	 * @param aResultObjects     if not NULL object IDs are deserialized from 
	 *                           buffer to this ID array
	 * @param aResultEvents      if not NULL event IDs are deserialized from 
	 *                           buffer to this ID array
	 * @param aResultRelations   if not NULL relation IDs are deserialized 
	 *                           from buffer to this ID array
	 *
	 * @return error code of first failed item, if no errors KErrNone
	 */
	virtual TInt DeserializeIdsL( RMdEDataBuffer& aSerializedItemIds, 
			RArray<TItemId>* aResultObjects = NULL, 
			RArray<TItemId>* aResultEvents = NULL, 
			RArray<TItemId>* aResultRelations = NULL ) = 0;
						   
	/**
	 * Deserialize serialized list of items
	 *
	 * @param aSerializedItems serialized list of items
	 * @param aItems items are deserialized from buffer to this item array
	 *
	 * @return first item error
	 */
	virtual TInt DeserializeItemsL( RMdEDataBuffer& aSerializedItems, 
			RPointerArray<CMdEInstanceItem>& aItems ) = 0;

	/**
	 * Removes the object with the specified identifier from metadata database.
	 *
	 * @param aId identifier
	 * @param aNamespaceDef namespace where object is removed, if NULL default 
	 *                      namespace is used
	 * @return KNoId if removing has failed, otherwise removed object's ID
	 */
	virtual TItemId RemoveObjectL( TItemId aId, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;

	/**
	 * Removes the object with the specified URI from metadata database.
	 *
	 * @param aUri URI
	 * @param aNamespaceDef namespace from remove object, if NULL default 
	 *                      namespace is used
	 * @return KNoId if removing has failed, otherwise removed object's ID
	 */
	virtual TItemId RemoveObjectL( const TDesC& aUri, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;

	/**
	 * Removes the array of objects with the specified identifier from 
	 * metadata database.
	 *
	 * @param aId object IDs to be removed
	 * @param aResult result array where succefully removed object IDs are 
	 *                added (KNoId is added from those objects which removing 
	 *                has failed)
	 * @param aNamespaceDef namespace where object is removed, if NULL default 
	 *                      namespace is used
	 * @return first item error
	 */
	virtual TInt RemoveObjectsL( 
			const RArray<TItemId>& aId, RArray<TItemId>& aResult, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;

	/**
	 * Removes the array of objects with the specified URI from metadata 
	 * database.
	 *
	 * @param aUri object URIs to be removed
	 * @param aResult result array where succefully removed object IDs are 
	 *                added (KNoId is added from those objects which removing 
	 *                has failed)
	 * @param aNamespaceDef namespace where object is removed, if NULL default 
	 *                      namespace is used
	 * @return first item error
	 */
	virtual TInt RemoveObjectsL( 
			const RPointerArray<TDesC>& aUri, RArray<TItemId>& aResult, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;

	/**
	 * Asynchronously removes the array of objects with the specified 
	 * object IDs from metadata database. Returned serialized list of item IDs 
	 * must be deserialized with DeserializeIdsL method.
	 *
	 * @param aId object IDs to be removed
     * @param aStatus returns the result code after the asynchronous call 
     *                completes.
     * @param aSerializedObjectIds returned serialized list of object IDs
	 * @param aNamespaceDef namespace where object is removed, if NULL default 
	 *                      namespace is used
	 */
	virtual void RemoveObjectsAsyncL( 
			const RArray<TItemId>& aId, TRequestStatus& aStatus, 
			RMdEDataBuffer& aSerializedObjectIds, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;

	/**
	 * Asynchronously removes the array of objects with the specified URI from 
	 * metadata database. Returned serialized list of item IDs must be 
	 * deserialized with DeserializeIdsL method.
	 *
	 * @param aUri object URIs to be removed
     * @param aStatus returns the result code after the asynchronous call 
     *                completes.
     * @param aSerializedObjectIds returned serialized list of object IDs
	 * @param aNamespaceDef namespace where object is removed, if NULL default 
	 *                      namespace is used
	 */
	virtual void RemoveObjectsAsyncL( 
			const RPointerArray<TDesC>& aUri, TRequestStatus& aStatus, 
			RMdEDataBuffer& aSerializedObjectIds, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;
	
	/**
	 * Returns the object with the specified ID and specified object 
	 * definition.
	 *
	 * Example:
	 *   CMdENamespaceDef& defaultNamespace = iSession->GetDefaultNamespaceDefL();
     * 	 CMdEObjectDef& imageObjDef = defaultNamespace.GetObjectDefL( MdeConstants::Image::KImageObject );
	 * 	 CMdEObject* object = iSession->GetObjectL( aObjectId, imageObjDef );
	 * 
	 * @param aId object ID
	 * @param aObjectDef object definition
	 *
	 * @return object or NULL, if there is no object with the specified 
	 *         identifier in the metadata engine database
	 */
	virtual CMdEObject* GetObjectL( const TItemId aId, 
			CMdEObjectDef& aObjectDef ) = 0;

	/**
	 * Returns the object (object may contain only properties defined in 
	 * "Object") with the specified ID and specified namespace (if namespace 
	 * is NULL, the default namespace is used).
	 *
	 * @param aId object ID
	 * @param aNamespaceDef namespace
	 *
	 * @return  object or NULL, if there is no object with the specified 
	 *          identifier in the metadata engine database
	 */
	virtual CMdEObject* GetObjectL( const TItemId aId, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;

	/**
	 * Returns the object (object will contain all it's properties) 
	 * with the specified ID and specified namespace 
	 * (if namespace is NULL, the default namespace is used).
	 *
	 * @param aId object ID
	 * @param aNamespaceDef namespace
	 *
	 * @return  object or NULL, if there is no object with the specified 
	 *          identifier in the metadata engine database
	 */
	virtual CMdEObject* GetFullObjectL( const TItemId aId, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;

	/**
	 * Returns the object with the specified GUID and specified object 
	 * definition.
	 * 
	 * Example:
	 *   CMdENamespaceDef& defaultNamespace = iSession->GetDefaultNamespaceDefL();
     * 	 CMdEObjectDef& imageObjDef = defaultNamespace.GetObjectDefL( MdeConstants::Image::KImageObject );
     *   CMdEObject* object = iMdeSession->GetObjectL( 12345, 67890, imageObjDef );
	 *
	 * @param aGuidHigh GUID high
	 * @param aGuidLow GUID low
	 * @param aObjectDef object definition
	 *
	 * @return  object or NULL, if there is no object with the specified 
	 *          identifier in the metadata engine database
	 */
	virtual CMdEObject* GetObjectL( 
			const TInt64 aGuidHigh, const TInt64 aGuidLow, 
			CMdEObjectDef& aObjectDef ) = 0;

	/**
	 * Returns the object (object may contain only properties definied in 
	 * "Object") with the specified GUID and specified namespace (if namespace 
	 * is NULL, the default namespace is used).
	 *
	 * Example:
     *   CMdEObject* object = iMdeSession->GetObjectL( 12345, 67890 );
	 * 
	 * @param aGuidHigh  GUID high
	 * @param aGuidLow  GUID low
	 * @param aNamespaceDef namespace
	 *
	 * @return  object or NULL, if there is no object with the specified 
	 *          identifier in the metadata engine database
	 */
	virtual CMdEObject* GetObjectL( 
			const TInt64 aGuidHigh, const TInt64 aGuidLow, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;

	/**
	 * Returns the object (object will contain all it's properties) 
	 * with the specified GUID and specified namespace 
	 * (if namespace is NULL, the default namespace is used).
	 * 
	 * Example:
     *   CMdEObject* object = iMdeSession->GetFullObjectL( 12345, 67890 );
	 *
	 * @param aGuidHigh GUID high
	 * @param aGuidLow GUID low
	 * @param aNamespaceDef namespace
	 *
	 * @return  object or NULL, if there is no object with the specified 
	 *          identifier in the metadata engine database
	 */
	virtual CMdEObject* GetFullObjectL( 
			const TInt64 aGuidHigh, const TInt64 aGuidLow, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;

	/**
	 * Returns the object with the specified URI and specified object 
	 * definition.
	 *
	 * Example:
	 *   CMdENamespaceDef& defaultNamespace = iSession->GetDefaultNamespaceDefL();
     * 	 CMdEObjectDef& imageObjDef = defaultNamespace.GetObjectDefL( MdeConstants::Image::KImageObject );
     *   CMdEObject* object = iMdeSession->GetObjectL( aUri, imageObjDef );
	 *
	 * @param aUri object URI
	 * @param aObjectDef object definition
	 *
	 * @return  object or NULL, if there is no object with the specified 
	 *          identifier in the metadata engine database
	 */
	virtual CMdEObject* GetObjectL( const TDesC& aUri, 
			CMdEObjectDef& aObjectDef ) = 0;

	/**
	 * Returns the object (object may contain only properties definied in 
	 * "Object") with the specified URI specified namespace (if namespace is 
	 * NULL, the default namespace is used).
	 *
	 * @param aUri object URI
	 * @param aNamespaceDef namespace
	 *
	 * @return  object or NULL, if there is no object with the specified 
	 *          identifier in the metadata engine database
	 */
	virtual CMdEObject* GetObjectL( const TDesC& aUri, 
			CMdENamespaceDef* aNamespaceDef = NULL) = 0;

	/**
	 * Returns the object (object will contain all it's properties) with the 
	 * specified URI specified namespace (if namespace is NULL, the default 
	 * namespace is used).
	 *
	 * @param aUri object URI
	 * @param aNamespaceDef namespace
	 *
	 * @return  object or NULL, if there is no object with the specified 
	 *          identifier in the metadata engine database
	 */
	virtual CMdEObject* GetFullObjectL( const TDesC& aUri, 
			CMdENamespaceDef* aNamespaceDef = NULL) = 0;

	/**
	 * Opens the object with the specified ID and specified object 
	 * definition for modifications. The object is locked so that nobody else 
	 * can open it for modifications. The modifications are committed by 
	 * calling the <code>CommitObject()</code> method, which releases the lock.
	 * The modifications can be canceled by calling the 
	 * <code>CancelObject()</code> method, which also releases the lock.
	 *
	 * Example:
	 *  CMdENamespaceDef& defaultNamespace = iMdeSession->GetDefaultNamespaceDefL();
     *	CMdEObjectDef& imageDef = defaultNamespace.GetObjectDefL( MdeConstants::Image::KImageObject );
	 *  CMdEObject* image = iMdESession->OpenObjectL( 17, imageDef );
	 * 
	 *  CMdEPropertyDef& lastModDatePropDef = imageDef.GetPropertyDefL(
     *   		Object::KLastModifiedDateProperty );
     * 
     *  CMdEProperty* lastModDateProp = NULL;
     *  image->Property( lastModDatePropDef, lastModDateProp, 0 );
     * 
     *  TTime currenttime( 0 );
     *	currenttime.UniversalTime();
     * 
     *  if ( lastModDateProp )
     *      {
     *      lastModDateProp->SetTimeValueL( currenttime );
     *      }
     *  else
     *      {
     *      image->AddTimePropertyL( lastModDatePropDef, currenttime );
     *      }
     *  }
     *  iMdeSession->CommitObjectL( *image );
	 *  
	 * 
	 * @param aId  object ID
	 * @param aObjectDef object definition
	 *
	 * @return  Object or NULL, if there is no object with the specified 
     *          identifier in the metadata engine database.
	 */
	virtual CMdEObject* OpenObjectL( const TItemId aId, 
			CMdEObjectDef& aObjectDef ) = 0;

	/**
	 * Opens the object (object may contain only properties definied in "Object") 
	 * with the specified ID and specified namespace 
	 * (if namespace is NULL, the default namespace is used) for modifications.
	 * The object is locked so that nobody else can open it for modifications.
	 * The modifications are committed by calling the <code>CommitObject()</code>
	 * method, which releases the lock. The modifications can be canceled by
	 * calling the <code>CancelObject()</code> method, which also releases the
	 * lock.
	 *
	 * @param aId  object ID.
	 * @param aNamespaceDef namespace
	 *
	 * @return  Object or NULL, if there is no object with the specified 
     *          identifier in the metadata engine database.
	 */
	virtual CMdEObject* OpenObjectL( const TItemId aId, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;

	/**
	 * Opens the object (object will contain all it's properties) 
	 * with the specified ID and specified namespace 
	 * (if namespace is NULL, the default namespace is used) for modifications.
	 * The object is locked so that nobody else can open it for modifications.
	 * The modifications are committed by calling the <code>CommitObject()</code>
	 * method, which releases the lock. The modifications can be canceled by
	 * calling the <code>CancelObject()</code> method, which also releases the
	 * lock.
	 *
	 * @param aId  object ID.
	 * @param aNamespaceDef namespace
	 *
	 * @return  Object or NULL, if there is no object with the specified 
     *          identifier in the metadata engine database.
	 */
	virtual CMdEObject* OpenFullObjectL( const TItemId aId, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;


	/**
	 * Opens the object with the specified GUID and specified object 
	 * definition for modifications. The object is locked so that nobody else 
	 * can open it for modifications. The modifications are committed by 
	 * calling the <code>CommitObject()</code> method, which releases the lock.
	 * The modifications can be canceled by calling the 
	 * <code>CancelObject()</code> method, which also releases the lock.
	 *
	 * @param aGuidHigh  GUID high
	 * @param aGuidLow  GUID low
	 * @param aObjectDef object definition
	 *
	 * @return  Object or NULL, if there is no object with the specified 
     *          identifier in the metadata engine database.
	 */
	virtual CMdEObject* OpenObjectL( 
			const TInt64 aGuidHigh, const TInt64 aGuidLow, 
			CMdEObjectDef& aObjectDef ) = 0;

	/**
	 * Opens the object (object may contain only properties definied in "Object") 
	 * with the specified GUID and specified namespace 
	 * (if namespace is NULL, the default namespace is used) for modifications.
	 * The object is locked so that nobody else can open it for modifications.
	 * The modifications are committed by calling the <code>CommitObject()</code>
	 * method, which releases the lock. The modifications can be canceled by
	 * calling the <code>CancelObject()</code> method, which also releases the
	 * lock.
	 *
	 * @param aGuidHigh  GUID high
	 * @param aGuidLow  GUID low
	 * @param aNamespaceDef namespace
	 *
	 * @return  Object or NULL, if there is no object with the specified 
     *          identifier in the metadata engine database.
	 */
	virtual CMdEObject* OpenObjectL( 
			const TInt64 aGuidHigh, const TInt64 aGuidLow, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;

	/**
	 * Opens the object (object will contain all it's properties) 
	 * with the specified GUID and specified namespace 
	 * (if namespace is NULL, the default namespace is used) for modifications.
	 * The object is locked so that nobody else can open it for modifications.
	 * The modifications are committed by calling the <code>CommitObject()</code>
	 * method, which releases the lock. The modifications can be canceled by
	 * calling the <code>CancelObject()</code> method, which also releases the
	 * lock.
	 *
	 * @param aGuidHigh  GUID high
	 * @param aGuidLow  GUID low
	 * @param aNamespaceDef namespace
	 *
	 * @return  Object or NULL, if there is no object with the specified 
     *          identifier in the metadata engine database.
	 */
	virtual CMdEObject* OpenFullObjectL( 
			const TInt64 aGuidHigh, const TInt64 aGuidLow, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;


	/**
	 * Opens the object with the specified URI for modifications and specified 
	 * object definition. The object is locked so that nobody else can open it 
	 * for modifications. The modifications are committed by calling the 
	 * <code>CommitObject()</code> method, which releases the lock. The 
	 * modifications can be canceled by calling the <code>CancelObject()</code> 
	 * method, which also releases the lock.
	 *
	 * @param aUri  object URI
	 * @param aObjectDef object definition
	 *
	 * @return  Object or NULL, if there is no object with the specified 
     *          identifier in the metadata engine database.
	 */
	virtual CMdEObject* OpenObjectL( const TDesC& aUri, 
			CMdEObjectDef& aObjectDef ) = 0;

	/**
	 * Opens the object (object may contain only properties definied in "Object") 
	 * with the specified ID and specified namespace 
	 * (if namespace is NULL, the default namespace is used) for modifications.
	 * The object is locked so that nobody else can open it for modifications.
	 * The modifications are committed by calling the <code>CommitObject()</code>
	 * method, which releases the lock. The modifications can be canceled by
	 * calling the <code>CancelObject()</code> method, which also releases the
	 * lock.
	 *
	 * @param aUri  Object URI.
	 * @param aNamespaceDef namespace
	 *
	 * @return  Object or NULL, if there is no object with the specified 
     *          identifier in the metadata engine database.
	 */
	virtual CMdEObject* OpenObjectL( const TDesC& aUri, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;

	/**
	 * Opens the object (object will contain all it's properties) 
	 * with the specified ID and specified namespace 
	 * (if namespace is NULL, the default namespace is used) for modifications.
	 * The object is locked so that nobody else can open it for modifications.
	 * The modifications are committed by calling the <code>CommitObject()</code>
	 * method, which releases the lock. The modifications can be canceled by
	 * calling the <code>CancelObject()</code> method, which also releases the
	 * lock.
	 *
	 * @param aUri  Object URI.
	 * @param aNamespaceDef namespace
	 *
	 * @return  Object or NULL, if there is no object with the specified 
     *          identifier in the metadata engine database.
	 */
	virtual CMdEObject* OpenFullObjectL( const TDesC& aUri, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;

	/**
	 * Gets metadata object id, definition and some flags by URI. Does not return the whole object.
	 * 
	 * @param aObject on return contains information about the metadata object
	 * @param aUri metadata object URI
	 * @param aNamespaceDef namespace definition. If namespace is NULL then the default namespace is used.
	 */
	virtual void CheckObjectL( TMdEObject& aObject, const TDesC& aUri, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;

	/**
	 * Gets metadata object id, definition and some flags by id. Does not return the whole object.
	 * 
	 * @param aObject on return contains information about the metadata object
	 * @param aId metadata object id
	 * @param aNamespaceDef namespace definition. If namespace is NULL then the default namespace is used.
	 */
	virtual void CheckObjectL( TMdEObject& aObject, TItemId aId, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;

	/**
	 * Gets an array of TMdEObject objects that contain metadata object ids, definitions and 
	 * some flags by object ids. Does not return whole objects.
	 * 
	 * @param aObjects on return contains an array of objects containing information about metadata objects
	 * @param aIds array of metadata object ids
	 * @param aNamespaceDef namespace definition. If namespace is NULL then the default namespace is used.
	 */
	virtual void CheckObjectL( RArray<TMdEObject>& aObjects, 
			const RArray<TItemId>& aIds, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;
	
	/**
     * Commits the modifications made to the object to the database
	 * and releases the modification lock.
     * 
	 * @param aObject  Object.
     */
    virtual void CommitObjectL(CMdEObject& aObject) = 0;

	/**
     * Commits the modifications made to objects to the database
	 * and releases the modification locks.
     * 
	 * @param aObject  Object.
     */
    virtual void CommitObjectsL(RPointerArray<CMdEObject>& aObjects) = 0;

    /**
     * Cancels the modifications made to the object and releases the
	 * modification lock.
     * 
	 * @param aObject  Object.
     */
    virtual TItemId CancelObjectL(CMdEObject& aObject) = 0;


	/* Methods for managing relations. */

	/**
	 * Returns the relation with the specified ID and specified namespace 
	 * (if namespace is NULL, the default namespace is used).
	 * 
	 * Note that the ownership of the returned relation is passed to the 
	 * caller of the method (that is, the caller is responsible for deleting 
	 * the relation).
	 *
	 * @param aId  relation ID
	 * @param aNamespaceDef namespace
	 *
	 * @return  relation or NULL, if there is no relation with the specified ID
	 *          in the metadata engine database
	 */
	virtual CMdERelation* GetRelationL(TItemId aId, 
			CMdENamespaceDef* aNamespacedef = NULL) = 0;

	/**
	 * Constructs a new empty relation. Note that the relation is not inserted 
	 * in the database. The ownership of the new relation is passed to the 
	 * caller (that is, the caller is responsible for deleting the relation).
	 * 
	 * Example:
     *	 CMdERelationDef& relationDef = namespaceDef.GetRelationDefL( MdeConstants::Relations::KContains );
     *
     *	 CMdERelation* relation = iMdeSession->NewRelationLC( relationDef, albumObject->Id(), audioObject->Id() );
     *	 iMdeSession->AddRelationL( *relation );
	 *
	 * @param aDef  definition of the new relation
	 * @param aLeftObjectId id of the left side of the relation
	 * @param aRightObjectId id of the right side of the relation
	 * @param aParameter the relation parameter
	 *
	 * @return  new relation
	 */
	virtual CMdERelation* NewRelationLC(CMdERelationDef& aDef, 
			TItemId aLeftObjectId, TItemId aRightObjectId, 
			TInt32 aParameter = 0) = 0;

	/**
	 * Constructs a new empty relation. Note that the relation is not inserted 
	 * in the database. The ownership of the new relation is passed to the 
	 * caller (that is, the caller is responsible for deleting the relation).
	 *
	 * Example:
     *	 CMdERelationDef& relationDef = namespaceDef.GetRelationDefL( MdeConstants::Relations::KContains );
     *
     *	 CMdERelation* relation = iMdeSession->NewRelationL( relationDef, albumObject->Id(), audioObject->Id() );
     *	 iMdeSession->AddRelationL( *relation );
	 * 
	 * @param aDef  definition of the new relation
	 * @param aLeftObjectId id of the left side of the relation
	 * @param aRightObjectId id of the right side of the relation
	 * @param aParameter the relation parameter
	 *
	 * @return  new relation
	 */
	virtual CMdERelation* NewRelationL(CMdERelationDef& aDef, 
			TItemId aLeftObjectId, TItemId aRightObjectId, 
			TInt32 aParameter = 0) = 0;

	/**
	 * Adds a new relation to the metadata engine database.
	 *
	 * @param relation
	 *
	 * @return  identifier of the new relation
	 */
	virtual TItemId AddRelationL( CMdERelation& aRelation ) = 0;

	/**
	 * Commits changes made to the relation to the metadata engine database.
     *
     * @param relation
	 *
	 * @return  identifier of the new relation
	 */
	virtual TItemId UpdateRelationL( CMdERelation& aRelation ) = 0;

	/**
	 * Removes the relation with the specified identifier and specified 
	 * namespace (if namespace is NULL, the default namespace is used)
	 * from the metadata engine database.
	 *
	 * @param aId   ID of the relation to remove.
	 * @param aNamespaceDef namespace
	 *
	 * @return  relation ID or KNoId, if there is no relation with the 
	 *          specified ID in the metadata engine database
	 */
	virtual TItemId RemoveRelationL( TItemId aId, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;
	
	/**
	 * Removes multiple relations with the specified IDs and specified 
	 * namespace (if namespace is NULL, the default namespace is used)
	 * from the metadata engine database.
	 *
	 * @param aId IDs of relations to remove.
	 * @param aSuccessful Successfully removed IDs of relations (if removing 
	 *                    has failed KNoId is added).
	 * @param aNamespaceDef namespace
	 */
	virtual TInt RemoveRelationsL( 
			const RArray<TItemId>& aId, RArray<TItemId>& aSuccessful, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;

	/**
	 * Removes multiple relations asynchronous with the specified IDs and 
	 * specified namespace (if namespace is NULL, the default namespace is 
	 * used) from the metadata engine database. Returned serialized list of 
	 * item IDs must be deserialized with DeserializeIdsL method.
	 *
	 * @param aId IDs of relations to remove.
     * @param aStatus returns the result code after the asynchronous call 
     *                completes.
     * @param aSerializedRelationIds returned serialized list of relation IDs
	 * @param aNamespaceDef namespace
	 */
	virtual void RemoveRelationsAsyncL( 
			const RArray<TItemId>& aId, TRequestStatus& aStatus, 
			RMdEDataBuffer& aSerializedRelationIds, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;

	/* Methods for managing events. */

	/**
	 * Constructs a new empty event. Note that the event is not inserted in the
	 * database. The ownership of the new event is passed to the caller (that 
	 * is, the caller is responsible for deleting the event).
	 *
	 * Example:
	 *   CMdEEventDef* eventDef = &iDefaultNamespace->GetEventDefL( Events::KCreated );
	 *   CMdEEvent* event = iMdeSession->NewEventLC( *eventDef, objectId, time );
	 * 
	 * @param aDef  definition of the new event
	 * @param aObjectId the target object id
	 * @param aTime time of the event
	 * @param aSource source of the event
	 * @param aParticipant participant of the event
	 *
	 * @return  new event
	 */
	virtual CMdEEvent* NewEventLC(CMdEEventDef& aDef, 
			TItemId aObjectId, TTime aTime, 
			const TDesC* aSource = NULL, const TDesC* aParticipant = NULL) = 0;

	/**
	 * Constructs a new empty event. Note that the event is not inserted in the
	 * database. The ownership of the new event is passed to the caller (that 
	 * is, the caller is responsible for deleting the event).
	 *
	 * Example:
	 *   CMdEEventDef* eventDef = &iDefaultNamespace->GetEventDefL( Events::KCreated );
	 *   CMdEEvent* event = iMdeSession->NewEventL( *eventDef, objectId, time );
	 * 
	 * @param aDef  definition of the new event
	 * @param aObjectId the target object id
	 * @param aTime time of the event
	 * @param aSource source of the event
	 * @param aParticipant participant of the event
	 *
	 * @return  new event
	 */
	virtual CMdEEvent* NewEventL(CMdEEventDef& aDef, 
			TItemId aObjectId, TTime aTime, 
			const TDesC* aSource = NULL, const TDesC* aParticipant = NULL) = 0;

	/**
	 * Returns the event with the specified identifier and specified namespace 
	 * (if namespace is NULL, the default namespace is used).
	 * Note that the ownership of the returned event is passed to the caller of
	 * the method (that is, the caller is responsible for deleting the event).
	 *
	 * @param aId  Identifier.
	 * @param aNamespaceDef namespace
	 *
	 * @return  Event or NULL, if there is no event with the specified 
	 *          identifier in the metadata engine database.
	 */
	virtual CMdEEvent* GetEventL(TItemId aId, 
			CMdENamespaceDef* aNamespaceDef = NULL) = 0;

	/**
	 * Adds a new event to the metadata engine database.
	 *
	 * @param event
	 *
	 * @return  Identifier of the new event.
	 */
	virtual TItemId AddEventL( CMdEEvent& aEvent ) = 0;

	/**
	 * Removes the event with the specified identifier and specified namespace 
	 * (if namespace is NULL, the default namespace is used) from the metadata 
	 * engine database.
	 *
	 * @param aId   ID of the event to remove.
	 * @param aNamespaceDef namespace
	 *
	 * @return  event ID or KNoId, if there is no event with the specified ID
	 *          in the metadata engine database
	 */
	virtual TItemId RemoveEventL( TItemId aId, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;
	
	/**
	 * Removes multiple events with the specified IDs and specified 
	 * namespace (if namespace is NULL, the default namespace is used)
	 * from the metadata engine database.
	 *
	 * @param aId IDs of events to remove.
	 * @param aSuccessful Successfully removed IDs of events (if removing has 
	 *                    failed KNoId is added).
	 * @param aNamespaceDef namespace
	 */
	virtual TInt RemoveEventsL(
			const RArray<TItemId>& aId, RArray<TItemId>& aSuccessful, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;
	
	/**
	 * Removes multiple events asynchronous with the specified IDs and 
	 * specified namespace (if namespace is NULL, the default namespace is 
	 * used) from the metadata engine database. Returned serialized list of 
	 * item IDs must be deserialized with DeserializeIdsL method.
	 *
	 * @param aId IDs of events to remove.
     * @param aStatus returns the result code after the asynchronous call 
     *                completes.
     * @param aSerializedEventIds returned serialized list of event IDs
	 * @param aNamespaceDef namespace
	 */
	virtual void RemoveEventsAsyncL(
			const RArray<TItemId>& aId, TRequestStatus& aStatus, 
			RMdEDataBuffer& aSerializedEventIds, 
			CMdENamespaceDef* aNamespaceDef = NULL ) = 0;


	/* Methods for searching objects, relations, and events. */

    /**
	 * Creates a new object query.
	 * 
	 * Note that the ownership of the returned query is passed to the caller of
	 * the method (that is, the caller is responsible for deleting the query).
	 *
	 * @param aNamespaceDef namespace where query will be run
	 * @param aObjectDef object definition which defines objects which will be 
	 *                   queried
	 * @param aObserver observer which callback methods will be called
	 *
	 * @return  New object query.
     */
    virtual CMdEObjectQuery* NewObjectQueryL(CMdENamespaceDef& aNamespaceDef, 
    		CMdEObjectDef& aObjectDef, MMdEQueryObserver* aObserver = 0) = 0;

    /**
	 * Creates a new object query.
	 * 
	 * Note that the ownership of the returned query is passed to the caller of
	 * the method (that is, the caller is responsible for deleting the query).
	 *
	 * @param aObjectDef Object definition which defines the parent object.
	 * @param aObjectDefs Object definitions which defines objects which will 
	 *                    be queried. Ownership of aObjectDefs will change to 
	 *                    query.
	 * @param aObserver Observer which callback methods will be called.
	 *
	 * @return  New object query.
     */
    virtual CMdEObjectQuery* NewObjectQueryL(
    		CMdEObjectDef& aObjectDef, 
    		RPointerArray<CMdEObjectDef>* aObjectDefs, 
    		MMdEQueryObserver* aObserver = 0) = 0;

    /**
	 * Creates a new relation query.
	 * 
	 * Note that the ownership of the returned query is passed to the caller of
	 * the method (that is, the caller is responsible for deleting the query).
	 *
	 * @param aNamespaceDef namespace where query will be run
	 * @param aObserver observer which callback methods will be called
	 *
     * @return  New relation query.
     */
    virtual CMdERelationQuery* NewRelationQueryL(
    		CMdENamespaceDef& aNamespaceDef, 
    		MMdEQueryObserver* aObserver = 0) = 0;

    /**
	 * Creates a new event query.
	 * 
	 * Note that the ownership of the returned query is passed to the caller of
	 * the method (that is, the caller is responsible for deleting the query).
	 *
	 * @param aNamespaceDef namespace where query will be run
	 * @param aObserver observer which callback methods will be called
	 *
     * @return  New event query.
     */
    virtual CMdEEventQuery* NewEventQueryL(CMdENamespaceDef& aNamespaceDef, 
    		MMdEQueryObserver* aObserver = 0) = 0;


	/* Methods for managing observers. */

	/**
	 * Adds a new schema observer to the session. No duplicate
	 * observers are allowed.
	 *
	 * @param aObserver  observer
	 */
	virtual void AddSchemaObserverL(MMdESchemaObserver& aObserver) = 0;

	/**
	 * Removes the specified schema observer from the session.
	 *
	 * @param aObserver  observer
	 */
	virtual void RemoveSchemaObserverL(MMdESchemaObserver& aObserver) = 0;

	/**
	 * Adds a new object observer to the session. No duplicate observers are 
     * allowed.
     *
     * The following restrictions are placed on the condition nodes:
     * - Only CMdEObjectCondition and CMdEPropertyCondition nodes can be
     *   used. CMdERangePropertyConditions are not allowed.
     * - No nested logic conditions are allowed.
	 *
	 * @param aObserver   Observer.
	 * @param aCondition  Condition that the objects, about which the observer
	 *                    wants to receive notifications, must fulfill or NULL,
	 *                    to receive notifications of all objects.
     *                    Ownership of the condition is transferred to the
     *                    session.
     * @param aNotificationType what event type (add, modify, remove) should
     *                          be notified to client
     * @param aNamespaceDef specified namespace (if namespace is NULL, the 
     *                      default namespace is used)
     * 
     * @leave KErrAlreadyExists if the same observer has already been added
	 */
	virtual void AddObjectObserverL(MMdEObjectObserver& aObserver,
			CMdELogicCondition* aCondition = NULL, 
			TUint32 aNotificationType = ENotifyAdd | ENotifyModify | ENotifyRemove,
			CMdENamespaceDef* aNamespaceDef = NULL) = 0;

	/**
	 * Removes the specified object observer from the session.
	 *
	 * @param aObserver  observer
     * @param aNamespaceDef specified namespace (if namespace is NULL, the 
     *                      default namespace is used)
	 */
	virtual void RemoveObjectObserverL(MMdEObjectObserver& aObserver, 
			CMdENamespaceDef* aNamespaceDef = NULL) = 0;

	/**
	 * Adds a new object present observer to the session. No duplicate 
	 * observers are allowed.
     *
	 * @param aObserver   Observer
	 * 
     * @leave KErrAlreadyExists if the same observer has already been added
	 */
	virtual void AddObjectPresentObserverL(
			MMdEObjectPresentObserver& aObserver) = 0;

	/**
	 * Removes the specified object present observer from the session.
	 *
	 * @param aObserver  observer
	 */
	virtual void RemoveObjectPresentObserverL(
			MMdEObjectPresentObserver& aObserver)= 0;

	/**
	 * Adds a new relation observer to the session. No duplicate observers 
     * are allowed.
	 *
     * The following restrictions are placed on the condition nodes:
     * - Only CMdERelationCondition nodes are allowed.
     * - No nested logic conditions are allowed.
     *
	 * @param aObserver   Observer.
	 * @param aCondition  Condition that the relations, about which the observer
	 *                    wants to receive notifications, must fulfill
	 *                    or NULL, to receive notifications of all relations.
     *                    Ownership of the condition is transferred to the
     *                    session.
     * @param aNotificationType what event type (add, modify, remove) should
     *                          be notified to client
     * @param aNamespaceDef specified namespace (if namespace is NULL, the 
     *                      default namespace is used)
     * 
     * @leave KErrAlreadyExists if the same observer has already been added
	 */
	virtual void AddRelationObserverL(MMdERelationObserver& aObserver,
			CMdECondition* aCondition = NULL, 
			TUint32 aNotificationType = ENotifyAdd | ENotifyModify | ENotifyRemove,
			CMdENamespaceDef* aNamespaceDef = NULL)= 0;
	/**
	 * Removes the specified relation observer from the session.
	 *
	 * @param aObserver  Observer.
     * @param aNamespaceDef specified namespace (if namespace is NULL, the 
     *                      default namespace is used)
	 */
	virtual void RemoveRelationObserverL(MMdERelationObserver& aObserver, 
			CMdENamespaceDef* aNamespaceDef = NULL) = 0;

	/**
	 * Adds a new relation item observer to the session.
	 * No duplicate observers are allowed.
	 *
     * The following restrictions are placed on the condition nodes:
     * - Only CMdERelationCondition nodes are allowed.
     * - No nested logic conditions are allowed.
     *
	 * @param aObserver   Observer.
	 * @param aCondition  Condition that the relations, about which the observer
	 *                    wants to receive notifications, must fulfill
	 *                    or NULL, to receive notifications of all relations.
     *                    Ownership of the condition is transferred to the
     *                    session.
     * @param aNotificationType what event type (add, modify, remove) should
     *                          be notified to client
     * @param aNamespaceDef specified namespace (if namespace is NULL, the 
     *                      default namespace is used)
     * 
     * @leave KErrAlreadyExists if the same observer has already been added
	 */
	virtual void AddRelationItemObserverL(MMdERelationItemObserver& aObserver,
			CMdECondition* aCondition = NULL, 
			TUint32 aNotificationType = /*ENotifyAdd | ENotifyModify |*/ ENotifyRemove,
			CMdENamespaceDef* aNamespaceDef = NULL)= 0;
	/**
	 * Removes the specified relation observer from the session.
	 *
	 * @param aObserver  Observer.
     * @param aNamespaceDef specified namespace (if namespace is NULL, the 
     *                      default namespace is used)
	 */
	virtual void RemoveRelationItemObserverL(MMdERelationItemObserver& aObserver, 
			CMdENamespaceDef* aNamespaceDef = NULL) = 0;
	
	/**
	 * Adds a new realation present observer to the session. No duplicate observers are 
     * allowed.
     *
	 * @param aObserver   Observer.
	 * 
     * @leave KErrAlreadyExists if the same observer has already been added
	 */
	virtual void AddRelationPresentObserverL(
			MMdERelationPresentObserver& aObserver) = 0;

	/**
	 * Removes the specified relation present observer from the session.
	 *
	 * @param aObserver  observer
	 */
	virtual void RemoveRelationPresentObserverL(
			MMdERelationPresentObserver& aObserver)= 0;

	/**
	 * Adds a new event observer to the session. No duplicate observers 
     * are allowed.
     *
     * The following restrictions are placed on the condition nodes:
     * - Only CMdEEventCondition nodes are allowed.
     * - No nested logic conditions are allowed.
	 *
	 * @param aObserver   Observer.
	 * @param aCondition  Condition that the events, about which the observer
	 *                    wants to receive notifications, must fulfill
	 *                    or NULL, to receive notifications of all events.
     * @param aNotificationType what event type (add or remove) should
     *                          be notified to client
     * @param aNamespaceDef specified namespace (if namespace is NULL, the 
     *                      default namespace is used)
     * 
     * @leave KErrAlreadyExists if the same observer has already been added
	 */
	virtual void AddEventObserverL(MMdEEventObserver& aObserver,
			CMdECondition* aCondition = NULL, 
			TUint32 aNotificationType = ENotifyAdd | ENotifyRemove,
			CMdENamespaceDef* aNamespaceDef = NULL) = 0;

	/**
	 * Removes the specified event observer from the session.
	 *
	 * @param aObserver  Observer.
     * @param aNamespaceDef specified namespace (if namespace is NULL, the 
     *                      default namespace is used)
	 */
	virtual void RemoveEventObserverL(MMdEEventObserver& aObserver, 
			CMdENamespaceDef* aNamespaceDef = NULL) = 0;


    /* Methods for import/export */

	/**
	 * Imports schemadata from file to default database.
	 *
	 * @param aFileName  filename where the schemadata to import is.
	 */
	virtual void ImportSchemaL( const TDesC& aFileName ) = 0;

	/**
	 * Imports metadata from file to default database.
	 *
	 * @param aFileName  filename where the metadata to import is.
	 * @return The number of failed imports
	 */
	virtual TInt ImportMetadataL( const TDesC& aFileName ) = 0;

	/**
	 * Imports metadata asynchronously from file to default database.
	 *
	 * @param aFileName Filename where the metadata to import is.
	 * @param aResult The number of failed imports and possible error code.
	 * @param aStatus Returned status of method call.
	 */
	virtual void ImportMetadata( const TDesC& aFileName, 
			TPckgBuf<TInt>& aResult, TRequestStatus& aStatus ) = 0;

	/**
	 * Exports metadata to file.
	 *
	 * @param aFileName  filename where the metadata is to be exported.
     * @param aNamespaceDef specified namespace (if namespace is NULL, the 
     *                      default namespace is used), ownership doesn't 
     *                      change
     * @param aObjectDefs object types to export (if NULL all objects are 
     *                    exported), ownership doesn't change
     * @param aRelationDefs relation types to export (if NULL all relations 
     *                      are exported), ownership doesn't change
     * @param aEventDefs event types to export (if NULL all events are 
     *                   exported), ownership doesn't change
	 */
	virtual void ExportMetadataL( const TDesC& aFileName, 
			const CMdENamespaceDef* aNamespaceDef = NULL, 
	        const RPointerArray<CMdEObjectDef>* aObjectDefs = NULL, 
	        const RPointerArray<CMdERelationDef>* aRelationDefs = NULL, 
            const RPointerArray<CMdEEventDef>* aEventDefs = NULL ) = 0;

	/**
	 * Exports metadata asynchronously to file.
	 *
	 * @param aFileName Filename where the metadata is to be exported.
	 * @param aStatus Returns the result code after the asynchronous call 
     *                completes.
	 * @param aBuffer Serialized buffer of export types, must be valid until 
	 *                aStatus is completed and can be closed after that.
     * @param aNamespaceDef Specified namespace (if namespace is NULL, the 
     *                      default namespace is used), ownership doesn't 
     *                      change.
     * @param aObjectDefs Object types to export (if NULL all objects are 
     *                    exported), ownership doesn't change.
     * @param aRelationDefs Relation types to export (if NULL all relations 
     *                      are exported), ownership doesn't change.
     * @param aEventDefs Event types to export (if NULL all events are 
     *                   exported), ownership doesn't change.
	 */
	virtual void ExportMetadataL( const TDesC& aFileName, 
			TRequestStatus& aStatus, RMdEDataBuffer& aBuffer, 
        	const CMdENamespaceDef* aNamespaceDef = NULL, 
	        const RPointerArray<CMdEObjectDef>* aObjectDefs = NULL, 
            const RPointerArray<CMdERelationDef>* aRelationDefs = NULL, 
            const RPointerArray<CMdEEventDef>* aEventDefs = NULL ) = 0;
	/**
	 * Load whole schema from server side.
	 */
	virtual void LoadSchemaL() = 0;

	/**
	 * Get engine session reference.
	 */
    virtual RMdEEngineSession& EngineSession() = 0;

	/**
	 * Get schema version's major and minor version.
	 *
	 * @param aMajorVersion returned major version
	 * @param aMinorVersion returned minor version
	 */
	virtual void GetSchemaVersionL(
			TInt& aMajorVersion, TInt& aMinorVersion) = 0;

	/**
	 * Set object to "present" state by GUID.
	 *
	 * @param aGuidHigh Guid's high part
	 * @param aGuidLow Guid's low part
	 *
	 * @leave KErrNotFound MdE can't find object in "not present" state 
	 *                     with matching GUID
	 */
	virtual void SetObjectToPresentByGuidL( 
			const TInt64& aGuidHigh, const TInt64& aGuidLow ) = 0;
	
    /**
      * Adds a new object observer to the session. No duplicate observers are 
      * allowed.
      *
      * The following restrictions are placed on the condition nodes:
      * - Only CMdEObjectCondition and CMdEPropertyCondition nodes can be
      *   used. CMdERangePropertyConditions are not allowed.
      * - No nested logic conditions are allowed.
      * 
      * Be adviced, this version with the URI in the callback is much less
      * efficient than using version without the URI
      *
      * @param aObserver   Observer.
      * @param aCondition  Condition that the objects, about which the observer
      *                    wants to receive notifications, must fulfill or NULL,
      *                    to receive notifications of all objects.
      *                    Ownership of the condition is transferred to the
      *                    session.
      * @param aNotificationType what event type (add, modify, remove) should
      *                          be notified to client
      * @param aNamespaceDef specified namespace (if namespace is NULL, the 
      *                      default namespace is used)
      * @param aUriRequired determines if uri is required in the callback
      * 
      * @leave KErrAlreadyExists if the same observer has already been added
      */
     virtual void AddObjectObserverWithUriL( MMdEObjectObserverWithUri& aObserver,
             CMdELogicCondition* aCondition = NULL, 
             TUint32 aNotificationType = ENotifyAdd | ENotifyModify | ENotifyRemove,
             CMdENamespaceDef* aNamespaceDef = NULL ) = 0;

     /**
      * Removes the specified object observer from the session.
      *
      * @param aObserver  observer
      */
     virtual void RemoveObjectObserverWithUriL( MMdEObjectObserverWithUri& aObserver, 
                             CMdENamespaceDef* aNamespaceDef = NULL ) = 0;
     
protected:

	/* Constructors. */

	/**
	 * Constructor.
	 */
	CMdESession();

	/**
	 * Second-phase constructor.
	 */
	void SessionConstruct();
	};

// includes only for client more convinient usage
#include <mdeitem.h>
#include <mdenamespacedef.h>
#include <mdepropertydef.h>
#include <mdeobjectdef.h>
#include <mderelationdef.h>
#include <mdeeventdef.h>
#include <mdeproperty.h>
#include <mdeobject.h>
#include <mderelation.h>
#include <mdeevent.h>
#include <mdedatabuffer.h>

#endif  // __MDESESSION_H__
