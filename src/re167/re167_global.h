#ifndef RE167_GLOBAL_H
#define RE167_GLOBAL_H

#include <Qt/qglobal.h>

#ifdef RE167_LIB
# define RE167_EXPORT Q_DECL_EXPORT
#else
# define RE167_EXPORT Q_DECL_IMPORT
#endif

// disable: "no suitable definition provided for explicit template
// instantiation request" Occurs for no justifiable reason on all
// #includes of Singleton
#   pragma warning( disable: 4661)

#endif // RE167_GLOBAL_H
