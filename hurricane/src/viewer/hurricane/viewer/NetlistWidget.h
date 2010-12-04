
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./NetlistWidget.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_NETLIST_WIDGET__
#define  __HURRICANE_NETLIST_WIDGET__


#include  <set>

#include  <QWidget>
#include  <QTableView>
#include  <QItemDelegate>
#include  <QSortFilterProxyModel>

#include  "hurricane/Commons.h"
#include  "hurricane/Bug.h"
#include  "hurricane/viewer/CellWidget.h"
#include  "hurricane/viewer/NetlistModel.h"
#include  "hurricane/viewer/CellWidget.h"


class QSortFilterProxyModel;
class QModelIndex;
class QTableView;
class QLineEdit;
class QComboBox;
class QHeaderView;


namespace Hurricane {


  using std::set;
  class Net;
  class Cell;


// -------------------------------------------------------------------
// Class  :  "SelectedNet".


  class SelectedNet {
    public:
      inline            SelectedNet   ();
      inline            SelectedNet   ( const Net*, size_t access=0 );
      inline const Net* getNet        () const;
      inline size_t     getAccesses   () const;
      inline void       incAccesses   () const;
      inline void       setInserted   () const;
      inline void       resetAccesses () const;
    private:
              const Net* _net;
      mutable size_t     _accesses;
  };


  inline            SelectedNet::SelectedNet   () : _net(NULL), _accesses(0) { }
  inline            SelectedNet::SelectedNet   ( const Net* net, size_t accesses ) : _net(net), _accesses(accesses) { }
  inline const Net* SelectedNet::getNet        () const { return _net; }
  inline void       SelectedNet::setInserted   () const { _accesses = 64; }
  inline size_t     SelectedNet::getAccesses   () const { return _accesses; }
  inline void       SelectedNet::incAccesses   () const { ++_accesses; }
  inline void       SelectedNet::resetAccesses () const { _accesses = 0; }


  struct SelectedNetCompare {
      inline bool operator() ( const SelectedNet& lhs, const SelectedNet& rhs );
  };


  inline bool SelectedNetCompare::operator() ( const SelectedNet& lhs, const SelectedNet& rhs )
  {
    return lhs.getNet()->getName() < rhs.getNet()->getName();
  }


// -------------------------------------------------------------------
// Class  :  "SelectedNetSet".


  class SelectedNetSet : public set<SelectedNet,SelectedNetCompare>{
    public:
      void  insert         ( const Net* );
      void  forceInserteds ();
      void  resetAccesses  ();
  };


  inline void  SelectedNetSet::insert ( const Net* net )
  {
    iterator  iselected = find(SelectedNet(net));
    if ( iselected != end() )
      iselected->incAccesses ();
    else
      set<SelectedNet,SelectedNetCompare>::insert ( SelectedNet(net,64) );
  }


  inline void  SelectedNetSet::forceInserteds ()
  {
    for ( iterator iselected=begin() ; iselected != end() ; ++iselected )
      iselected->setInserted ();
  }


  inline void  SelectedNetSet::resetAccesses ()
  {
    for ( iterator iselected=begin() ; iselected != end() ; ++iselected )
      iselected->resetAccesses ();
  }


// -------------------------------------------------------------------
// Class  :  "NetlistWidget".


  class NetlistWidget : public QWidget {
      Q_OBJECT;

    public:
                                     NetlistWidget        ( QWidget* parent=NULL );
      inline  Cell*                  getCell              ();
      template<typename InformationType>                  
              void                   setCellWidget        ( CellWidget* );
      template<typename InformationType>                  
              void                   setCell              ( Cell* );
              void                   goTo                 ( int );
              void                   updateSelecteds      ();
    signals:
              void                   netSelected          ( Occurrence );
              void                   netUnselected        ( Occurrence );
              void                   netFitted            ( const Net* );
    private slots:                                        
              void                   textFilterChanged    ();
              void                   updateSelecteds      ( const QItemSelection& , const QItemSelection& );
              void                   fitToNet             ();

    private:
              CellWidget*            _cellWidget;
              Cell*                  _cell;
              NetlistModel*          _baseModel;
              QSortFilterProxyModel* _sortModel;
              QTableView*            _view;
              QLineEdit*             _filterPatternLineEdit;
              int                    _rowHeight;
              SelectedNetSet         _selecteds;
              bool                   _forceReselect;
  };


  template<typename InformationType>
  void  NetlistWidget::setCellWidget ( CellWidget* cw )
  {
    if ( _cellWidget ) {
      disconnect ( this, 0, _cellWidget, 0 );
    }

    _cellWidget = cw;
    if ( _cellWidget ) {
      setCell<InformationType> ( _cellWidget->getCell() );
      connect ( this, SIGNAL(netFitted(const Net*)), _cellWidget, SLOT(fitToNet (const Net*)) );
    } else
      setCell<InformationType> ( NULL );
  }


  template<typename InformationType>
  void  NetlistWidget::setCell ( Cell* cell )
  {
    _cell = cell;
    _view->selectionModel()->clear ();
    _baseModel->setCell<InformationType> ( cell );
     
    string windowTitle = "Netlist" + getString(cell);
    setWindowTitle ( tr(windowTitle.c_str()) );

    _view->selectRow ( 0 );
    _view->resizeColumnToContents ( 0 );
  }


  inline  Cell* NetlistWidget::getCell () { return _cell; }


} // End of Hurricane namespace.


#endif // __HURRICANE_NETLIST_WIDGET_H__
