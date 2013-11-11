/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  mpxplaybackutilitytest header for STIF Test Framework TestScripter.
*
*/



#ifndef MPXPLAYBACKUTILITYTEST_H
#define MPXPLAYBACKUTILITYTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <mpxplaybackutility.h>
#include <mpxplaybackobserver.h>

// MACROS
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
#ifdef __WINSCW__
_LIT( KmpxplaybackutilitytestLogPath, "C:\\logs\\testframework\\mpxplaybackutilitytest\\" ); 
// Log file
_LIT( KmpxplaybackutilitytestLogFile, "mpxplaybackutilitytest.txt" ); 
_LIT( KmpxplaybackutilitytestLogFileWithTitle, "mpxplaybackutilitytest_[%S].txt" );
// data file
_LIT( KmpxplaybackutilityTestFilePath, "C:\\" );
#else
_LIT( KmpxplaybackutilitytestLogPath, "c:\\logs\\testframework\\mpxplaybackutilitytest\\" ); 
// Log file
_LIT( KmpxplaybackutilitytestLogFile, "mpxplaybackutilitytest.txt" ); 
_LIT( KmpxplaybackutilitytestLogFileWithTitle, "mpxplaybackutilitytest_[%S].txt" );
// data file
_LIT( KmpxplaybackutilityTestFilePath, "c:\\" );
#endif

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// DATA TYPES
typedef struct
{
    TInt               iEvent;
    TInt               iType;
    TInt               iData;
} TMpxPlaybackTestEvent;

typedef CArrayPtrFlat<TMpxPlaybackTestEvent> CMpxPlaybackTestEventArray;

// CLASS DECLARATION

/**
*  Cmpxplaybackutilitytest test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Cmpxplaybackutilitytest) : public CScriptBase,
                                             public MMPXPlaybackObserver,
                                             public MMPXPlaybackCallback
                                            
    {
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static Cmpxplaybackutilitytest* NewL( CTestModuleIf& aTestModuleIf );

    /**
    * Destructor.
    */
    virtual ~Cmpxplaybackutilitytest();

public: // Functions from base classes

    /**
    * From CScriptBase Runs a script line.
    * @since ?Series60_version
    * @param aItem Script line containing method name and parameters
    * @return Symbian OS error code
    */
    virtual TInt RunMethodL( CStifItemParser& aItem );
  
    // From base class MMPXPlaybackObserver
    /**
     * Handle playback message
     *
     * @since 3.1
     * @param aMessage playback message
     * @param aErr system error code.
     */
    void HandlePlaybackMessage( CMPXMessage* aMessage, TInt aError );
    
    /**
    *  Handle playback property.
    *
    *  @since S60 3.2.3
    *  @param aProperty the property
    *  @param aValue the value of the property
    *  @param aError error code
    */
    void HandlePropertyL(TMPXPlaybackProperty aProperty, TInt aValue, TInt aError);

    /**
    *  Method is called continously until aComplete=ETrue, signifying that 
    *  it is done and there will be no more callbacks.
    *  Only new items are passed each time.
    *
    *  @since S60 3.2.3
    *  @param aPlayer UID of the subplayer
    *  @param aSubPlayers a list of sub players
    *  @param aComplete ETrue no more sub players. EFalse more subplayer
    *                   expected
    *  @param aError error code
    */
    void HandleSubPlayerNamesL(TUid aPlayer, 
                               const MDesCArray* aSubPlayers,
                               TBool aComplete,
                               TInt aError);
    
    /**
    *  Call back of media request.
    *
    *  @since S60 3.2.3
    *  @param aMedia media 
    *  @param aError error code    
    */
    void HandleMediaL(const CMPXMedia& aProperties, TInt aError);
    
    /**
     *  Handle completion of a asynchronous command.
     *  Note: All clients should implement this callback.
     *
     *  @since S60 3.2.3
     *  @param aCommandResult result of the command, NULL if error
     *  @param aError error code    
     */
    void HandlePlaybackCommandComplete(CMPXCommand* aCommandResult, TInt aError);

private: // functions
    
    /**
    * C++ default constructor.
    */
    Cmpxplaybackutilitytest( CTestModuleIf& aTestModuleIf );
    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();
    // Prohibit copy constructor if not deriving from CBase.
    // ?classname( const ?classname& );
    // Prohibit assigment operator if not deriving from CBase.
    // ?classname& operator=( const ?classname& );
    /**
    * Frees all resources allocated from test methods.
    * @since ?Series60_version
    */
    void Delete();
    
    /*
     * Create resources 
     */
    void CreateL();
    
    void AddExpectedEventL(TInt aEvent, TInt aType, TInt aData);
    void RemoveExpectedEventL(TInt aEvent, TInt aType, TInt aData);
    
    /**
    * Test methods are listed below. 
    */
    TInt MMPXPlaybackUtilityNewL(CStifItemParser& /*aItem*/);
    TInt MMPXPlaybackUtilityNewWithCatL(CStifItemParser& /*aItem*/);
    TInt MMPXPlaybackUtilityUtilityL(CStifItemParser& /*aItem*/);
    TInt MMPXPlaybackUtilityUtilityWithCatL(CStifItemParser& /*aItem*/);
    TInt MMPXPlaybackUtilityInitWithPlaylistL(CStifItemParser& /*aItem*/);
    TInt MMPXPlaybackUtilityInitWithUriL(CStifItemParser& /*aItem*/);
    TInt MMPXPlaybackUtilityInitWithRFileL(CStifItemParser& /*aItem*/);
    TInt MMPXPlaybackUtilityInitStreamingWithUriL(CStifItemParser& /*aItem*/);
    TInt MMPXPlaybackUtilityInitStreamingWithRFileL(CStifItemParser& /*aItem*/);
    TInt MMPXPlaybackUtilityInit64L(CStifItemParser& /*aItem*/);
    TInt MMPXPlaybackUtilityInitStreaming64L(CStifItemParser& /*aItem*/);
    TInt MMPXPlaybackUtilityFile64L(CStifItemParser& /*aItem*/);
    TInt MMPXPlaybackUtilityAddObserverL(CStifItemParser& /*aItem*/);
    TInt MMPXPlaybackUtilityRemoveObserverL(CStifItemParser& /*aItem*/);
    TInt MMPXPlaybackUtilityPlayerManagerSelectPlayerL(CStifItemParser& /*aItem*/);
    
    TInt MMPXPlaybackUtilityCommandPlayL ( CStifItemParser & /*aItem*/ );
    TInt MMPXPlaybackUtilityCommandStopL ( CStifItemParser & /*aItem*/ );
    TInt MMPXPlaybackUtilityCommandPauseL ( CStifItemParser & /*aItem*/);
    TInt MMPXPlaybackUtilityCommandPlayPauseL ( CStifItemParser & /*Item*/);
    TInt MMPXPlaybackUtilityCommandPlayCompleteL ( CStifItemParser & /*Item*/);
    TInt MMPXPlaybackUtilityCommandSeekForwardL ( CStifItemParser & /*Item*/);
    TInt MMPXPlaybackUtilityCommandSeekBackwardL ( CStifItemParser & /*Item*/);
    TInt MMPXPlaybackUtilityCommandCloseL ( CStifItemParser & /*Item*/);
    TInt MMPXPlaybackUtilitySetL(CStifItemParser& /*aItem*/ );
    
    TInt EndTest(CStifItemParser& /*aItem*/);
   
    /**
     * Method used to log version of test class
     */
    void SendTestClassVersion();
    //ADD NEW METHOD DEC HERE
    //[TestMethods] - Do not remove 

private: // data
    MMPXPlaybackUtility* iMPXPlaybackUtility;
    CMpxPlaybackTestEventArray* iExpectedEventArray;
    RFs iFs;
    TInt iCallbackError;
    RFile iFile;
    TFileName iFileName;

#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    RFile64 iFile64;
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    };

#endif      // MPXPLAYBACKUTILITYTEST_H

// End of File
