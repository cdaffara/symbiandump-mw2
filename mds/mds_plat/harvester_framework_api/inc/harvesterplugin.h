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
* Description:  Definition of the Harvester plug-in ECom interface.*
*/

#ifndef __CHARVESTERPLUGIN_H__
#define __CHARVESTERPLUGIN_H__

#include <e32base.h>
#include <ecom.h>

// forward declarations
class CMdESession;
class CHarvesterBlacklist;
class CHarvesterData;
class CMdEPropertyDef;
class CHarvesterPluginFactory;

// Uid for this interface
const TUid KCHarvesterPluginInterfaceUid = { 0x200009F8 };

struct THarvestResult
    {
    HBufC16* iUri;   // URI of the file/item
    TInt iErrorCode; // Harvesting error code
    };

/**
 * Harvester Plug-in ECom interface definition. Harvester plug-in implementation
 * needs to inherit this class and implement the pure virtual method (HarvestL) which exist 
 * in this interface.
 *
 * Example:
 * 
 * class CMyHarvesterPlugin:  public CHarvesterPlugin
 *     {
 *     public:
 *
 *         // This method should also be overwritten in the plug-in
 *         void GetObjectType( const TDesC& aUri, TDes& aObjectType );
 *
 *         void HarvestL( CHarvesterData* aHD );
 *     }
 *
 * void GetObjectType( const TDesC& aUri, TDes& aObjectType )
 *     {
 *      // Idea in this method is that it MUST return one of the object types
 *	    // supported by the MdE DB - for example "Image", "Video" or "Audio".
 * 	    // If plug-in supports only one type of objects then it can just return
 * 	    // always that like:  
 * 	    // TPtrC ptrImage( KImage );
 * 	    // aObjectType.Copy( KImage ); return;
 * 	    //
 * 	    // However if multiple MdE object types are supported then file content
 * 	    // needs to be checked for example from file MIME-type like here:
 * 	  
 *	   	TRAPD( err, content = CContent::NewL( aUri ) );
 *	    if (err == KErrNone) 
 *		    {
 *		    err = content->GetStringAttribute( EMimeType, mime );
 *		    delete content;
 *		    }
 *
 *	    TPtrC ptrImage( KImage );
 *	    
 *	    // MdsUtils::Find tries to find word "image" from the MIME-type
 *      // so that we know the file is an image
 *		if( MdsUtils::Find( mime, ptrImage ) != KErrNotFound )
 *			{
 *			WRITELOG1( "CHarvesterOMADRMPlugin::GetObjectType - mimetype %S. Object type changed to Image", &mime );
 *			aObjectType.Copy( KImage );
 *			return;
 *			}
 *		
 *		TPtrC ptrVideo( KVideo );
 *		if( MdsUtils::Find( mime, ptrVideo ) != KErrNotFound )
 *			{
 *			WRITELOG1( "CHarvesterOMADRMPlugin::GetObjectType - mimetype %S. Object type changed to Video", &mime );
 *			aObjectType.Copy( KVideo );
 *			return;
 *			}
 *     }
 *
 * void HarvestL( CHarvesterData* aHD )
 *     {
 *     CMdEObject& mdeObject = aHD->MdeObject();
 *      
 *     CMdeObjectWrapper* wrapper = CMdeObjectWrapper::NewL();
 *     }	 
 */
class CHarvesterPlugin : public CActive
    {
public:
	    
	enum THarvesterState
	    {
	    EHarvesterIdle = 0,   // No harvesting requests to process
	    EHarvesterGathering,  // Harvesting a file
	    };
	
	/** 
	 * Construction 
	 */
	IMPORT_C static CHarvesterPlugin* NewL( const TUid& aUid );
	
    /**
     * Lists all available implementations which satisfy this given interface.
     * 
     * @param aImplInfoArray  Reference to a list which will be populated with 
     *                        plug-in implementation details. 
     */
	IMPORT_C static void ListImplementationsL( RImplInfoPtrArray& aImplInfoArray );
	
	/**
	 * Destructor - virtual and class not intended
	 * for derivation, so not exported.
	 */
	IMPORT_C virtual ~CHarvesterPlugin();
	
	/**
	 *  Method sets the Harvesting queue to the plug-in.
	 * 
	 * @param aQueue  Queue which contains harvesting requests.
	 */
	IMPORT_C virtual void SetQueue( RPointerArray<CHarvesterData>& aQueue );
	
	/** 
	 * Method which starts the harvesting of the file. Called by the 
	 * harvesting framework from CHarvesterPluginFactory::HarvestL.
	 */
	IMPORT_C virtual void StartHarvest();
	
	/** 
	 * Method which returns the MdE object type to harvesting FW when passing the file
	 * name to this method (aURI). The harvesting plug-in should overwrite method. In the
	 * case there are more than one plug-ins which support same file extension atleast
	 * one of these plug-ins needs to read the binary file for determining the valid MdE
	 * object type for the file.
	 *
	 * @param aUri         URI of the file/item.
	 * @param aObjectType  Reference to an object type which will returned to the FW 
	 *                     from the plug-in
	 */
	IMPORT_C virtual void GetObjectType( const TDesC& aUri, TDes& aObjectType );
	
	/** 
	 * Method for setting the blacklisting functionality for the plug-in.
	 * 
	 * @param aBlacklist  Reference to blacklisting component.
	 */
	IMPORT_C virtual void SetBlacklist( CHarvesterBlacklist& aBlacklist );
	
    /** 
	 * Method to stop harvester plugin if needed.
	 */
	IMPORT_C virtual void StopHarvest();

protected:
    
	/** 
	 * The method which does the actual harvesting. Harvesting plug-in MUST
	 * implelemt this method.
	 * 
	 * @param aHD  Pointer to harvesting data/request.
	 */
	IMPORT_C virtual void HarvestL( CHarvesterData* aHD ) = 0;
	
	/** 
	 * Active object RunL implementation.
	 */
	IMPORT_C virtual void RunL();
	  
	/** 
	 * Active object DoCancel implementation.
	 */
	IMPORT_C virtual void DoCancel();
	  
	/** 
	 * Active object RunError implementation.
	 */
	IMPORT_C virtual TInt RunError( TInt aError );
	
	 /** 
	  * Construction of the interface class
	  */
	 void ConstructL();
	
	 /** 
	  * Constuctor 
	  */
	 IMPORT_C CHarvesterPlugin();

public:

    /** 
     * Method for getting the mime type for given uri
     */
    IMPORT_C virtual void GetMimeType( const TDesC& aUri, TDes& aMimeType );
    
    /** 
     * Method for adding reference to harvester plugin factory
     */
    IMPORT_C void SetHarvesterPluginFactory( CHarvesterPluginFactory& aFactory );
         
    /** 
     * Method for checking if the plugin is in idle state
     */
    IMPORT_C TBool PluginInIdleState();
    
private:
    
	/** 
	 * Sets next state for the active object
	 *
	 * @param aState  Next state to execute with RunL.
	 */
	void SetNextRequest( THarvesterState aState );
		
	/** 
	 * Method sets some default properties to the metadata object
	 * 
	 * @param aData  Pointer to harvesting data/request.
	 */
	void SetDefaultPropertiesL(CHarvesterData& aData);
	
protected:
	
	/** 
	 * Handle to File server session.
	 */
	RFs iFs;
	
	/**
	 *  State of this active object.
	*/
	THarvesterState iState;
	     
	/** 
	 * Pointer to Harvesting request queue.
	 */
	RPointerArray<CHarvesterData>* iQueue;
	
	/** 
	 * Pointer to blacklisting functionality
	 */
	CHarvesterBlacklist* iBlacklist;

    // Not own
    CHarvesterPluginFactory* iFactory;
    
    TBool iFastModeEnabled;
    
private:
	
	/* Identification on cleanup */ 
	TUid iDtor_ID_Key;

	CMdEPropertyDef* iOriginPropertyDef;
	CMdEPropertyDef* iTitlePropertyDef;
	
    TBool iHarvesting;
    TBool iPaused;
	
    };

#endif // __CHARVESTERPLUGIN_H__
