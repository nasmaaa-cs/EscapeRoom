#include "NetworkManager.h"
#include <iostream>
#include <istream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

using boost::asio::ip::tcp;

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent), socket(std::make_unique<tcp::socket>(io_context))
{
}

NetworkManager::~NetworkManager()
{
    if (socket->is_open()) {
        boost::system::error_code ec;
        socket->close(ec);
    }

    io_context.stop();

    if (asioThread.joinable()) {
        asioThread.join();
    }
}

void NetworkManager::hostGame(int port)
{
    if (io_context.stopped()) io_context.restart();

    asioThread = std::thread([this, port]() {
        try {
            tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
            acceptor.accept(*socket);

            emit opponentConnected();

            startReading();
            io_context.run();
        } catch (std::exception& e) {
            emit networkError(QString(e.what()));
        }
    });
}

void NetworkManager::joinGame(const std::string& ip, int port)
{

    asioThread = std::thread([this, ip, port]() {
        try {
            tcp::resolver resolver(io_context);
            auto endpoints = resolver.resolve(ip, std::to_string(port));

            boost::asio::connect(*socket, endpoints);

            emit opponentConnected();

            startReading();
            io_context.run();
        } catch (std::exception& e) {
            emit networkError(QString(e.what()));
        }
    });
}

void NetworkManager::startReading()
{
    boost::asio::async_read_until(*socket, receiveBuffer, '\n', [this](boost::system::error_code ec, std::size_t) {
        if (!ec) {
            std::istream is(&receiveBuffer);
            std::string messageStr;
            std::getline(is, messageStr);


            QJsonDocument doc = QJsonDocument::fromJson(QString::fromStdString(messageStr).toUtf8());

            if (!doc.isNull() && doc.isObject()) {
                QJsonObject obj = doc.object();
                QString type = obj["type"].toString();

                if (type == "PUZZLE") {
                    emit opponentPuzzleSolved(obj["value"].toInt());
                }
                else if (type == "FINISH") {
                    emit opponentFinished(obj["value"].toString());
                }
            }

            startReading();
        } else {
            emit networkError("Opponent Disconnected");
        }
    });
}

void NetworkManager::sendPuzzleSolved(int puzzleNumber) {
    QJsonObject obj;
    obj["type"] = "PUZZLE";
    obj["value"] = puzzleNumber;
    sendJson(obj);
}

void NetworkManager::sendFinished(const QString& finalTime) {
    QJsonObject obj;
    obj["type"] = "FINISH";
    obj["value"] = finalTime;
    sendJson(obj);
}

void NetworkManager::sendJson(const QJsonObject& obj)
{
    if (!socket->is_open()) return;

    QJsonDocument doc(obj);

    QByteArray bytes = doc.toJson(QJsonDocument::Compact) + "\n";

    auto msg_ptr = std::make_shared<std::string>(bytes.toStdString());

    boost::asio::post(io_context, [this, msg_ptr]() {
        boost::asio::async_write(*socket, boost::asio::buffer(*msg_ptr),
                                 [msg_ptr](boost::system::error_code ec, std::size_t /*length*/) {
                                     if (ec) {
                                         std::cerr << "Write error: " << ec.message() << std::endl;
                                     }
                                 });
    });
}
