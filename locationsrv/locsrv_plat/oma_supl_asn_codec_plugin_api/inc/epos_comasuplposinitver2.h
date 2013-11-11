/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Header file for supl 2.0 pos init message
*
*/

#ifndef C_COMASuplPosInitVer2_H
#define C_COMASuplPosInitVer2_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>
#include "epos_comasuplasnmessagebase.h"
#include "epos_comasuplposinit.h"
#include "epos_suplconstants.h"


//Forward Declaration
class COMASuplSETCapabilitiesVer2;
class COMASuplLocationIdVer2;

/**
 *  Class for providing the suplposinit message for SUPL v2.0
 *  For holding the Supl Pos Init message and encoding it
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_5.2
 */
class COMASuplPosInitVer2  : public COMASuplPosInit
    {
    public:  // Constructors and destructor
    
        /**
         * Two phased constructor
         * @since S60 S60_5.2
         * @param None
         * @return COMASuplPosInitVer2 pointer to SuplPosInit Message
         */         
    
        IMPORT_C static COMASuplPosInitVer2* NewL();
    
        /**
         * Destructor
         * @since S60 S60_5.2
         * @param None
         * @return None
         */
        IMPORT_C virtual ~COMASuplPosInitVer2();
    
        /**
         * Cloning of SuplPosInit
         * @since S60 S60_5.2
         * @param aPosInit  pointer to pos init message to be cloned
         * @param aErrorCode error code
         * @return Returns None
         */
        IMPORT_C void Clone(const COMASuplAsnMessageBase* aPosInit,TInt& aErrorCode);
    
        /**
         * Setter method for setting location id 
         * @since S60 S60_5.2
         * @param  aLocationId2 location id to be set into pos init message
         *                      ownership is transferred to the pos init message
         *
         */
        IMPORT_C void SetLocationId2(COMASuplLocationIdVer2* aLocationId2);
    
        /**
         * Setter method for setting set capabilities
         * @since S60 S60_5.2
         * @param  aSETCapabilities set capabilities to be set into pos init message
         *                      ownership is transferred to the pos init message
         *
         */
        IMPORT_C void SetSETCapabilities2(COMASuplSETCapabilitiesVer2* aSETCapabilities);
    
 
    protected:    
    
        IMPORT_C void ConstructL();        
    
        IMPORT_C COMASuplPosInitVer2();  
    
    protected:    // Data
    
        /** SetCapabilities2*/
        COMASuplSETCapabilitiesVer2* iSETCapabilities2; 
    
        /** Location Id2*/
        COMASuplLocationIdVer2* iLocationId2;

    };


#endif //C_COMASuplPosInitVer2_H
