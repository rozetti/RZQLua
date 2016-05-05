#pragma once

template <typename TClass>
char const *ClassName();

#define DEFINE_CLASS_NAME(C) \
template <> char const *ClassName<C>() { return #C; }
