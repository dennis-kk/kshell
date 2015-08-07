#ifndef FRAMEWORK_EVENT_H
#define FRAMEWORK_EVENT_H

class Framework;

class FrameworkEvent {
public:
    FrameworkEvent(int type, Framework* framework) {
        _type = type;
        _framework = framework;
    }

    FrameworkEvent(const FrameworkEvent& frameworkEvent) {
        operator=(frameworkEvent);
    }

    const FrameworkEvent& operator=(const FrameworkEvent& frameworkEvent) {
        _type = frameworkEvent._type;
        _framework = frameworkEvent._framework;
        return *this;
    }

    int type() {
        return _type;
    }

    Framework* getFramework() {
        return _framework;
    }

private:
     FrameworkEvent() {
        _type = 0;
        _framework = 0;
    }

private:
    int _type;
    Framework* _framework;
};

#endif // FRAMEWORK_EVENT_H
