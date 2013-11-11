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
* Description:  Maintains a list of process ids and matching origin*
*/


#ifndef __PROCESSORIGINMAPPER_H__
#define __PROCESSORIGINMAPPER_H__

#include <e32cmn.h>
#include <s32file.h>

#include "mdeconstants.h"
#include "processoriginmapperinterface.h"

/**
 * @var Data transfer object used for process/origin
 * mapping table.
 */
struct TProcessOriginPair
    {
    TUid iProcessId;
    TOrigin iOrigin;
    };

/**
 * @var Maximum amount of items allowed in the mapping table.
 */
const TInt KMaxMappingSize = 100;


/**
 * Maintains a list of process ids and matching origin
 * values to be set for mde objects created by a process.
 * List is maintained in memory and in a text file.
 * 
 * API is provided to add and remove processes in the list
 * and to retrieve the matching origin by a process id (if any).
 * Mappings are read written to a file on request.
 */
class CProcessOriginMapper : public CBase,
                             public MProcessOriginMapperInterface
    {
    public:

        /**
         * Creates and constructs a new instance of CLocationContextPlugin.
         * Two-phase construction.
         *
         * @return A pointer to the created instance.
         */
        static CProcessOriginMapper* NewL();
    
        /**
         * Destructor.
         */
        virtual ~CProcessOriginMapper();

    private:

        /**
         * Private constructor, called by NewL() only.
         */
        CProcessOriginMapper();
    
        /**
         * Second phase construction called by NewL().
         */
        void ConstructL();

    public:

        /**
         * Add a process to the list.
         * @param aProcessUid  Uid of the process to add.
         * @param aOrigin      Origin to which this process will mapped to.
         */
        void RegisterProcessL( const TUid& aProcessId,
                               const TOrigin& aOrigin );
        
        /**
         * Remove a process from the list.
         * @param aProcessId  Uid of the process to remove.
         */
        void UnregisterProcessL( const TUid& aProcessId );

        /**
         * Check a process from the list.
         * @param aProcessId  Uid of the process to check.
         * @return  Origin the given process is mapped to
         *          or KErrNotFound if not found.
         */
        TOrigin OriginL( const TUid& aProcessId );

        /**
         * Read process id mappings from a file.
         * This doesn't clear the list of currently registered mappings.
         * @param aFile  File name and path.
         * @return Count of mapping pairs read.
         */
        TInt ReadFileL( const TDesC& aFile );

        /**
         * Read process id mappings from the default file.
         * @return Count of mapping pairs read.
         */
        TInt ReadFileL();

        /**
         * Write process id mappings to a file.
         * @param aFile  File name and path.
         */
        void WriteFileL( const TDesC& aFile );

        /**
         * Write process id mappings to the default file.
         */
        void WriteFileL();

        /**
         * Clear mapping table in memory.
         */
        void Clear();

        /**
         * Count of registered mappings.
         * @return Count
         */
        TInt Count();

    private:

        /**
         * Find an index by process id.
         * @param aProcessId  Process id.
         * @return  Index or KErrNotFound if not found.
         */
        TInt FindProcess( const TUid& aProcessId );
        
        /**
         * Read one process id and origin pair from file.
         * @param aProcessUid  Uid of the process to add.
         * @param aOrigin      Origin to which this process will mapped to.
         */
        void ReadProcessOriginPairL( TUid& aProcessId, TOrigin& aOrigin );

        /**
         * Write one related process/origin pair to a file.
         * @param aProcessUid  Uid of the process to add.
         * @param aOrigin      Origin to which this process will mapped to.
         */
        void WriteProcessOriginPairL( TUid& aProcessId, TOrigin& aOrigin );

        static TInt Compare(const TProcessOriginPair& aFirst, const TProcessOriginPair& aSecond);
        
    private: // data

        /**
         * @var Mapping table. It holds pairs of
         * matching process ids and origins.
         * Content is loaded from and saved to a file.
         */
        RArray<TProcessOriginPair> iProcessOriginMap;
        
        /**
         * File system session.
         */
        RFs iFs;
        
        /**
         *  File stream used for reading the import file
         */
        RFileReadStream iReadStream;

        /**
         *  File stream used for writing the export file
         */
        RFileWriteStream iWriteStream;

        /**
         * A buffer used for reading lines from files.
         */
        HBufC8* iLineBuffer;

        /**
         * Indicates read defult origin map file.
         * No write process should occur.
         */
        TBool iDefaultFileRead;
    };

#endif
