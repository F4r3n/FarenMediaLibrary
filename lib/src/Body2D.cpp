#include "Body2D.h"
#include <iostream>
using namespace fmc;
void Body2D::startContact() {
    number_contact++;
}

void Body2D::endContact() {
    number_contact--;
}