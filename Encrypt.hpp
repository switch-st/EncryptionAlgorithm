#ifndef ENCRYPTION_ALGORITHM_HPP
#define ENCRYPTION_ALGORITHM_HPP

/**
 * 封装的文件/数据加密算法
 * 先支持MD5、AES、DES、BASE64、位移等算法
 * 通过openssl实现。
 * AES采用128位cbc模式。
 * DES采用ede3-cbc模式。
 * 编译连接选项 -lcrypto
 *
 * 注意：该库没做必要性检查，不按正确方式使用，行为未知。
 * 欢迎补充。
 **/

#ifndef NAMESPACE_SWITCH_TOOL
    #define NAMESPACE_SWITCH_TOOL
    #define OPEN_NAMESPACE_SWITCHTOOL       namespace Switch { \
                                                namespace Tool {
    #define CLOSE_NAMESPACE_SWITCHTOOL          }; \
                                            };
    #define USING_NAMESPACE_SWITCHTOOL      using namespace Switch::Tool;
#endif


#include "MD5.hpp"
#include "AES.hpp"
#include "DES.hpp"
#include "Shift.hpp"
#include "Base64.hpp"
#include "FileIO.hpp"


#endif // ENCRYPTION_ALGORITHM_HPP
