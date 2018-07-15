#ifndef INCLUDE_ERROR_H
#define INCLUDE_ERROR_H

/* Human readable error codes */

typedef enum error_code {
	EC_SUCCESS,
	EC_FAILURE,
	EC_NULLPTR,
	EC_CORRUPT
} error_code;

#endif
