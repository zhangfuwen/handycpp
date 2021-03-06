//
// Created by zhangfuwen on 2022/5/8.
//

#ifndef HANDYCPP_SYNTAX_H
#define HANDYCPP_SYNTAX_H

#include <functional>

namespace handycpp::syntax {

/**
 * golang like defer syntax
 *
 * @usage
 *     Example: \n\n
 *
 * @code
 *      int fd = open("xx", O_CREAT | O_RDWR, 0644);
 *      if (fd < 0) {
 *           return;
 *      }
 *      defer dfd([](){
 *           close(fd);
 *      });
 *
 */
struct defer {
    defer(std::function<void(void)> f) : f_(f) {}
    ~defer(void) { f_(); }

private:
    std::function<void(void)> f_;
};

#define Defer(x) defer defer_##__LINE__([&]() { x; })
#ifdef HANDYCPP_TEST
TEST_CASE("handycpp::syntax::defer") {
    int i;
    {
        i = 0;
        defer dt([&]() { i++; });
        i = 5;
    }
    CHECK(i == 6);
}
#endif
} // namespace handycpp::syntax

#endif // HANDYCPP_SYNTAX_H
