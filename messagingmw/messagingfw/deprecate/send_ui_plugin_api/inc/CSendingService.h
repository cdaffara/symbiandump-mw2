/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Declares sending service interface for ECom plugins.
*
*/


#ifndef C_SENDINGSERVICE_H
#define C_SENDINGSERVICE_H

#include <e32base.h>
#include <badesca.h>
#include <txtrich.h>

class CSendingServiceInfo;
class CMessageData;
class CCoeEnv;
class CSendUiSingleton;


/**
 * Parameters for sending service construction.
 */
struct TSendingServiceParams
	{
	// Constructor
	TSendingServiceParams( CCoeEnv& aCoeEnv, CSendUiSingleton& aSingleton )
	    : iCoeEnv( aCoeEnv ), iSingleton( aSingleton ) {}
	// Data
	CCoeEnv& iCoeEnv;
	CSendUiSingleton& iSingleton;
	};


/**
 * Declares sending service interface for ECom plugins.
 *
 * CSendingService has to be implemented by each ECom based sending service.
 * CSendingService contains only pure virtual methods, which must be 
 * implemented by sending services.
 *
 * @lib Sendui
 * @since S60 v3.0
 */
class CSendingService : public CBase
    {
public:  // Constructors and destructor

   /**
    * Two-phased constructor.
    *
    * @param aImplementationUid The implementation UID of the
    *        sending service to be created.
    * @return Pointer to object of CSendingService
    */
    IMPORT_C static CSendingService* NewL( const TUid& aImplementationUid, CCoeEnv& aCoeEnv, CSendUiSingleton& aSingleton );

    /**
    * Destructor.
    */
    IMPORT_C virtual ~CSendingService();

public: // New functions

   /**
    * Populates given list with the provided services. Ownership of the
    * pointed services remains in the service provider plugin.
    *
    * @since Series 60 3.0
    * @param aList Service list to be populated. 
    * @return none
    */
    virtual void PopulateServicesListL(
        RPointerArray<CSendingServiceInfo>& aList ) = 0;

    /**
    * Creates message to be sent and either sends it directly or calls
    * MTM to edit the message.
    *
    * @since Series 60 3.0
    * @param aServiceUid Uid of the sending service.
    * @param aMessageData Data for the message.
    * @return none
    */
    virtual void CreateAndSendMessageL(
        TUid aServiceUid,
        const CMessageData* aMessageData,
        TBool aLaunchEditorEmbedded = ETrue) = 0;

   /**
    * Return the Id of the service provider.
    *
    * @since Series 60 3.0
    * @return Id of the service provider.
    */
    virtual TUid ServiceProviderId() const = 0;

protected:

    /**
    * C++ constructor
    */    
    IMPORT_C CSendingService( CCoeEnv& aCoeEnv, CSendUiSingleton& aSingleton );

private:  // Data
  
    /**
     * Implementation UID of the sending service
     * Not own.
     */
    TUid iDtor_ID_Key;

protected:

    /**
     * Reference to CCoeEnv
     * Not own.
     */
    CCoeEnv& iCoeEnv;

    /**
     * Pointer to CSendUiSingleton;
     * Not own.
     */
    CSendUiSingleton& iSingleton;
        
};

#endif      // C_SENDINGSERVICE_H

// End of File
