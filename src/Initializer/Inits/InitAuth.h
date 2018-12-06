#ifndef INIT_AUTH_H
#define INIT_AUTH_H

#include "../InitInterface.h"

#include <QObject>

#include <memory>
#include <future>

struct TypedException;

namespace auth {
class Auth;
class AuthJavascript;
}

class MainWindow;

namespace initializer {

class InitializerJavascript;

class InitAuth: public QObject, public InitInterface {
    Q_OBJECT
public:

    using Return = std::pair<auth::Auth*, auth::AuthJavascript*>;

    using Callback = std::function<void()>;

public:

    InitAuth(QThread *mainThread, Initializer &manager);

    ~InitAuth() override;

    void complete() override;

    Return initialize(std::shared_future<MainWindow*> mainWindow);

    static int countEvents() {
        return 2;
    }

private:

    void sendInitSuccess(const TypedException &exception);

    void sendLoginCheckedSuccess(const TypedException &exception);

signals:

    void callbackCall(const Callback &callback);

private slots:

    void onCallbackCall(const Callback &callback);

    void onCheckTokenFinished(const TypedException &exception);

private:

    std::unique_ptr<auth::Auth> authManager;
    std::unique_ptr<auth::AuthJavascript> authJavascript;

    bool isCheckTokenFinished = false;
    bool isInitSuccess = false;

};

}

#endif // INIT_AUTH_H