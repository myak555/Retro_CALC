#ifndef RRPN_Calculator_h
#define RRPN_Calculator_h

/*
  RPN_Calculator.h 
  Copyright (c) 2019 Pavel Travnik.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Keyboard commands:
  1 - arrow left - moves input cursor to the left 
  2 - arrow down - pop stack down
  3 - arrow up - push stack; previous result to X
  4 - arrow right - moves input cursor to the right
  5 - enter - push stack
  8 - backspace - delete character at cursor
  177 - +/- - sign change
*/

#include "RRPN_Parser.h"
#include "LCD_Console.h"

// uncomment for debugging
//#define RRPN_DEBUG_

#define STACK_SIZE 16
#define LABEL_SIZE 51
#define INPUT_SIZE 18

#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

class RRPN_Calculator
{    
  public:
    double stack[STACK_SIZE];
    double previousX;
    char input[ LABEL_SIZE];
    int cursorPosition = 0;
    void *init( void *xram, LCD_Console *terminal);
    inline void display(){
      _terminal->displayRPN( input, stack, _labels);
      };
    inline bool inputEmpty(){
      return !input[0];
      };
    void send( char c);
    void changeLabel( byte n, char *lbl);
    void changeLabel_P( byte n, const char *lbl);
    void setLabels( const char *lbl);
    void resetLabels();
    bool parseInput();
    bool unparseInput();
    void pop( size_t head=0);
    void push();
    void prev();
  private:
    LCD_Console *_terminal;
    volatile char *_labels[3];
    bool _command_expected = false;
    bool _exponent_allowed();
    bool _decimal_allowed();
    bool _greek_allowed();
    void _clearInput();
    void _insertCharacter( byte c);
    void _backspaceCharacter();
    void _deleteCharacter();
    void _changeInputSign();
    bool _processCommand( byte c);
    bool _processCharacterInput( byte c);
    bool _processOperator( byte c);
};

#endif
