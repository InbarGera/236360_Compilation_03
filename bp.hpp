#ifndef EX5_CODE_GEN
#define EX5_CODE_GEN

#include <vector>
#include <string>

class CodeBuffer{

public:
	CodeBuffer();
	CodeBuffer(CodeBuffer const&);
	void operator=(CodeBuffer const&);
	std::vector<std::string> buffer;
	std::vector<std::string> dataDefs;
	static CodeBuffer &instance();

	// ******** Methods to handle the code section ********** //

	//generate a jump location label for the next command, writes to buffer
	std::string genLabel();

	//write command to the buffer, returns its location in the buffer
	int emit(const std::string &command);

	//accepts a list of buffer locations generated by emit and a label
	//backpatches the commands at all buffer locations with the provided label.
	//example:
	//int loc = emit("j "); //jump missing a location
	//bpatch(makelist(loc),"my_label"); //location loc in the buffer will now have the command "j my_label"
	void bpatch(const std::vector<int>& address_list, const std::string &loc);


	//print the content of the code buffer to stdout including a .text header
	void printCodeBuffer();


	static std::vector<int> makelist(int litem);

	static std::vector<int> merge(const std::vector<int> &l1,const std::vector<int> &l2);

	// ******** Methods to handle the data section ********** //
	//write a line to the data section
	void emitData(const std::string& dataLine);
	//print the content of the data buffer to stdout including a .data header
	void printDataBuffer();

    std::string genDataLabel();
};

#endif

