#include "utils_init.h"

#define HASNOR_INIT
#include "utils_array.h"
#include "utils_bytestream.h"
#include "utils_ctools.h"
#include "utils_file.h"
#include "utils_list.h"
#include "utils_map.h"
#include "utils_matrix.h"
#include "utils_quaternion.h"
#include "utils_string.h"
#include "utils_time.h"
#include "utils_vector.h"
#undef HASNOR_INIT

void initHasnorUtils(void)
{
	initArrayFunctions();
	initByteStreamFunctions();
	initMemoryFunctions();
	initFileFunctions();
	initListFunctions();
	initMapFunctions();
	initMatrixFunctions();
	initQuaternionFunctions();
	initStringFunctions();
	initTimeFunctions();
	initVectorFunctions();
}