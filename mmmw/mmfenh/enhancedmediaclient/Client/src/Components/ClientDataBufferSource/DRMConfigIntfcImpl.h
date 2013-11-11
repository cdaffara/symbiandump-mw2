/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Header of DRMConfigIntfcImpl object.
*
*/


#ifndef C_CDRMCONFIGINTFCIMPL_H
#define C_CDRMCONFIGINTFCIMPL_H

#include <e32base.h>
#include <DRMConfigIntfc.h>
#include "ParentChildIntfc.h"
#include "ClientDataBufferSource.h"

using multimedia::CClientDataBufferSource;
/**
*  DRM Config Implementation
*  @lib EnhancedMediaClient.lib
*  @since S60 v3.2
*/    
class CDRMConfigIntcfImpl : public CBase,
                            public CDRMConfigIntfc,
                            public CChildIntfc
    {
    public:
        /**
        * Function to Create the Object.
        * @param aObserver Observer to callback the Event.
        */
        static CDRMConfigIntcfImpl* NewL( CClientDataBufferSource& aDRMHandler );
        /**
        * Destructor.
        */
        ~CDRMConfigIntcfImpl();

        /**
        * From CDRMConfigIntfc.
        * Gets the DRM type set by the Client
        *
        * @since S60 v3.2
        * @param aDRMType   DRM type set by the Client
        */            
        TInt GetDRMType( TDRMType& aDRMType );
        /**
        * From CDRMConfigIntfc.
        * DRM type set by the Client
        *
        * @since S60 v3.2
        * @param aDRMType   DRM type set by the Client
        */            
        TInt SetDRMType( TDRMType aDRMType );
        /**
        * From CDRMConfigIntfc.
        * Number of Allowed output Devices
        *
        * @since S60 v3.2
        * @param aCount   Number of Output Devices Allowed
        */            
        TInt GetAllowedOutputDeviceCount( TInt& aCount );
        /**
        * From CDRMConfigIntfc.
        * Gets the Output Device from a particular index in 
        * the array of Allowed Output Devices
        *
        * @since S60 v3.2
        * @param aIndex   Index into the Array
        * @param aOutputDevice   Output Device at the input index
        */            
        TInt GetAllowedOutputDevice(
                        TInt aIndex,
                        TDRMAllowedOutputDevice& aOutputDevice );
        /**
        * From CDRMConfigIntfc.
        * Append Allowed Output Devices
        *
        * @since S60 v3.2
        * @param aOutputDevice   Output Device to be appended to the Array
        */            
        TInt AppendAllowedOutputDevice(
                                TDRMAllowedOutputDevice aOutputDevice );
        /**
        * From CDRMConfigIntfc.
        * Append Allowed Output Devices
        *
        * @since S60 v3.2
        * @param aOutputDevice   Output Device to be removed from the Array
        */            
        TInt RemoveAllowedOutputDevice(
                                TDRMAllowedOutputDevice aOutputDevice );
        /**
        * From CDRMConfigIntfc.
        * Reset the Allowed Ooutput Device List
        *
        * @since S60 v3.2
        */            
        TInt Reset();
        /**
        * From CDRMConfigIntfc.
        * Commint the stored list to the ServerSide Source
        *
        * @since S60 v3.2
        */            
        TInt Commit();
    
    private:
        CDRMConfigIntcfImpl( CClientDataBufferSource& aDRMHandler );
        void ConstructL();
    
    private:
        /**
        * Pointer to the ClientDataBufferSource which requested this.
        */
        CClientDataBufferSource* iDRMHandler;
    };

#endif /*C_CDRMCONFIGINTFCIMPL_H*/

// End of file
