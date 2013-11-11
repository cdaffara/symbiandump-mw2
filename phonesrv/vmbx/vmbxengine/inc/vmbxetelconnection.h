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
* Description:  Encapsulates RTelServer and RMobilePhone sessions
*  Interface   : Private, CVmbxETelConnection
*
*/


#ifndef C_VMBXETELCONNECTION_H
#define C_VMBXETELCONNECTION_H

//  INCLUDES
#include <e32base.h>
#include <voicemailboxdefs.h>
#include <etelmm.h>

// FORWARD DECLARATION
class RTelServer;
class RMobilePhone;

// CLASS DECLARATION

/**
*  Creates connection to RTelServer and opens RMobilePhone
*
*/
NONSHARABLE_CLASS( CVmbxETelConnection ): public CBase
    {

public:  // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static CVmbxETelConnection* NewL();

    /**
     * Destructor.
     */
    virtual ~CVmbxETelConnection();


public: // New functions

    /**
     * Returns a reference to the ETel RTelServer
     *
     * @return a reference to the ETel RTelServer
     */
    RTelServer& TelServer();

    /**
     * Returns a reference to the ETel RMobilePhone
     *
     * @return a reference to the ETel RMobilePhone
     */
    RMobilePhone& Phone();

private:

    /**
     * C++ constructor.
     */
    CVmbxETelConnection();

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

private: // data

    /**
     * The ETel RTelServer
     */
    RTelServer iTelServer;

    /**
     * The ETel phone
     */
    RMobilePhone iPhone;
    };

#endif  // C_VMBXETELCONNECTION_H
