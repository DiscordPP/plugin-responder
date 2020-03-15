//
// Created by Aidan on 2/2/2020.
// Requires Plugin-Overload
//

#pragma once

#include <discordpp/plugin-overload.hh>

namespace discordpp{
	template<class BASE>
	class PluginResponder: public BASE, virtual BotStruct{
	public:
		std::string prefix = "!";

		virtual void respond(const std::string& command, const std::function<void(json)> handler){
			handlers.insert(
					{
							"MESSAGE_CREATE",
							[this, command, handler](json msg){
								// Check for the command
								const std::string& content = msg["content"];
								if(content.rfind(prefix + command, 0) == 0){
									// Run the response
									handler(msg);
								}
							}
					}
			);
		}

		virtual void respond(const std::string& command, const std::string response){
			respond(command, [this, response](json msg){
				// Echo the created message
				BASE::call(
						"POST",
						"/channels/" + msg["channel_id"].get<std::string>() + "/messages",
						json({{"content", response}})
				);
			});
		}
	};
}