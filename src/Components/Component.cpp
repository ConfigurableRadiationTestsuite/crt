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

    runManager->register_component(this, elementName);
    runManager->set_file_header(this, generate_header());
    logging = true;
}

void Component::stop_logging() {
    logging = false;
    runManager->deregister_component(this);
}
