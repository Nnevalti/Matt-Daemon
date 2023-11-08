#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <exception>

#include "ssl.hpp"

#define SMTP_PORT 25
#define SMTP_SUBMISSION_PORT 587
#define SMTPS_PORT 465

namespace smtp
{
	class Mail
	{
	public:
		std::string from;
		std::string to;
		std::string subject;
		std::string body;

		Mail(std::string from, std::string to, std::string subject, std::string body)
			: from(std::move(from)), to(std::move(to)), subject(std::move(subject)), body(std::move(body)) {}

		std::string toString() const
		{
			return "From: " + from + "\r\n" +
				   "To: " + to + "\r\n" +
				   "Subject: " + subject + "\r\n" +
				   "\r\n" +
				   body + "\r\n";
		}
	};


	template <typename S = ssl::SSocket>
	class Smtp
	{
	private:
		S _socket;

		enum ResponseCode
		{
			READY = 220,
			CLOSING = 221,
			OK = 250,
			START_MAIL_INPUT = 354
		};

		void __checkResponse(const std::string &command, int expectedCode)
		{
			this->_socket.send(command + "\r\n");
			this->__checkCode(expectedCode);
		}

		void __checkCode(int expectedCode)
		{
			auto response = this->_socket.recv();
			auto pos = response.first.find(std::to_string(expectedCode));
			if (pos == std::string::npos)
				throw std::runtime_error("SMTP error: " + __trim(response.first));
		}

		std::string	__trim(std::string &str) {
			str.erase(0, str.find_first_not_of(" \t\r\n"));
			str.erase(str.find_last_not_of(" \t\r\n") + 1);
			return (str);
		}

	public:
		Smtp() {}

		Smtp(const std::string &host, int port = SMTP_SUBMISSION_PORT) {
			this->connect(host, port);
		}

		~Smtp() {
			this->disconnect();
		}

		void connect(const std::string &host, int port = SMTP_SUBMISSION_PORT) {
			this->_socket.connect(host, port);
			this->__checkCode(READY);
			this->__checkResponse("EHLO localhost", OK);
		}

		void sendMail(const Mail &mail)
		{
			this->__checkResponse("MAIL FROM:<" + mail.from + ">", OK);
			this->__checkResponse("RCPT TO:<" + mail.to + ">", OK);
			this->__checkResponse("DATA", START_MAIL_INPUT);

			this->_socket.send(mail.toString() + "\r\n.\r\n");
			this->__checkResponse("QUIT", OK);
		}

		void sendMail(const std::string &from, const std::string &to, const std::string &subject, const std::string &body) {
			this->sendMail(Mail(from, to, subject, body));
		}

		void disconnect() {
			this->_socket.close();
		}
	};
}
