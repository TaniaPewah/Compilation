using namespace std;
#include <iostream>
#include <string>
#include "bp.hpp"
CodeBuffer& codeBuffer = CodeBuffer::instance();


class Register {
	int num;
    string name;
	bool used;
    
public: 
    Register() : num(0), name("invalid"), used(false) {}
    Register( int num, string name) : num(num), name(name), used(false){}
    bool isFree() {
		return !used;
	}
	void free() {
		used = false;
	}
	void setUsed() {
		used = true;
	}
	int getNum() {
		return num;
	}
	string getName() {
		return name;
	}
};

class IRManager {
private:
   static IRManager *instance;
    int register_index = 0;
    int global_register_index = 0;

   // Private constructor so that no objects can be created.
   IRManager() {  }

   public:
    IRManager(IRManager const&)       = delete;
    IRManager& operator=(IRManager const&)  = delete;
    static IRManager *getInstance() {
        if (!instance)
        instance = new IRManager;
        return instance;
    }

    Register* getFreshReg(){
        // TODO: add "t" after %
        Register* ret = new Register(register_index, "%.reg" + to_string(register_index));
        register_index++;
        return ret;
    }

    Register* getGlobalFreshReg(){
        // TODO: add "t" after %
        Register* ret = new Register(register_index, "@.g_reg" + to_string(global_register_index));
        global_register_index++;
        return ret;
    }

    void emitToBuffer(string command){
        codeBuffer.emit(command);
        cout << "command is: " << command << endl;
    }

    void emitGlobalToBuffer(string command){
        codeBuffer.emitGlobal(command);
        cout << "global command is: " << command << endl;
    }

    void endProgram(){
        codeBuffer.emitGlobal("\n \n; -----------------------  Program ------------------------ ");
        codeBuffer.printGlobalBuffer();
        codeBuffer.printCodeBuffer();
    }


};