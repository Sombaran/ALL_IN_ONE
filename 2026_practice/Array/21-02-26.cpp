#include <bits/stdc++.h>

using namespace std;

// Abstract Products
class Message {
public:
    virtual void setContent(const string& to, const string& body) = 0;
    virtual string format() const = 0;
    virtual ~Message() = default;
};

class Sender {
public:
    virtual void send(const Message& message) = 0;
    virtual ~Sender() = default;
};

// Email Products
class EmailMessage : public Message {
    string to, body;
public:
    void setContent(const string& to, const string& body) override {
        this->to = to;
        this->body = body;
    }
    string format() const override {
        return "Email to <" + to + ">: " + body;
    }
};

class EmailSender : public Sender {
public:
    void send(const Message& message) override {
        cout << "Sending via SMTP: " << message.format() << endl;
    }
};

// SMS Products
class SmsMessage : public Message {
    string to, body;
public:
    void setContent(const string& to, const string& body) override {
        this->to = to;
        this->body = body.length() > 160 ? body.substr(0, 160) : body;
    }
    string format() const override {
        return "SMS to " + to + ": " + body;
    }
};

class SmsSender : public Sender {
public:
    void send(const Message& message) override {
        cout << "Sending via carrier API: " << message.format() << endl;
    }
};

// Abstract Factory
class NotificationFactory {
public:
    virtual Message* createMessage() = 0;
    virtual Sender* createSender() = 0;
    virtual ~NotificationFactory() = default;
};

// Concrete Factories
class EmailFactory : public NotificationFactory {
public:
    Message* createMessage() override { return new EmailMessage(); }
    Sender* createSender() override { return new EmailSender(); }
};

class SmsFactory : public NotificationFactory {
public:
    Message* createMessage() override { return new SmsMessage(); }
    Sender* createSender() override { return new SmsSender(); }
};

// Client
class NotificationService {
    NotificationFactory* factory;
public:
    NotificationService(NotificationFactory* factory) : factory(factory) {}

    void notify(const string& to, const string& body) {
        Message* message = factory->createMessage();
        message->setContent(to, body);
        Sender* sender = factory->createSender();
        sender->send(*message);
        delete message;
        delete sender;
    }
};

int main() {
    cout << "=== Email Notification ===" << endl;
    EmailFactory emailFactory;
    NotificationService emailService(&emailFactory);
    emailService.notify("alice@example.com", "Your order has been shipped!");

    cout << endl;

    cout << "=== SMS Notification ===" << endl;
    SmsFactory smsFactory;
    NotificationService smsService(&smsFactory);
    smsService.notify("+1-555-0123", "Your order has been shipped!");

    return 0;
}