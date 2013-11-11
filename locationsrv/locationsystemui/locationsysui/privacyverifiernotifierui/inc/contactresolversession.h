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
 * Description:  RContactResolverSession
 *  This class provides the client-side interface to the server session
 */
#ifndef CONTACTRESOLVERSESSION_H
#define CONTACTRESOLVERSESSION_H

#include <e32std.h>

class CPosRequestor;

class RContactResolverSession : public RSessionBase
    {
public:
    // Constructors and destructors

    /**
     * Default constructor.
     * Constructs the object.
     */
    IMPORT_C RContactResolverSession();

public:
    // New functions

    /**
     * Connect.
     * Connects to the server and create a session.
     * @return Error code.
     */
    IMPORT_C TInt Connect();

    /**
     * Resolve the requestors. 
     * @param[IN,OUT] aRequestors Array of Requestors
     */
    IMPORT_C void ResolveRequestorsL(
            RPointerArray<CPosRequestor>& aRequestors);

    /**
     * closes the session.
     */
    IMPORT_C void Close();

    /**
     * Starts the server process if not already started.
     */
    static TInt StartServer();
    
    /**
     * creates the server process.
     */
    static TInt CreateServerProcess();

private:
    /**
     * Version.
     * Gets the version number.
     * @return The version.
     */
    TVersion Version() const;

    };

#endif //CONTACTRESOLVERSESSION_H
