#include "matt-daemon.hpp"

bool	doCommand(std::string const &command, Clients &clients, int fd)
{
	if (command == "quit") {
		g_global.logger.logInfo("Client " + std::to_string(fd) + " sent quit command.");
		broadcast(clients, fd, "Server is shutting down.\n");
		g_global.is_running = false;
		return (true);
	}
	else if (command == "mail") {
		try {
			smtp::Smtp<Socket>	smtp(SMTP_SERVER, SMTP_PORT);
			smtp::Mail			mail("matt-daemon@test.com", "matt-daemon@test.com", "Test", "Test");

			g_global.logger.logInfo("Client " + std::to_string(fd) + " sent mail command.");
			g_global.logger.log("\n" + mail.toString());
			smtp.sendMail(mail);
			g_global.logger.logInfo("Mail sent.");
		} catch (std::exception &e) {
			g_global.logger.logWarning(e.what());
		}
		return (true);
	}
	return (false);
}
