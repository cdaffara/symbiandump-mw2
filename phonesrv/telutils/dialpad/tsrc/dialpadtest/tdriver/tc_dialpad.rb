############################################################################
## 
## Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies). 
## All rights reserved. 
## 
## This file is part of Dialpad. Reference TDriver test script. 
## 
## 
############################################################################

require 'matti'
require 'orbitbehaviours.rb'
include MattiVerify

require 'test/unit'

class TC_Dialpad < Test::Unit::TestCase
  
  # setup and teardown methods are always executed before each and after each test case method
  def setup
    # get the calculator app running 
    @sut = MATTI.sut(:Id => "sut_qt")    
    @dtest = @sut.run(:name => "dialpadtest")
    
  end
  
  def teardown   
    @dtest.close        
  end
  
  def test_dialpad
    
    # tap toolbar to open dialpad
    @dtest.HbToolBar().tap
    verify( 5, "Dialpad should be open" ) { @dtest.Dialpad(:objectName => 'dialpad', 
                                                           :visible => 'true')}

    verify { @dtest.DialpadNumericButtonGrid.DialpadNumericButton(:position => '0,0', 
                                                                  :primaryLabel => '1', 
                                                                  :iconName => 'qtg_mono_voice_mailbox') }
                                                                  
    verify { @dtest.DialpadNumericButtonGrid.DialpadNumericButton(:position => '1,0', 
                                                                  :primaryLabel => '2', 
                                                                  :secondaryLabel => 'abc') }
                                                                  
    verify { @dtest.DialpadNumericButtonGrid.DialpadNumericButton(:position => '2,0', 
                                                                  :primaryLabel => '3', 
                                                                  :secondaryLabel => 'def') }
    
    verify { @dtest.DialpadNumericButtonGrid.DialpadNumericButton(:position => '0,1', 
                                                                  :primaryLabel => '4', 
                                                                  :secondaryLabel => 'ghi') }        
    
    verify { @dtest.DialpadNumericButtonGrid.DialpadNumericButton(:position => '1,1', 
                                                                  :primaryLabel => '5', 
                                                                  :secondaryLabel => 'jkl') }    

    verify { @dtest.DialpadNumericButtonGrid.DialpadNumericButton(:position => '2,1', 
                                                                  :primaryLabel => '6', 
                                                                  :secondaryLabel => 'mno') }    
    
    verify { @dtest.DialpadNumericButtonGrid.DialpadNumericButton(:position => '0,2', 
                                                                  :primaryLabel => '7', 
                                                                  :secondaryLabel => 'pqrs') }        
    
    verify { @dtest.DialpadNumericButtonGrid.DialpadNumericButton(:position => '1,2', 
                                                                  :primaryLabel => '8', 
                                                                  :secondaryLabel => 'tuv') }    

    verify { @dtest.DialpadNumericButtonGrid.DialpadNumericButton(:position => '2,2', 
                                                                  :primaryLabel => '9', 
                                                                  :secondaryLabel => 'wxyz') }    
    
    verify { @dtest.DialpadNumericButtonGrid.DialpadNumericButton(:position => '0,3', 
                                                                  :primaryLabel => '*', 
                                                                  :secondaryLabel => '+') }        
    
    verify { @dtest.DialpadNumericButtonGrid.DialpadNumericButton(:position => '1,3', 
                                                                  :primaryLabel => '0', 
                                                                  :secondaryLabel => '') }    

    verify { @dtest.DialpadNumericButtonGrid.DialpadNumericButton(:position => '2,3', 
                                                                  :primaryLabel => '#', 
                                                                  :secondaryLabel => '') }
                                                                  
    verify { @dtest.DialpadButton( :objectName => 'backspaceButton', :iconName => 'qtg_mono_backspace2' ) }
    
    verify { @dtest.DialpadButton( :objectName => 'callButton', :iconName => 'qtg_mono_call' ) }
    
    # enter 159
    @dtest.DialpadNumericButtonGrid.DialpadNumericButton(:position => '0,0').tap
    @dtest.DialpadNumericButtonGrid.DialpadNumericButton(:position => '1,1').tap
    @dtest.DialpadNumericButtonGrid.DialpadNumericButton(:position => '2,2').tap    
    
    verify( 5, "Editor text should be 159" ) { @dtest.HbLineEdit(:objectName => 'numberEdit', :text => "159")}    
        
    @dtest.DialpadButton({:objectName => 'backspaceButton'}).tap
    verify( 5, "Editor text should be 15" ) { @dtest.HbLineEdit(:objectName => "numberEdit", :text => "15")}    
    
    @dtest.Dialpad(:objectName => 'dialpad').flick(:Down)
    verify_not( 5, "Dialpad should be closed" ) { @dtest.Dialpad(:objectName => 'dialpad')}    
    
  end  
  
end
