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
* Description:  STIF testing for MPX Harvester
*
*/


#ifndef MPXHARVESTERTEST_H
#define MPXHARVESTERTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

#include <mpxmedia.h>
#include <mpxharvesterutilityobserver.h>
#include <mpxcollectionobserver.h>
#include <f32file.h>

// CONSTANTS
const TInt KErrBadTestParameter= -1000;         // Error on configuration file

// MACROS
// Logging path
#ifdef __WINSCW__
_LIT( KmpxharvestertestLogPath, "\\testing\\log\\" );
_LIT( Kmpxharvester_testPath, "\\testing\\data\\" );
_LIT( Kmpxharvester_soundsPath, "c:\\data\\sounds\\" );
_LIT( Kmpxharvester_digitalPath, "c:\\data\\sounds\\digital\\" );
#else
_LIT( KmpxharvestertestLogPath, "e:\\testing\\log\\" );
_LIT( Kmpxharvester_testPath, "e:\\testing\\data\\" );
_LIT( Kmpxharvester_soundsPath, "e:\\sounds\\" );
_LIT( Kmpxharvester_digitalPath, "e:\\sounds\\digital\\" );
#endif
// Log file
_LIT( KmpxharvestertestLogFile, "mpxharvestertest.txt" );

_LIT( KTagTestfile1, "mpxtest1.mp3" );
_LIT( KTagTestfile2, "mpxtest2.mp3" );
_LIT( KTagAll, "all" );
_LIT( KTagSync, "sync" );
_LIT( KTagAsync, "async" );
_LIT( KTagTrue, "true" );
_LIT( KTagFalse, "false" );
_LIT( KTagM3U, "M3U" );
_LIT( KTagPLA, "PLA" );
_LIT( KTagMPV, "MPV" );
_LIT( KTagXSPF, "XSPF" );
_LIT( KTagXML, "XML" );

// FORWARD DECLARATIONS
class Cmpxharvestertest;
class MMPXHarvesterUtility;
class MMPXCollectionUtility;

// CLASS DECLARATION

/**
*  Cmpxharvestertest test class for STIF Test Framework TestScripter.
*
*  @lib mpxharvestertest.lib
*  @since s60 5.0
*/
NONSHARABLE_CLASS(Cmpxharvestertest) : public CScriptBase,
									   public MMPXHarvesterUtilityObserver,
									   public MMPXCollectionObserver
    {
private:
	enum TExpectedEvents
		{
		EEventHandlePlaylistExportCompletedL,
		EEventHandlePlaylistImportCompletedL,
		EEventHandleFileAddCompletedL,
		EEventHandleDeleteCompletedL,
		EEventHandleFileImportCompletedL,
		EEventHandleFileGetMediaCompletedL
		};

public:  // Constructors and destructor

	/**
	* Two-phased constructor.
	*/
	static Cmpxharvestertest* NewL( CTestModuleIf& aTestModuleIf );

	/**
	* Destructor.
	*/
	virtual ~Cmpxharvestertest();

public: // Functions from base classes

	/**
	* From CScriptBase Runs a script line.
	* @since s60 5.0
	* @param aItem Script line containing method name and parameters
	* @return Symbian OS error code
	*/
	virtual TInt RunMethodL( CStifItemParser& aItem );

	/**
	* From MMPXHarvesterUtilityObserver.
	* @since s60 5.0
	*/
	virtual void HandlePlaylistExportCompletedL( CMPXMedia* aMedia, TInt aErr );
	virtual void HandlePlaylistImportCompletedL( CMPXMedia* aMedia, TInt aErr );
	virtual void HandleFileAddCompletedL( CMPXMedia* aMedia, TInt aErr );
	virtual void HandleDeleteCompletedL( TInt aErr );
	virtual void HandleFileImportCompletedL( CMPXMedia* aMedia, TInt aErr );
	virtual void HandleFileGetMediaCompletedL( CMPXMedia* aMedia, TInt aErr );

	/**
	* From MMPXCollectionObserver.
	* @since s60 5.0
	*/
	virtual void HandleCollectionMediaL( const CMPXMedia& aMedia, TInt aError );
	virtual void HandleCollectionMessage( CMPXMessage* aMsg, TInt aError );
	virtual void HandleOpenL( const CMPXMedia& aEntries, TInt aIndex, TBool aComplete, TInt aError );
	virtual void HandleOpenL( const CMPXCollectionPlaylist& aPlaylist, TInt aError );  

private:

	/**
	* C++ default constructor.
	*/
	Cmpxharvestertest( CTestModuleIf& aTestModuleIf );

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

	TInt CreateUtility( CStifItemParser& aItem );
	TInt CopyTestFile( CStifItemParser& aItem );
	TInt DeleteTestFile( CStifItemParser& aItem );
	TInt VerifyEvents( CStifItemParser& aItem );
	TInt Scan( CStifItemParser& aItem );
	TInt CancelScan( CStifItemParser& aItem );
	TInt AddFileByPath( CStifItemParser& aItem );
//	TInt AddFileByMedia( CStifItemParser& aItem );
	TInt RemoveFile( CStifItemParser& aItem );
	TInt RemoveFiles( CStifItemParser& aItem );
	TInt RecreateDatabases( CStifItemParser& aItem );
	TInt DeleteFile( CStifItemParser& aItem );
	TInt DeleteFiles( CStifItemParser& aItem );
//	TInt UpdateFile( CStifItemParser& aItem );
	TInt FindCollectionId( CStifItemParser& aItem );
//	TInt RenameFileByMedia( CStifItemParser& aItem );
	TInt RenameFileByPath( CStifItemParser& aItem );
//	TInt ExportPlaylist( CStifItemParser& aItem );
	TInt ImportPlaylist( CStifItemParser& aItem );
	TInt ImportFile( CStifItemParser& aItem );
	TInt RequiredAttributes( CStifItemParser& aItem );
	TInt OptionalAttributes( CStifItemParser& aItem );
	TInt PlaylistFileExtension( CStifItemParser& aItem );
	TInt IsPlaylist( CStifItemParser& aItem );
	TInt CheckForSystemEvents( CStifItemParser& aItem );

private:    // Data
	MMPXHarvesterUtility* iHarvester;
	MMPXCollectionUtility* iCollection;
	RFs iFs;
	CFileMan* iFileMan;
	TInt iResult;
	// List of expected events
    RArray<TExpectedEvents> iExpectedEvents;
    };

#endif      // MPXHARVESTERTEST_H
