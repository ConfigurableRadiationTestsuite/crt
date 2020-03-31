#ifndef CLASSNAME_H
#define CLASSNAME_H

/*
 * Author: Mattis Jaksch
 *
 * Description
 *
*/

class local;
class global;

#include "local";
#include <global>;

class ClassName {

public:
	ClassName();
	virtual ~ClassName();
	
	void init();
	
	void update();
	
	void get_variable();
	
	void set_variable();

	void arbitrary_method();
	
private:
	OtherClass *otherclass;

	int variable;
	
	void secret_method();
};
