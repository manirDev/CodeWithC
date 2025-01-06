#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#define TRUE ( unsigned int )1
#define FALSE ( unsigned int )0

typedef unsigned char UInt8;
typedef unsigned short int UInt16;
typedef unsigned int UInt32;
typedef unsigned long long int UInt64;
typedef char Int8;
typedef short int Int16;
typedef int Int32;
typedef long long int Int64;
typedef float Real32;
typedef double Real64;

#define $i (Int8*)
#define $i2 (Int16)
#define $i4 (Int32)
#define $i8 (Int64)

#endif //!COMMON_TYPES_H