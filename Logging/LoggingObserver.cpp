//
// Created by Khoa Trinh on 2021-11-03.
//

#include "LoggingObserver.h"

/**
 * create a list of observers
 */
Subject::Subject(){
    _observers = new list<Observer*>;
}

/**
 * clear list of observers
 */
Subject::~Subject(){
    delete _observers;
}

/**
 * attach an observer to the subject
 * @param o
 */
void Subject::attach(Observer* o){
    _observers->push_back(o);
};

/**
 * detach observer to subject
 * @param o
 */
void Subject::detach(Observer* o){
    _observers->remove(o);
};

/**
 * notify -> call update() on all observers
 */
void Subject::notify(){
    list<Observer *>::iterator i = _observers->begin();
    for (; i != _observers->end(); ++i)
        (*i)->update(this);
};

Observer::Observer(){
};
Observer::~Observer(){
};

/**
 * save stringToLog to gamelog.txt
 * @param s
 */
void LogObserver::update(Subject* s) {
    string stringToLog = s->stringToLog();
    // Create and open a text file
    ofstream MyFile("../Logging/gamelog.txt", fstream::app);

    // Write to the file
    MyFile << stringToLog << s->contentToLog << endl;

    // Close the file
    MyFile.close();
}

/**
 * attach itself to a provided subject
 * @param subject
 */
LogObserver::LogObserver(Subject *subject){
    _subject = subject;
    _subject->attach(this);
}

/**
 * detach itself from the subject
 */
LogObserver::~LogObserver() {
    _subject->detach(this);
}

Iloggable::Iloggable() {

}

Iloggable::~Iloggable() {

}
