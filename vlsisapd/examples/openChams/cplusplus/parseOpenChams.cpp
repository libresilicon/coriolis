#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;

#include "vlsisapd/openChams/Circuit.h"
#include "vlsisapd/openChams/Name.h"
#include "vlsisapd/openChams/Parameters.h"
#include "vlsisapd/openChams/Netlist.h"
#include "vlsisapd/openChams/Instance.h"
#include "vlsisapd/openChams/Device.h"
#include "vlsisapd/openChams/Net.h"
#include "vlsisapd/openChams/Transistor.h"
#include "vlsisapd/openChams/Schematic.h"
#include "vlsisapd/openChams/Sizing.h"
#include "vlsisapd/openChams/Operator.h"
#include "vlsisapd/openChams/Layout.h"
#include "vlsisapd/openChams/Node.h"
#include "vlsisapd/openChams/Port.h"
#include "vlsisapd/openChams/Wire.h"
#include "vlsisapd/openChams/OpenChamsException.h"

void printHBTree(OpenChams::Node* node, unsigned indent) {
    if (!node) return; // since we pass nnode->getRight and node-getTop without checking for NULL
    for (unsigned i = 0 ; i < indent ; i++) {
        cerr << " |";
    }
    string pos = "";
    switch(node->getPosition()) {
        case OpenChams::Node::TOP:
            pos = "top";
            break;
        case OpenChams::Node::RIGHT:
            pos = "right";
            break;
        default:
            break;
    }
    OpenChams::Bloc* bloc = dynamic_cast<OpenChams::Bloc*>(node);
    if (bloc) {
        cerr << " bloc: " << bloc->getName().getString() << " - " << pos << endl;
        printHBTree(bloc->getTop()  , indent+1);
        printHBTree(bloc->getRight(), indent+1);
        return;
    }
    OpenChams::Group* group = dynamic_cast<OpenChams::Group*>(node);
    if (group) {
        string align = "none";
        switch(group->getAlign()) {
            case OpenChams::Group::VERTICAL:
                align = "vertical";
                break;
            case OpenChams::Group::HORIZONTAL:
                align = "horizontal";
                break;
            default:
                break;
        }
        cerr << " group: " << group->getName().getString() << " - " << pos << " - align: " << align << " - isolated: " << group->isIsolated() << " - paired: " << group->isPaired() << endl; 
        printHBTree(group->getRootNode(), indent+1);
        printHBTree(group->getTop()     , indent+1);
        printHBTree(group->getRight()   , indent+1);
        return;
    }
    cerr << "[ERROR] printHBTree: node is nor a bloc nor a group !" << endl;
    return;
}

int main(int argc, char * argv[]) {
    string file = "";
    if (argc == 1)
        file = "./inverter.xml";
    else if (argc == 2)
        file = argv[1];
    else {
        cerr << "Usage: openChamsParser [filename]" << endl;
        exit(1);
    }

    OpenChams::Circuit* circuit = NULL;
    try {
        circuit = OpenChams::Circuit::readFromFile(file);
    } catch (OpenChams::OpenChamsException& e) {
        cerr << e.what() << endl;
        exit(48);
    }

    cerr << circuit->getName().getString() << endl;
    cerr << " + parameters" << endl;
    OpenChams::Parameters params = circuit->getParameters();
    if (!params.isEmpty()) {
        for (map<OpenChams::Name, string>::const_iterator it = params.getValues().begin() ; it != params.getValues().end() ; ++it) {
            cerr << " | | " << ((*it).first).getString() << " : " << (*it).second << endl;
        }
    }
    cerr << " + netlist" << endl;
    cerr << " | + instances" << endl;
    OpenChams::Netlist* netlist = circuit->getNetlist();
    if (netlist && !netlist->hasNoInstances()) {
        for (size_t i = 0 ; i < netlist->getInstances().size() ; i++) {
            OpenChams::Instance* inst = netlist->getInstances()[i];
            OpenChams::Device*   dev  = NULL;
            if (dynamic_cast<OpenChams::Device*>(inst)) {
                dev = static_cast<OpenChams::Device*>(inst);
                cerr << " | | + " << dev->getName().getString() << " : " << dev->getModel().getString() << " - " << dev->getOrder() << " - " << dev->getMosType().getString() << " - " << (dev->isSourceBulkConnected()?"true":"false") << endl;
            } else {
                cerr << " | | + " << inst->getName().getString() << " : " << inst->getModel().getString() << " - " << inst->getOrder() << endl;
            }
            cerr << " | | | + connectors" << endl;
            for (map<OpenChams::Name, OpenChams::Net*>::const_iterator cit = inst->getConnectors().begin() ; cit != inst->getConnectors().end() ; ++cit) {
                if ((*cit).second)
                    cerr << " | | | | " << ((*cit).first).getString() << " : " << ((*cit).second)->getName().getString() << endl;
                else
                    cerr << " | | | | " << ((*cit).first).getString() << endl; // no net connected !
            }
            if (dev) {
                cerr << " | | | + transistors" << endl;
                for (size_t j = 0 ; j < dev->getTransistors().size() ; j++) {
                    OpenChams::Transistor* tr = dev->getTransistors()[j];
                    cerr << " | | | | name: " <<  tr->getName().getString() << " - gate: " << tr->getGate().getString() << " - source: " << tr->getSource().getString() << " - drain: " << tr->getDrain().getString() << " - bulk: " << tr->getBulk().getString() << endl;
                }
            }
        }
    }
    cerr << " | + nets" << endl;
    bool schematicNet = false; // define wether net sections are needed in schematic section
    if (!netlist->hasNoNets()) {
        for (size_t i = 0 ; i < netlist->getNets().size() ; i++) {
            OpenChams::Net* net = netlist->getNets()[i];
            cerr << " | | + " << net->getName().getString() << " : " << net->getType().getString() << " - " << (net->isExternal()?"true":"false") << endl;
            cerr << " | | | + connections" << endl;
            for (size_t j = 0 ; j < net->getConnections().size() ; j++) {
                OpenChams::Net::Connection* connect = net->getConnections()[j];
                cerr << " | | | | " << connect->getInstanceName().getString() << "." << connect->getConnectorName().getString() << endl;
            }
            if (!net->hasNoPorts() || !net->hasNoWires())
                schematicNet = true;
        }
    }
    OpenChams::Schematic* schematic = circuit->getSchematic();
    if (schematic && !schematic->hasNoInstances()) {
        cerr << " + schematic" << endl;
        for (map<OpenChams::Name, OpenChams::Schematic::Infos*>::const_iterator sit = schematic->getInstances().begin() ; sit != schematic->getInstances().end() ; ++sit) {
            OpenChams::Schematic::Infos* inf = (*sit).second;
            cerr << " | + instance:  name: " << ((*sit).first).getString() << " - x: " << inf->getX() << " - y: " << inf->getY() << " - orientation: " << inf->getOrientation().getString() << endl;
        }
        if (schematicNet) {
            for (size_t i = 0 ; i < netlist->getNets().size() ; i++) {
                OpenChams::Net* net = netlist->getNets()[i];
                cerr << " | + net  name: " << net->getName().getString() << endl;
                if (!net->hasNoPorts()) {
                    for (size_t j = 0 ; j < net->getPorts().size() ; j++) {
                        OpenChams::Port* port = net->getPorts()[j];
                        cerr << " | | + port  type: " << port->getType().getString() << " - idx: " << port->getIndex() << " - x: " << port->getX() << " - y: " << port->getY() << " - orientation: " << port->getOrientation().getString() << endl;
                    }
                }
                if (!net->hasNoWires()) {
                    for (size_t j = 0 ; j < net->getWires().size() ; j++) {
                        OpenChams::Wire* wire = net->getWires()[j];
                        cerr << " | | + wire  ";
                        OpenChams::WirePoint* start = wire->getStartPoint();
                        if (dynamic_cast<OpenChams::InstancePoint*>(start)) {
                            OpenChams::InstancePoint* iP = static_cast<OpenChams::InstancePoint*>(start);
                            cerr << "<" << iP->getName().getString() << "," << iP->getPlug().getString() << "> ";
                        } else if (dynamic_cast<OpenChams::PortPoint*>(start)) {
                            OpenChams::PortPoint* pP = static_cast<OpenChams::PortPoint*>(start);
                            cerr << "<" << pP->getIndex() << "> ";
                        }
                        for (size_t k = 0 ; k < wire->getIntermediatePoints().size() ; k++) {
                            OpenChams::IntermediatePoint* iP = wire->getIntermediatePoints()[k];
                            cerr << "<" << iP->getX() << "," << iP->getY() << "> ";
                        }
                        OpenChams::WirePoint* end = wire->getEndPoint();
                        if (dynamic_cast<OpenChams::InstancePoint*>(end)) {
                            OpenChams::InstancePoint* iP = static_cast<OpenChams::InstancePoint*>(end);
                            cerr << "<" << iP->getName().getString() << "," << iP->getPlug().getString() << "> ";
                        } else if (dynamic_cast<OpenChams::PortPoint*>(end)) {
                            OpenChams::PortPoint* pP = static_cast<OpenChams::PortPoint*>(end);
                            cerr << "<" << pP->getIndex() << "> ";
                        }
                        cerr << endl;
                    }
                }

            }
        }

    }
    OpenChams::Sizing* sizing = circuit->getSizing();
    if (sizing) {
        cerr << " + sizing" << endl;
        if (!sizing->hasNoOperators()) {
            for (map<OpenChams::Name, OpenChams::Operator*>::const_iterator oit = sizing->getOperators().begin() ; oit != sizing->getOperators().end() ; ++oit) {
                OpenChams::Operator* op = (*oit).second;
                cerr << " | + instance name: " << ((*oit).first).getString() << " - operator: " << op->getName().getString() << " - simulModel: " << op->getSimulModel().getString() << endl;
                if (!op->hasNoConstraints()) {
                    for (map<OpenChams::Name, OpenChams::Operator::Constraint*>::const_iterator cit = op->getConstraints().begin() ; cit != op->getConstraints().end() ; ++cit) {
                        OpenChams::Operator::Constraint* cstr = (*cit).second;
                        cerr << " | | + param: " << ((*cit).first).getString() << " - ref: " << cstr->getRef().getString() << " - refParam: " << cstr->getRefParam().getString() << " - factor: " << cstr->getFactor() << endl;
                    }
                }
            }
        }
        // To update to the new equations.
        // if (!sizing->hasNoEquations()) {
        //     cerr << " | + equations" << endl;
        //     for (map<OpenChams::Name, string>::const_iterator eit = sizing->getEquations().begin() ; eit != sizing->getEquations().end() ; ++eit) {
        //         cerr << " | | " << ((*eit).first).getString() << " : " << (*eit).second << endl;
        //     }
        // }
    }
    OpenChams::Layout* layout = circuit->getLayout();
    if (layout) {
       if (!layout->hasNoInstance()) {
          cerr << " + layout" << endl;
          for (map<OpenChams::Name, OpenChams::Name>::const_iterator lit = layout->getInstances().begin() ; lit != layout->getInstances().end() ; ++lit) {
             cerr << " | | instance name: " << ((*lit).first).getString() << " - style: " << ((*lit).second).getString() << endl;
          }
       }
       OpenChams::Node* root = layout->getHBTreeRoot();
       if (root) {
           cerr << " | + hbtree" << endl;
           printHBTree(root, 2);
       }
    }


    return 0;
}

