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
* Description:   This class provides the base from which the Session object
*   			 in the protocol handler has to be derived.This is an
*   			 abstract class and has to be implemented by the Protocol
*  				 Handler plug-in
*
*/


#ifndef _SUPL_CPOSSUPLSESSIONBASE_H
#define _SUPL_CPOSSUPLSESSIONBASE_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>
#include <lbspositioninfo.h>

class TRequestStatus;
// CLASS DECLARATION

/**
*  Base class for SUPL Session Object in Protocol Handler
*  This is an abstract class that has to be implemented
*  by the Protocol Handler plug-in. 
*
*  @lib ?library
*  @since Series 60 Series60_3.1M
*/
class CSuplSessionBase : public CBase
    {
    public:  // Constructors and destructor
        
    public: // New functions
        

		/**
        * CleanUp Method.
        * @since Series 60 3.1M
        * @param None
        * @return None
        */
        virtual void SessionEnd() = 0;
        
        /**
        * HandleSuplMessage Method.
        * @since Series 60 3.1M
        * @param None
        * @return None
        */
      //  IMPORT_C virtual  void HandleSuplMessage(const TDesC &) = 0;
        
        /**
        * GetPosition Method.
        * @since Series 60 3.1M
        * @param aStatus,aPos
        * @return None
        */
       virtual TInt GetPosition(TPositionInfo& aSuplPosInfo)=0;

        /**
        * GetPosition Method.
        * @since Series 60 3.1M
        * @param aSuplPosInfo
        * @return None
        */
        
       virtual TInt GetPosition(HPositionGenericInfo& aSuplPosInfo)=0;
       
		/**
        * CancelRunSession Method.
        * @since Series 60 3.1M
        * @param aStatus,aPos
        * @return None
        */
       virtual void CancelRunSession()=0;

		/**
        * CancelTriggerSession Method.
        * @since Series 60 3.1M
        * @param aStatus,aPos
        * @return None
        */
       virtual void CancelTriggerSession()=0;
       
       virtual TInt GetServerAddressL(TDes& ahslpAddress)=0;
       
       virtual void SetSUPLVersion(TInt aVersion) { iVersion = aVersion; }
       
       virtual TInt GetSUPLVersion() const { return iVersion;}
       

    public: // Functions from base classes
        
    protected:  // New functions
        
       
    protected:  // Functions from base classes

    private:


    public:     // Data
    
    protected:  // Data

    private:    // Data
    
    TInt iVersion;
    
    };

#endif      // _SUPL_CPOSSUPLSESSIONBASE_H
            
// End of File
