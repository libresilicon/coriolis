
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
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Grid.cpp"                               |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  <cstdlib>
# include  <sstream>
# include  <algorithm>

# include  "katabatic/Grid.h"


namespace Katabatic {


  using namespace std;
  using namespace Hurricane;


// -------------------------------------------------------------------
// Class  :  "Katabatic::BaseGrid::Axis".


  void  BaseGrid::Axis::sort ()
  {
    std::sort ( _graduations.begin(), _graduations.end() );
  }


  unsigned int  BaseGrid::Axis::getGraduationNumber ( DbU::Unit position, bool& onGraduation ) const
  {
    vector<DbU::Unit>::const_iterator  it = lower_bound ( _graduations.begin()
                                                        , _graduations.end()
                                                        , position+1
                                                        );
    unsigned int  index = distance ( _graduations.begin(), it ) - 1;

    onGraduation = ( _graduations[index] == position );

    return index;
  }


  string  BaseGrid::Axis::_getString () const
  {
    return "<" + _getTypeName() + " ["
               + DbU::getValueString(_graduations.front()) + ":"
               + DbU::getValueString(_graduations.back() ) + "] "
               + getString(getSize()) + ">";
  }


  string  BaseGrid::Axis::_print () const
  {
    string  repr = "[";

    vector<DbU::Unit>::const_iterator   it = _graduations.begin();
    vector<DbU::Unit>::const_iterator  end = _graduations.end();
    for ( ; it != end ; it++ ) {
      repr += DbU::getValueString(*it);
      if ( it+1 != end ) repr += ",";
    }
    repr += "]";

    return repr;
  }


  Record* BaseGrid::Axis::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot ( "_graduations", &_graduations ) );
                                     
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "Katabatic::BaseGrid::Axis".


  BaseGrid::BaseGrid () : _xGraduations()
                        , _yGraduations()
                        , _rows(0)
                        , _columns(0)
                        , _rawSize(0)
  { }


  void  BaseGrid::_postCreate ()
  { }


  BaseGrid::~BaseGrid ()
  { }


  void  BaseGrid::_preDestroy ()
  { }


  Record* BaseGrid::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot ( "_xGraduations", &_xGraduations ) );
    record->add ( getSlot ( "_yGraduations", &_yGraduations ) );
    record->add ( getSlot ( "_rows"        , &_rows         ) );
    record->add ( getSlot ( "_columns"     , &_columns      ) );
    record->add ( getSlot ( "_rawSize"     , &_rawSize      ) );
    return record;
  }


} // End of Katabatic namespace.
