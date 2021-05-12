#include <xbond/snowflake.hpp>
#include <xbond/net/hardware_address.hpp>
#include <boost/crc.hpp>

namespace xbond {
// 计算当前节点标识（使用 MAC 地址 + PID 哈希生成）
static std::uint16_t current_node = ([] () {
    net::hardware_address ha;
    boost::crc_optimal<12ul, 0x0206> crc12; // 注意节点标识位宽须保持一致
    crc12.process_bytes(ha.data(), ha.size());
    auto pid = ::getpid();
    crc12.process_bytes(&pid, sizeof(pid));
    return crc12.checksum();
})();
// 默认节点标识
snowflake_node::snowflake_node()
: id_(current_node) {}

} // namespace xbond
