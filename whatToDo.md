# IRC Protocol and Commands

The IRC (Internet Relay Chat) protocol is defined in RFC 1459, and later updated by RFC 2810, RFC 2811, RFC 2812, and RFC 2813. These RFCs collectively describe the IRC protocol and its various functionalities and commands. Here is a summary of the key functionalities and commands defined in the IRC RFC:

## Subject Requirements

- You must be able to connect the reference client to your server without producing any errors.
- You must be able to authenticate, set a nickname, a username, join a channel, send and receive private messages using this client.
- All messages from one client on a channel are sent to all other clients of the channel.
- You must have operators and regular users.

</br>
</br>

<span style="color:green">(CD)</span> = Durrently Doing

## User Authentication and Connection

- `PASS`: Set a connection password.
- `NICK`: Set the user's nickname. ✅
- `USER`: Specify the user's username, hostname, servername, and real name.

## Channel Operations

- `JOIN`: Join a channel or a group of channels. <span style="color:green">(CD)</span>
- `INVITE`: Invite a user to a channel ✅
- `PART`: Leave a channel. ✅
- `TOPIC`: Set or get the topic of a channel. ✅
- `NAMES`: Retrieve the list of users in a channel. ✅
- `LIST`: List all channels or specific channels. <span style="color:green">(CD)</span>

## Messaging

- `PRIVMSG`: Send a private message to a user or a channel. <span style="color:green">(CD)</span>
- `NOTICE`: Send a notice message to a user or a channel. <span style="color:green">(CD)</span>

## Server Commands

- `PING`: PING triggers when a server sends a client aPING message to see if its still connected <span style="color:green">(CD)</span>
- `PONG`: Respond to a PING message. <span style="color:green">(CD)</span>

## Operator and Administrative Commands

Include any additional operator-specific actions or commands that your server supports here.
- `MODE`: Set channel or user modes, including user privileges.
- `KICK`: Kick a user from a channel.
- `KILL`: Remove a user from the network.
- `OPER`: Authenticate as an IRC operator.

## User Information and Tracking

- `WHO`: Get information about users on the network. <span style="color:green">(CD)</span>
- `WHOIS`: Get detailed information about a specific user. <span style="color:green">(CD)</span>
- `WHOWAS`: Get information about previously connected users.
- `ISON`: Check if a list of users is online. <span style="color:green">(CD)</span>

## Miscellaneous

- `VERSION`: Get the version of the server or a user's client. ?
- `TIME`: Get the local time of the server or a user. ?
