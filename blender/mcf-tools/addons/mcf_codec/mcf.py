
from enum import IntEnum

FILE_VERSION = 0

class McfBlockType(IntEnum):
	NONE    = 0
	VERTEX  = 1
	INDEX   = 2
	MAX     = 3

class McfComponentType(IntEnum):
	CHAR     = 0
	U8       = 1
	U16      = 2
	U32      = 3
	U64      = 4
	I8       = 5
	I16      = 6
	I32      = 7
	I64      = 8
	FLOAT    = 9
	DOUBLE   = 10
	MAX      = 11