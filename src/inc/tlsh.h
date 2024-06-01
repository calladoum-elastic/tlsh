#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "tlsh_impl.h"
#include "tlsh_version.h"

// -force option no longer used
// #define	TLSH_OPTION_FORCE	1
#define TLSH_OPTION_CONSERVATIVE 2
#define TLSH_OPTION_KEEP_BUCKET 4
#define TLSH_OPTION_PRIVATE 8
#define TLSH_OPTION_THREADED 16

#if NB_TLSH_BUCKETS == 256
#define TLSH_STRING_LEN_REQ 136
#define MIN_DATA_LENGTH 50
#define MIN_CONSERVATIVE_DATA_LENGTH 256

#elif NB_TLSH_BUCKETS == 128
#define TLSH_STRING_LEN_REQ 72
#define MIN_DATA_LENGTH 50
#define MIN_CONSERVATIVE_DATA_LENGTH 256

#elif NB_TLSH_BUCKETS == 48
#define TLSH_STRING_LEN 30
#define MIN_DATA_LENGTH 10
#define MIN_CONSERVATIVE_DATA_LENGTH 10

#else
#error "Invalid bucket size"
#endif

#define TLSH_STRING_BUFFER_LEN (TLSH_STRING_LEN_REQ + 1)

#ifdef _WINDOWS
#define TLSH_API __declspec(dllexport)
#else
#define TLSH_API __attribute__((visibility("default")))
#endif

class TlshImpl;

///
/// @brief Wrapper class for TLSH implementation
///
class TLSH_API Tlsh
{
public:
    Tlsh();
    Tlsh(const Tlsh &other) = delete;
    Tlsh(Tlsh &&other)      = default;


    // operators
    Tlsh &
    operator=(const Tlsh &other) = delete;
    Tlsh &
    operator=(Tlsh &&other) = default;
    bool
    operator==(const Tlsh &other) const;
    bool
    operator!=(const Tlsh &other) const;


    ///
    /// @brief allow the user to add data in multiple iterations
    ///
    /// @param [in] data
    ///
    void
    update(std::vector<u8> const &data);

    ///
    /// @brief Required step to signal the class there is no more data to be added
    ///
    /// @param [in] (optional) tlsh_option - default 0
    ///
    void
    final(u32 tlsh_option = 0);

    const std::vector<u8>
    getHashBytes(u8 showvers = 0) const;

    /* to get the hex-encoded hash code */
    const std::string
    getHashString(u8 showvers = 0) const;

    /* to get the hex-encoded hash code without allocating buffer in TlshImpl - bufSize should be
     * TLSH_STRING_BUFFER_LEN */
    // const char *
    // getHashString(char *buffer, unsigned int bufSize, int showvers = 0) const;

    /* to bring to object back to the initial state */
    void
    reset();

    // access functions
    int
    Lvalue();
    int
    Q1ratio();
    int
    Q2ratio();
    int
    Checksum(int k);
    int
    BucketValue(int bucket);
    int
    HistogramCount(int bucket);

    /* calculate difference */
    /* The len_diff parameter specifies if the file length is to be included in the difference
     * calculation (len_diff=true) or if it */
    /* is to be excluded (len_diff=false).  In general, the length should be considered in the
     * difference calculation, but there */
    /* could be applications where a part of the adversarial activity might be to add a lot of
     * content.  For example to add 1 million */
    /* zero bytes at the end of a file.  In that case, the caller would want to exclude the length
     * from the calculation. */
    int
    totalDiff(const Tlsh &, bool len_diff = true) const;

    /* validate TrendLSH string and reset the hash according to it */
    int
    fromTlshStr(const std::string &str);

    int
    fromTlshBytes(std::vector<u8> const &bytes);

    /* check if Tlsh object is valid to operate */
    bool
    isValid() const;

    /* display the contents of NOTICE.txt */
    static void
    display_notice();

    /* Return the version information used to build this library */
    static inline std::string version =
        STR(TLSH_VERSION_MAJOR) "." STR(TLSH_VERSION_MINOR) "." STR(TLSH_VERSION_PATCH) " " STR(
            TLSH_HASH) ", " STR(TLSH_CHECKSUM) " byte checksum, sliding_window=" STR(SLIDING_WND_SIZE);

    static inline std::array<u8, 5> version_info = {TLSH_VERSION_MAJOR, TLSH_VERSION_MINOR,
        TLSH_VERSION_PATCH, TLSH_CHECKSUM, SLIDING_WND_SIZE};

    std::unique_ptr<TlshImpl> const &
    impl() const
    {
        return m_Implementation;
    }

private:
    std::unique_ptr<TlshImpl> m_Implementation;
};

#ifdef TLSH_DISTANCE_PARAMETERS
void
set_tlsh_distance_parameters(int length_mult_value,
    int qratio_mult_value,
    int hist_diff1_add_value,
    int hist_diff2_add_value,
    int hist_diff3_add_value);
#endif
