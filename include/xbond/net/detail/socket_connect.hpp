#pragma once
#include "../../vendor.h"
#include "../address.hpp"

namespace xbond {
namespace net {
namespace detail {

template <class Protocol>
class socket_connect {
    const net::address&           address_;
    typename Protocol::resolver& resolver_;
    typename Protocol::socket&     socket_;

 public:
    socket_connect(typename Protocol::socket& socket, const net::address& address, typename Protocol::resolver& resolver)
    : address_(address), resolver_(resolver), socket_(socket) {}

    template <class AsyncOperation>
    void operator()(AsyncOperation& self, boost::system::error_code = {}) {
        resolver_.async_resolve(address_.host(), address_.service(), std::move(self));
    }

    template <class AsyncOperation>
    void operator()(AsyncOperation& self, boost::system::error_code error, typename Protocol::resolver::results_type rt) {
        if (error) return self.complete(error);
        boost::asio::async_connect(socket_, rt, std::move(self));
    }

    template <class AsyncOperation>
    void operator()(AsyncOperation& self, boost::system::error_code error, typename Protocol::endpoint ep) {
        return self.complete(error);
    }
};

} // namespace detail
} // namespace net
} // namespace xbond