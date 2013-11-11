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
* Description:  
*
*/


#ifndef __MESSAGEMONITORPLUGIN_H__
#define __MESSAGEMONITORPLUGIN_H__

//  INCLUDES 

#include <e32base.h>
#include <msvapi.h>
#include <monitorplugin.h>
#include "messagescannerao.h"

class CMessageMonitorPlugin : public CMonitorPlugin, public MMsvSessionObserver
  	{
	public:
		/**
		* NewL
		* @return
		*/
		static CMessageMonitorPlugin* NewL();

		/**
		* Destructor
		*/
		virtual ~CMessageMonitorPlugin();
		
		/**
		* From CMonitorPlugin
		*/
		TBool StartMonitoring( MMonitorPluginObserver& aObserver, 
                               CMdESession* aMdEClient, 
                               CContextEngine* aCtxEngine,
                               CHarvesterPluginFactory* aHarvesterPluginFactory );
	
		/**
		* From CMonitorPlugin
		*/
		TBool StopMonitoring();
		
		/**
		* From CMonitorPlugin
		*/		
		TBool ResumeMonitoring( MMonitorPluginObserver& aObserver, 
				                CMdESession* aMdEClient, 
								CContextEngine* aCtxEngine,
								CHarvesterPluginFactory* aHarvesterPluginFactory );
		
		/**
		* From CMonitorPlugin
		*/		
		TBool PauseMonitoring();
		
		/**
		* From MMsvSessionObserver
		*/
		virtual void HandleSessionEventL( TMsvSessionEvent aEvent, 
										  TAny* aArg1, 
										  TAny* aArg2, 
										  TAny* aArg3 );
		
		/**
		* Creates a URI for a message.
		*/
		static HBufC* CreateUriL( const TUid& aMsgTypeUid, const TMsvId aMsgId );
		
		/**
		 * Method CreateFileNameL.
		 * Creates file name in variable iFileName.
		 */
		void CreateFileNameL(); 
				
	private:

		/**
		* From Constructor
		*/
		CMessageMonitorPlugin();
		
		/**
		* 2nd phase construction
		*/
		void ConstructL();
		
		void HandleMsgMovedL( const TMsvId aFolderId1, 
							  const TMsvId aFolderId2, 
							  const CMsvEntrySelection& aSelection );

		void HandleMsgCreatedChangedL( const CMsvEntrySelection& aSelection,
                                       const TMsvId aFolderId,
                                       const TMsvSessionEvent aEvent );
		void HandleMsgDeletedL( const CMsvEntrySelection& aSelection );
		
		void RemoveObjectL( const TDesC& aUri );

		TBool IsValidMessageTypeL( const TMsvEntry& aEntry, CMsvSession& aSession );
		
		/**
		 * This method checks if the CRestoreWatcher has created a file in harvester server's
		 * private directory as a sign of restore and starts message scanner if it finds the file.
		 */
		void CheckRestoreL();
		
		
	private:
		MMonitorPluginObserver* iMonitorPluginObserver;
		CMsvSession* iMsvSession;			// Session Message server session, own
		CMdESession* iMdeSession;
		CMessageScannerAO* iMessageScannerAO;
		
		/*iFileName - File used for restore flag.*/
		TFileName iFileName;
	};

#endif // __MESSAGEMONITORPLUGIN_H__
