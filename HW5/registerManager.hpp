using namespace std;
#include <iostream>
#include <string>

class Register {
	int num;
    string name;
	bool used;
    
public: 
    Register() : num(-1), name("invalid"), used(false) {}
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

class RegisterManager {
private:
   static RegisterManager *instance;
    int register_index = 0;

   // Private constructor so that no objects can be created.
   RegisterManager() {  }

   public:
    RegisterManager(RegisterManager const&)       = delete;
    RegisterManager& operator=(RegisterManager const&)  = delete;
    static RegisterManager *getInstance() {
        if (!instance)
        instance = new RegisterManager;
        return instance;
    }

    Register* getFreshReg(){
        // TODO: add "t" after %
        Register* ret = new Register(register_index, "%" + to_string(register_index));
        register_index++;
        return ret;
    }
};