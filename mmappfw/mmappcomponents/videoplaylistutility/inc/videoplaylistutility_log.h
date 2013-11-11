/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Debug print macros
*
*/

// Version : %version: 6 %


#ifndef VIDEOPLAYLISTUTILITY_LOG_H
#define VIDEOPLAYLISTUTILITY_LOG_H


// INCLUDES
#include <e32std.h>
#include <e32svr.h>
#include <e32des16.h>
#include <e32property.h>
#include <e32def.h>


//
//  MACROS
//
#ifdef _DEBUG
    #define VIDEOPLAYLISTUTILITY_DEBUG        TVideoPlaylistUtilityLog::VideoPlaylistUtilityLog
    #define VIDEOPLAYLISTUTILITY_ENTER_EXIT   TEnterExitLog _s
#else
    #define VIDEOPLAYLISTUTILITY_DEBUG        TVideoPlaylistUtilityLog::NullLog
    #define VIDEOPLAYLISTUTILITY_ENTER_EXIT   TVideoPlaylistUtilityLog::NullLog
#endif


class TVideoPlaylistUtilityLog : public TDes16Overflow
{
    public:

        inline static void NullLog( TRefByValue<const TDesC16> /*aFmt*/, ... )
        {
        }

        inline static void VideoPlaylistUtilityLog( TRefByValue<const TDesC16> aFmt, ... )
        {
            TBuf< 512 > buffer;

            VA_LIST list;
            VA_START( list, aFmt );
            buffer.AppendFormatList( aFmt, list );
            VA_END(list);

            RDebug::Print(_L("#VideoPlaylistUtility# %S"), &buffer );
        }
};

class TEnterExitLog : public TDes16Overflow
{
    public:

        void Overflow(TDes16& /*aDes*/)
        {
            RDebug::Print(_L("%S Logging Overflow"), &iFunctionName);
        }

        TEnterExitLog( TRefByValue<const TDesC> aFunctionName,
                       TRefByValue<const TDesC> aFmt, ... )
        {
            iFunctionName = HBufC::New( TDesC(aFunctionName).Length() );

            if ( iFunctionName )
            {
                iFunctionName->Des().Copy(aFunctionName);
            }

            TBuf< 512 > buffer;

            VA_LIST list;
            VA_START( list, aFmt );
            buffer.AppendFormatList( aFmt, list, this );
            VA_END(list);

            RDebug::Print(_L("#VideoPlaylistUtility# --> %S %S"), iFunctionName, &buffer );
        }

        TEnterExitLog( TRefByValue<const TDesC> aFunctionName )
        {
            iFunctionName = HBufC::New( TDesC(aFunctionName).Length() );

            if ( iFunctionName )
            {
                iFunctionName->Des().Copy(aFunctionName);
            }

            RDebug::Print(_L("#VideoPlaylistUtility# --> %S"), iFunctionName );
        }

        ~TEnterExitLog()
        {
            RDebug::Print(_L("#VideoPlaylistUtility# <-- %S"), iFunctionName );
            delete iFunctionName;
        }

    private:
        HBufC*    iFunctionName;
};


_LIT(_KVPUErrorInfo, "#VideoPlaylistUtility# Error : error %d file %s line %d");

#define VIDEOPLAYLISTUTILITY_S(a) _S(a)

#define VIDEOPLAYLISTUTILITY_ERROR_LOG(aErr) \
    {\
        if (aErr) \
            VIDEOPLAYLISTUTILITY_DEBUG(_KVPUErrorInfo, aErr, VIDEOPLAYLISTUTILITY_S(__FILE__), __LINE__);\
    }

#define VIDEOPLAYLISTUTILITY_TRAP(_r, _s) TRAP(_r,_s);VIDEOPLAYLISTUTILITY_ERROR_LOG(_r);
#define VIDEOPLAYLISTUTILITY_TRAPD(_r, _s) TRAPD(_r,_s);VIDEOPLAYLISTUTILITY_ERROR_LOG(_r);

#endif  // VIDEOPLAYLISTUTILITY_LOG_H

// End of File
