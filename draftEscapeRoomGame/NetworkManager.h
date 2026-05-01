#pragma once
#include <QObject>
#include <QString>
#include <thread>
#include <boost/asio.hpp>

class NetworkManager : public QObject {
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    // Setup functions (Call these depending on if player hosts or joins)
    void hostGame(int port);
    void joinGame(const std::string& ip, int port);

    // Send data to the opponent
    void sendPuzzleSolved(int puzzleNumber);
    void sendFinished(const QString& finalTime);

signals:
    // UI thread will listen to these
    void opponentConnected();
    void opponentPuzzleSolved(int puzzleNumber);
    void opponentFinished(QString finalTime);
    void networkError(QString errorMsg);

private:
    void networkThreadLoop();
    void startReading();

    boost::asio::io_context io_context;
    std::unique_ptr<boost::asio::ip::tcp::socket> socket;
    std::thread asioThread;
    boost::asio::streambuf receiveBuffer;
};
