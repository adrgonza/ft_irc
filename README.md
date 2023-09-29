# ft_irc

This project is about creating our own IRC server in C++98. We used an actual IRC client to connect to our server and test it. IRC (Internet Relay Chat) is a protocol for real-time text messaging between internet-connected computers created in 1988. It is mainly used for group discussion in chat rooms called “channels” although it supports private messages between two users, data transfer, and various client-side commands.


## Commands we implemented

| Command | Description |
| :-----------: | :----------- |
| Invite | The `INVITE` command is used to invite a user to a channel. |
| Join | The `JOIN` command indicates that the client wants to join the given channel(s), each channel using the given key for it. |
| Kick | The `KICK` command can be used to request the forced removal of a user from a channel. |
| Kill | The `KILL` command is used to close the connection between a given client and the server they are connected to. `KILL` is a privileged command and is available only to IRC Operators. |
| List | The `LIST` command is used to get a list of channels along with some information about each channel. |
| Mode | The `MODE` command is used to set or remove options (or modes) from a given target. Our user modes : i, o. Our channels modes: b,k,m,o,p,s,t,v |
| Motd | The `MOTD` command is used to get the “Message of the Day” of the given server. |
| Names | The `NAMES` command is used to view the nicknames joined to a channel and their channel membership prefixes. |
| Nick | The `NICK` command is used to give the client a nickname or change the previous one. |
| Notice | The `NOTICE` command is used to send notices between users, as well as to send notices to channels. The difference between `NOTICE` and `PRIVMSG` is that automatic replies must never be sent in response to a `NOTICE` message.  |
| Oper | The `OPER` command is used by a normal user to obtain IRC operator privileges.  |
| Part | The `PART` command removes the client from the given channel(s). |
| Pass | The `PASS` command is used to set a ‘connection password’. If set, the password must be set before any attempt to register the connection is made. |
| Ping | The `PING` command is sent by either clients or servers to check the other side of the connection is still connected and/or to check for connection latency, at the application layer. |
| Privmsg | The `PRIVMSG` command is used to send private messages between users, as well as to send messages to channels. |
| Quit | The `QUIT` command is used to terminate a client’s connection to the server. The server acknowledges this by replying with an `ERROR` message and closing the connection to the client. |
| Topic | The `TOPIC` command is used to change or view the topic of the given channel. |
| User | The `USER` command is used at the beginning of a connection to specify the username and realname of a new user. |
