#include <iostream>
#include <algorithm>
#include <vector>


int main(void)
{
	std::vector<std::string> masks = {
		"salut.*",
		"*.salut",
		"*.ok.*",
		"*",
		"salut.*.bonjour",
		"*.salut.*.bonjour.*",
		"*.*",
		".*.*.*.",
	};


	for (std::vector<std::string>::iterator it = masks.begin(); it != masks.end(); it++)
	{
		std::cout << it - masks.begin() << std::endl;
	}

	// //  "salut.*"

	// parse_mask(masks[0], "salut.bonjour");					   // match
	// parse_mask(masks[0], "salut.klqsjdqskjd lqksjd lkqjs dk"); // match
	// parse_mask(masks[0], "salut.");							   // match
	// parse_mask(masks[0], "");								   // no match

	// std::cout << std::endl;

	// // "*.salut"

	// parse_mask(masks[1], ".salut");					 // match
	// parse_mask(masks[1], "commentcava.salut");		 // match
	// parse_mask(masks[1], "commentcava.salutqsdqsd"); // no match
	// parse_mask(masks[1], "");						 // no match

	// std::cout << std::endl;

	// // "*.ok.*"

	// parse_mask(masks[2], "salut.ok.salut"); // match
	// parse_mask(masks[2], "salut.ok.");		// match
	// parse_mask(masks[2], ".ok.");			// match
	// parse_mask(masks[2], "salut.ok");		// no match
	// parse_mask(masks[2], "");				// no match

	// std::cout << std::endl;

	// // "*"

	// parse_mask(masks[3], "salut");	  // match
	// parse_mask(masks[3], "salut.ok"); // match
	// parse_mask(masks[3], "");		  // no match

	// std::cout << std::endl;

	// // "salut.*.bonjour"

	// parse_mask(masks[4], "salut.ok.bonjour");			   // match
	// parse_mask(masks[4], "salut.qsdqsdqsdqsdqsd.bonjour"); // match
	// parse_mask(masks[4], "salut..bonjour");				   // match
	// parse_mask(masks[4], "salut.bonjour");				   // no match
	// parse_mask(masks[4], "");							   // no match

	// std::cout << std::endl;

	// // *.salut.*.bonjour.*

	// parse_mask(masks[5], "t1.salut.t2.bonjour.t3");				 // match
	// parse_mask(masks[5], ".salut..bonjour.");					 // match
	// parse_mask(masks[5], ".salut.qslkdqlskdlqskdlqks.bonjour."); // match
	// parse_mask(masks[5], "qsdqskdqsldkqs.qsdlkqsdl.bonjour.");	 // no match
	// parse_mask(masks[5], "");									 // no match

	// std::cout << std::endl;

	// // "*.*"

	// parse_mask(masks[6], "salut.ok"); // match
	// parse_mask(masks[6], "salut.");	  // match
	// parse_mask(masks[6], ".ok");	  // match
	// parse_mask(masks[6], "qsdqsd");	  // no match
	// parse_mask(masks[6], "");		  // no match

	// std::cout << std::endl;

	// // ".*.*.*."

	// parse_mask(masks[7], "...."); // match
	// parse_mask(masks[7], ".a.b.c.");	// match
	// parse_mask(masks[7], ".qsdqsd..qsdqsdqsd.");		// match
	// parse_mask(masks[7], ".qsdqds.qsdqsd.qdqsdqds");	// no match
	// parse_mask(masks[7], ".qsdqsd.qsdqsd.qsdqsd.");	// match

	return (0);
}