/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*
*/

#ifndef MDETESTSCRIPTER_H
#define MDETESTSCRIPTER_H

//  INCLUDES
#include <e32cmn.h>
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <mdesession.h>
#include <mdequery.h>

// CONSTANTS
const TInt KPathPrefixLength( 2 );

const TInt KLast = -1;

const TInt KById = 0;
const TInt KByUri = 1;
const TInt KByGuid = 2;
const TInt KByIds = 3;
const TInt KByDef = 4;
const TInt KByIndex = 5;

const TInt KL = 0;
const TInt KLC = 1;

const TInt KAsync = 0;
const TInt KSync = 1;

const TInt ESession = 0;
const TInt EObjectDef = 1;
const TInt ERelationDef = 2;
const TInt EPropertyDef = 3;
const TInt EEventDef = 4;
const TInt ENamespaceDef = 5;
const TInt EObject = 6;
const TInt ERelation = 7;
const TInt EProperty = 8;
const TInt EEvent = 9;

// MACROS
_LIT( KMdETestScripter, "MdETestScripter" );
#if ( defined (__WINS__) || defined (__WINSCW__) )
_LIT( KMdETestDataPath, "" );
#else 
_LIT( KMdETestDataPath, "\\testing" );
#endif

_LIT( KMdETestScripterLogPath, "\\logs\\testframework\\MdETestScripter\\" ); 

// Log file
_LIT( KMdETestScripterLogFile, "MdETestScripter.txt" ); 
_LIT( KMdETestScripterLogFileWithTitle, "MdETestScripter_[%S].txt" );

// Events
// Events for schema
_LIT( KMdESchemaModified, "ESchemaModified" );

// Events for async request
_LIT( KMdEItemsAsyncAdd, "EItemsAsyncAdd" );
_LIT( KMdEObjectsAsyncAdd, "EObjectsAsyncAdd" );
_LIT( KMdERelationsAsyncAdd, "ERelationsAsyncAdd" );
_LIT( KMdEEventsAsyncAdd, "EEventsAsyncAdd" );

_LIT( KMdEItemsAsyncUpdate, "EItemsAsyncUpdate" );
_LIT( KMdEObjectsAsyncUpdate, "EObjectsAsyncUpdate" );
_LIT( KMdERelationsAsyncUpdate, "ERelationsAsyncUpdate" );
_LIT( KMdEEventsAsyncUpdate, "EEventsAsyncUpdate" );

_LIT( KMdEObjectsAsyncRemoved, "EObjectsAsyncRemoved" );
_LIT( KMdERelationsAsyncRemoved, "ERelationsAsyncRemoved" );
_LIT( KMdEEventsAsyncRemoved, "EEventsAsyncRemoved" );


// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CMdETestScripter;
class CMdESession;
class CMdENamespaceDef;
class CMdEObject;
class CMdEInstanceItem;
class CMdEQuery;
class CMdELogicCondition;
class CMdETestScripterAO;

// DATA TYPES

// CLASS DECLARATION

/**
*  CMdETestScripter test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CMdETestScripter) : public CScriptBase,
                                      public MMdESchemaObserver,
                                      public MMdEObjectObserver,
                                      public MMdEObjectObserverWithUri,
                                      public MMdEObjectPresentObserver,
                                      public MMdERelationObserver,
                                      public MMdERelationItemObserver,
                                      public MMdERelationPresentObserver,
                                      public MMdESessionObserver,
                                      public MMdEEventObserver,
                                      public MMdEQueryObserver
    {
    friend class CMdETestScripterAO;
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMdETestScripter* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CMdETestScripter();

    public: // New functions

    public: // Functions from base classes
        virtual TInt RunMethodL( CStifItemParser& aItem );

    protected:  // New functions


    protected:  // Functions from base classes


    private:

        /**
        * C++ default constructor.
        */
        CMdETestScripter( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();
        
        template <class T> T* SanitizeArrayIndexL(
    		RPointerArray<T>& aArray, const CDesCArrayFlat& aNameArray, 
    		CStifItemParser& aItem );
        CMdEInstanceItem* GetInstanceItemByArrayIndexL(
            TInt aMode, CStifItemParser& aItem );
        void CheckTMdEObjectL( const TMdEObject& aTObject, const CMdEObject& aObject );
        void CheckTMdERelationL( const TMdERelation& aTRelation, const CMdERelation& aRelation );
        template <class T1, class T2> void CheckTInstanceL( const T1& aT1, const T2& aT2 );
        void CheckTMdEObjectArrayL(
            const RArray<TMdEObject>& aTObjects, TInt objIndex, TInt aCount );
        void CheckTMdERelationArrayL(
            const RArray<TMdERelation>& aTRelations, const TInt aObjIndex, const TInt aCount );
        template <class T> void CheckTItemIdArrayL(
            const RArray<TItemId>& aTItemIds, const RPointerArray<T>& aArray,
            const TInt aIndex, const TInt aCount );
        TInt CheckAsyncResultArrayL( TEventIf aEvent );
        void FormInstanceItemArrayL(
            CStifItemParser& aItem, RPointerArray<CMdEInstanceItem>& aItemArray ) const;

        /**
        * Test methods are listed below. 
        */

        // Observer add/remove test primitives
        virtual TInt AddSchemaObserverL( CStifItemParser& aItem );
        virtual TInt RemoveSchemaObserverL( CStifItemParser& aItem );
        virtual TInt AddObjectObserverL( CStifItemParser& aItem );
        virtual TInt RemoveObjectObserverL( CStifItemParser& aItem );
        virtual TInt AddObjectObserverWithUriL( CStifItemParser& aItem );
        virtual TInt RemoveObjectObserverWithUriL( CStifItemParser& aItem );
        virtual TInt AddObjectPresentObserverL( CStifItemParser& aItem );
        virtual TInt RemoveObjectPresentObserverL( CStifItemParser& aItem );
        virtual TInt AddRelationObserverL( CStifItemParser& aItem );
        virtual TInt RemoveRelationObserverL( CStifItemParser& aItem );
        virtual TInt AddRelationItemObserverL( CStifItemParser& aItem );
        virtual TInt RemoveRelationItemObserverL( CStifItemParser& aItem );
        virtual TInt AddRelationPresentObserverL( CStifItemParser& aItem );
        virtual TInt RemoveRelationPresentObserverL( CStifItemParser& aItem );
        virtual TInt AddEventObserverL( CStifItemParser& aItem );
        virtual TInt RemoveEventObserverL( CStifItemParser& aItem );

        // Notification resulated test primitives
        virtual TInt VerifyTItemIdsL( CStifItemParser& aItem );
        virtual TInt VerifyMdERelationsL( CStifItemParser& aItem );
        virtual TInt VerifyObserverNotificationTypeL( CStifItemParser& aItem );

        // Metadata/schema/namespace test primitives.
        virtual TInt ObjectDefCountL( CStifItemParser& aItem );
        virtual TInt ObjectDefL( CStifItemParser& aItem );
        virtual TInt GetObjectDefL( CStifItemParser& aItem );
        virtual TInt RelationDefCountL( CStifItemParser& aItem );
        virtual TInt RelationDefL( CStifItemParser& aItem );
        virtual TInt GetRelationDefL( CStifItemParser& aItem );
        virtual TInt EventDefCountL( CStifItemParser& aItem );
        virtual TInt EventDefL( CStifItemParser& aItem );
        virtual TInt GetEventDefL( CStifItemParser& aItem );

        virtual TInt ExportMetadataL( CStifItemParser& aItem );
        virtual TInt ImportMetadataL( CStifItemParser& aItem );
        virtual TInt ImportSchemaL( CStifItemParser& aItem );
        virtual TInt NamespaceDefCountL( CStifItemParser& aItem );
        virtual TInt NamespaceDefL( CStifItemParser& aItem );
        virtual TInt GetNamespaceDefL( CStifItemParser& aItem );
        virtual TInt ResetNamespaceDefArray( CStifItemParser& aItem );
        virtual TInt GetDefaultNamespaceDefL( CStifItemParser& aItem );
        virtual TInt LoadSchemaL( CStifItemParser& aItem );
        virtual TInt EngineSessionL( CStifItemParser& aItem );
        virtual TInt GetSchemaVersionL( CStifItemParser& aItem );
        virtual TInt SetObjectToPresentByGuidL( CStifItemParser& aItem );
        
        // Item / InstanceItem test primitives.
        virtual TInt BelongsToSessionL( CStifItemParser& aItem );
        virtual TInt SessionL( CStifItemParser& aItem );
        virtual TInt InDatabaseL( CStifItemParser& aItem );
        virtual TInt AddItemsL( CStifItemParser& aItem );
        virtual TInt UpdateItemsL( CStifItemParser& aItem );

        // Definition test primitives.
        virtual TInt AddRelationDefL( CStifItemParser& aItem );
        virtual TInt AddEventDefL( CStifItemParser& aItem );
        virtual TInt PropertyDefCountL( CStifItemParser& aItem );
        virtual TInt PropertyDefL( CStifItemParser& aItem );
        virtual TInt GetPropertyDefL( CStifItemParser& aItem );
        virtual TInt MandatoryL( CStifItemParser& aItem );
        virtual TInt MinInt32ValueL( CStifItemParser& aItem );
        virtual TInt MaxInt32ValueL( CStifItemParser& aItem );
        virtual TInt MinUint32ValueL( CStifItemParser& aItem );
        virtual TInt MaxUint32ValueL( CStifItemParser& aItem );
        virtual TInt MinInt64ValueL( CStifItemParser& aItem );
        virtual TInt MaxInt64ValueL( CStifItemParser& aItem );
        virtual TInt MinRealValueL( CStifItemParser& aItem );
        virtual TInt MaxRealValueL( CStifItemParser& aItem );
        virtual TInt MinTimeValueL( CStifItemParser& aItem );
        virtual TInt MaxTimeValueL( CStifItemParser& aItem );
        virtual TInt MinTextLengthL( CStifItemParser& aItem );
        virtual TInt MaxTextLengthL( CStifItemParser& aItem );
        virtual TInt CompareL( CStifItemParser& aItem );
        virtual TInt ParentL( CStifItemParser& aItem );
        virtual TInt NameL( CStifItemParser& aItem );
        virtual TInt ReadOnlyL( CStifItemParser& aItem );
        virtual TInt DefL( CStifItemParser& aItem );

        // Object test primitives.
        virtual TInt SetDefL( CStifItemParser& aItem );
        virtual TInt UriL( CStifItemParser& aItem );
        virtual TInt SetUriL( CStifItemParser& aItem );
        virtual TInt ConfidentialL( CStifItemParser& aItem );
        virtual TInt SetConfidentialL( CStifItemParser& aItem );
        virtual TInt PlaceholderL( CStifItemParser& aItem );
        virtual TInt SetPlaceholderL( CStifItemParser& aItem );
        virtual TInt MediaIdL( CStifItemParser& aItem );
        virtual TInt SetMediaIdL( CStifItemParser& aItem );
        virtual TInt UsageCountL( CStifItemParser& aItem );
        virtual TInt GuidL( CStifItemParser& aItem );
        virtual TInt SetGuidL( CStifItemParser& aItem );
        virtual TInt OpenForModificationsL( CStifItemParser& aItem );
        virtual TInt NewObjectL( CStifItemParser& aItem );
        virtual TInt AddObjectL( CStifItemParser& aItem );
        virtual TInt AddObjectsL( CStifItemParser& aItem );
        virtual TInt GetObjectL( CStifItemParser& aItem );
        virtual TInt GetFullObjectL( CStifItemParser& aItem );
        virtual TInt CheckObjectL( CStifItemParser& aItem );
        virtual TInt RemoveObjectL( CStifItemParser& aItem );
        virtual TInt RemoveObjectsL( CStifItemParser& aItem );
        virtual TInt RemoveAllObjectsL( CStifItemParser& aItem );
        virtual TInt OpenObjectL( CStifItemParser& aItem );
        virtual TInt OpenFullObjectL( CStifItemParser& aItem );
        virtual TInt CommitObjectL( CStifItemParser& aItem );
        virtual TInt CommitObjectsL( CStifItemParser& aItem );
        virtual TInt CancelObjectL( CStifItemParser& aItem );

        virtual TInt PropertyL( CStifItemParser& aItem );
        virtual TInt PropertyCountL( CStifItemParser& aItem );
        virtual TInt AddBoolPropertyL( CStifItemParser& aItem );
        virtual TInt AddInt8PropertyL( CStifItemParser& aItem );
        virtual TInt AddUint8PropertyL( CStifItemParser& aItem );
        virtual TInt AddInt16PropertyL( CStifItemParser& aItem );
        virtual TInt AddUint16PropertyL( CStifItemParser& aItem );
        virtual TInt AddInt32PropertyL( CStifItemParser& aItem );
        virtual TInt AddUint32PropertyL( CStifItemParser& aItem );
        virtual TInt AddInt64PropertyL( CStifItemParser& aItem );
        virtual TInt AddReal32PropertyL( CStifItemParser& aItem );
        virtual TInt AddReal64PropertyL( CStifItemParser& aItem );
        virtual TInt AddTextPropertyL( CStifItemParser& aItem );
        virtual TInt AddTimePropertyL( CStifItemParser& aItem );

        // Property test primitives.
        virtual TInt ObjectL( CStifItemParser& aItem );
        virtual TInt BoolValueL( CStifItemParser& aItem );
        virtual TInt Int8ValueL( CStifItemParser& aItem );
        virtual TInt Uint8ValueL( CStifItemParser& aItem );
        virtual TInt Int16ValueL( CStifItemParser& aItem );
        virtual TInt Uint16ValueL( CStifItemParser& aItem );
        virtual TInt Int32ValueL( CStifItemParser& aItem );
        virtual TInt Uint32ValueL( CStifItemParser& aItem );
        virtual TInt Int64ValueL( CStifItemParser& aItem );
        virtual TInt Real32ValueL( CStifItemParser& aItem );
        virtual TInt Real64ValueL( CStifItemParser& aItem );
        virtual TInt TimeValueL( CStifItemParser& aItem );
        virtual TInt TextValueL( CStifItemParser& aItem );
        virtual TInt SetBoolValueL( CStifItemParser& aItem );
        virtual TInt SetInt8ValueL( CStifItemParser& aItem );
        virtual TInt SetUint8ValueL( CStifItemParser& aItem );
        virtual TInt SetInt16ValueL( CStifItemParser& aItem );
        virtual TInt SetUint16ValueL( CStifItemParser& aItem );
        virtual TInt SetInt32ValueL( CStifItemParser& aItem );
        virtual TInt SetUint32ValueL( CStifItemParser& aItem );
        virtual TInt SetInt64ValueL( CStifItemParser& aItem );
        virtual TInt SetReal32ValueL( CStifItemParser& aItem );
        virtual TInt SetReal64ValueL( CStifItemParser& aItem );
        virtual TInt SetTimeValueL( CStifItemParser& aItem );
        virtual TInt SetTextValueL( CStifItemParser& aItem );
        virtual TInt RemovePropertyL( CStifItemParser& aItem );
        virtual TInt MovePropertiesL( CStifItemParser& aItem );

        // Free text test primitives.
        virtual TInt FreeTextCountL( CStifItemParser& aItem );
        virtual TInt FreeTextL( CStifItemParser& aItem );
        virtual TInt FreeTextIndexL( CStifItemParser& aItem );
        virtual TInt AddFreeTextL( CStifItemParser& aItem );
        virtual TInt RemoveFreeTextL( CStifItemParser& aItem );

        // Relation test primitives.
        virtual TInt NewRelationL( CStifItemParser& aItem );
        virtual TInt AddRelationL( CStifItemParser& aItem );
        virtual TInt LeftObjectIdL( CStifItemParser& aItem );
        virtual TInt RightObjectIdL( CStifItemParser& aItem );
        virtual TInt ParameterL( CStifItemParser& aItem );
        virtual TInt SetParameterL( CStifItemParser& aItem );
        virtual TInt SetLeftObjectIdL( CStifItemParser& aItem );
        virtual TInt SetRightObjectIdL( CStifItemParser& aItem );
        virtual TInt LastModifiedDateL( CStifItemParser& aItem );
        virtual TInt SetLastModifiedDateL( CStifItemParser& aItem );
        virtual TInt GetRelationL( CStifItemParser& aItem );
        virtual TInt UpdateRelationL( CStifItemParser& aItem );
        virtual TInt RemoveRelationL( CStifItemParser& aItem );
        virtual TInt RemoveRelationsL( CStifItemParser& aItem );

        // Event test primitives.
        virtual TInt NewEventL( CStifItemParser& aItem );
        virtual TInt AddEventL( CStifItemParser& aItem );
        virtual TInt ObjectIdL( CStifItemParser& aItem );
        virtual TInt TimeL( CStifItemParser& aItem );
        virtual TInt SourceL( CStifItemParser& aItem );
        virtual TInt ParticipantL( CStifItemParser& aItem );
        virtual TInt GetEventL( CStifItemParser& aItem );
        virtual TInt RemoveEventL( CStifItemParser& aItem );
        virtual TInt RemoveEventsL( CStifItemParser& aItem );
        //[TestMethods] - Do not remove

        // Metadata framework observer callback methods
        void HandleObjectNotification( CMdESession& aSession, 
            TObserverNotificationType aType,
            const RArray<TItemId>& aObjectIdArray );
        void HandleUriObjectNotification(CMdESession& aSession, 
            TObserverNotificationType aType,
            const RArray<TItemId>& aObjectIdArray,
            const RPointerArray<HBufC>& aObjectUriArray);
        void HandleObjectPresentNotification(CMdESession& aSession, 
            TBool aPresent, const RArray<TItemId>& aObjectIdArray);
        void HandleRelationNotification( CMdESession& aSession, 
            TObserverNotificationType aType,
            const RArray<TItemId>& aRelationIdArray );
        void HandleRelationItemNotification(CMdESession& aSession, 
            TObserverNotificationType aType,
            const RArray<TMdERelation>& aRelationArray);
        void HandleRelationPresentNotification(CMdESession& aSession,
            TBool aPresent, const RArray<TItemId>& aRelationIdArray);
        void HandleEventNotification( CMdESession& aSession,
            TObserverNotificationType aType,
            const RArray<TItemId>& aEventIdArray );
    	void HandleSessionOpened( CMdESession& aSession, TInt aError );
    	void HandleSessionError( CMdESession& aSession, TInt aError );
        void HandleSchemaModified();
		TInt QueryObjectL( CStifItemParser& aItem );
		TInt QueryRelationL( CStifItemParser& aItem );
		TInt QueryEventL( CStifItemParser& aItem );

		TInt TestPopulateDbL( CStifItemParser& aItem );
		
private:
		CMdEObjectDef& GetObjectDefStrL( CStifItemParser& aItem );
		CMdERelationDef& GetRelationDefStrL( CStifItemParser& aItem );
		static TQueryResultMode CMdETestScripter::ResultModeL(TDesC& aModeString);
		static TObjectConditionCompareMethod CompareMethodL( TDesC& aMethodString );
		static TMdERangeType RangeTypeL( TDesC& aTypeString );

		TInt GetObjectDefIndexL( CStifItemParser& aItem );	
		TInt GetRelationDefIndexL( CStifItemParser& aItem );
		TInt GetEventDefIndexL( CStifItemParser& aItem );
		TInt GetPropertyDefIndexL( CStifItemParser& aItem );

		TInt GetObjectIndexL( CStifItemParser& aItem );
		TInt GetEventIndexL( CStifItemParser& aItem );
		TInt GetRelationIndexL( CStifItemParser& aItem );
		TInt GetPropertyIndexL( CStifItemParser& aItem );

		TInt GetLogicConditionIndexL( CStifItemParser& aItem );
		TInt GetRelationConditionIndexL( CStifItemParser& aItem );

		void AppendNameL(CDesCArrayFlat* aNameArray, const TDesC& aName)
			{
			TInt pos;
			TBuf<32> name(aName);
			pos = name.Find(_L("ARRAY_INDEX"));
			if ( pos != KErrNotFound )
				{
				name.SetLength( pos );
				name.AppendNum( aNameArray->Count() );
				}
			if ( aNameArray->Find( name, pos ) )
				{
				aNameArray->AppendL( name );
				}
			else
				{ // 
				iLog->Log( _L( "indentifier = %S already used" ), &name );
				User::Leave( KErrAlreadyExists );			
				}						
			}
		

public:

		TInt NewObjectQueryL( CStifItemParser& aItem );
		TInt NewRelationQueryL( CStifItemParser& aItem );
		TInt NewEventQueryL( CStifItemParser& aItem );
		TInt AddPropertyFilterL( CStifItemParser& aItem );
		TInt AppendOrderRuleL( CStifItemParser& aItem );
		TInt NewLogicConditionL( CStifItemParser& aItem );
		TInt AddLogicConditionL( CStifItemParser& aItem );		
		TInt AddObjectConditionL( CStifItemParser& aItem );
		TInt AddPropertyConditionL( CStifItemParser& aItem );
		TInt AddRelationConditionL( CStifItemParser& aItem );
		TInt AddEventConditionL( CStifItemParser& aItem );
		
		TInt ObjectConditionsL( CStifItemParser& aItem );
		
		TInt RightL( CStifItemParser& aItem );
		TInt LeftL( CStifItemParser& aItem );
		TInt SetOperatorL( CStifItemParser& aItem );
		TInt SetNegateL( CStifItemParser& aItem );
		
		TInt FindL( CStifItemParser& aItem );		
			
		void HandleQueryCompleted( CMdEQuery& aQuery, TInt aError );
    	
    	void HandleQueryNewResults(CMdEQuery& aQuery,
                                       TInt aFirstNewItemIndex,
                                       TInt aNewItemCount);
        TInt SetObjectQueryResultL( CStifItemParser& aItem );
        TInt SetEventQueryResultL( CStifItemParser& aItem );
        TInt SetRelationQueryResultL( CStifItemParser& aItem );
        
    private:
        // Data
        TInt iError;

        RPointerArray<CMdEObjectDef> iObjectDefArray;
        RPointerArray<CMdERelationDef> iRelationDefArray;
        RPointerArray<CMdEEventDef> iEventDefArray;
        RPointerArray<CMdEPropertyDef> iPropertyDefArray;
        RPointerArray<CMdENamespaceDef> iNamespaceDefArray;
        
		RPointerArray<CMdEObject> iObjectArray;
        RPointerArray<CMdERelation> iRelationArray;
        RPointerArray<CMdEEvent> iEventArray;
        RPointerArray<CMdEProperty> iPropertyArray;
		RPointerArray<CMdELogicCondition> iLogicConditionArray;
		RPointerArray<CMdERelationCondition> iRelationConditionArray;
		RPointerArray<CMdEObjectCondition> iObjectConditionArray;
		RPointerArray<CMdEPropertyCondition> iPropertyConditionArray;
		RPointerArray<CMdEEventCondition> iEventConditionArray;

        // Result data from callbacks.
        TObserverNotificationType iNotificationType;
        CMdESession* iNotificationSession;
        RArray<TMdERelation> iNotificationRelationArray;
        RArray<TItemId> iNotificationItemIdArray;

        CDesCArrayFlat *iObjectDefNameArray;
        CDesCArrayFlat *iRelationDefNameArray;
		CDesCArrayFlat *iEventDefNameArray;
		CDesCArrayFlat *iPropertyDefNameArray;
		CDesCArrayFlat *iNamespaceDefNameArray;
		
		CDesCArrayFlat *iObjectNameArray;		
        CDesCArrayFlat *iRelationNameArray;
        CDesCArrayFlat *iEventNameArray;
        
        CDesCArrayFlat *iPropertyNameArray;
        
		CDesCArrayFlat *iRelationConditionNameArray;
		CDesCArrayFlat *iObjectConditionNameArray;
		CDesCArrayFlat *iEventConditionNameArray;
		
		CDesCArrayFlat *iLogicConditionNameArray;		
		CDesCArrayFlat *iPropertyConditionNameArray;

		RArray<TInt> iResultIndices;

        CMdESession* iMdeSession;
		CMdENamespaceDef* iDefaultNamespaceDef;
		CMdEQuery *iQuery;
		CMdELogicCondition *iQueryRoot;
        TFileName iFilePath;
        
        CMdETestScripterAO* iAsyncHandler;
        RMdEDataBuffer iMdEDataBuf; // Buffer for mde async request results
        RPointerArray<CMdEInstanceItem> iItemArray;
        TInt iItemCount;
    };

#endif      // MDETESTSCRIPTER_H

// End of File
