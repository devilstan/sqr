#include "qdr_version.h"

//=================================================================================
// qdr_use_cairo_version
//=================================================================================
int qdr_use_cairo_version()
{
	return cairo_version();
}

//=================================================================================
// qdr_use_pikl_version
//=================================================================================
int qdr_use_pikl_version()
{
	return pikl_version();
}

