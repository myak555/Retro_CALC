/////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef _MATHFUNCTIONS_HPP
#define _MATHFUNCTIONS_HPP

#include "Keywords.hpp"
#include "Variables.hpp"

#define _MODE_DEGREES_  0
#define _MODE_RADIAN_   1 
#define _MODE_GRADIAN_  2

#define NMATH_RETURNS   3
#define _MATH_PI_       3.14159265359

struct MathFunction{
  int16_t id = 0;
  const char *name0;
  const char *name1;
  byte nArgs = 1;
  byte nRets = 1;
  size_t RPNtag = _RPN_COMMON_;
  size_t VarTag = 2;
};

class MathFunctions{
  public:
    //byte angleMode = _MODE_DEGREES_;

    void init( void *components[]);
    void setAngleMode(byte m);
    MathFunction *getFunction(byte *str);
    double *Compute( MathFunction *mf, double *args);
    double *Compute( MathFunction *mf, double arg);
    double *Compute( MathFunction *mf);
    double *quad( double *args);
    double *goff2( double *args);
  private:
    Variables *_vars;
    size_t _id;
    double _rets[NMATH_RETURNS];
    MathFunction _mf[NMATH_FUNCTIONS];
    double _gain = 1;
    double _offset = 0;
    void _addFunction( const char *name0, const char *name1, byte nArgs, byte nRets, byte RPNtag=_RPN_COMMON_);
    MathFunction *_setVariable( MathFunction *f, VariableToken vt);
    inline void _clearRets(){
      for(byte i=0; i<NMATH_RETURNS; i++)_rets[i] = 0.0; 
    };
};

#endif // _MATHFUNCTIONS_HPP
