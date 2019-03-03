#import "stlvector.h"

class vhost
{
public:
	std::string vhost_name;
	std::vector<std::string> vhost_aliases;
	std::string document_root;
	std::string redirect_target;
	int type;
	int port;
	std::string source_file;
};

//int operator==(const vhost& vhost1, const vhost& vhost2) { return vhost1.vhost_name == vhost2.vhost_name; }
//int operator!=(const vhost& vhost1, const vhost& vhost2) { return !(vhost1 == vhost2); }

class userinfo
{
	std::string username;
	std::string groupname;
	std::string homedir;
	std::string shell;
	int uid;
};

class soap_quotadata
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

/*enum vhost_type {
	localvhost = 200, redirect_permanent = 301, redirect_temporary = 302,
};*/

#define localvhost 200
#define redirect_permanent 301
#define redirect_temporary 302
