typedef char   INT;
typedef unsigned char UCHAR;
typedef unsigned short UINT;
typedef unsigned long ULONG;

#define MC_GET_CHAR(__data__)  (*((char *)(__data__)))

#define MC_GET_SHORT(__data__)  ((UINT)(( ((UINT)(*((char *)(__data__))))<<8 )|( ((UINT)(*((char *)(__data__)+1)))&0x00ff )))

#define MC_GET_LONG(__data__)  ( ( ((ULONG)MC_GET_SHORT((__data__)))<<16 )|( ((ULONG)MC_GET_SHORT((char *)(__data__)+2))&0x0000ffff ) )
#define MC_GET_3BN(__data__)  ( ( ((ULONG)MC_GET_CHAR((__data__)))<<16 )|( ((ULONG)MC_GET_SHORT((char *)(__data__)+1))&0x0000ffff ) )

#pragma once
