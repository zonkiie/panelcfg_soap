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
	bool vhost_equals(vhost vhost1, vhost vhost2) { return vhost1.vhost_name == vhost2.vhost_name; }
};

 
