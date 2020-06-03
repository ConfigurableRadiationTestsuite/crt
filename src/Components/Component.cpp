#include "Component.h"

Component::Component(const QString &elementName, RunManager *runManager, uint time)
    : runManager(runManager), elementName(elementName) {

    configure_timer(time);

    /* RunManager info */
    runManager->set_run_mode(AddComponent, elementName);
}

Component::Component(RunManager *runManager, const QString &config, uint time)
    : runManager(runManager) {

    load_config(config);
    assert(parse_config({"name"}));

    elementName = get_value("name");

    configure_timer(time);

    /* RunManager info */
    runManager->set_run_mode(AddComponent, elementName);
}

Component::~Component() {
    runManager->set_run_mode(RemoveComponent, elementName);
    runManager->deregister_component(this);

    delete logTimer;
}

void Component::start_logging() {
    assert(elementName.isEmpty() == false);

    if(logging)
        return ;

    runManager->register_component(this, elementName);
    runManager->set_file_header(this, generate_header());

    runManager->set_run_mode(StartLog, elementName);

    logging = true;
    emit is_logging(true);
}

void Component::stop_logging() {
    if(!logging || early_logging)
        return ;

    logging = false;
    emit is_logging(false);

    runManager->deregister_component(this);

    runManager->set_run_mode(StopLog, elementName);
}

void Component::set_early_logging(int early_logging) {
    if(early_logging > 0) {
        start_logging();
        this->early_logging = true;
    }
    else {
        stop_logging();
        this->early_logging = false;
    }
}

void Component::configure_timer(uint time) {
    if(logTimer == nullptr) {
        logTimer = new QTimer;
        connect(logTimer, SIGNAL(timeout()), this, SLOT(update()));
    }

    logTimer->start(time);
}
