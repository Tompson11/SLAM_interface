#include <QObject>
#include <QProcess>
#include <unordered_map>
#include <mutex>

namespace utils {

enum ShellType{
    SHELL_SH = 0,
    SHELL_BASH,
    SHELL_ZSH
};

template <ShellType T>
class ShellPool
{
public:

private:
    ShellPool();

    std::unordered_map<QProcess*, bool> available_process;

    std::mutex map_mutex;

    QObject *parent_ptr = nullptr;

public:
    static ShellPool& getInstance() {
        static ShellPool singleton;
        return singleton;
    };

    void setParent(QObject *parent) {
        parent_ptr = parent;
    }

    QProcess* getOneProcess() {
        QProcess *ret_process = nullptr;

        if(available_process.size() > 0) {
            map_mutex.lock();
            for(auto it = available_process.begin(); it != available_process.end(); it++) {
                if(it->second == true) {
                    ret_process = it->first;
                }
            }
            map_mutex.unlock();
        }

        if(ret_process == nullptr && parent_ptr != nullptr){
            ret_process = new QProcess();

            switch (T) {
            case SHELL_SH:
                ret_process->start("sh");
                break;
            case SHELL_ZSH:
                ret_process->start("zsh");
                break;
            default:
                ret_process->start("bash");
                break;
            }
        }

        return ret_process;
    };

    void returnOneProcess(QProcess *p) {
        if(p != nullptr) {
            auto it = available_process.find(p);

            if(it != available_process.end() && it->second == false) {
                p->disconnect();

                map_mutex.lock();
                it->second = true;
                map_mutex.unlock();
            }
        }
    }

};

}

