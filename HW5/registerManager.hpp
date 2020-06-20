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

// class RegisterManager {

//     private: 
//     /* Here will be the instance stored. */
//     static RegisterManager* instance;

//     bool is_initalized = false;
//     /* Private constructor to prevent instancing. */
//     RegisterManager(); 
//     int register_index = 0;
    
//     public:
//     /* Static access method. */
//     static RegisterManager* getInstance(){
//         if (instance == 0)
//         {
//             instance = new RegisterManager();
//         }

//         return instance;
//     }
//     Register* getFreshReg(){
//         // TODO: add "t" after %
//         Register* ret = new Register(register_index, "%" + to_string(register_index));
//         register_index++;
//         return ret;
//     }
// };

// RegisterManager* RegisterManager::instance = 0;
// RegisterManager::RegisterManager()
// {}

class Singleton
{
    private:
        /* Here will be the instance stored. */
        static Singleton* instance;

        /* Private constructor to prevent instancing. */
        Singleton();

    public:
        /* Static access method. */
        static Singleton* getInstance() {
            if (instance == 0)
            {
                instance = new Singleton();
            }

            return instance;
        }
};

/* Null, because instance will be initialized on demand. */
Singleton* Singleton::instance = 0;


//RegisterManager* RegisterManager::instance = 0;