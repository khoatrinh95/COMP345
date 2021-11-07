//
// Created by Khoa Trinh on 2021-11-03.
//

#include "LoggingObserver.h"

Subject::Subject(){
    _observers = new list<Observer*>;
}
Subject::~Subject(){
    delete _observers;
}
void Subject::attach(Observer* o){
    _observers->push_back(o);
};
void Subject::detach(Observer* o){
    _observers->remove(o);
};
void Subject::notify(){
    list<Observer *>::iterator i = _observers->begin();
    for (; i != _observers->end(); ++i)
        (*i)->update(this);
};

Observer::Observer(){
};
Observer::~Observer(){
};

void LogObserver::update(Subject* s) {
    string stringToLog = s->stringToLog();
    // Create and open a text file
    ofstream MyFile("../Logging/gamelog.txt", fstream::app);

    // Write to the file

    MyFile << stringToLog << s->contentToLog << endl;

    // Close the file
    MyFile.close();
}

LogObserver::LogObserver(Subject *subject){
    _subject = subject;
    _subject->attach(this);
}

LogObserver::~LogObserver() {
    _subject->detach(this);
}

Iloggable::Iloggable() {

}

Iloggable::~Iloggable() {

}
