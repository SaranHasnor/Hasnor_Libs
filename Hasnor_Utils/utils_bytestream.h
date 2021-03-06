#ifndef HASNOR_BYTESTREAM_DEFINED
#define HASNOR_BYTESTREAM_DEFINED

#include "utils_types.h"

typedef struct {
	byte	*data;
	uint	len;
	uint	cursor;
} bytestream_t;

typedef struct {
	void (*init)(bytestream_t *stream, uint size);
	int (*write)(bytestream_t *stream, byte *data, uint size);
	int (*read)(bytestream_t *stream, byte *out, uint size);
	void (*free)(bytestream_t *stream);
	char* (*toString)(bytestream_t *stream);
} _bytestream_functions;

_bytestream_functions ByteStream;

#ifdef HASNOR_INIT
void initByteStreamFunctions(void);
#endif

#endif