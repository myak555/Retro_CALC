//////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include "RPNCalculator.hpp"
#include "./src/TerminalBox.hpp"

class FileManager{
  public:
    unsigned long init(void *components[]);
    unsigned long tick();
    void show();
    void redraw();
    void sendChar( byte c);
    void processInput(bool silent=false);
    void updateIOM(bool refresh=true);
  private:
    byte *_io_buffer;
    IOManager *_iom;
    Variables *_vars;
    Functions *_funs;
    LCDManager *_lcd;
    SDManager *_sdm;
    ExpressionParser *_epar;
    Lexer *_lex;
    CommandLine *_clb;
    RPNStackBox *_rsb;
    MessageBox *_mbox;
    RPNCalculator *_rpn;
    TerminalBox *_trm;
    void _evaluateString();
};

#endif // FILEMANAGER_HPP
