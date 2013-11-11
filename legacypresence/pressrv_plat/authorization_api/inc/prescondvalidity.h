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
* Description:    RLS and Presence XDM, This class represents Presence Condition
*  validity part.
*
*/



#ifndef __PRESCONDVALIDITY_H__
#define __PRESCONDVALIDITY_H__

// Includes
#include <e32std.h>
#include <e32base.h>
#include <badesca.h>
#include <rlspresxdmconsts.h>

// Forward declarations
class CPresenceXDM;
class CXdmDocumentNode;

// Class declaration
/** 
* This class should be used in conjunction with an already created CPresenceXDM.
*  The CPresenceXDM deals with the Presence rules as a whole and has communication
*  with the server.
*  TPresCondValidity is used to manipulate the validity part of Condition in a rule. 
*  Remember to call Init function after constructing this object.
*  
*  @lib rlspresxdm.lib
*  @since S60 3.2
*/
class TPresCondValidity
    {
    public:
    
        /**
        * C++ Constructor, After contructing Init must be called before using
        * this object.
        */                                        
        IMPORT_C TPresCondValidity();
        
        /**
        * Initializes the object with the given pointer to CPresenceXDM.
        * This function must be called before using other functions.
        *
        * @param CPresenceXDM* pointer to created CPresenceXDM
        * @return none
        */
        IMPORT_C void Init(CPresenceXDM* const aPresXDM);
        
        /**
        * Gets condition validity values from a rule
        *
        * @param TDesC& aRuleID rule ID.
        * @param RArray<TTimePeriod>& returned array of valid times in local time.
        * @return TInt KErrNone, KErrNotFound if rule doesnt found or none
        *   of validities found
        */
        IMPORT_C TInt GetValidityL(const TDesC& aRuleID, 
                                        RArray<TTimePeriod>& aValidTimes);
                                        
        /**
        * Deletes condition validity values from a rule
        *
        * @param TDesC& aRuleID rule ID.
        * @return TInt KErrNone, KErrNotFound if rule doesnt found.
        */
        IMPORT_C TInt DeleteAllValidityL(const TDesC& aRuleID);
        
        /**
        * Adds condition validity to a rule
        *
        * @param TDesC& aRuleID rule ID.
        * @param TTimePeriod valid time to be added. In local time.
        * @return TInt KErrNone, KErrNotFound if rule doesnt found.
        */
        IMPORT_C TInt AddValidityL(const TDesC& aRuleID, 
                                                const TTimePeriod aValidTime);
                                                
        /**
        * Checks if rule is valid for given time
        *
        * @param TDesC& aRuleID rule ID.
        * @param TTimePeriod valid time to be checked, in local time
        * @return ETrue if given time is a subset of valid times in a rule
        */
        IMPORT_C TBool IsTimeValidL(const TDesC& aRuleID, 
                                                const TTimePeriod aTime);
                                                
    private:
    
        /**
        * Gets the Validity node pointer in a rule
        *
        * @param TDesC& aRuleID rule ID.
        * @param TBool aCreate if ETrue, creates Validity node if not exist
        * @return CXdmDocumentNode* pointer to Validity node or NULL
        */
        CXdmDocumentNode* GetValidityNodeL(const TDesC& aRuleID, 
                                                        TBool aCreate);

        /**
        * Parse from XML time string to TTime
        *
        * @param TDesC& aXMLTimeString, XML time string to parse
        * @param TTime& aDateTime, returned here in TTime format
        * @return error code
        */
        TInt ParseFromXMLTimeString(const TDesC& aXMLTimeString, 
                                                          TTime& aDateTime);
                                                          
        /**
        * Parse to XML time string from TTime
        *
        * @param TDes& aXMLTimeString, retuned XML time string
        * @param TTime aDateTime, input TTime
        * @return error code
        */
        TInt FormatToXMLTimeStringL(TDes& aXMLTimeString, const TTime aDateTime);
        
        /**
        * Convert local time to universal time
        *
        * @param TTime& aLocalDateTime, coverted to universal TTime
        * @return none
        */
        void ConvertToUniversal(TTime& aLocalDateTime);
        
        /**
        * Convert universal time to local time
        *
        * @param TTime& aUniversalDateTime, coverted to local TTime
        * @return none
        */
        void ConvertToLocal(TTime& aUniversalDateTime);
        

        /**
        * Logs Given dateTime
        *
        * @param TDateTime aDateTime date time to log
        * @return None
        */
        void LogDateTime(TDateTime aDateTime);

    private: // data
        
        // Pointer to CPresenceXDM - not owned
        CPresenceXDM*           iPresXDM;      // not owned
        
    };
    
   
#endif //__PRESCONDVALIDITY_H__
        
