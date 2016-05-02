#include "Http.h"

Http::Http() :
    m_qlua(":/http.lua")
{
    emit closed();
}
