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
* Description:    RLS and Presence XDM, This class represents sphere part of 
*  Presence Condition
*
*/



#ifndef __PRESCONDSPHERE_H__
#define __PRESCONDSPHERE_H__

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
*  TPresCondSphere is used to manipulate the sphere part of a Condition in a rule. 
*  Remember to call Init function after constructing this object.
*  
*  @lib rlspresxdm.lib
*  @since S60 3.2
*/
class TPresCondSphere
    {
    public:
    
        /**
        * C++ Constructor, After contructing Init must be called before using
        * this object.
        */                                        
        IMPORT_C TPresCondSphere();
        
        /**
        * Initializes the object with the given pointer to CPresenceXDM.
        * This function must be called before using other functions.
        *
        * @param CPresenceXDM* pointer to created CPresenceXDM
        */
        IMPORT_C void Init(CPresenceXDM* const aPresXDM);
        
        /**
        * Adds a condition sphere to a rule
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aSphere Value of the sphere to be added
        * @return TInt KErrNone, KErrNotFound if rule doesnt found
        */
        IMPORT_C TInt AddOrReplaceSphereL(const TDesC& aRuleID, 
                                                        const TDesC& aSphere);
                                                        
                                                        
        /**
        * Gets a condition sphere from a rule
        *
        * @param TDesC& aRuleID rule ID
        * @param TDesC& aSphere returned value of the sphere
        * @return TInt KErrNone, KErrNotFound if rule or no sphere found
        */
        IMPORT_C TInt GetSphereL(const TDesC& aRuleID, TPtrC& aSphere);
                                                        
                                                        
        /**
        * Deletes condition sphere from a rule
        *
        * @param TDesC& aRuleID rule ID
        * @return TInt KErrNone, KErrNotFound if rule or sphere doesnt found
        */
        IMPORT_C TInt DeleteSphereL(const TDesC& aRuleID);

                                       
    private:
    
        /**
        * Gets the Sphere node pointer in a rule
        *
        * @param TDesC& aRuleID rule ID.
        * @param TBool aCreate if ETrue, creates Sphere node if not exist
        * @return CXdmDocumentNode* pointer to Sphere node or NULL
        */
        CXdmDocumentNode* GetSphereNodeL(const TDesC& aRuleID,
                                                        TBool aCreate);
                                                            
    private: // data
        
        // Pointer to CPresenceXDM - not owned
        CPresenceXDM*           iPresXDM;      // not owned
        
    };
    
   
#endif //__PRESCONDSPHERE_H__
        
