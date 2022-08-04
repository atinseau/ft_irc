#include <iostream>
#include <vector>

typedef std::vector<std::string> v_string;

int main(void)
{
	v_string v {
		"salut tout le monde",
		"bye"
	};

	v_string::iterator it = std::find(v.begin(), v.end(), "ok les gars");
	if (it != v.end())
		v.erase(it);
		
	for (auto &i: v)
		std::cout << i << std::endl;

	return (0);
}