//
// Created by Aidan on 2/2/2020.
// Requires Plugin-Overload
//

#pragma once

#include <discordpp/plugin-overload.hh>

#include <boost/lexical_cast.hpp>

namespace discordpp {
template <class BASE> class PluginResponder : public BASE, virtual BotStruct {
    Snowflake id_;

  public:
    std::string prefix = "!";

    virtual void
    initBot(unsigned int apiVersionIn, const std::string &tokenIn,
            std::shared_ptr<boost::asio::io_context> aiocIn) override {
        BASE::initBot(apiVersionIn, tokenIn, aiocIn);

        handlers.insert({"READY", [this](json data) {
                             id_ = boost::lexical_cast<snowflake>(
                                 data["user"]["id"].get<std::string>());
                         }});
    }

    virtual void respond(const std::string &command,
                         const handleEvent handler) {
        handlers.insert(
            {"MESSAGE_CREATE", [this, command, handler](json msg) {
                 // Check for self and prefixed command
                 if (boost::lexical_cast<snowflake>(
                         msg["author"]["id"].get<std::string>()) != id_ &&
                     msg["content"].get<std::string>().rfind(prefix + command,
                                                             0) == 0) {
                     // Run the response
                     handler(msg);
                 }
             }});
    }

    virtual void respond(const std::string &command,
                         const std::string response) {
        respond(command, [this, response](json msg) {
            // Echo the created message
            BASE::call("POST",
                       "/channels/" + msg["channel_id"].get<std::string>() +
                           "/messages",
                       json({{"content", response}}));
        });
    }
};
} // namespace discordpp