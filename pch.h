#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <random>
#include <cmath>
#include <algorithm>
#include <functional>
#include <limits>
#include <fstream>
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#include<io.h>
#include<direct.h>
#include<fstream>
#include<string>
#include<sstream>
#include<cstring>
#include<climits>
#include<set>
#pragma warning(disable:4996)
#ifndef _MSC_VER
inline int fopen_s(FILE** stream, const char* filename, const char* mode)
{
	*stream = fopen(filename, mode);
	return *stream ? 0 : 1;
}
#define sprintf_s sprintf
#endif

#endif //PCH_H
