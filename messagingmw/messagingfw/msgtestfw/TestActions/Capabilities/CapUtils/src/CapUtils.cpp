// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//


#include <f32file.h>
#if defined( __WINS__ ) || defined( __WINSCW__ )
#include <e32wins.h>
#include <emulator.h>
#include <stdlib.h>
#else
#include <f32image.h>
#define MAX_PATH 512
#endif



TBool ParseRenameIfEmulator(TInt& err, TParse& pathFrom , TParse& pathTo)
	{
	err=KErrNone;	
	
	TBuf<MAX_PATH> c;
	User::CommandLine(c);

	TLex l(c);	

	
	_LIT( LTokenRenameIfEmulator, "RenameIfEmulator" );
	TPtrC commandTokenRenameIfEmulator(l.NextToken());
	
	if (commandTokenRenameIfEmulator!=LTokenRenameIfEmulator )
		{ // Must be something else.
		err =  KErrNone;
		return EFalse;
		}

	// Get source path
	if(pathFrom.SetNoWild(l.NextToken(),0,0)!=KErrNone)
		{ // Not considered a path.
		err =  KErrArgument;
		return ETrue;
		}
	
	// Get target path
	if(pathTo.SetNoWild(l.NextToken(),0,0)!=KErrNone)
		{ // Not considered a path.		
		err =  KErrArgument;
		return ETrue;
		}

	// Managed to parse to parse the command line.
	// And get in all the parameters correctly.
	err = KErrNone;	
	
	return ETrue;
	}



TBool HasValidPath(TParse& pathTo)
	{
	
	TBuf<MAX_PATH> dName1;		
	TBuf<MAX_PATH> dName2;		
	dName1 = pathTo.FullName();
	dName2 = pathTo.NameAndExt();
	
	if (dName1 == dName2)
		{	// No directory to create. Its a server or exe at release\winscw\udeb
		return EFalse;
		}

	return ETrue;
	} 


#if defined( __WINS__ ) || defined( __WINSCW__ )

TBool CreateDirectory(RFs& Fs, TParse& pathTo) 
	{
	TInt err = KErrNone;
	TBool ret=EFalse;

	
	TBuf<MAX_PATH> dName1;		
	err = MapEmulatedFileName(dName1, pathTo.DriveAndPath());
	if(err!=KErrNone)
		return EFalse;
	
	TBuf8<MAX_PATH>  dName1Char;
	dName1Char.Copy(dName1);
				
	ret = Emulator::CreateAllDirectories( (LPCSTR) dName1Char.PtrZ() );
	if( !ret )
		{ // Problem with directory creation.
		err=KErrGeneral;
		Fs.Close();
		return EFalse;
		}
		
	return ETrue;
	}

#endif

// ETrue=> carried out the action succesfully.
// EFalse=> some sort of error.

TBool DoRenameIfEmulator(TInt& err , TParse& pathFrom , TParse& pathTo)
	{
	err=KErrNone;	

#if defined( __WINS__ ) || defined( __WINSCW__ )

	RFs Fs;
	
	err = Fs.Connect();
	if(err!=KErrNone)
		{		
		err=KErrGeneral;	
		return EFalse;
		}
		
		
	if ( HasValidPath(pathTo) )
		{		
		if ( !CreateDirectory( Fs , pathTo) )
			{ // Unable to create the directory.
			err=KErrGeneral;
			Fs.Close();
			return EFalse;
			}			
		}
		// else probably just release\winscw\udeb which is where exe's go ( for the moment ) in the emulator.
					
				
	TBuf<MAX_PATH> sName;
	TBuf<MAX_PATH> sName1 = pathFrom.FullName();
	err = MapEmulatedFileName(sName, sName1);
	if(err!=KErrNone)
		{			
		Fs.Close();
		return EFalse;
		}

	TBuf<MAX_PATH> dName;
	TBuf<MAX_PATH> dName1=pathTo.FullName();
	err = MapEmulatedFileName(dName, dName1);
	if(err!=KErrNone)
		{			
		Fs.Close();		
		return EFalse;
		}


	Emulator::DeleteFile((LPCTSTR)dName.PtrZ());
		// Always delete before we move.

	if(!Emulator::MoveFile((LPCTSTR)sName.PtrZ(),(LPCTSTR)dName.PtrZ()))
		{
				
		WIN32_FIND_DATA ff;
		if ( FindFirstFile((LPCTSTR)sName.PtrZ() , &ff) == INVALID_HANDLE_VALUE )
			{ // The file does not exists just fail the move silently.		
			err=KErrNone;
			Fs.Close();
			return EFalse;			
			}
		else
			{ // The file still exists probably locked by CW. 
			  // Only thing can do is to issue an error.
			err=KErrGeneral;
			Fs.Close();
			return EFalse;			
			}
		}
	
		
	Fs.Close();

#endif
// #else not emulator so do nothing.


	// Managed to rename the file succesfully.
	// or did nothing at all if not emulator.
	err=KErrNone;		
	return ETrue;
	}


//**************************************	


// ret == ETrue => That was the command and it was succesful. 
//       or the file did not exist and we failed silently.
//		either way we do not want to continue parsing.
// ret == EFalse => that the  

TBool RenameIfEmulator(TInt& err)
	{
	TBool ret=EFalse;
	err=KErrNone;	

	TParse pathFrom;
	TParse pathTo;

	
	ret = ParseRenameIfEmulator(err , pathFrom, pathTo);
	if ( ret )
		{
			if (err==KErrNone)
				{ // Okay have parsed good arguments.
				DoRenameIfEmulator(err , pathFrom, pathTo);			
				}
				// else some sort of error.
		}
		// else not recognised the command.
	
	return ret;
	}
	
//**************************************	
//**************************************	

TBool ParseCopyIfEmulator(TInt& err, TParse& pathFrom , TParse& pathTo)
	{
	err=KErrNone;	
	
	TBuf<MAX_PATH> c;
	User::CommandLine(c);

	TLex l(c);	

	
	_LIT( LTokenRenameIfEmulator, "CopyIfEmulator" );
	TPtrC commandTokenRenameIfEmulator(l.NextToken());
	
	if (commandTokenRenameIfEmulator!=LTokenRenameIfEmulator )
		{ // Must be something else.
		err =  KErrNone;
		return EFalse;
		}

	// Get source path
	if(pathFrom.SetNoWild(l.NextToken(),0,0)!=KErrNone)
		{ // Not considered a path.
		err =  KErrArgument;
		return ETrue;
		}
	
	// Get target path
	if(pathTo.SetNoWild(l.NextToken(),0,0)!=KErrNone)
		{ // Not considered a path.		
		err =  KErrArgument;
		return ETrue;
		}

	// Managed to parse to parse the command line.
	// And get in all the parameters correctly.
	err = KErrNone;	
	
	return ETrue;
	}


TBool DoCopyIfEmulator(TInt& err , TParse& pathFrom , TParse& pathTo)
	{
	err=KErrNone;	

#if defined( __WINS__ ) || defined( __WINSCW__ )

	RFs Fs;
	
	err = Fs.Connect();
	if(err!=KErrNone)
		return EFalse;
	
	if ( HasValidPath(pathTo) )
		{		
		if ( !CreateDirectory( Fs , pathTo) )
			{ // Unable to create the directory.
			err=KErrGeneral;
			Fs.Close();
			return EFalse;
			}			
		}
		// else probably just release\winscw\udeb which is where exe's go ( for the moment ) in the emulator.
						
		
	TBuf<MAX_PATH> sName;
	err = MapEmulatedFileName(sName, pathFrom.FullName());
	if(err!=KErrNone)
		{
		Fs.Close();
		return EFalse;
		}

	TBuf<MAX_PATH> dName;
	err = MapEmulatedFileName(dName, pathTo.FullName());
	if(err!=KErrNone)
		{			
		Fs.Close();
		return EFalse;
		}

	if(!Emulator::CopyFile((LPCTSTR)sName.PtrZ(),(LPCTSTR)dName.PtrZ(),FALSE))
		{
		err=KErrGeneral;
		Fs.Close();
		return EFalse;
		}		
		
	Fs.Close();

#endif
// #else not emulator so do nothing.


	// Managed to rename the file succesfully.
	// or did nothing at all if not emulator.
	err=KErrNone;	
	
	return ETrue;
	}


	
//**************************************	
	
TBool CopyIfEmulator(TInt& err)
	{
	TBool ret=EFalse;
	err=KErrNone;	

	TParse pathFrom;
	TParse pathTo;

	
	ret = ParseCopyIfEmulator(err , pathFrom, pathTo);
	if ( ret )
		{
			if (err==KErrNone)
				{ // Okay have parsed good arguments.
				DoCopyIfEmulator(err , pathFrom, pathTo);
				}
				// else some sort of error.
		}
		// else not recognised the command.
	
	return ret;
	}

//**************************************	

TBool ParseRenameIfExists(TInt& err, TParse& pathFrom , TParse& pathTo)
	{
	err=KErrNone;	
	
	TBuf<MAX_PATH> c;
	User::CommandLine(c);

	TLex l(c);	
	
	_LIT( LTokenRenameIfEmulator, "RenameIfExists" );
	TPtrC commandTokenRenameIfEmulator(l.NextToken());
	
	if (commandTokenRenameIfEmulator!=LTokenRenameIfEmulator )
		{ // Must be something else.
		err =  KErrNone;
		return EFalse;
		}

	// Get source path
	if(pathFrom.SetNoWild(l.NextToken(),0,0)!=KErrNone)
		{ // Not considered a path.
		err =  KErrArgument;
		return ETrue;
		}
	
	// Get target path
	if(pathTo.SetNoWild(l.NextToken(),0,0)!=KErrNone)
		{ // Not considered a path.		
		err =  KErrArgument;
		return ETrue;
		}

	// Managed to parse to parse the command line.
	// And get in all the parameters correctly.
	err = KErrNone;	
	
	return ETrue;
	}



TBool DoRenameIfExists(TInt& err , TParse& pathFrom , TParse& pathTo)
	{
	TBool ret=EFalse;
	err=KErrNone;	

	RFs Fs;
	
	err = Fs.Connect();
	if(err!=KErrNone)
		return EFalse;
	
	err = Fs.MkDirAll(pathTo.FullName());
	
	if ( (err==KErrNone ) || (err==KErrAlreadyExists) )
		{
		RFile Source;

		err=Source.Open(Fs,pathFrom.FullName(),EFileRead);
		if(err!=KErrNone)
			{			
			// Source does not exist no copy no error.
			err=KErrNone;
			Fs.Close();
			return EFalse;
			}
		Source.Close();
			// Okay source exists.


		ret = Fs.Replace(pathFrom.FullName() , pathTo.FullName());
		if(ret!=KErrNone)
			{			
			Fs.Close();
			err=KErrNone;
			return EFalse;		
			}
		}
		
	Fs.Close();


	// Managed to rename the file succesfully.
	// or did nothing at all if not emulator.
	ret=ETrue;
	
	return ret;
	}




//**************************************	
//**************************************	

TBool RenameIfExists(TInt& err)
	{
	TBool ret=EFalse;
	err=KErrNone;	

	TParse pathFrom;
	TParse pathTo;


	ret = ParseRenameIfExists(err , pathFrom, pathTo);
	if ( ret )
		{
			if (err==KErrNone)
				{ // Okay have parsed good arguments.				
				DoRenameIfExists(err , pathFrom, pathTo);
				}
				// else some sort of error.
		}
		// else not recognised the command.


	return ret;
	}
//**************************************	
	


// RenameIfEmulator z:\a.x c:\a.x 
// CopyIfEmulator z:\a.x c:\a.x 
// RenameIfExists z:\a.x c:\a.x 

TInt E32Main()
	{
	TInt err=KErrNone;		
	TBool done=EFalse;
	
	
	done=RenameIfEmulator( err);
	if ( done )
		{ // Assumed command was RenameIfEmulator, which may or may not have run correctly (check err).
		}
	else 
		{
		done=CopyIfEmulator( err);		
		if ( done )
			{ // Assumed command was CopyIfEmulator, which may or may not have run correctly (check err).
			}
		else 
			{
			done=RenameIfExists( err);
			if ( done )
				{ // Assumed command was RenameIfExists, which may or may not have run correctly (check err).
				}
			else
				{
				err=KErrNotFound;
				}
			}
		}
	
	return err;	
	}
