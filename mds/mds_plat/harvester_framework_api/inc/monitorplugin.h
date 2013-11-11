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
* Description:  Definition of the Monitor plug-in ECom interface.*
*/

#ifndef __MONITORPLUGIN_H__
#define __MONITORPLUGIN_H__

#include <e32base.h>
#include <ecom.h>

#include <harvesterdata.h>

// FORWARD DECLARATION
class CMdESession;
class CContextEngine;
class CHarvesterPluginFactory;

// Uid for this interface
const TUid KMonitorPluginInterfaceUid = { 0x20007181 };

class TMountData
	{
	public:
	enum TMountType
		{
		EMount,   // Drive was mounted 
		EUnmount, // Drive was unmounted
		EFormat   // Drive was formatted
		};
		
	TMountType iMountType;  // See above
	TUint32 iMediaID;       // Storage MediaID - TVolumeInfo::iUniqueID
	TBuf<2> iDrivePath;     // Drive letter
	};

/**
 * Monitor Plug-in ECom interface definition. Monitor plug-in implementation
 * needs to inherit this class and implement the pure virtual methods which exist 
 * in this interface. All monitor plug-ins are provided a reference to the Harvester 
 * server (MMonitorPluginObserver) which hanles the new harvesting requests, open session 
 * to MdE for DB maintenance operations, pointer to ContextEngine and a pointer Harvester 
 * plug-in factory.  
 *
 * Example:
 * 
 * // interface for notifying Monitor plug-in about new files
 * class MMonitorNotifier
 *	{
 *   public:
 *   	/**
 *	    * Called when we notice a new file/item
 *	    *
 *	    * @param aHarvesterData  Pointer to harvesting related data/harvesting request data
 *	    *
 *   	virtual void NewFile( CHarvesterData* aHarvesterData ) = 0;
 *  }
 * 
 * // Monitor plug-in implementation
 * class CMyMonitorPlugin: public CMonitorPlugin, public MMonitorNotifier
 *     {
 *     public:
 *         TBool StartMonitoring(MMonitorPluginObserver& aObserver, CMdESession* aMdEClient,
 *				CContextEngine* aCtxEngine, CHarvesterPluginFactory* aHarvesterPluginFactory );
 *	       TBool StopMonitoring();
 *	       TBool ResumeMonitoring( MMonitorPluginObserver& aObserver, CMdESession* aMdEClient,
 *				CContextEngine* aCtxEngine, CHarvesterPluginFactory* aHarvesterPluginFactory );				
 *	       TBool PauseMonitoring();
 *         void NewFile( CHarvesterData* aHarvesterData );
 *
 *     private:
 *          MMonitorPluginObserver* iObserver;
 *          CMyActiveObjectNotifier* iNotifier;
 *     }
 *
 * TBool CMyMonitorPlugin::StartMonitoring(MMonitorPluginObserver& aObserver, CMdESession* aMdEClient,
 *				CContextEngine* aCtxEngine, CHarvesterPluginFactory* aHarvesterPluginFactory )
 *     {
 *     iObserver = &aObserver;  
 *     
 *     // Active object which observes the system and notices new files/items in the system
 *     iNotifier = CMyActiveObjectNotifier::NewL(*this);
 *     }   	       
 * TBool CMyMonitorPlugin::StopMonitoring()
 *     {
 *	   iNotifier->StopNotifying();	
 *     }
 *	       
 * TBool CMyMonitorPlugin::ResumeMonitoring( MMonitorPluginObserver& aObserver, CMdESession* aMdEClient,
 *				CContextEngine* aCtxEngine, CHarvesterPluginFactory* aHarvesterPluginFactory )
 *     {
 *     // Do what needs to be done when operation is resumed - for example handle
 *     // events received through callback CMyMonitorPlugin::NewFile during pause. 
 *     }	      				
 *	       
 * TBool CMyMonitorPlugin::PauseMonitoring()
 *     {
 *     // Do what needs to be done during pause - for example buffer all events coming 
 *     // through callback CMyMonitorPlugin::NewFile
 *     }
 *
 * void CMyMonitorPlugin::NewFile( CHarvesterData* aHarvesterData )
 *    {
 *	  // Our CMyActiveObjectNotifier notifies us about new file and we notify the Harvester server about it.
 *	  // Before this CMyActiveObjectNotifier has filled the need data to aHarvesterData like:
 *	  aHarvesterData->SetEventType( EHarvesterAdd );
 *    aHarvesterData->SetOrigin( aOrigin );
 *    iObserver->MonitorEvent( aHarvesterData );
 *    }
 */
class CMonitorPlugin : public CBase
  	{
	public:
		/**
		* Creates and constructs a new instance of CMonitorPlugin.
		*
		* @param aUid  An UID of desired implementation.
		* @return A pointer to the new instance of CMonitorPlugin.
		*/
		IMPORT_C static CMonitorPlugin* NewL(const TUid& aUid);
	
		/**
		* A static method which list all available implementations which satisfy this given interface.
		*
		* @param aImplInfoArray  On return this contains information about all available implementations.
		*/
		IMPORT_C static void ListImplementationsL(RImplInfoPtrArray& aImplInfoArray);
		
		/**
		* Destructor
		*/
		IMPORT_C virtual ~CMonitorPlugin();
		
		/**
		* A pure virtual method which starts the monitoring.
		*
		* @param aObserver  All events are notified via the aObserver.
		* @param aMdEClient  A pointer to MdE client.
		* @param aCtxEngine  A pointer to context engine.
		* @param aHarvesterPluginFactory  A pointer to harvester plugin factory.
		* @return ETrue if success, EFalse if not.
		*/
		virtual TBool StartMonitoring(MMonitorPluginObserver& aObserver, CMdESession* aMdEClient,
				CContextEngine* aCtxEngine, CHarvesterPluginFactory* aHarvesterPluginFactory ) = 0;
		
		/**
		* A pure virtual method which stops the monitoring.
		*
		* @return ETrue if success, EFalse if not.
		*/
		virtual TBool StopMonitoring() = 0;
		
		/**
		* A pure virtual method which resumes the paused plug-in.
		*
		* @param aObserver  All events are notified via the aObserver.
		* @param aMdEClient  A pointer to MdE client.
		* @param aCtxEngine  A pointer to context engine.
		* @param aHarvesterPluginFactory  A pointer to harvester plugin factory.
		* @return ETrue if success, EFalse if not.
		*/
		virtual TBool ResumeMonitoring(MMonitorPluginObserver& aObserver, CMdESession* aMdEClient, 
				CContextEngine* aCtxEngine, CHarvesterPluginFactory* aHarvesterPluginFactory ) = 0;
		
		/**
		* A pure virtual method which pauses the plug-in.
		*
		* @return ETrue if success, EFalse if not.
		*/
		virtual TBool PauseMonitoring() = 0;

	private:
		
		/**
		* Identification on cleanup.
		*/
		TUid iDtor_ID_Key;
	};

#endif // __MONITORPLUGIN_H__
