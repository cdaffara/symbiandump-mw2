/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Class to handle registration and unregistration of subsessions.
*
*
*/


#ifndef CPOSLMSUBSESSIONREGISTRY_H
#define CPOSLMSUBSESSIONREGISTRY_H

// INCLUDE FILES
#include <e32base.h>

// FORWARD DECLARATIONS
class CPosLmSubsessionBase;

/**
* Class to handle registration and unregistration of subsessions.
*/
class CPosLmSubsessionRegistry : public CBase
    {

    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @return A new instance of this class.
        */
        static CPosLmSubsessionRegistry* NewL();

        /**
        * Destructor.
        */
        ~CPosLmSubsessionRegistry();

    public: // New functions

        /**
        * Returns a subsession corresponding to a handle.
        *
        * @param aHandle The subsession handle.
        * @return A subsession.
        */
        CPosLmSubsessionBase* SubsessionFromHandleL(
        /* IN */        TUint aHandle
        );

        /**
        * Returns a subsession corresponding to an index.
        *
        * Range: [0, Count()[
        *
        * Note: this function returns NULL if a subsession has been
        * removed at the position of the supplied index.
        *
        * To iterate all available subsessions, use:
        * for (TInt i = 0; i < registry->Count(); i++)
        *     {
        *     CPosLmSubsessionBase* subs = registry->SubsessionFromIndex(i);
        *     if (subs)
        *         {
        *         // Do something with the subsession
        *         }
        *     }
        *
        * @param aIndex An index within the registry.
        * @return A subsession or NULL.
        */
        CPosLmSubsessionBase* SubsessionFromIndex(
        /* IN */        TInt aIndex
        );

        /**
        * Returns the number of allocated spots in the memory for subsessions.
        *
        * Note: this is not the same as the total number of subsessions! Please
        *  refer to @ref SubsessionFromIndex.
        *
        * @return The number of subsessions in the registry.
        */
        TInt Count();

        /**
        * Closes the subsession with the specified handle.
        *
        * @param aHandle A subsession handle.
        * @return KErrNone on success. KErrNotFound otherwise.
        */
        TInt CloseSubsession(
        /* IN */        TUint aHandle
        );

        /**
        * Adds a subsession to the instance. The subsession ownership is
        * transferred to the registry.
        *
        * @param aSubsession A subsession.
        * @return A registry handle to the subsession.
        */
        TInt AddInstanceL(
        /* IN */        CPosLmSubsessionBase* aSubsession
        );

    private:

        /**
        * C++ default constructor.
        */
        CPosLmSubsessionRegistry();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        CObjectIx*      iRegistryIndex;
        CObjectCon*     iRegistryContainer;
        CObjectConIx*   iRegistryContainerIndex;
    };


#endif //CPOSLMSUBSESSIONREGISTRY_H

// End of File
