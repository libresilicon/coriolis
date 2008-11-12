
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./DebugSession.h"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_DEBUG_SESSION_H__
#define  __HURRICANE_DEBUG_SESSION_H__


#include  <set>
#include  <stack>

#include  "hurricane/Commons.h"


namespace Hurricane {


  class Name;
  class Net;
  class Cell;

  using std::set;
  using std::stack;


// -------------------------------------------------------------------
// Class  :  "Hurricane::DebugSession".
 

  class DebugSession {

    public:
    // Static Access.
      static        DebugSession* create       ();
      static inline DebugSession* get          ();
      static inline bool          isTraced     ( const void* symbol );
      static inline void          isTracedNet  ( const Net* );
      static inline void          addToTrace   ( const void* symbol );
      static inline void          addToTrace   ( const Cell*, const Name& );
      static inline void          addToTrace   ( const Net* );
      static inline void          open         ( const void* symbol, unsigned int traceLevel=80 );
      static inline void          close        ();
    // Singleton Access.
             inline bool          _isTraced    ( const void* symbol ) const;
             inline void          _addToTrace  ( const void* symbol );
                    void          _addToTrace  ( const Cell*, const Name& );
             inline void          _addToTrace  ( const Net*  net );
    // Inspector Management.
                    Record*       _getRecord   () const;
                    string        _getString   () const;
                    string        _getTypeName () const;

    protected:
    // Internal: Attributes.
      static DebugSession*        _singleton;
             set<const void*>     _symbols;
             stack<unsigned int>  _levels;

    protected:
    // Internal: Constructor & Destructor.
                                  DebugSession ();
                                 ~DebugSession ();
    private:
                                  DebugSession ( const DebugSession& );
             DebugSession&        operator=    ( const DebugSession& );
  };


// Inline Functions.


  void  DebugSession::open ( const void* symbol, unsigned int traceLevel )
  {
    if ( _singleton->_isTraced(symbol) )
      _singleton->_levels.push ( ltracelevel(traceLevel) );
  }


  void  DebugSession::close ()
  {
    if ( !_singleton->_levels.empty() ) {
      ltracelevel ( _singleton->_levels.top() );
      _singleton->_levels.pop ();
    }
  }


  DebugSession* DebugSession::get         ()                     { return _singleton; }
  bool          DebugSession::isTraced    ( const void* symbol ) { return _singleton->_isTraced(symbol); }
  void          DebugSession::addToTrace  ( const void* symbol ) { _singleton->_addToTrace(symbol); }
  void          DebugSession::addToTrace  ( const Net*  net    ) { _singleton->_addToTrace ( net ); }
  void          DebugSession::addToTrace  ( const Cell* cell
                                          , const Name& name   ) { _singleton->_addToTrace ( cell, name ); }
  bool          DebugSession::_isTraced   ( const void* symbol ) const { return _symbols.find(symbol) != _symbols.end(); }
  void          DebugSession::_addToTrace ( const void* symbol ) { _symbols.insert ( symbol ); }
  void          DebugSession::_addToTrace ( const Net*  net    ) { _addToTrace ( static_cast<const void*>(net) ); }



} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::DebugSession);


#endif  // __HURRICANE_DEBUG_SESSION__
