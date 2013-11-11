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
* Description:  Data transfer object for harvest data
 *
*/


#ifndef HARVESTERDATA_H
#define HARVESTERDATA_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <badesca.h>
#include <mdccommon.h>
#include <locationdatatype.h>

#include <harvesterclientdata.h>
#include <placeholderdata.h>

#include "mdeconstants.h"

// FORWARD DECLARATIONS
class CContextEngine;
class CHarvesterBlacklist;
class CMdEObject;
class CHarvesterPluginInfo;
class CHarvesterData;

// ENUMERATIONS
enum THarvesterEventType
	{
	EHarvesterAdd,      // New item to harvest
	EHarvesterEdit,     // Re-harvest existing item
	EHarvesterDelete,   // Delete item
	EHarvesterUnknown   // Unknown command
	};

enum THarvesterObjectType
	{
	ENormal,           // Normal harvesting
	EPlaceholder,      // Placeholder harvesting
	EFastHarvest       // Fast harvesting - Harvester client request
	};

/**
 * Observer interface for notifications of harvesting completion from Harvesting
 * plug-ins to Harvester server side
 */
class MHarvesterPluginObserver
	{
    public:
    	/**
	    * Called when harvesting of a file/item is completed
	    *
	    * @param aHarvesterData  Pointer to harvesting related data/harvesting request data
	    */
    	virtual void HarvestingCompleted( CHarvesterData* aHarvesterData ) = 0;
	};

/**
 * Observer interface for adding new harvesting requests to Harvester server queue from
 * Monitor plug-ins
 */
class MMonitorPluginObserver
	{
    public:
    	/**
    	 * Method which is called by the monitor plug-in when event is monitored/
    	 * new harvesting request inserted to Harvester server queue.
    	 *
    	 * @param aHarvesterData  harvester data type
    	 */
    	virtual void MonitorEvent( CHarvesterData* aHarvesterData ) = 0;
    	
    	/**
    	 * Method which is called by the monitor plug-in when event is monitored/
    	 * new harvesting request inserted to Harvester server queue.
    	 * 
    	 * @param aHarvesterDataArray  Pointer to an array of harvesting related 
    	 *                             data/harvesting request data
    	 */
    	virtual void MonitorEvent( RPointerArray<CHarvesterData>& aHarvesterDataArray ) = 0;
	};

/**
 *  Class that encapsulates harvesting related data/harvesting request data. These instances
 *  are moved around the MdS Harvesting framework and this class encapsulates all information
 *  needed when harvesting a file/item.
 * 
 */
NONSHARABLE_CLASS( CHarvesterData ) : public CBase
	{
    public:
    
    	/**
    	 * Destructor.
    	 */
    	~CHarvesterData();
    
    	/**
    	 * Two-phased constructor.
    	 * 
    	 * @param aUri  URI of the file, usually filename. With messages can be messageID.
    	 */
    	IMPORT_C static CHarvesterData* NewL(const HBufC* aUri);
    	
    	/**
    	 * Two-phased constructor.
    	 *
    	 * @param aUri  URI of the file, usually filename. With messages can be messageID.
    	 */
    	IMPORT_C static CHarvesterData* NewLC(const HBufC* aUri);
    
    	/**
    	 * Method returns the URI.
    	 *
    	 * @return  URI.
    	 */
    	IMPORT_C const TDesC& Uri();
  
    	/**
    	 * Method returns error code of harvesting.
    	 *
    	 * @return  harvesting error code.
    	 */    	  
    	IMPORT_C TInt ErrorCode();

    	/**
    	 * Method sets the error code for harvesting.
    	 * 
    	 * @param aErrorCode  Error code for harvesting 
    	 */    	    	
    	IMPORT_C void SetErrorCode(TInt aErrorCode);

    	/**
    	 * Method sets harvesting type.
    	 * 
    	 * @param aEventType  Harvesting type
    	 */    	    
    	IMPORT_C void SetEventType(THarvesterEventType aEventType);

    	/**
    	 * Method returns harvesting type.
    	 * 
    	 * @return  Harvesting type
    	 */ 
      	IMPORT_C THarvesterEventType EventType();

    	/**
    	 * Method sets observer for this harvesting request (Harvester server)
    	 * 
    	 * @param aObserver  Pointer to the harvesting observer in the harvesting FW
    	 */     	
    	IMPORT_C void SetPluginObserver(MHarvesterPluginObserver& aObserver);

    	/**
    	 * Method returns pointer to the harvesting observer (Harvester server).
    	 * 
    	 * @return  Pointer to harvesting observer
    	 */     
    	IMPORT_C MHarvesterPluginObserver* PluginObserver();
    	
    	/**
    	 * Method sets the origin information for the file/item.
    	 * 
    	 * @param aOrigin  Origin of the file/item.
    	 */     	
    	IMPORT_C void SetOrigin(TOrigin aOrigin);

    	/**
    	 * Method returns origin of the file/item.
    	 * 
    	 * @return  Origin of the file/item.
    	 */ 
      	IMPORT_C TOrigin Origin();

    	/**
    	 * Method sets flag whether Context snapshot (with Context Engine)
    	 * is required during harvesting.
    	 * 
    	 * @param aTakeSnapshot  Whether Context snapshot is required or not.
    	 */       	    	
    	IMPORT_C void SetTakeSnapshot(TBool aTakeSnapshot);

    	/**
    	 * Method returns whether Context snapshot is required or not.
    	 * 
    	 * @return  Whether Context snapshot is required or not.
    	 */     	
    	IMPORT_C TBool TakeSnapshot();
  
    	/**
    	 * Method sets (object) type of the harvesting procedure.
    	 * 
    	 * @param aObjectType  Type of the harvesting procedure. 
    	 */     	  	
    	IMPORT_C void SetObjectType(THarvesterObjectType aObjectType);
    	
    	/**
         * Method retursn the (object) type of the harvesting procedure.
         *
         * @return  Type of the harvesting procedure.
    	 */     	
    	IMPORT_C THarvesterObjectType ObjectType();
    
    	/**
    	 * Method sets the custom harvesting data from client.
    	 * 
    	 * @param aClientData  Custom harvesting data from client.
    	 */     	
    	IMPORT_C void SetClientData(CBase* aClientData);
    	
    	/**
         * Method returns the custom harvesting data (received from client)
         *
         * @return  Custom harvesting data (received from client)
    	 */     	
    	IMPORT_C CBase* ClientData();
    	
    	/**
    	 * Method sets metadata object which will be used to store the metadata
    	 * in the harvesting plug-in.
    	 * 
    	 * @param aMdeObject  Pointer to the metadata object that will be used 
    	 *                    to store the metadata from the file/item.
    	 */     	
    	IMPORT_C void SetMdeObject( CMdEObject* aMdeObject );

    	/**
         * Method returns the metadata objects which is used to store the metadata
         * from the file/item that is harvested.
         *
         * @return  Metadata object that is used to store the metadata.
    	 */     	    	
    	IMPORT_C CMdEObject& MdeObject();

    	/**
    	 * Method sets location metadadata (GPS).
    	 * 
    	 * @param aLD  Pointer to the location metadata structure.
    	 */     	    	
    	IMPORT_C void SetLocationData( TLocationData* aLD );
    	
    	/**
         * Method returns the location metadata structure.
         *
         * @return  Location metadata structure.
    	 */     	
    	IMPORT_C TLocationData* LocationData();
    
    	/**
    	 * Method sets the client application UID.
    	 * 
    	 * @param aClientId  Client application UID.
    	 */     	
        IMPORT_C void SetClientId(TUid aClientId);
    	
    	/**
         * Method returns the client application UID.
         *
         * @return  Client application UID.
    	 */         
        IMPORT_C TUid ClientId();
    	
    	/**
         * Method returns whether location data should be added to
         * MdE DB or not.
         *
         * @return  Flag which defines whether location data needs to 
         *          be added or not.
    	 */ 		
        IMPORT_C TBool AddLocation();
		
    	/**
    	 * Method sets a flag which defines whether location data
    	 * should be added to MdE DB or not.
    	 * 
    	 * @param aAdd  Client application UID.
    	 */         
        IMPORT_C void SetAddLocation( TBool aAdd );
		
    	/**
    	 * Method sets harvesting plug-in information which
    	 * is needed to carry out the harvesting procedure
    	 * 
    	 * @param aPluginInfo  Pointer to harvester plug-in info structure.
    	 */         
		IMPORT_C void SetHarvesterPluginInfo( CHarvesterPluginInfo* aPluginInfo );
		
    	/**
         * Method returns harvester plug-in info structure.
         *
         * @return  Harvester plug-in info structure.
    	 */ 		
		IMPORT_C CHarvesterPluginInfo* HarvesterPluginInfo();
		
    	/**
         * Method returns whether item in harvesting is a binary file.
         *
         * @return  Whether item in harvesting is a binary file.
    	 */ 		
		IMPORT_C TBool IsBinary();

    	/**
    	 * Method sets a flag determinning 
    	 * 
    	 * @param aPluginInfo  Pointer to harvester plug-in info structure.
    	 */    		
		IMPORT_C void SetBinary( TBool aValue );

    
    private:
    
    	/**
    	 * Constructor for performing 1st stage construction
    	 * 
    	 * @param aUri  URI of the item ie. filename.
    	 */
    	CHarvesterData(const HBufC* aUri);
    
    	/**
    	 * Default constructor for performing 2nd stage construction
    	 */
    	void ConstructL();
    
    	/* URI descriptor */
    	const HBufC* iUri;
    	
    	/* Harvesting error code */
    	TInt iErrorCode;
    	
    	/* Harvesting event type */
    	THarvesterEventType iEventType;
    	
    	/* Harvesting observer ie. Harvester server */
    	MHarvesterPluginObserver* iObserver;
    	
    	/* Origin of the file/item */
    	TOrigin iOrigin;
    	
    	/* Whether Context snapshot is needed */
    	TBool iTakeSnapshot;
    	
    	/* Object type for harvesting */
    	THarvesterObjectType iObjectType;
    	
    	/* Custom client data */
    	CBase* iClientData;
    	
    	/* Pointer to metadata object */
    	CMdEObject* iMdeObject;
    	
    	/* Harvesting plug-in info */
    	CHarvesterPluginInfo* iPluginInfo;
    	
    	/* GPS and other location related data */
    	TLocationData* iLocationData;
    	
    	/* Whether location data needs to be added to DB */
    	TBool iAddLocation;
    	
    	/* Whether item to harvest is a binary file */
    	TBool iIsBinary;
    	
    	/* Executable UID which created the binary file */
		TUid iClientId;
	};

#endif // HARVESTERDATA_H

