#ifndef CLASSNAME_H
#define CLASSNAME_H

/*
 * Author: Mattis Jaksch
 *
 * Description
 *
*/

#define SOMETHING

class local;
class global;

#include "local";
#include <global>;

namespace acroynm {

struct StructureName {
	int variable;
}

enum MyTypes{};

class ClassName {

public:
	ClassName();
	virtual ~ClassName();
	
	void init();
	
	void update();
	
	int get_variable() const {return variable;}
	
	void set_variable(int desired_variable);

	bool is_binary() const {return binary;}

	void arbitrary_method();

	void quick_method();
	
private:
	OtherClass *otherclass;

	int variable;
	bool binary;
	
	void secret_method();
};

inline void ClassName::quick_method() {}

}

#endif
