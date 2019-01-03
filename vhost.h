#import "stlvector.h"

class vhost
{
public:
	std::string vhost_name;
	std::vector<std::string> vhost_aliases;
	std::string document_root;
	std::string redirect_target;
	int vhost_type;
	std::string source_file;
};

//int operator==(const vhost& vhost1, const vhost& vhost2) { return vhost1.vhost_name == vhost2.vhost_name; }
//int operator!=(const vhost& vhost1, const vhost& vhost2) { return !(vhost1 == vhost2); }

