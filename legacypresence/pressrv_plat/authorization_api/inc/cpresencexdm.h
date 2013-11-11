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
* Description:    RLS and Presence XDM, This is primary class for Presence XDM
*
*/



#ifndef __PRESENCEXDM_H__
#define __PRESENCEXDM_H__

// INCLUDE FILES
#include <e32base.h>
#include <badesca.h>
#include <rlspresxdmconsts.h>

#include "prescondidentityone.h"
#include "prescondidentitymany.h"
#include "prescondmisc.h"
#include "prescondvalidity.h"
#include "prescondsphere.h"
#include "presenceactionxdm.h"
#include "presencetransformxdm.h"

// Forward declarations
class MRLSPresXDMAsyncHandler;
class CXdmDocumentNode;
class CXdmProtocolInfo;
class CXdmEngine;
class CXdmDocument;

// Class declaration
/** 
* This class deals with the Presence rules as a whole and has communication
*  with the server. Rules can be manipulated using this class, however the 
*  three childrens of rule namely condition, action and transform manipulation
*  also requires their respective classes in conjunction with this class.
*  
*  @lib rlspresxdm.lib
*  @since S60 3.2
*/
class CPresenceXDM : public CActive
    {
    public:
        
        /**
        * Create CPresenceXDM
        *
        * @param TInt XDMSettingsList ID, see XDMSettingsApi.h for more info
        * @return Pointer to created CPresenceXDM
        */
        IMPORT_C static CPresenceXDM* NewL(const TInt aSettingId);
        
        /**
        * Create CPresenceXDM, leaves pointer to stack
        *
        * @param TInt XDMSettingsList ID, see XDMSettingsApi.h for more info
        * @return Pointer to created CPresenceXDM, pushed on cleanup stack
        */
        IMPORT_C static CPresenceXDM* NewLC( const TInt aSettingId );

        
        /**
        * Updates information (document) to server. This results in
        * HandlePresUpdateDocumentL on completion.
        *
        * @param aHandler, pointer to MRLSPresXDMAsyncHandler callback handler
        * @return Error code, KErrAlreadyExist if some async already going on
        */
        IMPORT_C TInt UpdateToServerL(MRLSPresXDMAsyncHandler* const aHandler);
        
        /**
        * Updates all information (document) from server. This results in
        * HandlePresUpdateDocumentL on completion.
        *
        * @param aHandler, pointer to MRLSPresXDMAsyncHandler callback handler
        * @return Error code, KErrAlreadyExist if some async already going on
        */
        IMPORT_C TInt UpdateAllFromServerL(MRLSPresXDMAsyncHandler* const aHandler);
        
        /**
        * Cancels document update to/from server if going on. This results in
        * HandlePresUpdateCancelL on completion.
        */
        IMPORT_C void CancelUpdateL();
        
        /**
        * Gets all rule IDs in an array
        *
        * @param CDesCArray& aRuleIds, reference to rule IDs
        */
        IMPORT_C void GetAllRulesL(CDesCArray& aRuleIds);
        
        /**
        * Delete Rule and associated coditions/actions/transforms
        *
        * @param TDesC& aRuleId rule ID
        * @return KErrNotFound if not found. System wide error codes
        */
        IMPORT_C TInt DeleteRuleL(const TDesC& aRuleId);
        
        /**
        * Add a new rule. If aRuleId contains a valid descriptor it is used as
        * rule id for the new rule. If a NULL descriptor is provided instead,
        * a random id is generated automatically and returned in aRuleId. 
        * The new rule is created without condition, action or transformation.
        * Those elements can be created using their respective classes.
        *
        * @param TDesC& aRuleId, if provided treated as rule id. Otherwise a
        /  random rule id is created and returned.
        * @return Error code, KErrAlreadyExists, KErrNone, KErrNotFound if
        *  document root doesnt found.
        */
        IMPORT_C TInt CreateNewRuleL(TDes& aRuleId);
        
        /**
        * Finds if a Rule Exist
        *
        * @param TDesC& aRuleId rule ID
        * @return TBool if rule exist
        */
        IMPORT_C TBool IsRuleExist(const TDesC& aRuleId);
        
        /**
        * Delete all data by deleting all rules and their children
        */
        IMPORT_C void DeleteAllRules();
        
        /**
        * Deletes all rules which are empty
        */
        IMPORT_C void DeleteEmptyRulesL();
        

        /**
        * From CActive
        *
        */
        void DoCancel();
        
        /**
        * From CActive
        *
        */
        void RunL();
        
        /**
        * From CActive
        *
        */
        TInt RunError(TInt aError);
        
        /**
        * Destructor
        *
        */
        ~CPresenceXDM();
        
        /**
        * Friend classes
        *
        */
        friend class TPresenceActionXDM;
        friend class TPresenceTransformXDM;
        friend class TPresCondIdentityOne;
        friend class TPresCondIdentityMany;
        friend class TPresCondSphere;
        friend class TPresCondValidity;
        friend class TPresCondMisc;
        

        
    protected:
    
        /**
        * Removes given node from document model
        *
        * @param CXdmDocumentNode* aNode
        */
        void RemoveFromModelL( CXdmDocumentNode* aNode );
        
       
    private:

        /**
        * Second phase constructor
        *
        * @param TInt XDMSettingsList ID
        */
        void ConstructL(const TInt aSettingId );
        
        /**
        * C++ constructor
        *
        * @param none
        * @return none
        */
        CPresenceXDM();
        
        /**
        * Creates the document root if needed
        */
        void CreateRootIfNeededL();
        
        /**
        * Finds if a Rule Exist, returns its pointer
        *
        * @param TDesC& aRuleId rule ID
        * @return CXdmDocumentNode if rule exist
        */
        CXdmDocumentNode* IsRuleExistInt(const TDesC& aRuleId);
        
        /**
        * Gets the condition, action or transform node. It is
        * caller's responsiblity to provide a valid child name.
        *
        * @param TDesC& aRuleId rule ID.
        * @param aRuleChild child node of a rule
        * @param TBool aCreate if ETrue, creates child node if not exist
        * @return CXdmDocumentNode* pointer to rule child node or NULL
        */
        CXdmDocumentNode* GetRuleChildNodeL(const TDesC& aRuleId, 
                                const TDesC& aRuleChild, TBool aCreate);

        /**
        * Gets the conditions child node pointer in a rule. It is
        * caller's responsiblity to provide a valid child name.
        *
        * @param TDesC& aRuleId rule ID.
        * @param aCondChild child node of condition
        * @param TBool aCreate if ETrue, creates child node if not exist
        * @return CXdmDocumentNode* pointer to condition child node or NULL
        */
        CXdmDocumentNode* GetConditionChildNodeL(const TDesC& aRuleId,
                                const TDesC& aCondChild, TBool aCreate);
        
        /**
        * Gets a unique rule id
        *
        * @param TDesC& aRuleId unique rule id
        */
        void GetUniqueRuleId(TDes& aRuleId);
        
    private: // data
        
        CXdmProtocolInfo*       iXDMProtocolInfo;
        CXdmEngine*             iXDMEngine;
        CXdmDocument*           iPresDocument;
        TInt                    iAsyncReq;
        MRLSPresXDMAsyncHandler* iAsyncHandler;

    };
    
#endif //__PRESENCEXDM_H__
        
        
