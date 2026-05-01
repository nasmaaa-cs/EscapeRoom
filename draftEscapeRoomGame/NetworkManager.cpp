#include "NetworkManager.h"
#include <iostream>
#include <istream>

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

void NetworkManager::sendPuzzleSolved(int puzzleNumber)
{
    if (!socket->is_open()) return;

    std::string msg = "PUZZLE:" + std::to_string(puzzleNumber) + "\n";

    auto msg_ptr = std::make_shared<std::string>(msg);

    boost::asio::post(io_context, [this, msg_ptr]() {
        boost::asio::async_write(*socket, boost::asio::buffer(*msg_ptr), [msg_ptr](boost::system::error_code ec, std::size_t /*length*/) {
            if (ec) {
                std::cerr << "Write error: " << ec.message() << std::endl;
                    }
        });
    });
}

void NetworkManager::sendFinished(const QString& finalTime)
{
    if (!socket->is_open()) return;

    std::string msg = "FINISH:" + finalTime.toStdString() + "\n";
    auto msg_ptr = std::make_shared<std::string>(msg);

    boost::asio::post(io_context, [this, msg_ptr]() {
        boost::asio::async_write(*socket, boost::asio::buffer(*msg_ptr), [msg_ptr](boost::system::error_code ec, std::size_t /*length*/) {
            if (ec) {
                std::cerr << "Write error: " << ec.message() << std::endl;
                    }
        });
    });
}

void NetworkManager::startReading()
{
    boost::asio::async_read_until(*socket, receiveBuffer, '\n', [this](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
            std::istream is(&receiveBuffer);
            std::string message;
            std::getline(is, message);

            if (message.rfind("PUZZLE:", 0) == 0) {
                int pNum = std::stoi(message.substr(7));
                emit opponentPuzzleSolved(pNum);
            }

            else if (message.rfind("FINISH:", 0) == 0) {
                QString time = QString::fromStdString(message.substr(7));
                emit opponentFinished(time);
            }


            startReading();
            } else {
                emit networkError("Opponent Disconnected");
            }
        });
}
