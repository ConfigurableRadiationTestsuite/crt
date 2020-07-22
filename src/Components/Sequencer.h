#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "Component.h"

class Task;

class Sequencer : public Component {
Q_OBJECT

public:
    Sequencer(RunManager *runManager, const QString &config);
    Sequencer(RunManager *runManager, const QString &m_element_name, int task_number);
    ~Sequencer() override;

    void set_config() override;

    QVector<Task*> get_task_vec() const {return task_vec;}

signals:
    void task_vec_changed();

public slots:
    void init() override;
    void update() override;

    void set_loop(int);

private slots:
    void update_task_vec();

private:
    int task_number = 0;
    QVector<Task*> task_vec;
    QVector<Task*>::iterator task_it;

    bool loop = false;

    QStringList generate_header() override;
};

inline QStringList Sequencer::generate_header() {
    return {"number", "time", "signal"};
}

inline void Sequencer::set_loop(int loop) {
    this->loop = loop == 0 ? false : true;
}

class Task : public QObject {
Q_OBJECT

uint number;
uint time;
QString signal_name;
struct RegisteredSignal *sig;

public:
    Task(uint number, uint time, QString signal_name, RegisteredSignal *sig)
        : number(number), time(time), signal_name(signal_name), sig(sig) {}

    uint get_number() const {return number;}
    uint get_time() const {return time;}
    QString get_signal_name() const {return signal_name;}
    RegisteredSignal * get_signal() const {return sig;}

public slots:
    void set_signal(RegisteredSignal *);
    void set_time(const QString &time) {this->time = time.toUInt();}

    void signal_button_clicked() {emit update_task(this);}

signals:
    void update_task(Task *);
    void signal_name_changed(const QString &);
};

inline void Task::set_signal(RegisteredSignal *sig) {
    this->sig = sig;

    if(sig != nullptr) {
        this->signal_name = sig->name;

        emit signal_name_changed(this->signal_name);
    }
}

#endif // SEQUENCER_H
