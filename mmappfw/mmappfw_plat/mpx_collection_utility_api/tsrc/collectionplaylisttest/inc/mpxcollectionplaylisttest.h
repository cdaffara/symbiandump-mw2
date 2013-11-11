/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  STIF testing for MPX Collection Playlist
*
*/


#ifndef MPXCOLLECTIONPLAYLISTTEST_H
#define MPXCOLLECTIONPLAYLISTTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

#include <mpxmedia.h>
#include <mpxcollectionplaylistobserver.h>
#include <mpxcollectionobserver.h>
#include <f32file.h>

// CONSTANTS
const TInt KErrBadTestParameter= -1000;         // Error on configuration file

// MACROS
// Logging path
#ifdef __WINSCW__
_LIT( KMPXCollectionPlaylistTestLogPath, "\\testing\\log\\" );
#else
_LIT( KMPXCollectionPlaylistTestLogPath, "e:\\testing\\log\\" );
#endif
// Log file
_LIT( KMPXCollectionPlaylistTestLogFile, "mpxcollectionplaylisttest.txt" );

// FORWARD DECLARATIONS
class CMPXCollectionPlaylistTest;
class MMPXCollectionUtility;
class CMPXCollectionPath;
class CMPXCollectionOpenUtility;//

// CLASS DECLARATION

/**
*  CMPXCollectionPlaylistTest test class for STIF Test Framework TestScripter.
*
*  @lib mpxcollectionplaylisttest.lib
*  @since s60 5.0
*/
NONSHARABLE_CLASS(CMPXCollectionPlaylistTest) : public CScriptBase,
									                              public MMPXCollectionObserver,
									                              public MMPXCollectionPlaylistObserver
    {
private:
	enum TExpectedEvents
		{
		EEventHandlePlaylistExportCompletedL,
		EEventHandlePlaylistImportCompletedL,
		EEventHandleFileAddCompletedL,
		EEventHandleDeleteCompletedL,
		EEventHandleFileImportCompletedL
		};

public:  // Constructors and destructor

	/**
	* Two-phased constructor.
	*/
	static CMPXCollectionPlaylistTest* NewL( CTestModuleIf& aTestModuleIf );

	/**
	* Destructor.
	*/
	virtual ~CMPXCollectionPlaylistTest();

public: // Functions from base classes

	/**
	* From CScriptBase Runs a script line.
	* @since s60 5.0
	* @param aItem Script line containing method name and parameters
	* @return Symbian OS error code
	*/
	virtual TInt RunMethodL( CStifItemParser& aItem );

	/**
	* From MMPXCollectionObserver.
	* @since s60 5.0
	*/
	virtual void HandleCollectionMediaL( const CMPXMedia& aMedia, TInt aError );
	virtual void HandleCollectionMessage( CMPXMessage* aMsg, TInt aError );
	virtual void HandleOpenL( const CMPXMedia& aEntries, TInt aIndex, TBool aComplete, TInt aError );
	virtual void HandleOpenL( const CMPXCollectionPlaylist& aPlaylist, TInt aError );  

	/**
	* From MMPXCollectionPlaylistObserver.
	* @since s60 5.0
	*/
	virtual void HandleCollectionPlaylistChange( TInt aErr );
	
private:

	/**
	* C++ default constructor.
	*/
	CMPXCollectionPlaylistTest( CTestModuleIf& aTestModuleIf );

	/**
	* By default Symbian 2nd phase constructor is private.
	*/
	void ConstructL();

	/**
	* Frees all resources allocated from test methods.
	* @since s60 5.0
	*/
	void Delete();

	/**
	* Add event to the expected event list
	* @since S60 5.0
	*/
	void AddExpectedEvent( TExpectedEvents aEvent );

	/**
	* Remove event from the expected event list
	* @since S60 5.0
	*/
	TBool RemoveExpectedEvent( TExpectedEvents aEvent );

    /**
     * Build a MPX Collection Path
     * @since S60 5.0
     */
    void BuildCollectionPathL();

	/**
	* Test methods are listed below.
	*/

	/**
	* Example test method.
	* @since s60 5.0
	* @param aItem Script line containing parameters.
	* @return Symbian OS error code.
	*/
	TInt ExampleL( CStifItemParser& aItem );
	//ADD NEW METHOD DEC HERE

	TInt CreateUtilityNewPlaylist( CStifItemParser& aItem );
    TInt CreateUtilityPlaylistExist( CStifItemParser& aItem );
    TInt CreateUtilityCollectionPath( CStifItemParser& aItem );
    TInt CreateUtilityCollectionStream( CStifItemParser& aItem );
	TInt CopyContentL( CStifItemParser& aItem );
	TInt Next( CStifItemParser& aItem );
	TInt Previous( CStifItemParser& aItem );
	TInt SetToFirst( CStifItemParser& aItem );
	TInt SetToLast( CStifItemParser& aItem );
	TInt SetToIndex( CStifItemParser& aItem );
	TInt Count( CStifItemParser& aItem );
	TInt Levels( CStifItemParser& aItem );
	TInt Index( CStifItemParser& aItem );
	TInt Remote( CStifItemParser& aItem );
	TInt RepeatMode( CStifItemParser& aItem );
	TInt Shuffle( CStifItemParser& aItem );
	TInt Path( CStifItemParser& aItem );
	TInt MediaL( CStifItemParser& aItem );
	TInt SetRepeatMode( CStifItemParser& aItem );
	TInt SetRepeatEnabled( CStifItemParser& aItem );
	TInt SetShuffleL( CStifItemParser& aItem );
	TInt SetShuffleEnabledL( CStifItemParser& aItem );
	TInt StreamingL( CStifItemParser& aItem );
	TInt SetObserver( CStifItemParser& aItem );
	TInt EmbeddedPlaylist( CStifItemParser& aItem );
	TInt SetEmbeddedPlaylist( CStifItemParser& aItem );
	TInt SetL( CStifItemParser& aItem );
	TInt CancelRequest( CStifItemParser& aItem );
	TInt Invalidate( CStifItemParser& aItem );	
	//add function
	TInt NextIndex( CStifItemParser& aItem );
	TInt PathIndex( CStifItemParser& aItem );
	TInt CMPXCollectionPlaylistDestructor( CStifItemParser& aItem );
	
	TInt CMPXCollectionOpenUtilityCreate(CStifItemParser& aItem);
	TInt CMPXCollectionOpenUtilityStartL(CStifItemParser& aItem);
    TInt CMPXCollectionOpenUtilityPathL( CStifItemParser& aItem );  
	TInt CMPXCollectionOpenUtilityStop( CStifItemParser& aItem );
	TInt CMPXCollectionOpenUtilitySetDelay( CStifItemParser& aItem);
	TInt CMPXCollectionOpenUtilityStartUsingPath( CStifItemParser& aItem );
	TInt CMPXCollectionOpenUtilitySetDirection(CStifItemParser& aItem );
	
	TInt NewLMMPXCollectionUtility( CStifItemParser& aItem ); 

	
private:    // Data
	CMPXCollectionPlaylist* iCollectionPlaylist;
    CMPXCollectionPath*     iCollectionPath;
    CMPXCollectionOpenUtility* iCollectionOpenUtility ;
	TInt iResult;
	// List of expected events
    RArray<TExpectedEvents> iExpectedEvents;
    };

#endif      // MPXcollectionplaylistTEST_H
