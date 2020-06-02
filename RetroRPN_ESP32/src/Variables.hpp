/////////////////////////////////////////////////////////
//
//  RetroRPN - "Электроника МК-90" reborn
//  Copyright (c) 2019 Pavel Travnik.  All rights reserved.
//  See main file for the license
//
//////////////////////////////////////////////////////////

#ifndef _VARIABLES_HPP
#define _VARIABLES_HPP

#include "Keywords.hpp"

#define VARIABLE_SPACE    32000
#define RPN_PI            3.14159265359
#define CURRENT_DIR_LEN   255

#define VARTYPE_NONE      0
#define VARTYPE_NUMBER    1
#define VARTYPE_VECTOR    2
#define VARTYPE_MATRIX    3
#define VARTYPE_STRING    4

#define VariableToken size_t

//
// Variables are placed in the variable space as following:
//
// byte  0      - variable type
// byte  1      - variable name length (N)
// bytes 2 to N+2 - variable name (null-terminated)
// Then:
// byte  N+3   - either an 8-byte double or an 8-byte integer
// Or:
// byte  N+3   - total length as a 2-byte unsigned integer
// bytes N+5 and above - either a null-terminated string or a vector of 8-byte numbers
// Or:
// byte  N+3   - total length as a 2-byte unsigned integer
// byte  N+5   - row length as a 2-byte unsigned integer
// byte  N+7 and above - a matrix of numbers composed by row
//

class Variables{
  public:
    size_t _var_bottom = 0;
    size_t _standard_bottom = 0;
    size_t _const_top = VARIABLE_SPACE;
    size_t _standard_top = VARIABLE_SPACE;
    byte _buffer[ VARIABLE_SPACE];
    char *currentDir;
    byte *scrMessage;
    byte *rpnLabelX;
    byte *rpnLabelY;
    byte *rpnLabelZ;
    double *gain;
    double *offset;

    void init( void *components[]);

    //
    // List available variables or constants
    //
    inline VariableToken getFirstVar(){ return 2;};
    inline VariableToken getFirstConst(){ return _const_top+2;};
    VariableToken getNextVar( VariableToken vt);
    inline byte getVarType( VariableToken vt){ return _buffer[vt-2];};
    inline byte getVarNameLength( VariableToken vt){ return _buffer[vt-1];};
    inline byte *getVarName( VariableToken vt){ return _buffer + vt;};
    uint16_t getTotalSize( VariableToken vt);
    uint16_t getRowSize( VariableToken vt);
    uint16_t getColumnSize( VariableToken vt);
    inline bool isConstant( VariableToken vt){ return vt>_const_top;};
    inline bool isReadOnly( VariableToken vt){ return vt>_standard_top;};
    inline bool isUnremovable( VariableToken vt){
        if( vt < 2) return true; 
        if( vt-2 <=_standard_bottom) return true;
        return vt > _standard_top;};
    byte nameEndsWith( VariableToken vt);
    bool isNameBASICReal( VariableToken vt);
    bool isNameBASICString( VariableToken vt);
    bool isNameBASICInteger( VariableToken vt);

    //
    // Set and return values
    //
    void setValueReal( VariableToken vt, double v, uint16_t i=0, uint16_t j=0);
    void setValueInteger( VariableToken vt, int64_t v, uint16_t i=0, uint16_t j=0);
    inline void setValueInteger( VariableToken vt, int v, uint16_t i=0, uint16_t j=0){
        setValueInteger( vt, (int64_t)v, i, j);};
    inline void setValueInteger( VariableToken vt, long v, uint16_t i=0, uint16_t j=0){
        setValueInteger( vt, (int64_t)v, i, j);};
    void setValueString( VariableToken vt, const char *v);
    void setValueRealArray( VariableToken vt, double v);
    void setValueIntegerArray( VariableToken vt, int64_t v);
    inline void setValueIntegerArray( VariableToken vt, long v){
        setValueIntegerArray( vt, (int64_t)v);};
    inline void setValueIntegerArray( VariableToken vt, int v){
        setValueIntegerArray( vt, (int64_t)v);};
    double realValue( VariableToken vt, uint16_t i=0, uint16_t j=0);
    int64_t integerValue( VariableToken vt, uint16_t i=0, uint16_t j=0);
    byte *stringValue( VariableToken vt);

    //
    // Looks for constants and variables
    //
    VariableToken findByName( const char *name);
    inline VariableToken findByName( byte *name){
        return findByName( (const char *)name);};
    VariableToken _findConstantByName( const char *name);
    inline VariableToken _findConstantByName( byte *name){
        return _findConstantByName( (const char *)name);};
    VariableToken _findVariableByName( const char *name);
    inline VariableToken _findVariableByName( byte *name){
        return _findVariableByName( (const char *)name);};
    VariableToken getOrCreateNumber( bool asConstant, byte *name);
    byte *findDataPtr( const char * name, uint16_t i=0, uint16_t j=0);

    // TODO
    VariableToken getOrCreate2( bool asConstant, byte *name);
    inline VariableToken getOrCreate2( bool asConstant, const char *name){
        return getOrCreate2( asConstant, (byte *)name);}

    void removeByToken( VariableToken vt);
    void removeByName( const char *name);
    inline void removeByName( byte *name){
        removeByName( (const char *)name);};
    inline void removeAllVariables(){ _var_bottom = _standard_bottom;};
    inline void removeAllConstants(){ _const_top = _standard_top;};
    
    // VariableToken removeVariable( const char *name);
    // VariableToken removeConstant( const char *name);

    //
    // RPN banging methods
    //
    void swapRPNXY();
    void popRPNStack(byte start=1);
    void pushRPNStack();
    void pushRPNStack( double v);
    inline double getRPNRegister( byte i=0){ return _rpnStack[i];};
    inline void setRPNRegister( double v, byte i=0){ _rpnStack[i] = v;};
    inline void negateRPNX(){ _rpnStack[0] = -_rpnStack[0];};    
    inline void inverseRPNX(){ _rpnStack[0] = 1.0/_rpnStack[0];};    
    inline void clearRPNStack(){
      for( byte i=0; i<RPN_STACK; i++) _rpnStack[i] = 0.0;};
    inline double *getRPNStackPtr(){ return _rpnStack;};
    inline double getRPNPrev(){ return *_prev;};
    inline void setRPNPrev( double v){ *_prev = v;};
    inline void saveRPNPrev( byte i=0){ *_prev = _rpnStack[i];};
    inline void restoreRPNPrev(){ _rpnStack[0] = *_prev;};
    inline bool isRPNRegisterZero( byte i=0){ return _rpnStack[i] == 0.0;};

    //
    // angle mode is used in trig computations
    //
    inline byte getAngleMode(){ return (byte)(*_amode);};
    inline void setAngleMode( byte m){
      if(m != _MODE_RADIAN_ && m != _MODE_GRADIAN_) m = _MODE_DEGREES_;
      *_amode = (int64_t)m;};
    double getConvertedAngle( double a); // converts argument to radians
    double getUnconvertedAngle( double a); // converts argument to current angle representation
    inline double getConvertedAngleRPNX(){return getConvertedAngle( _rpnStack[0]);};

    //
    // returns available variable memory
    //
    inline size_t getMemoryAvailable(){
        return _const_top - _var_bottom;};
    inline void setMemoryAvailable( size_t prg = 0){
        *_varAvailble = (int64_t)getMemoryAvailable();
        *_prgAvailble = (int64_t)prg;};

  private:
    Keywords *_kwds;
    double *_rpnStack = NULL;
    double *_prev = NULL;
    int64_t *_amode = NULL;
    int64_t *_varAvailble = NULL;
    int64_t *_prgAvailble = NULL;

    int64_t _limitHuge( double v);
    uint16_t _getVarLength( uint16_t nameLen, byte vtype=VARTYPE_NONE,
                          uint16_t total_size=0);
    byte *_getLengthBlockPtr( VariableToken vt);
    byte *_getVarBlockPtr( VariableToken vt);
    byte *_getDataPtr( VariableToken vt, uint16_t i=0, uint16_t j=0);
    VariableToken _allocateVarSpace( bool isConst,
        uint16_t nameLen, byte varType, uint16_t totalSize);
    VariableToken _placeVarName( bool isConst, const char *name,
        byte varType, uint16_t totalSize);
    VariableToken _placeNumber( bool isConst,
        const char *name, double value=0.0);
    VariableToken _placeVector( bool isConst,
        const char *name, uint16_t length, double value=0.0);
    VariableToken _placeMatrix( bool isConst,
        const char *name, uint16_t rows, uint16_t cols, double value=0.0);
    VariableToken _placeString( bool isConst,
        const char *name, uint16_t length, const char* value=NULL);
    VariableToken _getNextVar( VariableToken vt);
    void _removeVariable( VariableToken vt);
    void _removeConstant( VariableToken vt);
};

#endif // _VARIABLES_HPP
