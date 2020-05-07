#include "Component.h"

Component::Component(const QString &elementName, RunManager *runManager)
    : runManager(runManager), elementName(elementName) {

    logTimer = new QTimer;
    connect(logTimer, SIGNAL(timeout()), this, SLOT(update()));
}

Component::Component(RunManager *runManager, const QString &config)
    : runManager(runManager) {

    load_config(config);
    assert(parse_config({"name"}));

    this->elementName = get_value("name");

    logTimer = new QTimer;
    connect(logTimer, SIGNAL(timeout()), this, SLOT(update()));
}

Component::~Component() {
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
    if(!logging)
        return ;

    logging = false;
    emit is_logging(false);

    runManager->deregister_component(this);

    runManager->set_run_mode(StopLog, elementName);
}


void Component::set_early_logging(int early_logging) {
    if(early_logging > 0) {
        start_logging();
        logging = true;
    }
    else {
        stop_logging();
        logging = false;
    }
}
