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
* Description: This file contains testclass implementation.
*
*/

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "PresetUtilityTestModule.h"
#include <RadioPresetUtility.h>

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;
_LIT( KStifScriptEmptyString, "EMPTYSTRING" );
_LIT( KEmptyString, "" );

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CPresetUtilityTestModule::Delete() 
    {
    FUNC_LOG;

    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CPresetUtilityTestModule::RunMethodL( 
    CStifItemParser& aItem ) 
    {
    FUNC_LOG;

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", CPresetUtilityTestModule::ExampleL ),
        // [test cases entries] - Do not remove
        ENTRY( "CreateRadioPresetL", CPresetUtilityTestModule::CreateRadioPresetL ),
        ENTRY( "DestroyRadioPreset", CPresetUtilityTestModule::DestroyRadioPresetL ),
        ENTRY( "CreatePresetUtility", CPresetUtilityTestModule::CreatePresetUtilityL ), 
        ENTRY( "DestroyPresetUtility", CPresetUtilityTestModule::DestroyPresetUtilityL ),
        ENTRY( "CreateFmPresetUtility", CPresetUtilityTestModule::CreateFmPresetUtilityL ),
        ENTRY( "DestroyFmPresetUtility", CPresetUtilityTestModule::DestroyFmPresetUtilityL ),
        ENTRY( "SavePresetL", CPresetUtilityTestModule::SavePresetL ),
        ENTRY( "ReadPresetL", CPresetUtilityTestModule::ReadPresetL ),
        ENTRY( "MaxNumberOfPresets", CPresetUtilityTestModule::MaxNumberOfPresets ),
        ENTRY( "PresetCount", CPresetUtilityTestModule::PresetCount ),
        ENTRY( "FirstPreset", CPresetUtilityTestModule::FirstPreset ),
        ENTRY( "NextPreset", CPresetUtilityTestModule::NextPreset ),
        ENTRY( "DeletePreset", CPresetUtilityTestModule::DeletePreset ),
        ENTRY( "SetPresetL", CPresetUtilityTestModule::SetPresetL ),
        ENTRY( "GetPresetL", CPresetUtilityTestModule::GetPresetL ),
        ENTRY( "GetPresetNameL", CPresetUtilityTestModule::GetPresetNameL ),
        ENTRY( "GetPresetFrequencyL", CPresetUtilityTestModule::GetPresetFrequencyL ),      
        ENTRY( "GetMaxNumberOfPresets", CPresetUtilityTestModule::GetMaxNumberOfPresets ),
        ENTRY( "GetNumberOfPresets", CPresetUtilityTestModule::GetNumberOfPresets ),
        ENTRY( "GetFirstPresetL", CPresetUtilityTestModule::GetFirstPresetL ),
        ENTRY( "GetNextPresetL", CPresetUtilityTestModule::GetNextPresetL ),
        ENTRY( "DeletePresetL", CPresetUtilityTestModule::DeletePresetL ),

        ENTRY( "SetPresetUrlL", CPresetUtilityTestModule::SetPresetUrlL ),
        ENTRY( "SetPresetPtyL", CPresetUtilityTestModule::SetPresetPtyL ),
        ENTRY( "SetPresetPICodeL", CPresetUtilityTestModule::SetPresetPICodeL ),
        ENTRY( "SetPresetFavouriteInfoL", CPresetUtilityTestModule::SetPresetFavouriteInfoL ),
        ENTRY( "SetPresetUserRenameInfoL", CPresetUtilityTestModule::SetPresetUserRenameInfoL ),
        ENTRY( "GetPresetUrlL", CPresetUtilityTestModule::GetPresetUrlL ),
        ENTRY( "GetPresetPtyL", CPresetUtilityTestModule::GetPresetPtyL ),
        ENTRY( "GetPresetPICodeL", CPresetUtilityTestModule::GetPresetPICodeL ),
        ENTRY( "GetPresetFavouriteInfoL", CPresetUtilityTestModule::GetPresetFavouriteInfoL ),
        ENTRY( "GetPresetUserRenameInfoL", CPresetUtilityTestModule::GetPresetUserRenameInfoL ),

        ENTRY( "SetFavorite", CPresetUtilityTestModule::SetFavorite ),
        ENTRY( "Favorite", CPresetUtilityTestModule::Favorite ),
        ENTRY( "SetLocalStation", CPresetUtilityTestModule::SetLocalStation ),
        ENTRY( "LocalStation", CPresetUtilityTestModule::LocalStation ),
        ENTRY( "SetRenamedByUser", CPresetUtilityTestModule::SetRenamedByUser ),
        ENTRY( "RenamedByUser", CPresetUtilityTestModule::RenamedByUser ),
        ENTRY( "SetGenre", CPresetUtilityTestModule::SetGenre ),
        ENTRY( "Genre", CPresetUtilityTestModule::Genre ),
        ENTRY( "SetPiCode", CPresetUtilityTestModule::SetPiCode ),
        ENTRY( "PiCode", CPresetUtilityTestModule::PiCode )
        //ADD NEW ENTRY HERE

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPresetUtilityTestModule::ExampleL( CStifItemParser& aItem )
    {
    FUNC_LOG;

    // Print to UI
    _LIT( KPresetUtilityTestModule, "PresetUtilityTestModule" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KPresetUtilityTestModule, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KPresetUtilityTestModule, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

//-----------------------------------------------------------------------------
// CPresetUtilityTest::CreateRadioPresetL
//-----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::CreateRadioPresetL(
    CStifItemParser& /*aItem*/ )
    {
    FUNC_LOG;
    
    TInt err = KErrNone;
    if( !iPreset )
        {        
        iPreset = new( ELeave ) TPreset;
        }
    else
        {
        err = KErrArgument;
        }

    return err;
    }

//-----------------------------------------------------------------------------
// CPresetUtilityTest::CreatePresetUtilityL
//-----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::CreatePresetUtilityL(
    CStifItemParser& /*aItem*/ )
    {
    FUNC_LOG;
    
    TInt err = KErrNone;
    if( !iPresetUtility )
        {        
        iPresetUtility = CPresetUtility::NewL();
        }
    else
        {
        err = KErrArgument;
        }

    return err;
    }

//-----------------------------------------------------------------------------
// CPresetUtilityTest::CreatePresetFmUtilityL
//-----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::CreateFmPresetUtilityL(
    CStifItemParser& /*aItem*/ )
    {
    FUNC_LOG;
    
    TInt err = KErrNone;
    if( !iRadioFmPresetUtility )
        {        
        iRadioFmPresetUtility = CRadioFmPresetUtility::NewL(*this);
        }
    else
        {
        err = KErrArgument;
        }

    return err;
    }

//-----------------------------------------------------------------------------
// CPresetUtilityTestModule::DestroyRadioPresetL
//-----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::DestroyRadioPresetL(
    CStifItemParser& /*aItem*/ )
    {
    FUNC_LOG;

    delete iPreset;    
    iPreset = NULL;
    
    return KErrNone;
    }



//-----------------------------------------------------------------------------
// CPresetUtilityTestModule::DestroyPresetUtilityL
//-----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::DestroyPresetUtilityL(
    CStifItemParser& /*aItem*/ )
    {
    FUNC_LOG;

    delete iPresetUtility;    
    iPresetUtility = NULL;
    
    return KErrNone;
    }

//-----------------------------------------------------------------------------
// CPresetUtilityTestModule::DestroyFmPresetUtilityL
//-----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::DestroyFmPresetUtilityL(
    CStifItemParser& /*aItem*/ )
    {
    FUNC_LOG;

    delete iRadioFmPresetUtility;    
    iRadioFmPresetUtility = NULL;
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::SavePresetL()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::SavePresetL( CStifItemParser& aItem )
{
    FUNC_LOG;
    TInt err=KErrNone;
    TInt index;
    TPtrC name;
    TInt freq;
    TPtrC url;
    TInt usedIndex( -1 );
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        err = KErrBadTestParameter;
        ERROR( err, "Missing Parameter: Preset index." );
    }
    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    if ( aItem.GetNextString(name) )
    {
        err = KErrBadTestParameter;
        ERROR( err, "Missing Parameter: Preset name." );
    }
    aItem.SetParsingType( CStifItemParser::ENormalParsing );
    if ( aItem.GetNextInt(freq) )
    {
        err = KErrBadTestParameter;
        ERROR( err, "Missing Parameter: Preset frequency." );
    }
    
    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    if ( aItem.GetNextString(url) )
    {
        INFO( "Missing Parameter: url." );
    }
    aItem.SetParsingType( CStifItemParser::ENormalParsing );

    TPreset preset;
    preset.SetName( name );
    preset.SetFrequency( freq );
    preset.SetUrl( url );
    
    TRAP(err, usedIndex = iPresetUtility->SavePresetL(preset, index));
    INFO_4( "SavePreset: Index=%d, Name=%S, Freq=%d, Url=%S", usedIndex, &name, freq, &url  );
    if ( KErrNone != err )
        {
        INFO_1( "SavePresetL Failed error code=%i", err );
        User::Leave( err );
        }
    return usedIndex;
}

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::ReadPresetL()
// Read preset channel.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::ReadPresetL( CStifItemParser& aItem )
{
    FUNC_LOG;
    TInt err = KErrNone;
    TInt index;
    TPtrC expectedName;
    TInt expectedFreq;
    TPtrC expectedUrl;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        err = KErrBadTestParameter;
        ERROR( err, "Missing Parameter: Preset index." );
    }
    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    if ( aItem.GetNextString(expectedName) )
    {
        err = KErrBadTestParameter;
        ERROR( err, "Missing Parameter: Expected preset name." );
    }
    INFO_1( "ExpectedName=%S.", &expectedName  );

    aItem.SetParsingType( CStifItemParser::ENormalParsing );
    if ( aItem.GetNextInt(expectedFreq) )
    {
        err = KErrBadTestParameter;
        ERROR( err, "Missing Parameter: Expected preset frequency." );
    }
    INFO_1( "ExpectedFreq=%d.", expectedFreq);
    
    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    if ( aItem.GetNextString(expectedUrl) )
    {
        INFO( "Missing Parameter: Expected Url." );
    }
    aItem.SetParsingType( CStifItemParser::ENormalParsing );

    TPreset preset;
    TRAP(err, iPresetUtility->ReadPresetL(index,preset));
    TPtrC actualName( preset.Name() );
    TPtrC actualUrl( preset.Url() );
    INFO_4( "ReadPreset Index=%d, Name=%S, Freq=%d, Url=%S", index, &expectedName, preset.Frequency(), &expectedUrl  );
    if ( !err )
    {
        // Verification
        if ( preset.Name() != expectedName )
        {
            INFO_2( "Failed: name=%S, expectedName=%S.", &actualName, &expectedName  );
            err = KErrUnexpectedValue;
        }
        else if ( preset.Frequency() != expectedFreq )
        {
            INFO_2( "Failed: freq=%d, expectedFreq=%d.", preset.Frequency(), expectedFreq);
            err = KErrUnexpectedValue;
        }
        else if ( ( 0 < expectedUrl.Length() ) && ( preset.Url() != expectedUrl ) )
        {
            INFO_2( "Failed: Url=%S, expectedUrl=%S.", &actualUrl, &expectedUrl  );
            err = KErrUnexpectedValue;            
        }
    }
    return err;
}

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::MaxNumberOfPresets()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::MaxNumberOfPresets( CStifItemParser& /*aItem*/ )
{
    FUNC_LOG;
    TInt maxPreset;
    
    maxPreset = iPresetUtility->MaxNumberOfPresets();
    INFO_1( "MaxNumberOfPresets: maxPreset=%d", maxPreset);
    return maxPreset;
}

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::PresetCount()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::PresetCount( CStifItemParser& aItem )
{
    FUNC_LOG;
    TInt err=KErrNone;
    TInt numPresets;
    TInt expectedNumPresets;
    
    // read parameters
    if ( aItem.GetNextInt(expectedNumPresets) )
    {
        INFO( "Missing Parameter: Expected number of presets.");
        err = KErrBadTestParameter;
    }

    numPresets =iPresetUtility->PresetCount();
    INFO_1( "PresetCount: numPresets=%d", numPresets);
   // Verification
    if ( numPresets != expectedNumPresets )
    {
        INFO_2( "Failed: numPresets=%d, expectedNumPresets=%d.", numPresets, expectedNumPresets);
        err = KErrUnexpectedValue;
    }
    return err;
}

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::FirstPreset()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::FirstPreset( CStifItemParser& aItem )
{
    FUNC_LOG;
    TInt err = KErrNone;
    TInt index;
    TInt expectedIndex;
    
    // read parameters
    if ( aItem.GetNextInt(expectedIndex) )
    {
        INFO( "Missing Parameter: Expected preset index.");
        err = KErrBadTestParameter;
    }

    index = iPresetUtility->FirstPreset();
    INFO_1( "FirstPreset: index=%d", index);
    // Verification
    if ( index != expectedIndex )
    {
        INFO_2( "Failed: index=%d, expectedIndex=%d.", index, expectedIndex);
        err = KErrUnexpectedValue;
    }
    return err;
}

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::NextPreset()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::NextPreset( CStifItemParser& aItem )
{
    FUNC_LOG;
    TInt err=KErrNone;
    TInt index;
    TInt nextIndex;
    TInt expectedNextIndex;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        INFO( "Missing Parameter: Current preset index.");
        err = KErrBadTestParameter;
    }
    if ( aItem.GetNextInt(expectedNextIndex) )
    {
        INFO( "Missing Parameter: Expected next preset index.");
        err = KErrBadTestParameter;
    }

    nextIndex = iPresetUtility->NextPreset(index);
    INFO_1( "NextPreset: nextIndex=%d", nextIndex);
    // Verification
    if ( nextIndex != expectedNextIndex )
    {
        INFO_2( "Failed: nextIndex=%d, expectedNextIndex=%d.", nextIndex, expectedNextIndex);
        err = KErrUnexpectedValue;
    }
    return err;
}

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::DeletePreset()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::DeletePreset( CStifItemParser& aItem )
{
    FUNC_LOG;
    TInt err=KErrNone;
    TInt index;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        INFO( "Missing Parameter: Preset index to delete.");
        err = KErrBadTestParameter;
    }

    err = iPresetUtility->DeletePreset(index);
    INFO_1( "DeletePreset: index=%d", index);
    return err;
}

// Old preset utility API functions

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::SetPresetL()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::SetPresetL( CStifItemParser& aItem )
{
    FUNC_LOG;
    TInt err=KErrNone;
    TInt index;
    TPtrC name;
    TInt freq;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        err = KErrBadTestParameter;
        ERROR( err, "Missing Parameter: Preset index." );
    }
    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    if ( aItem.GetNextString(name) )
    {
        err = KErrBadTestParameter;
        ERROR( err, "Missing Parameter: Preset name." );
    }
    TPtrC stifScriptEmptyString( KStifScriptEmptyString );
    if ( !stifScriptEmptyString.Compare( name ) )
        {
        TPtrC emptyString( KEmptyString );
        name.Set(emptyString);
        }
    aItem.SetParsingType( CStifItemParser::ENormalParsing );
    if ( aItem.GetNextInt(freq) )
    {
        err = KErrBadTestParameter;
        ERROR( err, "Missing Parameter: Preset frequency." );
    }
    
    TPresetName presetName( name );   
    TRAP(err, iRadioFmPresetUtility->SetPresetL(index, presetName, freq));
    INFO_3("SetPresetL: Index=%d, Name=%S, Freq=%d", index, &name, freq );
    return err;
}

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::GetPresetL()
// Read preset channel.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::GetPresetL( CStifItemParser& aItem )
{
    FUNC_LOG;
    TInt err = KErrNone;
    TInt index;
    TPtrC expectedName;
    TInt expectedFreq;
    TPtrC expectedUrl;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        err = KErrBadTestParameter;
        ERROR( err, "Missing Parameter: Preset index." );
    }
    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    if ( aItem.GetNextString(expectedName) )
    {
        err = KErrBadTestParameter;
        ERROR( err, "Missing Parameter: Expected preset name." );
    }
    TPtrC stifScriptEmptyString( KStifScriptEmptyString );
    if ( !stifScriptEmptyString.Compare( expectedName ) )
        {
        TPtrC emptyString( KEmptyString );
        expectedName.Set(emptyString);
        }
    INFO_1( "ExpectedName=%S.", &expectedName  );

    aItem.SetParsingType( CStifItemParser::ENormalParsing );
    if ( aItem.GetNextInt(expectedFreq) )
    {
        err = KErrBadTestParameter;
        ERROR( err, "Missing Parameter: Expected preset frequency." );
    }
    INFO_1( "ExpectedFreq=%d.", expectedFreq);
    
    aItem.SetParsingType( CStifItemParser::ENormalParsing );

    TPresetName presetName;
    TInt actualFreq( 0 );
    TRAP(err, iRadioFmPresetUtility->GetPresetL(index, presetName, actualFreq));
    INFO_3( "GetPresetL Index=%d, Name=%S, Freq=%d", index, &presetName, actualFreq  );
    if ( !err )
    {
        // Verification
        if ( presetName != expectedName )
        {
            INFO_2( "Failed: name=%S, expectedName=%S.", &presetName, &expectedName  );
            err = KErrUnexpectedValue;
        }
        else if ( actualFreq != expectedFreq )
        {
            INFO_2( "Failed: freq=%d, expectedFreq=%d.", actualFreq, expectedFreq);
            err = KErrUnexpectedValue;
        }
    }
    return err;
}

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::GetMaxNumberOfPresets()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::GetMaxNumberOfPresets( CStifItemParser& /*aItem*/ )
{
    FUNC_LOG;
    TInt maxPreset;
    
    iRadioFmPresetUtility->GetMaxNumberOfPresets( maxPreset );
    INFO_1( "GetMaxNumberOfPresets: maxPreset=%d", maxPreset);
    return maxPreset;
}

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::GetNumberOfPresets()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::GetNumberOfPresets( CStifItemParser& aItem )
{
    FUNC_LOG;
    TInt err=KErrNone;
    TInt numPresets;
    TInt expectedNumPresets;
    
    // read parameters
    if ( aItem.GetNextInt(expectedNumPresets) )
    {
        INFO( "Missing Parameter: Expected number of presets.");
        err = KErrBadTestParameter;
    }

    err = iRadioFmPresetUtility->GetNumberOfPresets( numPresets );
    INFO_1( "GetNumberOfPresets: numPresets=%d", numPresets);
   // Verification
    if ( numPresets != expectedNumPresets )
    {
        INFO_2( "Failed: numPresets=%d, expectedNumPresets=%d.", numPresets, expectedNumPresets);
        err = KErrUnexpectedValue;
    }
    return err;
}

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::GetFirstPresetL()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::GetFirstPresetL( CStifItemParser& aItem )
{
    FUNC_LOG;
    TInt err = KErrNone;
    TInt index;
    TInt expectedIndex;
    
    // read parameters
    if ( aItem.GetNextInt(expectedIndex) )
    {
        INFO( "Missing Parameter: Expected preset index.");
        err = KErrBadTestParameter;
    }

    iRadioFmPresetUtility->GetFirstPresetL( index );
    INFO_1( "GetFirstPresetL: index=%d", index);
    // Verification
    if ( index != expectedIndex )
    {
        INFO_2( "Failed: index=%d, expectedIndex=%d.", index, expectedIndex);
        err = KErrUnexpectedValue;
    }
    return err;
}

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::GetNextPresetL()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::GetNextPresetL( CStifItemParser& aItem )
{
    FUNC_LOG;
    TInt err=KErrNone;
    TInt index;
    TInt nextIndex;
    TInt expectedNextIndex;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        INFO( "Missing Parameter: Current preset index.");
        err = KErrBadTestParameter;
    }
    if ( aItem.GetNextInt(expectedNextIndex) )
    {
        INFO( "Missing Parameter: Expected next preset index.");
        err = KErrBadTestParameter;
    }

    iRadioFmPresetUtility->GetNextPresetL( index, nextIndex );
    INFO_1( "GetNextPresetL: nextIndex=%d", nextIndex);
    // Verification
    if ( nextIndex != expectedNextIndex )
    {
        INFO_2( "Failed: nextIndex=%d, expectedNextIndex=%d.", nextIndex, expectedNextIndex);
        err = KErrUnexpectedValue;
    }
    return err;
}

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::DeletePresetL()
// Get program status.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::DeletePresetL( CStifItemParser& aItem )
{
    FUNC_LOG;
    TInt err=KErrNone;
    TInt index;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        INFO( "Missing Parameter: Preset index to delete.");
        err = KErrBadTestParameter;
    }

    TRAP( err, iRadioFmPresetUtility->DeletePresetL(index) );
    INFO_1( "DeletePresetL: index=%d", index);
    return err;
}

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::GetPresetNameL()
// Read preset channel.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::GetPresetNameL( CStifItemParser& aItem )
{
    FUNC_LOG;
    TInt err = KErrNone;
    TInt index;
    TPtrC expectedName;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        err = KErrBadTestParameter;
        ERROR( err, "Missing Parameter: Preset index." );
    }
    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    if ( aItem.GetNextString(expectedName) )
    {
        err = KErrBadTestParameter;
        ERROR( err, "Missing Parameter: Expected preset name." );
    }
    TPtrC stifScriptEmptyString( KStifScriptEmptyString );
    if ( !stifScriptEmptyString.Compare( expectedName ) )
        {
        TPtrC emptyString( KEmptyString );
        expectedName.Set(emptyString);
        }
    INFO_1( "ExpectedName=%S.", &expectedName  );


    TFmPresetName presetName;
    TRAP(err, iRadioFmPresetUtility->GetPresetNameL(index,presetName));
    INFO_2( "GetPresetNameL Index=%d, Name=%S", index, &presetName  );
    if ( !err )
    {
        // Verification
        if ( presetName != expectedName )
        {
            INFO_2( "Failed: name=%S, expectedName=%S.", &presetName, &expectedName  );
            err = KErrUnexpectedValue;
        }
    }
    return err;
}

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::GetPresetFrequencyL()
// Read preset channel's frequency value.
// Returns: Errors.
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::GetPresetFrequencyL( CStifItemParser& aItem )
    {
    FUNC_LOG;
    TInt index;
    TInt expectedFreq( 0 );
    TInt freq( 0 );
    TInt err( KErrNone );

    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        err = KErrBadTestParameter;
        ERROR( err, "Missing Parameter: Preset index." );
    }
    if ( aItem.GetNextInt(expectedFreq) )
    {
        err = KErrBadTestParameter;
        ERROR( err, "Missing Parameter: Expected preset frequency." );
    }
    iRadioFmPresetUtility->GetPresetFrequencyL( index, freq );  
    INFO_1( "GetPresetrequency=%d.", freq );
    if ( freq != expectedFreq )
    {
        INFO_2( "Failed: freq=%d, expectedFreq=%d.", freq, expectedFreq);
        err = KErrUnexpectedValue;
    }    
    return err;
}

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::PresetChanged()
// Returns: None
// -----------------------------------------------------------------------------
void CPresetUtilityTestModule::PresetChanged
    ( MPresetUtilityObserver::TPresetChangeEvent /*aChange*/, TInt /*aIndex*/ )
    {
    FUNC_LOG;   
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::MrpeoPresetChanged()
// Returns: None
// -----------------------------------------------------------------------------
void CPresetUtilityTestModule::MrpeoPresetChanged
    ( MRadioPresetObserver::TPresetChangeEvent /*aChange*/, TInt /*aIndex*/ )
    {
    FUNC_LOG;   
    }

// -----------------------------------------------------------------------------
// CFmPresetUtilityTestModule::SetPresetUrlL()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::SetPresetUrlL( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TInt index;
    TPtrC urlName;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: Preset index." );
    }
    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    if ( aItem.GetNextString(urlName) )
    {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: URL name." );
    }
    aItem.SetParsingType( CStifItemParser::ENormalParsing );
    
    TFmPresetUrl presetName( urlName );   
    TRAP(ret, iRadioFmPresetUtility->SetPresetUrlL(index, urlName ));
    INFO_2("SetPresetUrlL: Index=%d, urlName=%S", index, &urlName  );
    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::SetPresetPtyL()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::SetPresetPtyL( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TInt index( 0 );
    TInt PTYCode( 0 );
    
    // read parameters
    if ( aItem.GetNextInt(index) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: Preset index." );
        }
    if  ( !ret && aItem.GetNextInt(PTYCode) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: PTYCode." );
        }
    iRadioFmPresetUtility->SetPresetPtyL( index, PTYCode );  
    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::SetPresetPICodeL()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::SetPresetPICodeL( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TInt index( 0 );
    TInt PICode( 0 );
    
    // read parameters
    if ( aItem.GetNextInt(index) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: Preset index." );
        }
    if  ( !ret && aItem.GetNextInt(PICode) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: PICode." );
        }
    iRadioFmPresetUtility->SetPresetPICodeL( index, PICode );  
    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::SetPresetFavouriteInfoL()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::SetPresetFavouriteInfoL( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TInt index( 0 );
    TInt favouriteInt( 0 );
    TBool favouriteStatus( EFalse );
    
    // read parameters
    if ( aItem.GetNextInt(index) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: Preset index." );
        }
    if  ( !ret && aItem.GetNextInt(favouriteInt) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: favouriteInt." );
        }
    if ( favouriteInt )
        {
        favouriteStatus = ETrue;
        }
    iRadioFmPresetUtility->SetPresetFavouriteInfoL( index, favouriteStatus );  
    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::SetPresetUserRenameInfoL()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::SetPresetUserRenameInfoL( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TInt index( 0 );
    TInt renameInt( 0 );
    TBool renameStatus( EFalse );
    
    // read parameters
    if ( aItem.GetNextInt(index) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: Preset index." );
        }
    if  ( !ret && aItem.GetNextInt(renameInt) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: renameInt." );
        }
    if ( renameInt )
        {
        renameStatus = ETrue;
        }
    iRadioFmPresetUtility->SetPresetUserRenameInfoL ( index, renameStatus );  
    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::GetPresetUrlL()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::GetPresetUrlL( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TInt index;
    TPtrC expectedUrlName;
    
    // read parameters
    if ( aItem.GetNextInt(index) )
    {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: Preset index." );
    }
    aItem.SetParsingType( CStifItemParser::EQuoteStyleParsing );
    if ( aItem.GetNextString(expectedUrlName) )
    {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: Expected preset name." );
    }
    TPtrC stifScriptEmptyString( KStifScriptEmptyString );
    if ( !stifScriptEmptyString.Compare( expectedUrlName ) )
        {
        TPtrC emptyString( KEmptyString );
        expectedUrlName.Set(emptyString);
        }
    INFO_1( "ExpectedUrlName=%S.", &expectedUrlName  );

    TFmPresetUrl urlName;
    TRAP(ret, iRadioFmPresetUtility->GetPresetUrlL(index,urlName));
    INFO_2( "GetPresetUrlL Index=%d, urlName=%S", index, &urlName  );
    if ( !ret )
    {
        // Verification
        if ( urlName != expectedUrlName )
        {
            INFO_2( "Failed: urlName=%S, expectedUrlName=%S.", &urlName, &expectedUrlName  );
            ret = KErrUnexpectedValue;
        }
    }
    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::GetPresetPtyL()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::GetPresetPtyL( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TInt index( 0 );
    TUint expectedPTYCode( 0 );
    TUint PTYCode( 0 );
    
    // read parameters
    if ( aItem.GetNextInt(index) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: Preset index." );
        }
    if  ( !ret && aItem.GetNextInt(expectedPTYCode) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: expectedPTYCode." );
        }
    if ( !ret )
        {
        INFO_1( "GetPresetPTYCodeL=%d.", expectedPTYCode );
        TRAP( ret, iRadioFmPresetUtility->GetPresetPtyL( index, PTYCode ) );
        if ( PTYCode != expectedPTYCode )
            {
            INFO_2( "Failed: PTYCode=%d, expectedPTYCode=%d.", PTYCode, expectedPTYCode );
            ret = KErrUnexpectedValue;
            }
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::GetPresetPICodeL()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::GetPresetPICodeL( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TInt index( 0 );
    TUint expectedPICode( 0 );
    TUint PICode( 0 );
    
    // read parameters
    if ( aItem.GetNextInt(index) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: Preset index." );
        }
    if  ( !ret && aItem.GetNextInt(expectedPICode) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: expectedPICode." );
        }
    if ( !ret )
        {
        INFO_1( "GetPresetPICodeL=%d.", expectedPICode );
        TRAP( ret, iRadioFmPresetUtility->GetPresetPICodeL( index, PICode ) );
        if ( PICode != expectedPICode )
            {
            INFO_2( "Failed: PICode=%d, expectedPICode=%d.", PICode, expectedPICode );
            ret = KErrUnexpectedValue;
            }
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::GetPresetFavouriteInfoL()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::GetPresetFavouriteInfoL( CStifItemParser& aItem )
    {
    FUNC_LOG;
    TInt ret( KErrNone );
    TInt index( 0 );
    TInt expectedFavourite( 0 );
    TBool favouriteStatus( EFalse );
    
    // read parameters
    if ( aItem.GetNextInt(index) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: Preset index." );
        }
    if  ( !ret && aItem.GetNextInt(expectedFavourite) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: expectedFavourite." );
        }
    if ( !ret )
        {
        INFO_1( "GetPresetFavouriteInfoL=%d.", expectedFavourite );
        TRAP( ret, iRadioFmPresetUtility->GetPresetFavouriteInfoL( index, favouriteStatus ) );
        if ( !( ( favouriteStatus && expectedFavourite ) || 
                ( !favouriteStatus && !expectedFavourite ) ) )
            {
            INFO_2( "Failed: favouriteStatus=%d, expectedFavourite=%d.", favouriteStatus, expectedFavourite );
            ret = KErrUnexpectedValue;
            }
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::GetPresetUserRenameInfoL()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::GetPresetUserRenameInfoL( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TInt index( 0 );
    TInt expectedRename( 0 );
    TBool renameStatus( EFalse );
    
    // read parameters
    if ( aItem.GetNextInt(index) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: Preset index." );
        }
    if  ( !ret && aItem.GetNextInt(expectedRename) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: expectedRename." );
        }
    if ( !ret )
        {
        INFO_1( "GetPresetUserRenameInfoL=%d.", expectedRename );
        TRAP( ret, iRadioFmPresetUtility->GetPresetUserRenameInfoL( index, renameStatus ) );
        if ( !( ( renameStatus && expectedRename ) || 
                ( !renameStatus && !expectedRename ) ) )
            {
            INFO_2( "Failed: renameStatus=%d, expectedRename=%d.", renameStatus, expectedRename );
            ret = KErrUnexpectedValue;
            }
       }

    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::SetFavorite()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::SetFavorite( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TBool favoriteStatus( EFalse );
    if  ( !ret && aItem.GetNextInt(favoriteStatus) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: favoriteStatus." );
        }
    iPreset->SetFavorite( favoriteStatus );
    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::Favorite()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::Favorite( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TInt expectedFavorite( 0 );
    TBool favoriteStatus( EFalse );
    if  ( !ret && aItem.GetNextInt(expectedFavorite) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: expectedFavorite." );
        }
    favoriteStatus = iPreset->Favorite();
    if ( !( ( favoriteStatus && expectedFavorite ) || 
            ( !favoriteStatus && !expectedFavorite ) ) )
        {
        INFO_2( "Failed: favoriteStatus=%d, expectedFavorite=%d.", favoriteStatus, expectedFavorite );
        ret = KErrUnexpectedValue;
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::SetLocalStation()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::SetLocalStation( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TBool localStationStatus( EFalse );
    if  ( !ret && aItem.GetNextInt(localStationStatus) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: localStationStatus." );
        }
    iPreset->SetLocalStation( localStationStatus );
    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::LocalStation()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::LocalStation( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TInt expectedLocalStationStatus( 0 );
    TBool localStationStatus( EFalse );
    if  ( !ret && aItem.GetNextInt(expectedLocalStationStatus) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: expectedLocalStationStatus." );
        }
    localStationStatus = iPreset->LocalStation();
    if ( !( ( localStationStatus && expectedLocalStationStatus ) || 
            ( !localStationStatus && !expectedLocalStationStatus ) ) )
        {
        INFO_2( "Failed: localStationStatus=%d, expectedLocalStationStatus=%d.", localStationStatus, expectedLocalStationStatus );
        ret = KErrUnexpectedValue;
        }
    return ret;
    }


// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::SetRenamedByUser()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::SetRenamedByUser( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TBool renameStatus( EFalse );
    if  ( !ret && aItem.GetNextInt(renameStatus) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: renameStatus." );
        }
    iPreset->SetRenamedByUser( renameStatus );
    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::RenamedByUser()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::RenamedByUser( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TInt expectedRename( 0 );
    TBool renameStatus( EFalse );
    if  ( !ret && aItem.GetNextInt(expectedRename) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: expectedRename." );
        }
    renameStatus = iPreset->RenamedByUser();
    if ( !( ( renameStatus && expectedRename ) || 
            ( !renameStatus && !expectedRename ) ) )
        {
        INFO_2( "Failed: renameStatus=%d, expectedRename=%d.", renameStatus, expectedRename );
        ret = KErrUnexpectedValue;
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::SetGenre()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::SetGenre( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TInt genre( 0 );
    if  ( !ret && aItem.GetNextInt(genre) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: genre." );
        }
    iPreset->SetGenre( genre );
    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::Genre()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::Genre( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TInt expectedGenre( 0 );
    TInt genre( 0 );
    if  ( !ret && aItem.GetNextInt(expectedGenre) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: expectedGenre." );
        }
    genre = iPreset->Genre();
    if ( genre != expectedGenre )
        {
        INFO_2( "Failed: genre=%d, expectedGenre=%d.", genre, expectedGenre );
        ret = KErrUnexpectedValue;
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::SetPiCode()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::SetPiCode( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TInt piCode( 0 );
    if  ( !ret && aItem.GetNextInt(piCode) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: piCode." );
        }
    iPreset->SetPiCode( piCode );
    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModule::PiCode()
// Returns: None
// -----------------------------------------------------------------------------
TInt CPresetUtilityTestModule::PiCode( CStifItemParser& aItem )
    {
    FUNC_LOG;   
    TInt ret( KErrNone );
    TInt expectedPiCode( 0 );
    TInt piCode( 0 );
    if  ( !ret && aItem.GetNextInt(expectedPiCode) )
        {
        ret = KErrBadTestParameter;
        ERROR( ret, "Missing Parameter: expectedPiCode." );
        }
    piCode = iPreset->PiCode();
    if ( piCode != expectedPiCode )
        {
        INFO_2( "Failed: piCode=%d, expectedPiCode=%d.", piCode, expectedPiCode );
        ret = KErrUnexpectedValue;
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CPresetUtilityTestModuleModule::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt CPresetUtilityTestModule::?member_function(
   CItemParser& aItem )
   {
   FUNC_LOG;

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
