/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Declaration of subsession registry class.
*
*/
#ifndef POSINDICATORSUBSESSIONREGISTRY_H
#define POSINDICATORSUBSESSIONREGISTRY_H

#include <e32base.h>

// FORWARD DECLARATIONS
class CObjectCon;
class CObjectConIx;
class CPosIndicatorServerSubsession;

class CPosIndicatorSubSessionRegistry : public CBase
    {
public:
    /**
     * Instantiates a new object of 
     * CPosIndicatorSubSessionRegistry
     *
     * @return pointer to an instance of 
     * CPosIndicatorSubSessionRegistry
     */
    static CPosIndicatorSubSessionRegistry* NewL();
    
    /**
     * C++ destructor
     *
     */
    ~CPosIndicatorSubSessionRegistry();
    
    /**
     * Adds an object instance to the registry and
     * takes ownership of the object
     *
     * @param aSubSession an sub-session object instance
     * @return a registry handle to the object instance.
     */
    TInt AddSubSessionL( CPosIndicatorServerSubsession* aSubSession );
    
    /**
     * Removes the subsession from the registry and 
     * also closes the sub-session handle
     *
     * @param aHandle a handle to the subsession
     * @return KErrNone if handle close successfully, 
     * ELbtServerBadHandle if handle not found
     *
     */
    TInt CloseSubSession( TUint aHandle );
    
    /**
     * Retrieves the appropriate subsession handle
     * with the specified index
     *
     * @param aHandle index to the subsession handle
     * @return pointer the subsession 
     *
     */
    CPosIndicatorServerSubsession* SubSessionFromHandle(
                                        TUint aHandle );
    
private:
    /**
     * Default C++ Constructor
     *
     */
    CPosIndicatorSubSessionRegistry();
    
    /**
     * 2nd phase constructor for instantiating 
     * member variables
     *
     */
    void ConstructL();

private:// Data
    /**
     * CObject to create the object container
     * Own
     */ 
    CObjectIx*      iRegistryIndex;
    
    /**
     *  Object index container
     *  Own
     */
    CObjectCon*     iRegistryContainer;
    
    /**
     * Object index
     * Own
     */ 
    CObjectConIx*   iRegistryContainerIndex;        
    };

#endif /* POSINDICATORSUBSESSIONREGISTRY_H */
