/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Factory class for different mailbox engines
*  Interface   : Private, CVmbxEngineFactory
*
*/

#ifndef C_VMBXENGINEFACTORY_H
#define C_VMBXENGINEFACTORY_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CVmbxEngineBase;


// CLASS DECLARATION

/**
*  CVmbxEngineFactory declaration
*
*  @lib vmbxengine.lib
*/
NONSHARABLE_CLASS( CVmbxEngineFactory ) : public CBase
    {

public:  // Constructors and destructor

    /**
     * Two-phased constructor
     * @param aProvider Reference to MVmbxResourceProvider
     */
    static CVmbxEngineFactory* NewL( MVmbxResourceProvider& aProvider );

    /**
     * Destructor.
     */
    virtual ~CVmbxEngineFactory();

public:  // New functions

    /**
     * Creates instance of the mailbox entry
     *
     * @param aEngine ownership is transferred
     * @param aType mailbox type
     */
    void CreateEngineL( CVmbxEngineBase*& aEngine, TVmbxType aType );

private:

    /**
     * C++ default constructor.
     * @param aProvider Reference to MVmbxResourceProvider
     */
    CVmbxEngineFactory( MVmbxResourceProvider& aProvider );

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

private: // data

    /**
     * Reference to MVmbxResourceProvider
     */
    MVmbxResourceProvider& iProvider;
    };

#endif  // C_VMBXENGINEFACTORY_H
