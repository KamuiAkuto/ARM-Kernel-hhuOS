#pragma once
#include <stdint.h>

#define VAL(...) #__VA_ARGS__
#define TOSTRING(...) VAL(__VA_ARGS__)

#define SET_SYS_REG_FCT(name)   inline void set(int64_t value) { \
                                    __asm __volatile__ (TOSTRING(MSR name, %[value])\
                                    : /*no out*/\
                                    : [value] "r" (value));\
                                }
                                        
#define GET_SYS_REG_FCT(name)   inline int64_t get() {\
                                    int64_t res = 0;\
                                    __asm __volatile__ (TOSTRING(MRS %[result], name) : [result] "=r" (res));\
                                    return res;\
                                }

#define STANDARD_FUNCTIONS(name)    SET_SYS_REG_FCT(name) \
                                    GET_SYS_REG_FCT(name)


#define SET_FIELD_FCT(fieldName, fieldEnd, fieldStart)    \
                                    inline void fieldName(int64_t value) {\
                                        int64_t mask = ((0xFFFFFFFFFFFFFFFF >> (fieldStart + fieldEnd + 1)) << fieldStart);\
                                        set((get() & ~mask) | (value & mask));\
                                    }

#define GET_FIELD_FCT(fieldName, fieldEnd, fieldStart)    \
                                    inline int64_t fieldName() {\
                                        return get() & ((0xFFFFFFFFFFFFFFFF >> (fieldStart + fieldEnd + 1)) << fieldStart);\
                                    }

#define FIELD_FUNCTIONS(fieldName, fieldEnd, fieldStart) \
                                    SET_FIELD_FCT(fieldName, fieldEnd, fieldStart) \
                                    GET_FIELD_FCT(fieldName, fieldEnd, fieldStart)


#define FIELD_BIT_FCT(fieldName, bit)   inline void fieldName(bool value) {\
                                            value = !!value; /*force bolean*/\
                                            int64_t data = get();\
                                            data = (data & ~(1 << bit)) | (value << bit);\
                                            set(data);\
                                        }\
                                        inline bool fieldName() {\
                                            return ((get() & (1 << bit)) >> bit);\
                                        }

