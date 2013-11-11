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
* Description: ECOM Event watcher class, listens for ecom events
*
*/

#ifndef C_COMASUPLECOMEVENTWATCHER_H
#define C_COMASUPLECOMEVENTWATCHER_H

#include <e32base.h>
#include <ecom/ecom.h>

#include "epos_momasuplecomeventobserver.h"

// CLASS DECLARATION

/**
*  Class to listen for Ecom events
*/

class COMASuplEcomEventWatcher : public CActive
	{
	
	public: // enum

		enum TSuplEcomPlugInStatus
			{
				EExist,
				ENotExist
			} ;

	public:
	
		/**
		* NewL,constructor.
		* @since  S60 v3.1
		* @param  aSuplEcomEventObserver,observer for ecom-events
		* @param  aInterfaceUid,Interface id on which ecom will watch for events.
		* @param  aImplId,implementation id  on which ecom will watch for events.
		* @return Instance of COMASuplEcomEventWatcher.
		*/
        static COMASuplEcomEventWatcher* NewL(MSuplEcomEventObserver& aSuplEcomEventObserver, TUid aInterfaceUid, TUid aImplId);
        
		/**
		* ~COMASuplEcomEventWatcher,destructor.
		* @since  S60 v3.1
		* @param  None
		* @return None.
		*/
		virtual ~COMASuplEcomEventWatcher();
		
		/**
		* NotifyOnPlugInUnInstallation,notify about uninstallation of plugin
		* @since  S60 v3.1
		* @param  None.
		* @return None
		*/
		void NotifyOnPlugInUnInstallation();
		
	protected:  // From CActive
	
		/**
		* RunL,from CActive
		* @since  S60 v3.1
		* @param  None.
		* @return None
		*/
		void RunL();                
		
		/**
		* RunError,from CActive
		* @since  S60 v3.1
		* @param  aError,errocode if RunL leaves.
		* @return TInt
		*/
        TInt RunError(TInt aError);               
        
		/**
		* DoCancel,from CActive
		* @since  S60 v3.1
		* @param  None.
		* @return None
		*/
        void DoCancel();
        
	private:
		    
		/**
		* COMASuplEcomEventWatcher,constructor.
		* @since  S60 v3.1
		* @param  aSuplEcomEventObserver,observer for ecom-events
		* @param  aInterfaceUid,Interface id on which ecom will watch for events.
		* @param  aImplId,implementation id  on which ecom will watch for events.
		* @return None
		*/

		COMASuplEcomEventWatcher(MSuplEcomEventObserver& aSuplEcomEventObserver, TUid aInterfaceUid, TUid aImplId);
		
		/**
		* ConstructL,Symbian 2nd phase construction
		* @since  S60 v3.1
		* @param  None.
		* @return None
		*/
		void	ConstructL();
		
		/**
		* IsImplementationExistL,checks wheather implementation exists or not.
		* @since  S60 v3.1
		* @param  None.
		* @return TBool
		*/
		TBool	IsImplementationExistL();
        
	private:
	
		/**
        * Reference to CSuplServer.
        */        
	    MSuplEcomEventObserver& 	iSuplEcomEventObserver;
	    
	    /**
        * Address of REComSession.
        */        
	    REComSession* 				iEComSession;
	    
	    /**
	    * Interface Id of respective Plug-In
	    */	    
	    TUid                        iPlugInInterfaceUid;
	    	     
	    /**
        * Implementation Id of respective Plug-In
        */        
	    TUid 						iPlugInImplUid;
	    
	    /**
        * Current status of this plug-in whether plug-in exists or not.
        */ 
	    TSuplEcomPlugInStatus       iSuplEcomPlugInStatus;  
	    
	};
	
#endif // C_COMASUPLECOMEVENTWATCHER_H