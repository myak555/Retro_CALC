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
// Creates an array (vector or matrix)
//
static bool _operator_DIM_( Lexer *lex){
  return lex->operator_DIM();
}
bool Lexer::operator_DIM(){
  #ifdef __DEBUG
  Serial.println("DIM called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  byte *ptr = _epar->nameParser.parse( _lexer_position);
  if( !_epar->nameParser.result){
    _skipToNextOperator( ptr);
     return true;
  }
  lastVariable = _vars->getOrCreateNumber( false, _epar->nameParser.Name());
  if( lastVariable == 0){
    _mbox->setLabel(LEX_Error_OutOfMemory);
    _skipToNextOperator( _lexer_position);
    return true;
  }
  _lexer_position = ptr; // name found, looking for assignment
  if( !_findAssignment()){ // no assignment, take from stack
    _vars->setValueReal( lastVariable, _vars->getRPNRegister());
    _skipToNextOperator( _lexer_position);
     return true;
  }
  // parse expression
  _lexer_position = _epar->parseAlgebraic(_lexer_position);
  switch( _epar->result ){
    case _RESULT_INTEGER_:
      _vars->setValueInteger( lastVariable, _epar->numberParser.integerValue());
      break;
    case _RESULT_REAL_:
      _vars->setValueReal( lastVariable, _epar->numberParser.realValue());
      break;
    default:
      _vars->setValueReal( lastVariable, _vars->getRPNRegister());
      break;
  }
  _skipToNextOperator( _lexer_position);
  return true;
}
