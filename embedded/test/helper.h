/*
 * This header file is used for tracking function calls for
 * the test environment
 */

#ifndef __HELPER_H__
#define __HELPER_H__

// #include <stdio.h>
// printf("%p\n",&static_payload.message);


#define PRINT(x) std::cout << x << std::endl;

#define PACKAGE(x) std::cout << "*********************" << std::endl << x << std::endl;
#define PACKAGE_END(x) std::cout << x << std::endl << std::endl;

#define LOWBYTE(v)   ((unsigned char) (v))
#define HIGHBYTE(v)  ((unsigned char) (((unsigned int) (v)) >> 8))

// send.cpp
static int c_send_radio = 0;
int called_send_radio();

#endif
