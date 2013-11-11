/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This class is for Server logging usage*
*/

#ifndef __MDSLOGGER_H__
#define __MDSLOGGER_H__

#include <e32std.h>
#include <e32base.h>
#include <s32file.h>
#include <charconv.h>
#include <convgeneratedcpp.h>

#include "flogger.h"
#include "harvesterlog.h"

class CMdsClauseBuffer;
class RRowData;
class TColumn;

/**
* Logging categories.
* If adding new categories, remember to add:
* - a new activation flag to MMP file
* - activation flag recognition code to logger.cpp
*/
enum TLogCategory
    {
    ELogAlways      = 0x0001, // logged always (if feature is on)
    ELogQuery       = 0x0002, // DB queries
    ELogDb          = 0x0004, // DB interface activity
    ELogMutex       = 0x0008, // mutex
    ELogServer      = 0x0010, // client/server commands
    };

#ifdef LOG_MASTER_FLAG
#ifdef _DEBUG

const TChar KNewLine = '\n';
const TInt KLineLength = 80;
const TInt KLineBuffer = 512;
const TUint32 KLogfileMaxLength = 10240;

_LIT(KDirectory, "Metadata");
_LIT(KFilename, "server.txt");
_LIT(KAltFilename, "server2.txt");

#ifdef LOG_QUERY
#define MDE_FILE_LOGGING
#endif

// CLASS DECLARATION
/**
* CMdSLogger.
* Class created log for server.
*/
class CMdSLogger : public CBase
    {
    public: // Constructors and destructor

	    /**
	    * Constructs a new logger implementation.
	    *
	    * @return  metadata logger implementation
	    */
        static CMdSLogger* NewInstanceL();

        virtual ~CMdSLogger();

        /**
	    * Writes text to logfile, active version
	    */
        void LogLit( const TDesC8& aText );

        /**
	    * Writes text to logfile, active version
	    */
        void LogLit( const TDesC16& aText );

        /**
        * returns a reference to the file logger resource
        */
        inline RFileLogger& Log();

        /**
        * tests if a logging category is active
        */
        inline TBool IsActive( TLogCategory aCategory );

        /**
        * activates a certain logging category
        */
        inline void Activate( TLogCategory aCategory );

        /**
        * deactivates a certain logging category
        */
        inline void Deactivate( TLogCategory aCategory );

        /**
        * Checks if size exceeds maximum limit
        * @param aLines how many lines has been added since last update
        */
        void CheckSize( TInt aLines );

		// used for logging
		CMdsClauseBuffer* DescribeL( const CMdsClauseBuffer& aBuffer, const RRowData& aRowData );
	
		CMdsClauseBuffer* DescribeFullL( const TDesC& aBuffer, const RRowData& aRowData );
	
		void LogVariableL( CMdsClauseBuffer& aBuf, const TColumn& aColumn );

    private: // Private constructors

        /**
        * CMdSLogger.
        * C++ default constructor.
        */
        CMdSLogger();

        /**
        * ConstructL.
        * 2nd phase constructor.
        */
        void ConstructL();

    private:

        /** the file logger resource */
        RFileLogger iLog;

        /** counter for lines */
        TUint32 iLineCounter;

        /**
        * currently active logging categories (masked)
        */
        TUint32 iActiveCategories;
        
        TBool iAltFileFlag;
        
        TBool iValid;
    };

#include "mdslogger.inl"
    
    #define __DEFINE_LOGGER             class CMdSLogger* gLogger;
    #define __USES_LOGGER               extern CMdSLogger* gLogger;
    #define __INIT_LOGGER               gLogger = CMdSLogger::NewInstanceL();
    #define __DESTROY_LOGGER            {if(gLogger){delete gLogger; gLogger=NULL;}}
	#define __LOGLB(CATEGORY,A)         {if (gLogger->IsActive(CATEGORY)) { gLogger->LogLit( _L(A) ); }}
	#define __LOG(CATEGORY,A)           {if (gLogger->IsActive(CATEGORY)) { gLogger->LogLit( A ); }}
#ifdef MDE_FILE_LOGGING
    #define __LOG1(CATEGORY,A,B)        {if (gLogger->IsActive(CATEGORY)) { gLogger->Log().WriteFormat( _L(A), B ); gLogger->CheckSize( 1 ); }}
    #define __LOG2(CATEGORY,A,B,C)      {if (gLogger->IsActive(CATEGORY)) { gLogger->Log().WriteFormat( _L(A), B, C ); gLogger->CheckSize( 1 ); }}
    #define __LOG3(CATEGORY,A,B,C,D)    {if (gLogger->IsActive(CATEGORY)) { gLogger->Log().WriteFormat( _L(A), B, C, D ); gLogger->CheckSize( 1 ); }}
#else // MDE_FILE_LOGGING
	#define __LOG1(CATEGORY,A,B)        {if (gLogger->IsActive(CATEGORY)) { RDebug::Print( _L(A), B ); }}
	#define __LOG2(CATEGORY,A,B,C)      {if (gLogger->IsActive(CATEGORY)) { RDebug::Print( _L(A), B, C ); }}
	#define __LOG3(CATEGORY,A,B,C,D)    {if (gLogger->IsActive(CATEGORY)) { RDebug::Print( _L(A), B, C, D ); }}
#endif // MDE_FILE_LOGGING

	#ifdef LOG_QUERY
	    #define __LOGQUERY_16(INFO, BUFFER, ROWDATA) \
	    	{__LOG( ELogQuery, INFO); \
	        CMdsClauseBuffer* queryText = gLogger->DescribeFullL(BUFFER,ROWDATA); \
	        if ( queryText ) { __LOG( ELogQuery, queryText->ConstBufferL() ); \
	        delete queryText; queryText = NULL;} }
	#else //LOG_QUERY
	    #define __LOGQUERY_16(INFO, BUFFER, ROWDATA)
	#endif //LOG_QUERY

#else // _DEBUG

#define __DEFINE_LOGGER 
#define __USES_LOGGER
#define __INIT_LOGGER
#define __DESTROY_LOGGER
#define __LOGLB(CATEGORY,A)
#define __LOG(CATEGORY,A)
#define __LOG1(CATEGORY,A,B)
#define __LOG2(CATEGORY,A,B,C)
#define __LOG3(CATEGORY,A,B,C,D)
#define __LOGQUERY_16(INFO, BUFFER, ROWDATA)

#endif // _DEBUG

#else // LOG_MASTER_FLAG

    #define __DEFINE_LOGGER 
    #define __USES_LOGGER
    #define __INIT_LOGGER
    #define __DESTROY_LOGGER
    #define __LOGLB(CATEGORY,A)
    #define __LOG(CATEGORY,A)
    #define __LOG1(CATEGORY,A,B)
    #define __LOG2(CATEGORY,A,B,C)
    #define __LOG3(CATEGORY,A,B,C,D)
	#define __LOGQUERY_16(INFO, BUFFER, ROWDATA)

#endif  // LOG_MASTER_FLAG


#endif  //__MDSLOGGER_H__
