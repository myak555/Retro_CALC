//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

// This include is for convenience of coding; comment out
//#include "Lexer.hpp"

//
// Creates a variable
//
static bool _operator_INJ_( Lexer *lex){
  return lex->operator_INJ();
}
bool Lexer::operator_INJ(){
  #ifdef __DEBUG
  Serial.println("INJ called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  _epar->numberParser.stringValue( _vars->getRPNRegister(), _io_buffer);
  _iom->injectKeyboard();
  _skipToNextOperator();
  return true;
}
