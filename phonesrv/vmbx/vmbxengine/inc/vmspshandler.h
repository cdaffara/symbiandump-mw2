/*
* Copyright (c) 2009-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Service provider settings handler
 *
*/

#ifndef C_VMSPSHANDLER_H
#define C_VMSPSHANDLER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <mspnotifychangeobserver.h>
#include <spsettingsvoiputils.h>
#include "mvmbxservicenotify.h"


// FORWARD DECLARATIONS
class CSPSettings;
class CSPNotifyChange;
class TVmbxServiceInfo;

// CLASS DECLARATION

/**
 *  CVmSPSHandler declaration.
 *  Service provider settings handler is used to access service and
 *  sub-service configuration data.
 *
 *  @since S60 3.2
 */
NONSHARABLE_CLASS( CVmSPSHandler ) : public CBase,
                                     private MSPNotifyChangeObserver
    {

public: // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CVmSPSHandler* NewL();

    /**
     * Two-phased constructor.
     */
    static CVmSPSHandler* NewLC();

    /**
     * Destructor.
     */
    virtual ~CVmSPSHandler();

public: // New functions

    /**
     * Get service ids which support Voice Mailbox
     * @since S60 3.2
     * @param aServiceIds On completion contains service ids
     */
    void GetServiceIdsL( RArray<TUint>& aServiceIds ) const;

    /**
     * Get service's info
     * @since S60 3.2
     * @param aServiceInfo On completion contains service info
     */
    TInt GetServiceInfo( TVmbxServiceInfo& aServiceInfo ) const;

    /**
     * Get service's VMBX address
     * @since S60 3.2
     * @param aServiceId Used Service
     * @param aService On completion, contains service's address
     */
    void GetVmAddressL( const TUint32 aServiceId, TDes& aAddress ) const;

    /**
     * Get service's name
     * @since S60 3.2
     * @param aServiceId Used Service
     * @param aName On completion, contains service's name
     */
    void GetServiceNameL( const TUint32 aServiceId, TDes& aName ) const;

    /**
     * Get SNAP id
     * @since S60 3.2
     * @param aServiceId Used Service
     * @param aSNAPId On completion, contains SNAP Id
     */
    void GetSNAPIdL( TUint32 aServiceId, TInt& aSNAPId ) const;

    /**
     * Get brand id
     * @since S60 3.2
     * @param aServiceId Used Service
     * @param aBrandId On completion, contains Brand Id
     */
    void BrandIdL( TInt aServiceId, TDes8& aBrandId );

    /**
     * Check is given service already activated
     * @since S60 3.2
     * @param aServiceId Used Service
     * @return ETrue if Service is already activated
     */
    TBool IsActivatedL( TInt aServiceId );

    /**
     * Issues a notify request on a Service profile change.
     * @param aHandler user's notify handler
     */
    void NotifyServiceChange( MServiceNotifyHandler* aHandler );

    /**
     * Cancels a notify request on a VoIP profile change.
     */
    void NotifyServiceChangeCancel();

    /**
     * Check from SPSettings if there are VoIP profiles
     * @since S60 3.2.3
     * @param None
     * @return ETrue if VoIP profiles were found, otherwise EFalse
     */
    TBool IsVoIPProfilesFound();

protected:

    /**
     * Handle notify change event.
     * From MSPNotifyChangeObserver;
     *
     * @since S60 3.2
     * @param aServiceId The service ID of changed service
     */
    void HandleNotifyChange( TServiceId aServiceId );

    /**
     * Handle error
     *
     * @since S60 3.2
     */
    void HandleError( TInt aError );

private:

    /**
     * Is VMBX supported
     * @since S60 3.2
     * @param aServiceId Used Service
     * @return ETrue if VMBX is supported
     */
    TBool IsVbmxSupportedL( TUint32 aServiceId ) const;

    /**
     * C++ default constructor.
     */
    CVmSPSHandler();

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

private: // data

    /**
     * CSPSettings. Own
     */
    CSPSettings*                    iSettings;

    /**
     * CSPSettingsVoIPUtils. Own
     */
    CSPSettingsVoIPUtils* iSpsSettingsVoipUtils;

    /**
     * Observer. Not own
     */
    MServiceNotifyHandler*          iObserver;

    /**
     * Pointer to CSPNotifyChange
     */
    CSPNotifyChange*                 iNotifier;

    /**
     * Service id array
     */
    RIdArray                        iServiceIds;

    };

#endif // C_VMSPSHANDLER_H

// End of file
