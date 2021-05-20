#pragma once
#include "../vendor.h"
#include "../utility/data_view.hpp"
#include <boost/crc.hpp>

namespace xbond {
namespace hash {

class crc16 {
    boost::crc_optimal<16ul, 0x8005, 0, 0, true, true>
        hash_;
 public:
    template <class DataView, typename = typename std::enable_if<std::is_convertible<DataView, detail::data_view>::value, DataView>::type>
    inline void update(DataView data) {
        detail::data_view dv = data;
        hash_.process_bytes(dv.data(), dv.size());
    }
    inline void update(const char* data, std::size_t size) {
        hash_.process_bytes(data, size);
    }
    inline std::uint16_t digest() {
        return hash_.checksum();
    }
    template <class DataView, typename = typename std::enable_if<std::is_convertible<DataView, detail::data_view>::value, DataView>::type>
    static std::uint16_t hash(DataView data) {
        DataView dv = data;
        crc16 ctx;
        ctx.update(dv);
        return ctx.digest();
    }
};

} // namespace hash
} // namespace xbond
