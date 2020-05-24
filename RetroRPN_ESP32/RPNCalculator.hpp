//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

/*
  RPN_Calculator.hpp 
  Implements an RPN terminal for immediate BASIC execution

  Keyboard commands, additional to Command Line:
  1 - arrow down - SWAP X and Y
  2 - LF - "silent" command line execution (stack operation follows)
  3 - CR = enter - command line execution
  4 - (+/-) (177) - sign change
*/

#ifndef RPNCALCULATOR_HPP
#define RPNCALCULATOR_HPP

#include <Arduino.h>
#include "./src/Keywords.hpp"
#include "./src/Variables.hpp"
#include "./src/IOManager.hpp"
#include "./src/LCDManager.hpp"
#include "./src/RPNStackBox.hpp"
#include "./src/MessageBox.hpp"
#include "./src/CommandLine.hpp"
#include "SDManager.hpp"
#include "./src/Parser.hpp"

#define INPUT_COLS    256
#define INPUT_LIMIT   255
#define HSCROLL_LIMIT  18

// Move must be one less than RPN_STACK times sizeof( double)
#define RPN_MOVE   (RPN_STACK-1)*sizeof( double)

class RPNCalculator{
  public:
    byte nextUI = UI_UNDEFINED;
    bool expectCommand = false;
    unsigned long init( void *components[]);
    unsigned long tick();
    void show();
    void redraw();
    void sendChar( byte c);
    void processInput( bool silent=false);
    void push( bool refresh=true);
    void pop( bool refresh=true);
    void swap( bool refresh=true);
    void roll( bool refresh=true);
    void prev( bool refresh=true);
    void add( bool refresh=true);
    void subtract( bool refresh=true);
    void multiply( bool refresh=true);
    void divide( bool refresh=true);
    void power( bool refresh=true);
    void signchange( bool refresh=true);
    void quad( bool refresh=true);
    void goff2( bool refresh=true);
    void loadState();
    void saveState();
    void updateIOM( bool refresh=true);
    void resetRPNLabels( bool refresh=true);
    void setRPNLabel( byte label, byte *message, bool refresh=true);
    inline void setRPNLabel( byte label, char *message, bool refresh=true){
      setRPNLabel( label, (byte *)message, refresh);
    };
    inline void setRPNLabel( byte label, const char *message, bool refresh=true){
      setRPNLabel( label, (byte *)message, refresh);
    };
    inline void setStackRedraw(){
      _rsb->setStackRedraw();
    };
  private:
    byte *_io_buffer;
    IOManager *_iom;
    Variables *_vars;
    LCDManager *_lcd;
    SDManager *_sd;
    ExpressionParser *_ep;
    RPNStackBox *_rsb;
    MessageBox *_mb;
    CommandLine *_cl;
    void _processCommand(byte c);
    void _evaluateCommand();
    void _evaluateString();
    void _checkTrigAccuracy();
    void _swapQuick();
    void _popPartial();
    void _popPartial( double v);
    void _pushQuick();
    void _pushQuick(double v);
    void _popQuick(byte start=1);
    inline void _setRedrawAndUpdateIOM( bool refresh){    
      setStackRedraw();
      updateIOM(refresh);
    };
    inline void _savePopAndUpdate( double v, bool refresh) {
      _vars->rpnSavePreviousX();
      _popPartial(v);
      updateIOM(refresh);
    };
};

#endif //RPNCALCULATOR_HPP
