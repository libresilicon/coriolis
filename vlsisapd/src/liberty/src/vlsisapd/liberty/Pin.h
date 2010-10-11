#ifndef __LIB_PIN_H__
#define __LIB_PIN_H__

#include<fstream>
#include<vector>

#include "vlsisapd/liberty/Name.h"
#include "vlsisapd/liberty/Attribute.h"
#include "vlsisapd/liberty/Timing.h"

namespace LIB {

class Pin {
    public:
    Pin(Name name);
    
    inline Name getName();
    inline std::map<Name, Attribute*> getAttributes();
    inline std::vector<Timing*> getTimings();
    Attribute* getAttribute(Name attrName);
    Timing* getTiming(Name pinName);

    void addAttribute(Name attrName, Attribute::Type attrType, std::string& attrValue);
    void addTiming();

    void print();
    bool write(std::ofstream &file);

    private:
    Name _name;
    std::map<Name, Attribute*> _attributes;
    std::vector<Timing*> _timings;
};
    
inline Name Pin::getName() { return _name; };
inline std::map<Name, Attribute*> Pin::getAttributes() { return _attributes; };
inline std::vector<Timing*> Pin::getTimings() { return _timings; };
    
} // namespace LIB
#endif
