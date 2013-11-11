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
* Description:  Composer Plug-in ECom interface
*
*/

#ifndef __CCOMPOSERPLUGIN_H__
#define __CCOMPOSERPLUGIN_H__

#include <e32base.h>
#include <ecom.h>
#include <badesca.h>

#include <mdesession.h>

/** @var Uid for this interface */
const TUid KCComposerPluginInterfaceUid = { 0x2000717E };

 /**
 * Composer Plug-in ECom interface definition. Composer plug-in implementation
 * needs to inherit this class and implement the pure virtual methods which exist 
 * in this interface.
 *
 * Example:
 *
 * class CMyVideoComposerPlugin: public CComposerPlugin
 *     {
 *     protected:
 *         void SetObservers();    
 *         void RemoveObservers();
 * 
 *         // Because CComposerPlugin inherits MMdEObjectObserver
 *         void HandleObjectNotification(CMdESession& aSession, 
 *				TObserverNotificationType aType,
 *				const RArray<TItemId>& aObjectIdArray);
 *     
 *     private:
 *         // Active object which does the actual binary writing
 *         // so that this interface and its observers remain responsive
 *         CMyVideoComposerAO* iVideoComposerAO;       
 *     }
 *
 * void SetObservers()
 *     {
 *	   // We want to listen to changes in Video metadata objects
 *     // and setup an observer for this  
 *     CMdENamespaceDef& defaultNamespace = iSession->GetDefaultNamespaceDefL();
 *     CMdEObjectDef& videoDef = 
 *        defaultNamespace.GetObjectDefL( MdeConstants::Video::KVideoObject );
 *     CMdELogicCondition* condition = CMdELogicCondition::NewL( ELogicConditionOperatorAnd );
 *     condition->AddObjectConditionL( videoDef );
 *       
 *     // iSession is reference to CMdESession which is always set in CComposerPlugin::SetSession()
 *     iSession->AddObjectObserverL( *this, condition, ENotifyModify, &defaultNamespace );
 *     }
 * 
 * void RemoveObservers()
 *     {
 *     iSession->RemoveObjectObserverL( *this )
 *     }
 *
 * void CComposerImagePlugin::HandleObjectNotification(CMdESession& aSession, 
 *		TObserverNotificationType aType, const RArray<TItemId>& aObjectIdArray)
 *     {
 *     // We give ID-array of changed Video objects to our active object
 *     // which adds then to a its internal queue and processes them one-by-one 
 *     // with different RunL-calls so that we don't stuck the thread by composing
 *     // for example 1000 binary files in a row.
 *     iVideoComposerAO->AddToQueue( aObjectIdArray );
 *     }
 */
class CComposerPlugin : public CBase, public MMdEObjectObserver
    {
    public:

        /**
         * Construction.
         *
         * @param aUid  Implementation UID which plug-in to invoke.
         */
        IMPORT_C static CComposerPlugin* NewL( const TUid& aUid );

        /**
         * Lists all available implementations which satisfy this given interface.
         * 
         * @param aImplInfoArray  Reference to a list which will be populated with 
         *                        plug-in implementation details. 
         */
        IMPORT_C static void ListImplementationsL(RImplInfoPtrArray& aImplInfoArray);

        /**
         * Destructor.
         */ 
        IMPORT_C virtual ~CComposerPlugin();

        /**
         * Sets Mde session to be used.
         * 
         * @param aSession  Open MdE session to utilize in the Composer plug-in
         */
        IMPORT_C void SetSession( CMdESession& aSession );

        /**
         * Unsets Mde session.
         */
        IMPORT_C void RemoveSession();

        /**
         * Checks if composing is completed.
         *
         * @return  Whether composing is on-going
         */
        virtual TBool IsComposingComplete() = 0;

    protected:
    
        /**
         * Sets observers to be notified. Called by the interface itself when
         * SetSession method is called from Harvester server side.
         */
        virtual void SetObservers() = 0;

        /**
         * Unsets observers. Called by the interface itself when
         * RemoveSession method is called from Harvester server side.
         */
        virtual void RemoveObservers() = 0;

    protected:

        /* Pointer to Mde session used by the plugin */
    CMdESession* iSession;

    private:

        /* Identification on cleanup */
        TUid iDtor_ID_Key;
    };

#endif // __CCOMPOSERPLUGIN_H__
