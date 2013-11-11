/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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

#ifndef __CCALENDARCONTEXTPLUGIN_H__
#define __CCALENDARCONTEXTPLUGIN_H__

//  INCLUDES 
#include <contextplugin.h>
#include <calprogresscallback.h>

#include <mdesession.h>
#include "mdeharvestersession.h"

// CONSTANTS


// FORWARD DECLARATIONS
class CCalInstanceView;
class CCalEntry;
class CCalSession;

const TInt KInitRetryTimeout = 5000000; //5 sec
const TInt KMaxRetryCount = 6; // retry for up to 30 sec

// CLASS DECLARATION ===========================================================

/**
*  Main class of the plugin.
*/
class CCalendarContextPlugin : public CContextPlugin,
                               public MMdESessionObserver,
                               public MCalProgressCallBack
      {
    public:
    	/** 
    	* Static constructor
        * @return           - pointer to the new object
    	*/
        static CCalendarContextPlugin* NewL();

    	/** 
    	* Destructor
    	*/
        virtual ~CCalendarContextPlugin();
        
    // CContextPlugin functions
        void Init( MContextPluginObserver& aObserver );
        void ContextSnapshot( MContextPluginObserver& aObserver,
                               CHarvesterData& aHD );
        void ContextSnapshot( MContextPluginObserver&    aObserver,
                               RPointerArray<CHarvesterData>& aHDArray );
                               
    	void SetMdeSession( CMdESession& aSession );                           

    // MMdESessionObserver functions, not used
        virtual void HandleSessionOpened( CMdESession& aSession, TInt aError );
        virtual void HandleSessionError( CMdESession& aSession, TInt aError );
        
    // MCalProgressCallBack functions, not used
        virtual void Progress( TInt aPercentageCompleted );
        virtual void Completed( TInt aError );
        virtual TBool NotifyProgress();

    private:
    	/** 
    	* C++ constructor
    	*/
        CCalendarContextPlugin();

    	/** 
    	* 2nd phase construction
    	*/
        void ConstructL(); // second-phase constructor

    	/** 
    	* Adds the calendar metadata to the MdEObject
    	* @param aMdEObject - the CMdEObject to add the metadata to
    	*/
        void AddMetaDataL( CMdEObject& aMdEObject ) const;
            

    	/** 
    	* Gets a time property value from MDS
    	* @param aNameSpace - reference to the namespace
    	* @param aProp      - reference to the property name
    	* @param aMdEObject - reference to the CMdEObject to be examined
        * @return           - value of the property, leaves if it does not exist
    	*/
        TTime GetTimeMdEPropL( const TDesC& aNameSpace,
                               const TDesC& aProp,
                               CMdEObject&  aMdEObject ) const;

    	/** 
    	* Gets a text property value from MDS
    	* @param aNameSpace - reference to the namespace
    	* @param aProp      - reference to the property name
    	* @param aMdEObject - reference to the CMdEObject to be examined
        * @return           - value of the property, NULL if it does not exist
    	*/
        HBufC* GetTextMdEPropLC( const TDesC& aNameSpace,
                                 const TDesC& aProp,
                                 CMdEObject&  aMdEObject ) const;

        
        /**
         * Initialise calendar serve instance
         *
         * @since S60 v5.0
         * @param aRetry - is retry initialisation
         */
        void OpenViewL(TBool aRetry = EFalse);
        
        /**
         * Callback for initialisation retry
         *
         * @since S60 v5.0
         */
        static TInt InitRetryCallback(TAny* aAny);
        
		 /**
         * Starts initialisation retry
         *
         * @since S60 v5.0
         */
        void StartRetryL();
		
    private:
        
        CCalInstanceView* iCalInstanceView;    ///< owned
        CMdESession* iMdeSession;            ///< NOT owned
        CCalSession* iCalSession; ///< owned
        TBool iCalInstanceViewCompleted;
        CPeriodic* iInitTimer;
        TInt iRetryCount;
    };
#endif // __CCALENDARCONTEXTPLUGIN_H__
