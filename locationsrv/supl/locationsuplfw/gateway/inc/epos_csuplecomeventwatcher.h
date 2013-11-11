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
* Description: Ecom Event watcher..Notifies when plugin uninstalled
*
*/

#ifndef __CSuplEComEventWatcher_H__
#define __CSuplEComEventWatcher_H__


#include "epos_msuplecomeventobserver.h"
#include <e32base.h>
#include <ecom/ecom.h>


// CLASS DECLARATION

/**
*  Class to listen for Ecom events
*/

class CSuplEcomEventWatcher : public CActive
	{
	public: // enum

		typedef enum
			{
			EExist,
			ENotExist
			} TSuplEcomPlugInStatus;

	public:
	
	    /**
        * Two-phased constructor.
        * @param aSuplEcomEventObserver is the reference of observer(CSuplServer).
        */
        static CSuplEcomEventWatcher* NewL(MSuplEcomEventObserver& aSuplEcomEventObserver, TUid aInterfaceUid, TUid aImplId);
        /**
        * Destructor.
        */        
		virtual ~CSuplEcomEventWatcher();
		
		void NotifyOnPlugInUnInstallation();
		
	protected:  // From CActive
	
		void RunL();                
        TInt RunError(TInt aError);               
        void DoCancel();
        
	private:
		    
	    /**
        * Constructor.
        */
        
		CSuplEcomEventWatcher(MSuplEcomEventObserver& aSuplEcomEventObserver, TUid aInterfaceUid, TUid aImplId);
		
		/**
        * ConstructL.
        */
        
		void	ConstructL();
		
		/**
        * The following method returns version of plug-in.
        * @param None.
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
	
#endif //__CSuplEComEventWatcher_H__