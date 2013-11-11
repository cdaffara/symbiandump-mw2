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
* Description: Declaration of CCCECallParameters class.
*
*/

#ifndef CCCECALLPARAMETERS_H
#define CCCECALLPARAMETERS_H


#include <cccpcallparameters.h>
#include <ccpdefs.h>



const TUint KCCESubAddressMaxSize       = 21;
const TUint KCCEBearerMaxSize           = 15;

/**
* Call parameters struct.
*/
class CCCECallParameters: public CCCPCallParameters
    {
    public:

    /**
    * The type of line.
    */
    enum TCCELineType
        {
        /** This is the primary voice line. */
        ECCELineTypePrimary,    
        /** This is the secondary voice line. */
        ECCELineTypeAux  
        };

    /**
    * Call type enumeration.
    */
        
    enum TCCECallOrigin {
        ECCECallOriginPhone = 0,
        ECCECallOriginSAT,
        };     



    
       IMPORT_C static CCCECallParameters* NewL();
       ~CCCECallParameters();
       virtual CCCPCallParameters* CloneL() const;

        // New functions
        /** Sets the line type of call. */
        virtual void SetLineType(TCCELineType aLineType);
        /** Gets the line type of the call. */
        virtual TCCELineType LineType() const;

        /** Sets the service id identifying the service provider of the call. */
        virtual void SetUUSId(TBuf<KCCPUUSIdSize> aUUSId);
        /** Gets the UUS id of the call. */
        virtual TBuf<KCCPUUSIdSize> UUSId() const;

        /** Sets the bearer of the call. */
        virtual void SetBearer(TBuf8<KCCEBearerMaxSize> aBearer);
        /** Gets the bearer of the call. */
        virtual TBuf8<KCCEBearerMaxSize> Bearer() const;

        /** Sets the sub address of the dialled phone number. */
        virtual void SetSubAddress(TBuf<KCCESubAddressMaxSize> aSubAddress);
        /** Gets the sub address of the dialled phone number. */
        virtual TBuf<KCCESubAddressMaxSize> SubAddress() const;

        /** Sets the origin of the call. */
        virtual void SetOrigin(TCCECallOrigin aCallOrigin);
        /** Gets the origin of the call. */
        virtual TCCECallOrigin Origin() const;

        /** Sets the SAT alpha id identifying the text to be shown instead of number in         
            call bubble */
        virtual void SetAlphaId(TBuf<KCCPAlphaIdMaxSize> aAlphaId);
        /** Gets the SAT alpha id of the call. */
        virtual TBuf<KCCPAlphaIdMaxSize> AlphaId() const;
    
    protected:
       CCCECallParameters();
       void ConstructL();
       
       
       
    private:
        /** Line type, used only in CS plugin in CCP API. */
        TCCELineType iLineType; 
        /** UUS-Id. Used in CS. */
        TBuf<KCCPUUSIdSize> iUUSId;
        /** Capability configuration parameters for the first bearer. */
        TBuf8<KCCEBearerMaxSize> iBearer;
        /** A buffer to hold the sub-address of a telephone number.  */
        TBuf<KCCESubAddressMaxSize> iSubAddress;
        /** Call origin.  */
        TCCECallOrigin iOrigin;
        /** Alpha id.  */
        TBuf<KCCPAlphaIdMaxSize> iAlphaId;
    
    };

#endif