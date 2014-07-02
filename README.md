EncryptionAlgorithm
===================

地址
	https://github.com/switch-st/EncryptionAlgorithm.git

说明
 * 封装的文件/数据加密算法
 * 先支持MD5、AES、DES、位移等算法
 * 通过openssl实现。
 * AES采用128位cbc模式。
 * DES采用ede3-cbc模式。
 * 编译连接选项 -lcrypto
 *
 * 注意：该库没做必要性检查，不按正确方式使用，行为未知。
 * 欢迎补充。

调用
 	调用方法详见test目录下的测试文件

			by switch
			switch.st@gmail.com

