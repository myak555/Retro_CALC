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
// Clears programs, variables or constants
//
static bool _operator_CLEAR_( Lexer *lex){
  return lex->operator_CLEAR();
}
void Lexer::_operatorClearProgramCode(uint16_t lFrom, uint16_t lTo){
  Serial.print("These lines will be gone: ");
  char *buff = (char *)_iom->getIOBuffer();
  ProgramLine pl = _code->getFirstLine();
  while( true){
    if( pl.line == NULL) return;
    if( pl.lineNumber > lTo) return;
    if( pl.lineNumber < lFrom){
      pl = _code->getNextLine( pl);
      continue;
    }
    int n = snprintf( buff, INPUT_COLS, "%05u ", pl.lineNumber);
    convertToUTF8( buff+n, pl.line, INPUT_COLS-n);
    _iom->sendStringUTF8Ln( buff);
    pl = _code->getNextLine( pl);
  }
}
bool Lexer::operator_CLEAR(){
  #ifdef __DEBUG
  Serial.println("CLEAR called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  _ignore_Blanks();
  _lexer_position = _kwds->parse(_lexer_position);
  if( _kwds->lastKeywordFound == NULL){
    return operator_CLEAR_Vars( false); 
  }
  switch(_kwds->lastKeywordFound->id){
    case _OPR_PROGRAM_KW:
      return operator_CLEAR_Program();
    case _OPR_CONST_KW:
      return operator_CLEAR_Vars( true);
    case _OPR_STACK_KW:
      _vars->clearRPNStack();
      _rsb->setStackRedrawAll();
      _skipToNextOperator( _lexer_position);
    return true;
    case _OPR_SUM_KW:
      _vars->clearRPNSum();
      _skipToNextOperator( _lexer_position);
      return true;
    case _OPR_SUMXY_KW:
      _vars->clearRPNSumXY();
      _skipToNextOperator( _lexer_position);
      return true;
    case _OPR_SUMS_KW:
      _vars->clearRPNSum();
      _vars->clearRPNSumXY();
      _skipToNextOperator( _lexer_position);
    return true;
    default:
      break;
  }
  return operator_CLEAR_Program(); 
}

bool Lexer::operator_CLEAR_Program(){
  // TODO
  Serial.println("CLEAR PROGRAM not yet implemented (Program class undefined)");
  byte *ptr = _epar->parse( _lexer_position);
  size_t lFrom = 0;
  if( _epar->result != _RESULT_INTEGER_){
    _operatorClearProgramCode( lFrom);
    _skipToNextOperator( _lexer_position);
    return true;
  }
  else{
    _lexer_position = ptr;
    _ignore_Blanks();
    lFrom = clipLineNumber(_epar->numberParser.integerValue());
  }
  if( _validate_NextCharacter( _COMMA_)){
    _operatorClearProgramCode( lFrom);
    _skipToNextOperator( _lexer_position);
    return true;
  }
  ptr = _epar->parse( _lexer_position);
  if( _epar->result == _RESULT_INTEGER_){
    _operatorClearProgramCode( lFrom, clipLineNumber(_epar->numberParser.integerValue(), lFrom));
    _skipToNextOperator( ptr);
    return true;
  }
  _operatorClearProgramCode( lFrom);
  _skipToNextOperator( ptr);
  return true;
}

bool Lexer::operator_CLEAR_Vars(bool constants){
  _ignore_Blanks();
  #ifdef __DEBUG
  Serial.println("CLEAR vars called");
  Serial.print("Evaluating: |");
  Serial.println((char *)_lexer_position);
  #endif
  byte *ptr = _lexer_position;
  if( *ptr == '*' && ptr[1] == '*' && ptr[2] == '*'){
    _skipToNextOperator( ptr);
    if( constants) _vars->removeAllConstants();
    else _vars->removeAllVariables();
    return true;
  }
  ptr = _epar->nameParser.parse( _lexer_position);
  _skipToNextOperator( ptr);
  if( !_epar->nameParser.result) return true;
  _vars->removeByName( _epar->nameParser.Name());
  return true;
}