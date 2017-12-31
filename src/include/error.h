#ifndef INCLUDE_ERROR_H
#define INCLUDE_ERROR_H

	/* Human readable error codes */

	typedef enum error_code {
		EC_SUCCESS,
		EC_FAILURE,
		EC_NULLPTR,
		EC_CORRUPT
	} error_code;

	/*typedef int error_code;
	#define ERC_SUCCESS 0
	#define ERC_FAILURE 1
	#define ERC_NULL    2 
	#define ERC_CORRUPT 3
	#define */
#endif
