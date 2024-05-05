#include "EpollEvent.h"

bool EpollEvent::is_valid()
{
    return port_range.is_valid();
}