
#ifndef NRFX_LOG_H__
#define NRFX_LOG_H__


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrfx_log nrfx_log.h
 * @{
 * @ingroup nrfx
 *
 * @brief This file contains macros that should be implemented according to
 *        the needs of the host environment into which @em nrfx is integrated.
 */

/**
 * @brief Macro for logging a message with the severity level ERROR.
 *
 * @param format  printf-style format string, optionally followed by arguments
 *                to be formatted and inserted in the resulting string.
 */
#define NRFX_LOG_ERROR(format, ...)

/**
 * @brief Macro for logging a message with the severity level WARNING.
 *
 * @param format  printf-style format string, optionally followed by arguments
 *                to be formatted and inserted in the resulting string.
 */
#define NRFX_LOG_WARNING(format, ...)

/**
 * @brief Macro for logging a message with the severity level INFO.
 *
 * @param format  printf-style format string, optionally followed by arguments
 *                to be formatted and inserted in the resulting string.
 */
#define NRFX_LOG_INFO(format, ...)

/**
 * @brief Macro for logging a message with the severity level DEBUG.
 *
 * @param format  printf-style format string, optionally followed by arguments
 *                to be formatted and inserted in the resulting string.
 */
#define NRFX_LOG_DEBUG(format, ...)


/**
 * @brief Macro for logging a memory dump with the severity level ERROR.
 *
 * @param[in] p_memory  Pointer to the memory region to be dumped.
 * @param[in] length    Length of the memory region in bytes.
 */
#define NRFX_LOG_HEXDUMP_ERROR(p_memory, length)

/**
 * @brief Macro for logging a memory dump with the severity level WARNING.
 *
 * @param[in] p_memory  Pointer to the memory region to be dumped.
 * @param[in] length    Length of the memory region in bytes.
 */
#define NRFX_LOG_HEXDUMP_WARNING(p_memory, length)

/**
 * @brief Macro for logging a memory dump with the severity level INFO.
 *
 * @param[in] p_memory  Pointer to the memory region to be dumped.
 * @param[in] length    Length of the memory region in bytes.
 */
#define NRFX_LOG_HEXDUMP_INFO(p_memory, length)

/**
 * @brief Macro for logging a memory dump with the severity level DEBUG.
 *
 * @param[in] p_memory  Pointer to the memory region to be dumped.
 * @param[in] length    Length of the memory region in bytes.
 */
#define NRFX_LOG_HEXDUMP_DEBUG(p_memory, length)


/**
 * @brief Macro for getting the textual representation of a given error code.
 *
 * @param[in] error_code  Error code.
 *
 * @return String containing the textual representation of the error code.
 */
#define NRFX_LOG_ERROR_STRING_GET(error_code)

/** @} */

#ifdef __cplusplus
}
#endif

#endif // NRFX_LOG_H__
