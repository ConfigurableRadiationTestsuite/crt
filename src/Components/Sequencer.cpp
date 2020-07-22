#include "Sequencer.h"

Sequencer::Sequencer(RunManager * runManager, const QString &config)
    : Component(runManager, config) {

    load_config(config);
    assert(parse_config({"Tasks"}));

    this->task_number = get_value("Tasks").toInt();
}

Sequencer::Sequencer(RunManager *runManager, const QString &m_element_name, int task_number)
    : Component(m_element_name, runManager), task_number(task_number) {}

Sequencer::~Sequencer() {
    foreach(Task *task, task_vec)
        delete task;
}

void Sequencer::set_config() {
    set_value("Tasks", QString::number(task_vec.size()));

    for(int i = 0; i < task_vec.size(); ++i) {
        set_value("T" + QString::number(i) + "t", QString::number(task_vec[i]->get_time()));
        set_value("T" + QString::number(i) + "s", task_vec[i]->get_signal_name());
    }
}

void Sequencer::init() {

    connect(runManager->get_eventManager(), SIGNAL(signal_added()), this, SLOT(update_task_vec()));
    connect(runManager->get_eventManager(), SIGNAL(signal_deleted(RegisteredSignal *)), this, SLOT(update_task_vec()));

    for(int i = 0; i < task_number; ++i) {
        if(is_empty())
            task_vec.push_back(new Task{(uint)i, 0, "", nullptr});
        else {
            uint time = get_value("T" + QString::number(i) + "t").toUInt();
            QString signal_name = get_value("T" + QString::number(i) + "s");
            RegisteredSignal *sig = runManager->get_eventManager()->get_signal(signal_name);

            task_vec.push_back(new Task{(uint)i, time, signal_name, sig});
        }
    }

    task_it = task_vec.begin();

    emit init_done();
}

void Sequencer::update() {
    if(!runManager->is_running() || (task_it == task_vec.end() && !loop))
        return ;

    /* Stop timer */
    logTimer->stop();

    /* Send signal */
    RegisteredSignal *sig = (*task_it)->get_signal();

    if(sig != nullptr) {
        runManager->append_values_to_file(this, {QString::number((*task_it)->get_number()), QString::number((*task_it)->get_time()), (*task_it)->get_signal_name()});

        if(sig->sub != nullptr && sig->sp != nullptr)
            emit ((sig->sub)->*(sig->sp))();
    }

    /* Start next timer */
    task_it++;
    if(loop && task_it == task_vec.end())
        task_it = task_vec.begin();

    if(task_it != task_vec.end())
        logTimer->start((*task_it)->get_time());
}

void Sequencer::update_task_vec() {
    // Synchronize the task list with the available signals
    foreach (Task *task, task_vec)
        task->set_signal(runManager->get_eventManager()->get_signal(task->get_signal_name()));

    emit task_vec_changed();
}
