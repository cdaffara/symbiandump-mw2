/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface for mds item origin mappers.*
*/

#ifndef PROCESSORIGINMAPPERINTERFACE_H
#define PROCESSORIGINMAPPERINTERFACE_H

/**
 * Interface for component that maps mds origin values
 * based on process id that created the file.
 */
class MProcessOriginMapperInterface
    {
    public:
    
        /**
         * Add a process to the list.
         * @param aProcessUid  Uid of the process to add.
         * @param aOrigin      Origin to which this process will mapped to.
         */
        virtual void RegisterProcessL( const TUid& aProcessId,
                               const TOrigin& aOrigin ) = 0;
        
        /**
         * Remove a process from the list.
         * @param aProcessId  Uid of the process to remove.
         */
        virtual void UnregisterProcessL( const TUid& aProcessId ) = 0;
    };

#endif // PROCESSORIGINMAPPERINTERFACE_H
