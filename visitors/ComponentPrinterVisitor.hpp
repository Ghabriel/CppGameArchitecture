#ifndef COMPONENT_PRINTER_HPP
#define COMPONENT_PRINTER_HPP

#include <iostream>

struct ComponentPrinterVisitor {
    void visit(Health& health) {
        std::cout << "Health(HP = " << health.health << ")\n";
    }

    void visit(Position& position) {
        std::cout << "Position(x = " << position.x
            << ", y = " << position.y << ")\n";
    }
};

#endif