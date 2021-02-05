/*
 *   stdtypes.h     Standard Types used for VOS applications.
 */
#include "char_varying.h"
//#ifndef char_varying
//#define char_varying(maxlen_) \
//	  struct { short len ; char str [(maxlen_)]; }
//#endif
#define strcpy_vstr_nstr strcpy

typedef char_varying(32)      object_t;
typedef char_varying(66)      device_t;
typedef char_varying(66)      module_t;
typedef char_varying(256)     path_t;
typedef char_varying(300)     line_t;

