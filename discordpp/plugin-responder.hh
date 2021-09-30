//
// Created by Aidan on 2/2/2020.
// Requires Plugin-Overload
//

#pragma once

#include <discordpp/plugin-overload.hh>

namespace discordpp {
template <class BASE> class PluginResponder : public BASE, virtual BotStruct {
    Snowflake id_;

  public:
    std::string prefix = "!";

    virtual void
    initBot(unsigned int apiVersionIn, const std::string &tokenIn,
            std::shared_ptr<asio::io_context> aiocIn) override {
        BASE::initBot(apiVersionIn, tokenIn, aiocIn);

        handlers.insert({"READY", [this](json data) {
                             id_ = data["user"]["id"].get<Snowflake>();
                         }});
    }

    virtual void respond(const std::string &command,
                         const handleEvent handler) {
        handlers.insert({"MESSAGE_CREATE", [this, command, handler](json msg) {
                             // Check for self and prefixed command
                             if (msg["author"]["id"].get<Snowflake>() != id_ &&
                                 msg["content"].get<std::string>().rfind(
                                     prefix + command, 0) == 0) {
                                 // Run the response
                                 handler(msg);
                             }
                         }});
    }

    virtual void respond(const std::string &command,
                         const std::string &response) {
        respond(command, [this, response](json msg) {
            // Echo the created message
            BASE::callJson()
                ->method("POST")
                ->target("/channels/" + msg["channel_id"].get<std::string>() +
                         "/messages")
                ->payload({{"content", response}})
                ->run();
        });
    }
};
} // namespace discordpp