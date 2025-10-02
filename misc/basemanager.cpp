#include "basemanager.h"

BaseManager::BaseManager(BaseModel *model, QObject *parent)
    : QObject(parent),m_model(model)
{

}


void BaseManager::onUserLoggedIn() {
    fetch();
}


void BaseManager::fakeFetch() {
    m_model->reset();
}
