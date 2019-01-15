#import "stlvector.h"

class userinfo
{
	std::string username;
	std::string groupname;
	std::string homedir;
	std::string shell;
	int uid;
};

class quotadata
{
	std::string device;
	std::string username;
	std::string groupname;
	uint64_t block_softlimit;
	uint64_t block_hardlimit;
	uint64_t inode_softlimit;
	uint64_t inode_hardlimit;
	uint64_t blocks_used;
	uint64_t inodes_used;
};
