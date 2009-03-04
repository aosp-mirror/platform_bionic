#include "typeinfo"
#include <stdlib.h>

type_info::type_info() {
}

type_info::~type_info() {
}

char const *
type_info::name() const {
    return "N/A";
}

bool 
type_info::operator==(type_info const & right) const {
    return false;
}

bool 
type_info::operator!=(type_info const & right) const {
    return false;
}

bool
type_info::before(type_info const & right) const {
    return false;
}

type_info::type_info(type_info const & right) {
}

