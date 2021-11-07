//
// Created by Khoa Trinh on 2021-11-03.
//
#include <iostream>
#include <fstream>
#include <list>
#include <string>
using namespace std;
#ifndef COMP345_N11_LOGGINGOBSERVER_H
#define COMP345_N11_LOGGINGOBSERVER_H
class Iloggable;
class Subject;

class Observer {
public:
    Observer();
    ~Observer();
    virtual void update(Subject* i) = 0;
};

class Iloggable{
public:
    Iloggable();
    ~Iloggable();
    virtual string stringToLog() = 0;
};

class Subject : public Iloggable{
public:
    string contentToLog;
    Subject();
    ~Subject();
    virtual void attach(Observer* o);
    virtual void detach(Observer* o);
    virtual void notify();
private:
    list<Observer*> * _observers;
};



class LogObserver : public Observer {
public:
    LogObserver(Subject *subject);
    virtual ~LogObserver();
    void update(Subject* s);
private:
    Subject * _subject;
};



#endif //COMP345_N11_LOGGINGOBSERVER_H
