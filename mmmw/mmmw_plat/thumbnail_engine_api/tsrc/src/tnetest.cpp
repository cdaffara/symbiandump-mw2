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
* Description:  API test automation for thumbnail engine
*
*/


// INCLUDE FILES

#include <gdi.h>
#include "tnetest.h"
#include <flogger.h>



// PRINT macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

//#ifdef _DEBUG
#define ENABLE_S60_TNE_LOGGING 1
//#endif

//implementation
#ifdef ENABLE_S60_TNE_LOGGING
#define FLOG FileLog
inline static void FileLog(TRefByValue<const TDesC16> aFmt,...)
{
    VA_LIST  args;
    VA_START (args, aFmt);
    RFileLogger::WriteFormat(_L("tne"), _L("S60tne.log"), 
                        EFileLoggingModeAppend, aFmt, args);
}

#else // logging is disabled
#define FLOG LogFNull
inline static void LogFNull(TRefByValue<const TDesC16> aFmt,...){}

#endif // enable logging




// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::CRadioUtilityTestClass
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTneTestClass::CTneTestClass( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf ), 
        iTestModuleIf(aTestModuleIf)
    {    
    }

// -----------------------------------------------------------------------------
// CTneTestClass::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//

void CTneTestClass::ConstructL()
    {
    // intialize memeber variables 
   //  iLog->Log(_L("CTneTestClass::ConstructL()in "));
 
   
    iTNEVideoClipInfo = NULL;
	User::LeaveIfError(iWS.Connect());
	User::LeaveIfError(iFs.Connect());


    }


// -----------------------------------------------------------------------------
// CTneTestClass::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTneTestClass* CTneTestClass::NewL(CTestModuleIf& aTestModuleIf )
    {
   FLOG(_L("CTneTestClass::NewL in "));
    CTneTestClass* self = new (ELeave) CTneTestClass( aTestModuleIf );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
	FLOG(_L("CTneTestClass::NewL out "));
    }
    
TInt CTneTestClass::FindFileName(CStifItemParser& aItem) 
{
   TPtrC string;
   //TFileName aFileName;
    TInt lRetVal = KErrNone;
   aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );

   lRetVal = aItem.GetNextString ( string );

   if ( lRetVal != KErrNone )
	 {
    //   FLOG(_L("CHelixSTIFClient:: File name missing in config file "));
       return lRetVal;
	 }
   else
   {
      iFileName.Copy(string);
   }
   
   return lRetVal;

}
    
    
    
// intialize thumbnail Newl FIle name
TInt CTneTestClass::TNENewLFileNameL(CStifItemParser& aItem)
{
 FLOG(_L("CTneTestClass::TNENewLFileNameL in "));
   TInt error = KErrNone;
   
   if ((error = FindFileName(aItem)) == KErrNone)
    {  
     FLOG(_L("CTneTestClass:Filename=%S"),&iFileName ); 
      if( iTNEVideoClipInfo )
            {
            delete iTNEVideoClipInfo;
            iTNEVideoClipInfo = NULL;
            }
      TRAPD( err,iTNEVideoClipInfo = CTNEVideoClipInfo::NewL( iFileName, *this ) ); 
      error = err;
       FLOG(_L("TNENewLFileNameL error returned = %d"),error ); 
      return error;
    }
    FLOG(_L(" TNENewLFileNameL error returned if not KErrnone = %d"),error ); 
    return error;
}



// intialize thumbnail Newl File Handle
TInt CTneTestClass::TNENewLFileHandleL(CStifItemParser& aItem)
{
 FLOG(_L("CTneTestClass::TNENewLFileHandleL in "));
 TInt error = KErrNone;
  RFile afileHandle; 
   if ((error = FindFileName(aItem)) == KErrNone)
    {  
     FLOG(_L("CTneTestClass:Filename=%S"),&iFileName ); 
      // convert into a file handle and intialize tne     
          if( (error = afileHandle.Open( iFs,(TDesC &) iFileName, EFileShareReadersOnly | EFileStream | EFileRead)) != KErrNone )
            {
            return error;
            }
         if( iTNEVideoClipInfo )
            {
            delete iTNEVideoClipInfo;
            iTNEVideoClipInfo = NULL;
            }    
      TRAPD( err,iTNEVideoClipInfo = CTNEVideoClipInfo::NewL( afileHandle, *this ) ); 
      afileHandle.Close();
      error = err;
       FLOG(_L("TNENewLFileHandleL error returned = %d"),error ); 
      return error;
    }
     FLOG(_L("TNENewLFileHandleL error returned if not KErrnone = %d"),error ); 
    return error;

}
  
   
  

TInt CTneTestClass::TNENewLCFileNameL(CStifItemParser& aItem)
{
 FLOG(_L("CTneTestClass::TNENewLCFileNameL in "));
TInt error = KErrNone;
   
   if ((error = FindFileName(aItem)) == KErrNone)
    {  
     FLOG(_L("CTneTestClass:Filename=%S"),&iFileName ); 
      if( iTNEVideoClipInfo )
            {
            delete iTNEVideoClipInfo;
            iTNEVideoClipInfo = NULL;
            }
      TRAPD( err,iTNEVideoClipInfo = CTNEVideoClipInfo::NewLC( iFileName, *this ) ); 
      error = err;
       FLOG(_L(" TNENewLCFileNameL error returned = %d"),error ); 
      return error;
    }
     FLOG(_L(" TNENewLCFileNameL error returned if not KErrnone = %d"),error ); 
    return error;

}


TInt CTneTestClass::TNENewLCFileHandleL(CStifItemParser& aItem)
{
 FLOG(_L("CTneTestClass::TNENewLCFileHandleL in "));
TInt error = KErrNone;
  RFile afileHandle; 
   if ((error = FindFileName(aItem)) == KErrNone)
    {  
     FLOG(_L("CTneTestClass : Filename=%S"),&iFileName ); 
      // convert into a file handle and intialize tne     
          if( (error = afileHandle.Open( iFs,(TDesC &) iFileName, EFileShareReadersOnly | EFileStream | EFileRead)) != KErrNone )
            {
            return error;
            }
         if( iTNEVideoClipInfo )
            {
            delete iTNEVideoClipInfo;
            iTNEVideoClipInfo = NULL;
            }    
      TRAPD( err,iTNEVideoClipInfo = CTNEVideoClipInfo::NewLC( afileHandle, *this ) ); 
      afileHandle.Close();
      error = err;
       FLOG(_L("TNENewLCFileHandleL error returned = %d"),error ); 
      return error;
    }
     FLOG(_L("TNENewLCFileHandleL error returned if not KErrnone = %d"),error ); 
    return error;

}




// get thumb

TInt CTneTestClass::GetThumb(CStifItemParser& aItem)

{
  _LIT(KTagId, "");
  TInt lRetVal = KErrNone;
  TInt iThumbnailFrameIndex;
  TSize iThumbnailSize;
  TDisplayMode iDisplayMode;
  TInt iDisplInt;
  TInt ienh;
  TBool iEnhance;
  TInt iWidth;
  TInt iHeight;
  TPtrC string;
  
  // thumbindex
//
 //if ((lRetVal = aItem.GetInt(KTagId,iThumbnailFrameIndex)) != KErrNone )
 if ((lRetVal = aItem.GetNextInt(iThumbnailFrameIndex)) != KErrNone )
  { 
     FLOG(_L("CTneTestClass: Getthumb    if ((lRetVal = aItem.GetNextInt(iThumbnailFrameIndex)) != KErrNone ) error = %d"),lRetVal );
     return lRetVal ;
  }
 //width
 else if ((lRetVal = aItem.GetNextInt(iWidth)) != KErrNone )
  { 
  FLOG(_L("CTneTestClass: Getthumb   else if ((lRetVal = aItem.GetNextInt(iWidth)) != KErrNone ) error = %d"),lRetVal );
  return lRetVal ;
  }
  //height
 else if ((lRetVal = aItem.GetNextInt(iHeight)) != KErrNone )
  { 
    FLOG(_L("CTneTestClass : Getthumb   else if ((lRetVal = aItem.GetNextInt(iHeight)) != KErrNone ) error = %d"),lRetVal );
    return lRetVal ;
  }
  //enhance
  else if ((lRetVal = aItem.GetNextInt(ienh)) != KErrNone )
  {
    FLOG(_L("CTneTestClass : Getthumb   else if ((lRetVal = aItem.GetNextInt(ienh)) != KErrNone ) error = %d"),lRetVal ); 
    return lRetVal ;
  }
  
  // thumbnail size 
  iThumbnailSize = TSize(iWidth,iHeight);
  
  // display mode
  
  
     
 if ((lRetVal = aItem.GetNextInt(iDisplInt)) != KErrNone )
 {
  FLOG(_L(" CTneTestClass : Getthumb  if ((lRetVal = aItem.GetNextInt(iDisplInt)) != KErrNone ) error = %d"),lRetVal ); 
  return lRetVal;
 }
 else 
 {
 iDisplayMode =  (TDisplayMode)iDisplInt;
 }
 
 // set enhance boolean value
 
 
     if (ienh == 0)
     {
        iEnhance =  ETrue;
     }
     else
     {
        iEnhance =  EFalse;
     }
     
     // getThumbL call to TNE
 TRAPD( error,iTNEVideoClipInfo->GetThumbL( *this, iThumbnailFrameIndex, &iThumbnailSize, iDisplayMode, iEnhance, CActive::EPriorityIdle ) );
      lRetVal = error;
       FLOG(_L(" CTneTestClass : error returned from Getthumb = %d"),lRetVal ); 
      return lRetVal;
}



void CTneTestClass::NotifyVideoClipInfoReady( CTNEVideoClipInfo& aInfo, TInt aError )
{
  FLOG(_L("error returned  CTneTestClass::NotifyVideoClipInfoReady = %d"),aError ); 
  Signal(aError);
}


void CTneTestClass::NotifyVideoClipThumbCompleted( CTNEVideoClipInfo& /*aInfo*/, TInt aError, CFbsBitmap* aThumb )
{
  FLOG(_L(" CTneTestClass : error returned  CTneTestClass::NotifyVideoClipThumbCompleted = %d"),aError ); 
 if ( aError == KErrNone && aThumb == NULL)
 {
  // there is no error code returned but the bitmap pointer is NULL
  // This is still an error.
  aError = KErrNotFound;
 }
  Signal(aError);
}


//////////////RunMethodL()/////////////////

TInt CTneTestClass::RunMethodL( 
    CStifItemParser& aItem ) 
    {
    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "TNENewLFileName", CTneTestClass::TNENewLFileNameL ),
        ENTRY( "TNENewLFileHandle", CTneTestClass::TNENewLFileHandleL ),
        ENTRY( "TNENewLCFileName", CTneTestClass::TNENewLCFileNameL ),
        ENTRY( "TNENewLCFileHandle", CTneTestClass::TNENewLCFileHandleL ),
        ENTRY( "GetThumb", CTneTestClass::GetThumb ),
       
         };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );
    }





// Destructor
CTneTestClass::~CTneTestClass()
    { 
     FLOG(_L("CTneTestClass::~CTneTestClass() in") ); 
    // delete iLog; 
     if ( iTNEVideoClipInfo )
        {
        delete iTNEVideoClipInfo;
        }
        iWS.Close();
        iFs.Close();
    
    // Delete logger
   // delete iLog; 
	// delete iTimeoutController;
 FLOG(_L("CTneTestClass::~CTneTestClass() out") ); 
    }





/* this will be looked on later 
// Timeout

// -----------------------------------------------------------------------------
// CRadioUtilityTestClass::SetTimeout
// Create a timer and set a timeout
// When the timeout is reached the test case is marked as failed
// It's used rather than the "timeout" keyword in the configuration file
// because in this way the log continues
// -----------------------------------------------------------------------------
TInt CTneTestClass::SetTimeout( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::SetTimeout")));
	TInt timeout=0;
	TInt error = aItem.GetNextInt(timeout) ;
	if ( iTimeoutController )
	{
		if ( timeout > 0 )
		{
			iTimeoutController->Start( TTimeIntervalMicroSeconds(timeout*1000) );
		}
		else
		{
			iTimeoutController->Start( TTimeIntervalMicroSeconds(1000000) );
		}
	}
	else
	{
		FLOG(_L("Timeout Controller doesn't exist"));
		error = KErrTimeoutController;
	}
	return error;
}

*/


// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( 
    CTestModuleIf& aTestModuleIf ) // Backpointer to STIF Test Framework
    {

    return ( CScriptBase* ) CTneTestClass::NewL( aTestModuleIf );

    }

// -----------------------------------------------------------------------------
// E32Dll is a DLL entry point function.
// Returns: KErrNone
// -----------------------------------------------------------------------------
//
#ifndef EKA2 // Hide Dll entry point to EKA2
GLDEF_C TInt E32Dll(
    TDllReason /*aReason*/) // Reason code
    {
    return(KErrNone);

    }
#endif // EKA2

//  End of File


